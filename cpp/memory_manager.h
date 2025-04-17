#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <vector>
#include <string>
#include <chrono>
#include <map>
#include <mutex>
#include <memory>
#include <functional>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <queue>

// Forward declarations
class GarbageCollector;
class MemoryBlock;
class GcAlgorithm;
class GcActivity;
class MemoryRecord;
class GcSettings;

// Memory block status
enum class BlockStatus {
    ACTIVE,
    FREE,
    FRAGMENTED
};

// Collection priority
enum class CollectionPriority {
    BALANCED,
    SPEED,
    MEMORY
};

// Memory block class
class MemoryBlock {
public:
    MemoryBlock(int id, size_t size, BlockStatus status);
    
    int getId() const;
    size_t getSize() const;
    BlockStatus getStatus() const;
    
    void setStatus(BlockStatus status);
    
private:
    int id;
    size_t size;
    BlockStatus status;
};

// GC Algorithm class
class GcAlgorithm {
public:
    GcAlgorithm(int id, const std::string& name, const std::string& description, bool enabled, int performanceScore);
    
    int getId() const;
    std::string getName() const;
    std::string getDescription() const;
    bool isEnabled() const;
    int getPerformanceScore() const;
    
    void setEnabled(bool enabled);
    void setPerformanceScore(int score);
    
    // Virtual method for algorithm-specific collection
    virtual size_t collect(std::vector<std::shared_ptr<MemoryBlock>>& blocks) = 0;
    
protected:
    int id;
    std::string name;
    std::string description;
    bool enabled;
    int performanceScore;
};

// Mark-Sweep algorithm
class MarkSweepAlgorithm : public GcAlgorithm {
public:
    MarkSweepAlgorithm(int id);
    size_t collect(std::vector<std::shared_ptr<MemoryBlock>>& blocks) override;
};

// Generational algorithm
class GenerationalAlgorithm : public GcAlgorithm {
public:
    GenerationalAlgorithm(int id);
    size_t collect(std::vector<std::shared_ptr<MemoryBlock>>& blocks) override;
};

// Reference Counting algorithm
class ReferenceCountingAlgorithm : public GcAlgorithm {
public:
    ReferenceCountingAlgorithm(int id);
    size_t collect(std::vector<std::shared_ptr<MemoryBlock>>& blocks) override;
};

// Concurrent GC algorithm
class ConcurrentGcAlgorithm : public GcAlgorithm {
public:
    ConcurrentGcAlgorithm(int id);
    size_t collect(std::vector<std::shared_ptr<MemoryBlock>>& blocks) override;
};

// GC Activity class
class GcActivity {
public:
    GcActivity(int id, int algorithmId, const std::chrono::system_clock::time_point& timestamp,
               int durationMs, size_t memoryReclaimed, int objectsCollected, float cpuImpact);
    
    int getId() const;
    int getAlgorithmId() const;
    std::chrono::system_clock::time_point getTimestamp() const;
    int getDurationMs() const;
    size_t getMemoryReclaimed() const;
    int getObjectsCollected() const;
    float getCpuImpact() const;
    
private:
    int id;
    int algorithmId;
    std::chrono::system_clock::time_point timestamp;
    int durationMs;
    size_t memoryReclaimed;
    int objectsCollected;
    float cpuImpact;
};

// Memory Record class
class MemoryRecord {
public:
    MemoryRecord(int id, const std::chrono::system_clock::time_point& timestamp,
                size_t totalMemory, size_t usedMemory, size_t freeMemory, float fragmentation);
    
    int getId() const;
    std::chrono::system_clock::time_point getTimestamp() const;
    size_t getTotalMemory() const;
    size_t getUsedMemory() const;
    size_t getFreeMemory() const;
    float getFragmentation() const;
    
private:
    int id;
    std::chrono::system_clock::time_point timestamp;
    size_t totalMemory;
    size_t usedMemory;
    size_t freeMemory;
    float fragmentation;
};

// GC Settings class
class GcSettings {
public:
    GcSettings(bool autoCollection, int memoryThreshold, int timeInterval,
               bool backgroundCollection, int cpuLimit, CollectionPriority collectionPriority);
    
    bool isAutoCollection() const;
    int getMemoryThreshold() const;
    int getTimeInterval() const;
    bool isBackgroundCollection() const;
    int getCpuLimit() const;
    CollectionPriority getCollectionPriority() const;
    
    void setAutoCollection(bool autoCollection);
    void setMemoryThreshold(int memoryThreshold);
    void setTimeInterval(int timeInterval);
    void setBackgroundCollection(bool backgroundCollection);
    void setCpuLimit(int cpuLimit);
    void setCollectionPriority(CollectionPriority collectionPriority);
    
private:
    bool autoCollection;
    int memoryThreshold;
    int timeInterval;
    bool backgroundCollection;
    int cpuLimit;
    CollectionPriority collectionPriority;
};

// Memory Manager class
class MemoryManager {
public:
    MemoryManager();
    ~MemoryManager();
    
    // Memory operations
    size_t getTotalMemory() const;
    size_t getUsedMemory() const;
    size_t getFreeMemory() const;
    float getFragmentation() const;
    
    // GC operations
    size_t runGarbageCollection();
    size_t optimizeMemory();
    size_t defragmentMemory();
    
    // Algorithm operations
    std::vector<std::shared_ptr<GcAlgorithm>> getAllAlgorithms() const;
    std::shared_ptr<GcAlgorithm> getAlgorithm(int id) const;
    bool updateAlgorithm(int id, bool enabled, int performanceScore);
    
    // Activity operations
    std::vector<std::shared_ptr<GcActivity>> getRecentActivities(int limit = 20) const;
    
    // Memory record operations
    std::vector<std::shared_ptr<MemoryRecord>> getRecentMemoryRecords(int limit = 100) const;
    
    // Memory block operations
    std::vector<std::shared_ptr<MemoryBlock>> getAllBlocks() const;
    
    // Settings operations
    std::shared_ptr<GcSettings> getSettings() const;
    bool updateSettings(const GcSettings& settings);
    
    // Stats operations
    int getGcRunsToday() const;
    std::chrono::system_clock::time_point getLastGcRun() const;
    int getAverageGcDuration() const;
    float getCpuImpact() const;
    
    // WebSocket interface
    void startWebSocketServer(int port = 8080);
    void stopWebSocketServer();
    
private:
    // Memory management
    void initializeMemory(size_t totalMemory = 10 * 1024 * 1024 * 1024); // 10 GB default
    void allocateMemory(size_t size);
    void freeMemory(size_t size);
    void updateMemoryUsage();
    void updateFragmentation();
    
    // GC management
    void initializeAlgorithms();
    void startBackgroundGc();
    void stopBackgroundGc();
    void backgroundGcThread();
    
    // WebSocket management
    void handleWebSocketMessage(const std::string& message);
    void sendWebSocketMessage(const std::string& message);
    
    // Data members
    std::vector<std::shared_ptr<MemoryBlock>> memoryBlocks;
    std::vector<std::shared_ptr<GcAlgorithm>> algorithms;
    std::vector<std::shared_ptr<GcActivity>> activities;
    std::vector<std::shared_ptr<MemoryRecord>> memoryRecords;
    std::shared_ptr<GcSettings> settings;
    
    size_t totalMemory;
    size_t usedMemory;
    size_t freeMemory;
    float fragmentation;
    
    int gcRunsToday;
    std::chrono::system_clock::time_point lastGcRun;
    int averageGcDuration;
    float cpuImpact;
    
    std::atomic<bool> running;
    std::thread backgroundGcThreadObj;
    std::mutex memoryMutex;
    std::mutex activitiesMutex;
    std::mutex recordsMutex;
    std::condition_variable gcCondition;
    
    // WebSocket server
    void* wsServer; // Opaque pointer to WebSocket server implementation
};

#endif // MEMORY_MANAGER_H 