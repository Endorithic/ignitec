use std::path;

pub struct ProjectStructure {
    pub project_root: path::PathBuf,
    pub source_directory: path::PathBuf,
    pub include_directory: path::PathBuf,
    pub target_directory: path::PathBuf,
}

impl ProjectStructure {
    /// Takes the project root and calculates the paths to all the necessary project directories once
    /// for easy reuse
    pub fn from_root(root: path::PathBuf) -> anyhow::Result<Self> {
        let source: path::PathBuf = root.join("src");
        if !source.is_dir() {
            anyhow::bail!("Cannot find source directory.");
        }

        let include: path::PathBuf = root.join("include");
        if !include.is_dir() {
            anyhow::bail!("Cannot find include directory.");
        }

        let target: path::PathBuf = root.join("target");
        if !target.is_dir() {
            anyhow::bail!("Cannot find target directory.");
        }

        Ok(Self {
            project_root: root,
            source_directory: source,
            include_directory: include,
            target_directory: target,
        })
    }

    /// Returns the location of the profile-specific target directory (target/<profile>/)
    pub fn get_profiled_target(&self, profile: &str) -> path::PathBuf {
        self.target_directory.join(profile)
    }

    /// Returns the location of the profile-specific object directory (target/<profile>/objects)
    pub fn get_profiled_object(&self, profile: &str) -> path::PathBuf {
        self.target_directory.join(profile).join("objects")
    }
}
