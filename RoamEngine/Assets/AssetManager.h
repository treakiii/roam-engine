// AssetManager.h - The asset librarian
// This keeps track of all your game stuff so you don't lose it

#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <mutex>
#include <functional>

// Asset types - what kind of stuff do we have?
enum class AssetType {
    Texture,
    Mesh,
    Audio,
    Script,
    Font,
    Material,
    Shader,
    Animation,
    Prefab,
    Scene,
    Custom
};

// Load states - is it ready yet?
enum class LoadState {
    NotLoaded,
    Loading,
    Loaded,
    Error
};

// Asset handle - smart pointer for assets
template<typename T>
class AssetHandle {
public:
    AssetHandle() : asset(nullptr), refCount(0) {}
    AssetHandle(T* assetPtr) : asset(assetPtr), refCount(1) {
        if (asset) asset->AddRef();
    }
    AssetHandle(const AssetHandle& other) : asset(other.asset), refCount(other.refCount + 1) {
        if (asset) asset->AddRef();
    }
    ~AssetHandle() {
        Release();
    }

    AssetHandle& operator=(const AssetHandle& other) {
        if (this != &other) {
            Release();
            asset = other.asset;
            refCount = other.refCount + 1;
            if (asset) asset->AddRef();
        }
        return *this;
    }

    T* operator->() { return asset; }
    const T* operator->() const { return asset; }
    T& operator*() { return *asset; }
    const T& operator*() const { return *asset; }
    operator bool() const { return asset != nullptr; }

    void Release() {
        if (asset && --refCount <= 0) {
            asset->Release();
            asset = nullptr;
        }
    }

    int GetRefCount() const { return refCount; }

private:
    T* asset;
    int refCount;
};

// Base asset class - the foundation of all assets
class Asset {
public:
    Asset(AssetType type, const std::string& name);
    virtual ~Asset();

    // Basic properties
    AssetType GetType() const { return type; }
    const std::string& GetName() const { return name; }
    LoadState GetLoadState() const { return loadState; }
    const std::string& GetFilePath() const { return filePath; }

    // Loading
    virtual bool LoadFromFile(const std::string& path) = 0;
    virtual bool SaveToFile(const std::string& path) = 0;
    virtual void Unload() = 0;

    // Memory management
    void AddRef() { ++refCount; }
    void Release() { if (--refCount <= 0) delete this; }
    int GetRefCount() const { return refCount; }

    // Metadata
    void SetMetadata(const std::string& key, const std::string& value);
    std::string GetMetadata(const std::string& key) const;

protected:
    AssetType type;
    std::string name;
    std::string filePath;
    LoadState loadState;
    int refCount;
    std::unordered_map<std::string, std::string> metadata;
};

// Load request - for asynchronous loading
struct LoadRequest {
    std::string assetName;
    std::string filePath;
    AssetType type;
    std::function<void(Asset*)> onComplete;
    std::function<void(const std::string&)> onError;
    int priority;

    LoadRequest() : priority(0) {}
};

// The AssetManager class - our asset warehouse manager
class AssetManager {
public:
    static AssetManager& GetInstance();

    // Initialization - set up the warehouse
    bool Initialize();
    void Shutdown();

    // Asset registration - tell us what you have
    template<typename T>
    void RegisterAssetType(AssetType type, std::function<T*()> factory) {
        assetFactories[type] = [factory]() -> Asset* { return factory(); };
    }

    // Asset loading - get stuff from the warehouse
    template<typename T>
    AssetHandle<T> LoadAsset(const std::string& name, const std::string& path = "") {
        std::lock_guard<std::mutex> lock(assetMutex);

        // Check if already loaded
        auto it = loadedAssets.find(name);
        if (it != loadedAssets.end()) {
            return AssetHandle<T>(dynamic_cast<T*>(it->second));
        }

        // Create new asset
        auto factoryIt = assetFactories.find(T::GetAssetType());
        if (factoryIt == assetFactories.end()) {
            // Error: no factory for this type
            return AssetHandle<T>();
        }

        T* asset = dynamic_cast<T*>(factoryIt->second());
        if (!asset) {
            return AssetHandle<T>();
        }

        std::string assetPath = path.empty() ? GetAssetPath(name) : path;
        if (asset->LoadFromFile(assetPath)) {
            loadedAssets[name] = asset;
            return AssetHandle<T>(asset);
        }

        delete asset;
        return AssetHandle<T>();
    }

    // Synchronous loading
    Asset* LoadAssetSync(const std::string& name, AssetType type, const std::string& path = "");

    // Asynchronous loading - load in the background
    void LoadAssetAsync(const LoadRequest& request);
    void UpdateAsyncLoading();

    // Asset unloading - clean up the warehouse
    void UnloadAsset(const std::string& name);
    void UnloadAllAssets();
    void UnloadUnusedAssets();

    // Asset queries - what's in the warehouse?
    Asset* GetAsset(const std::string& name) const;
    std::vector<std::string> GetAssetNames(AssetType type = AssetType::Custom) const;
    std::vector<Asset*> GetAssetsOfType(AssetType type) const;

    // Path management - where do assets live?
    void SetAssetRootPath(const std::string& path) { assetRootPath = path; }
    std::string GetAssetPath(const std::string& assetName) const;
    std::string GetAssetRootPath() const { return assetRootPath; }

    // Memory management - how much space are we using?
    size_t GetMemoryUsage() const;
    void SetMemoryLimit(size_t limit) { memoryLimit = limit; }
    size_t GetMemoryLimit() const { return memoryLimit; }

    // Caching - remember what we loaded
    void EnableCaching(bool enable) { cachingEnabled = enable; }
    void ClearCache();

    // Hot reloading - update assets when files change
    void EnableHotReloading(bool enable) { hotReloadingEnabled = enable; }
    void CheckForChanges();

    // Asset processing - modify assets
    void RegisterProcessor(AssetType type, std::function<void(Asset*)> processor);
    void ProcessAsset(Asset* asset);

    // Debug - see what's happening
    void EnableDebugLogging(bool enable) { debugLogging = enable; }
    void DrawDebugInfo();

private:
    AssetManager();
    ~AssetManager();

    // Prevent copying - one manager is enough
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    // Asset storage
    std::unordered_map<std::string, Asset*> loadedAssets;
    std::unordered_map<AssetType, std::function<Asset*()>> assetFactories;
    std::vector<LoadRequest> asyncLoadQueue;

    // Settings
    std::string assetRootPath;
    size_t memoryLimit;
    bool cachingEnabled;
    bool hotReloadingEnabled;
    bool debugLogging;

    // Processing
    std::unordered_map<AssetType, std::vector<std::function<void(Asset*)>>> assetProcessors;

    // Threading
    std::mutex assetMutex;

    // Internal helpers
    std::string GetFileExtension(const std::string& filename) const;
    AssetType GetAssetTypeFromExtension(const std::string& extension) const;
    void ProcessAsyncLoadQueue();
    void EvictLRUAssets();
};

#endif // ASSETMANAGER_H
