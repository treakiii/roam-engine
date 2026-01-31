// ResourceManager.h - The librarian of the engine
// This manages all our assets and resources

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>

// Resource types - what we can load
enum class ResourceType {
    Texture,
    Mesh,
    Shader,
    Audio,
    Font,
    Script,
    Config,
    Custom
};

// Base resource class - the foundation of all resources
class Resource {
public:
    Resource(const std::string& path) : filePath(path), loaded(false), refCount(0) {}
    virtual ~Resource() = default;

    // Load the resource - bring it into memory
    virtual bool Load() = 0;

    // Unload the resource - free the memory
    virtual void Unload() = 0;

    // Get resource type - what are you?
    virtual ResourceType GetType() const = 0;

    // Reference counting - manage lifetime
    void AddRef() { ++refCount; }
    void Release() { if (--refCount <= 0) Unload(); }
    int GetRefCount() const { return refCount; }

    // Get file path - where did you come from?
    const std::string& GetFilePath() const { return filePath; }

    // Is loaded? - are you ready?
    bool IsLoaded() const { return loaded; }

protected:
    std::string filePath;
    bool loaded;
    int refCount;
};

// Resource handle - smart pointer for resources
template<typename T>
class ResourceHandle {
public:
    ResourceHandle() : resource(nullptr) {}
    ResourceHandle(T* res) : resource(res) { if (resource) resource->AddRef(); }
    ResourceHandle(const ResourceHandle& other) : resource(other.resource) { if (resource) resource->AddRef(); }
    ~ResourceHandle() { if (resource) resource->Release(); }

    ResourceHandle& operator=(const ResourceHandle& other) {
        if (this != &other) {
            if (resource) resource->Release();
            resource = other.resource;
            if (resource) resource->AddRef();
        }
        return *this;
    }

    T* operator->() { return resource; }
    const T* operator->() const { return resource; }
    T& operator*() { return *resource; }
    const T& operator*() const { return *resource; }
    operator bool() const { return resource != nullptr; }

private:
    T* resource;
};

// The ResourceManager class - our asset librarian
class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    // Load resource - get it from the shelf
    template<typename T>
    ResourceHandle<T> Load(const std::string& path) {
        std::lock_guard<std::mutex> lock(resourceMutex);

        auto it = resources.find(path);
        if (it != resources.end()) {
            return ResourceHandle<T>(dynamic_cast<T*>(it->second.get()));
        }

        // Create new resource
        auto resource = std::make_unique<T>(path);
        if (resource->Load()) {
            T* ptr = resource.get();
            resources[path] = std::move(resource);
            return ResourceHandle<T>(ptr);
        }

        return ResourceHandle<T>();
    }

    // Unload resource - put it back on the shelf
    void Unload(const std::string& path);

    // Clear all resources - close the library
    void Clear();

    // Get memory usage - how much are we hoarding?
    size_t GetMemoryUsage() const;

private:
    // Our resource collection - the library
    std::unordered_map<std::string, std::unique_ptr<Resource>> resources;

    // Mutex for thread safety - don't let threads ransack the library
    std::mutex resourceMutex;
};

#endif // RESOURCEMANAGER_H
