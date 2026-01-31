// FileSystem.h - The file system manager
// This handles all file operations and path management

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <vector>
#include <filesystem>

// The FileSystem class - our file system guru
class FileSystem {
public:
    FileSystem();
    ~FileSystem();

    // Path operations
    static std::string GetCurrentDirectory();
    static bool SetCurrentDirectory(const std::string& path);
    static std::string GetAbsolutePath(const std::string& path);
    static std::string GetRelativePath(const std::string& path, const std::string& base);

    // Directory operations
    static bool CreateDirectory(const std::string& path);
    static bool RemoveDirectory(const std::string& path);
    static bool DirectoryExists(const std::string& path);
    static std::vector<std::string> GetDirectories(const std::string& path);
    static std::vector<std::string> GetFiles(const std::string& path, const std::string& extension = "");

    // File operations
    static bool FileExists(const std::string& path);
    static bool RemoveFile(const std::string& path);
    static bool CopyFile(const std::string& source, const std::string& destination);
    static bool MoveFile(const std::string& source, const std::string& destination);
    static uint64_t GetFileSize(const std::string& path);
    static std::string GetFileExtension(const std::string& path);
    static std::string GetFileName(const std::string& path);
    static std::string GetFileNameWithoutExtension(const std::string& path);
    static std::string GetDirectoryName(const std::string& path);

    // File reading/writing
    static bool ReadTextFile(const std::string& path, std::string& content);
    static bool WriteTextFile(const std::string& path, const std::string& content);
    static bool ReadBinaryFile(const std::string& path, std::vector<char>& data);
    static bool WriteBinaryFile(const std::string& path, const std::vector<char>& data);

    // Path utilities
    static std::string CombinePath(const std::string& path1, const std::string& path2);
    static std::string NormalizePath(const std::string& path);
    static bool IsAbsolutePath(const std::string& path);

    // File watching - monitor changes
    class FileWatcher {
    public:
        FileWatcher(const std::string& path);
        ~FileWatcher();

        // Check for changes - has anything changed?
        bool HasChanges();

        // Get changed files - what changed?
        std::vector<std::string> GetChangedFiles();

    private:
        std::string watchPath;
        std::filesystem::file_time_type lastWriteTime;
        std::vector<std::string> changedFiles;
    };

    // Asset paths - where our game assets live
    static void SetAssetsPath(const std::string& path);
    static std::string GetAssetsPath();
    static std::string GetAssetPath(const std::string& assetName);

private:
    // Assets path
    static std::string assetsPath;
};

#endif // FILESYSTEM_H
