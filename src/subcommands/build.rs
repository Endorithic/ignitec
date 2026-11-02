use std::env;
use std::ffi;
use std::fs;
use std::path;
use std::process;

use anyhow::Context;
use clap::Args;

use crate::core::directories::ProjectStructure;
use crate::info;
use crate::logger::Logger;
use crate::warn;

#[derive(Args)]
pub struct BuildArgs {}

/// Delete all the stale object files
fn delete_objects(object_dir: &path::Path) -> anyhow::Result<()> {
    if !object_dir.exists() {
        return Ok(());
    }

    let dir_contents: fs::ReadDir = object_dir
        .read_dir()
        .context("Failed to read object directory while deleting objects.")?;

    for entry in dir_contents {
        let entry = entry?;

        let path: path::PathBuf = entry.path();
        if path.is_dir() {
            fs::remove_dir_all(&path)?;
        } else {
            fs::remove_file(&path)?;
        }
    }

    Ok(())
}

/// Compile the files in the provided directory,
/// and place the object files into the destination directory
fn compile_into(
    logger: &mut Logger,
    source_dir: &path::Path,
    include_dir: &path::Path,
    object_dir: &path::Path,
) -> anyhow::Result<()> {
    fs::create_dir_all(object_dir).context("Could not create object directory")?;

    let dir_contents: fs::ReadDir = source_dir
        .read_dir()
        .context("Failed to read source directory")?;

    for entry in dir_contents {
        let entry = entry?;

        let entry_path: path::PathBuf = entry.path();
        if entry_path.is_dir() {
            compile_into(
                logger,
                &entry_path,
                include_dir,
                &object_dir.join(entry.file_name()),
            )?;
            continue;
        }

        let Some(extension) = entry_path.extension() else {
            warn!(logger, "Ignoring file: {:?}", entry.file_name());
            continue;
        };

        if !extension.eq_ignore_ascii_case("cpp") {
            warn!(logger, "Ignoring file: {:?}", entry.file_name());
            continue;
        }

        let Some(file_name) = entry_path.file_name() else {
            anyhow::bail!("Failed to get filename");
        };

        let mut output: path::PathBuf = object_dir.join(file_name);
        output.set_extension("o");

        info!(logger, "Compiling file: {:?}", entry_path);
        let status: process::ExitStatus = process::Command::new("clang++")
            .arg(format!("-I{}", include_dir.to_str().unwrap_or("include/")))
            .arg("-c")
            .arg(entry.path())
            .arg("-std=c++23")
            .arg("-o")
            .arg(&output)
            .status()
            .context("Failed to compile file")?;

        if !status.success() {
            anyhow::bail!("clang++ failed while compiling {:?}", entry_path);
        }
    }

    Ok(())
}

/// Add all the object files in the directory to the command for linking
fn link_dir(
    logger: &mut Logger,
    object_dir: &path::Path,
    command: &mut process::Command,
) -> anyhow::Result<()> {
    let dir_contents: fs::ReadDir = object_dir
        .read_dir()
        .context("Failed to read object directory")?;

    for entry in dir_contents {
        let entry = entry?;

        let entry_path: path::PathBuf = entry.path();
        if entry_path.is_dir() {
            link_dir(logger, &entry_path, command)?;
            continue;
        }

        let Some(extension) = entry_path.extension() else {
            warn!(logger, "Ignoring file: {:?}", entry_path.file_name());
            continue;
        };

        if !extension.eq_ignore_ascii_case("o") {
            warn!(logger, "Ignoring file: {:?}", entry_path.file_name());
            continue;
        };

        command.arg(entry_path);
    }

    Ok(())
}

/// Link all the object files into the final executable
fn link_objects(
    logger: &mut Logger,
    object_dir: &path::Path,
    destination: &path::Path,
    application_name: &ffi::OsStr,
) -> anyhow::Result<()> {
    let mut command: process::Command = process::Command::new("clang++");

    link_dir(logger, object_dir, &mut command)?;

    command.arg("-o");
    command.arg(destination.join(application_name));

    let status: process::ExitStatus = command
        .status()
        .context("Failed to link final executable.")?;

    if !status.success() {
        anyhow::bail!("Failed to link final executable.");
    }

    Ok(())
}

/// Build the current project
pub fn build(logger: &mut Logger, _args: &BuildArgs) -> anyhow::Result<()> {
    let project_dir: path::PathBuf =
        env::current_dir().context("Failed to get project directory")?;
    let project_structure: ProjectStructure = ProjectStructure::from_root(project_dir)?;

    let profiled_object_dir: path::PathBuf = project_structure.get_profiled_object("debug");
    let profiled_target_dir: path::PathBuf = project_structure.get_profiled_target("debug");

    delete_objects(&profiled_object_dir)?;
    compile_into(
        logger,
        &project_structure.source_directory,
        &project_structure.include_directory,
        &profiled_object_dir,
    )?;

    let application_name: &ffi::OsStr = project_structure
        .project_root
        .file_name()
        .unwrap_or(ffi::OsStr::new("app"));

    link_objects(
        logger,
        &profiled_object_dir,
        &profiled_target_dir,
        application_name,
    )?;

    Ok(())
}
