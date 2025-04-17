#include "memory_manager.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <json/json.h> // Requires JsonCpp library

// MemoryBlock implementation
MemoryBlock::MemoryBlock(int id, size_t size, BlockStatus status)
    : id(id), size(size), status(status) {}

int MemoryBlock::getId() const {
    return id;
}

size_t MemoryBlock::getSize() const {
    return size;
}

BlockStatus MemoryBlock::getStatus() const {
    return status;
}

void MemoryBlock::setStatus(BlockStatus status) {
    this->status = status;
}

// GcAlgorithm implementation
GcAlgorithm::GcAlgorithm(int id, const std::string& name, const std::string& description, bool enabled, int performanceScore)
    : id(id), name(name), description(description), enabled(enabled), performanceScore(performanceScore) {}

int GcAlgorithm::getId() const {
    return id;
}

std::string GcAlgorithm::getName() const {
    return name;
}

std::string GcAlgorithm::getDescription() const {
    return description;
}

bool GcAlgorithm::isEnabled() const {
    return enabled;
}

int GcAlgorithm::getPerformanceScore() const {
    return performanceScore;
}

void GcAlgorithm::setEnabled(bool enabled) {
    this->enabled = enabled;
}

void GcAlgorithm::setPerformanceScore(int score) {
    this->performanceScore = score;
}

// MarkSweepAlgorithm implementation
MarkSweepAlgorithm::MarkSweepAlgorithm(int id)
    : GcAlgorithm(id, "Mark-Sweep", "A basic GC algorithm that marks all reachable objects and then sweeps away the unmarked ones.", true, 72) {}

size_t MarkSweepAlgorithm::collect(std::vector<std::shared_ptr<MemoryBlock>>& blocks) {
    size_t memoryReclaimed = 0;
    
    // Mark phase - in a real implementation, this would traverse the object graph
    // For simulation, we'll just randomly mark some blocks as free
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    for (auto& block : blocks) {
        if (block->getStatus() == BlockStatus::ACTIVE && dis(gen) < 0.3) {
            memoryReclaimed += block->getSize();
            block->setStatus(BlockStatus::FREE);
        }
    }
    
    return memoryReclaimed;
}

// GenerationalAlgorithm implementation
GenerationalAlgorithm::GenerationalAlgorithm(int id)
    : GcAlgorithm(id, "Generational", "Groups objects by age and collects younger generations more frequently than older ones.", true, 89) {}

size_t GenerationalAlgorithm::collect(std::vector<std::shared_ptr<MemoryBlock>>& blocks) {
    size_t memoryReclaimed = 0;
    
    // In a real implementation, this would focus on younger generations
    // For simulation, we'll just randomly mark some blocks as free
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    for (auto& block : blocks) {
        if (block->getStatus() == BlockStatus::ACTIVE && dis(gen) < 0.4) {
            memoryReclaimed += block->getSize();
            block->setStatus(BlockStatus::FREE);
        }
    }
    
    return memoryReclaimed;
}

// ReferenceCountingAlgorithm implementation
ReferenceCountingAlgorithm::ReferenceCountingAlgorithm(int id)
    : GcAlgorithm(id, "Reference Counting", "Keeps track of the number of references to each object and collects when count reaches zero.", true, 65) {}

size_t ReferenceCountingAlgorithm::collect(std::vector<std::shared_ptr<MemoryBlock>>& blocks) {
    size_t memoryReclaimed = 0;
    
    // In a real implementation, this would check reference counts
    // For simulation, we'll just randomly mark some blocks as free
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    for (auto& block : blocks) {
        if (block->getStatus() == BlockStatus::ACTIVE && dis(gen) < 0.25) {
            memoryReclaimed += block->getSize();
            block->setStatus(BlockStatus::FREE);
        }
    }
    
    return memoryReclaimed;
}

// ConcurrentGcAlgorithm implementation
ConcurrentGcAlgorithm::ConcurrentGcAlgorithm(int id)
    : GcAlgorithm(id, "Concurrent GC", "Performs collection alongside program execution to minimize pauses.", true, 78) {}

size_t ConcurrentGcAlgorithm::collect(std::vector<std::shared_ptr<MemoryBlock>>& blocks) {
    size_t memoryReclaimed = 0;
    
    // In a real implementation, this would run concurrently with the application
    // For simulation, we'll just randomly mark some blocks as free
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    for (auto& block : blocks) {
        if (block->getStatus() == BlockStatus::ACTIVE && dis(gen) < 0.35) {
            memoryReclaimed += block->getSize();
            block->setStatus(BlockStatus::FREE);
        }
    }
    
    return memoryReclaimed;
}

// GcActivity implementation
GcActivity::GcActivity(int id, int algorithmId, const std::chrono::system_clock::time_point& timestamp,
                      int durationMs, size_t memoryReclaimed, int objectsCollected, float cpuImpact)
    : id(id), algorithmId(algorithmId), timestamp(timestamp), durationMs(durationMs),
      memoryReclaimed(memoryReclaimed), objectsCollected(objectsCollected), cpuImpact(cpuImpact) {}

int GcActivity::getId() const {
    return id;
}

int GcActivity::getAlgorithmId() const {
    return algorithmId;
}

std::chrono::system_clock::time_point GcActivity::getTimestamp() const {
    return timestamp;
}

int GcActivity::getDurationMs() const {
    return durationMs;
}

size_t GcActivity::getMemoryReclaimed() const {
    return memoryReclaimed;
}

int GcActivity::getObjectsCollected() const {
    return objectsCollected;
}

float GcActivity::getCpuImpact() const {
    return cpuImpact;
}

// MemoryRecord implementation
MemoryRecord::MemoryRecord(int id, const std::chrono::system_clock::time_point& timestamp,
                          size_t totalMemory, size_t usedMemory, size_t freeMemory, float fragmentation)
    : id(id), timestamp(timestamp), totalMemory(totalMemory), usedMemory(usedMemory),
      freeMemory(freeMemory), fragmentation(fragmentation) {}

int MemoryRecord::getId() const {
    return id;
}

std::chrono::system_clock::time_point MemoryRecord::getTimestamp() const {
    return timestamp;
}

size_t MemoryRecord::getTotalMemory() const {
    return totalMemory;
}

size_t MemoryRecord::getUsedMemory() const {
    return usedMemory;
}

size_t MemoryRecord::getFreeMemory() const {
    return freeMemory;
}

float MemoryRecord::getFragmentation() const {
    return fragmentation;
}

// GcSettings implementation
GcSettings::GcSettings(bool autoCollection, int memoryThreshold, int timeInterval,
                      bool backgroundCollection, int cpuLimit, CollectionPriority collectionPriority)
    : autoCollection(autoCollection), memoryThreshold(memoryThreshold), timeInterval(timeInterval),
      backgroundCollection(backgroundCollection), cpuLimit(cpuLimit), collectionPriority(collectionPriority) {}

bool GcSettings::isAutoCollection() const {
    return autoCollection;
}

int GcSettings::getMemoryThreshold() const {
    return memoryThreshold;
}

int GcSettings::getTimeInterval() const {
    return timeInterval;
}

bool GcSettings::isBackgroundCollection() const {
    return backgroundCollection;
}

int GcSettings::getCpuLimit() const {
    return cpuLimit;
}

CollectionPriority GcSettings::getCollectionPriority() const {
    return collectionPriority;
}

void GcSettings::setAutoCollection(bool autoCollection) {
    this->autoCollection = autoCollection;
}

void GcSettings::setMemoryThreshold(int memoryThreshold) {
    this->memoryThreshold = memoryThreshold;
}

void GcSettings::setTimeInterval(int timeInterval) {
    this->timeInterval = timeInterval;
}

void GcSettings::setBackgroundCollection(bool backgroundCollection) {
    this->backgroundCollection = backgroundCollection;
}

void GcSettings::setCpuLimit(int cpuLimit) {
    this->cpuLimit = cpuLimit;
}

void GcSettings::setCollectionPriority(CollectionPriority collectionPriority) {
    this->collectionPriority = collectionPriority;
}

// MemoryManager implementation
MemoryManager::MemoryManager()
    : totalMemory(0), usedMemory(0), freeMemory(0), fragmentation(0.0f),
      gcRunsToday(0), averageGcDuration(0), cpuImpact(0.0f), running(false), wsServer(nullptr) {
    
    // Initialize memory
    initializeMemory();
    
    // Initialize algorithms
    initializeAlgorithms();
    
    // Initialize settings
    settings = std::make_shared<GcSettings>(true, 75, 30, true, 20, CollectionPriority::BALANCED);
    
    // Start background GC
    startBackgroundGc();
}

MemoryManager::~MemoryManager() {
    // Stop background GC
    stopBackgroundGc();
    
    // Stop WebSocket server
    stopWebSocketServer();
}

// Memory operations
size_t MemoryManager::getTotalMemory() const {
    return totalMemory;
}

size_t MemoryManager::getUsedMemory() const {
    return usedMemory;
}

size_t MemoryManager::getFreeMemory() const {
    return freeMemory;
}

float MemoryManager::getFragmentation() const {
    return fragmentation;
}

// GC operations
size_t MemoryManager::runGarbageCollection() {
    std::lock_guard<std::mutex> lock(memoryMutex);
    
    // Find an enabled algorithm
    std::shared_ptr<GcAlgorithm> selectedAlgorithm = nullptr;
    for (const auto& algorithm : algorithms) {
        if (algorithm->isEnabled()) {
            selectedAlgorithm = algorithm;
            break;
        }
    }
    
    if (!selectedAlgorithm) {
        return 0;
    }
    
    // Record start time
    auto startTime = std::chrono::system_clock::now();
    
    // Run the algorithm
    size_t memoryReclaimed = selectedAlgorithm->collect(memoryBlocks);
    
    // Record end time
    auto endTime = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    
    // Update memory usage
    usedMemory -= memoryReclaimed;
    freeMemory += memoryReclaimed;
    
    // Update fragmentation
    updateFragmentation();
    
    // Update GC stats
    gcRunsToday++;
    lastGcRun = endTime;
    averageGcDuration = (averageGcDuration * (gcRunsToday - 1) + duration) / gcRunsToday;
    
    // Simulate CPU impact
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(5.0, 20.0);
    cpuImpact = static_cast<float>(dis(gen));
    
    // Create activity record
    int activityId = activities.size() + 1;
    int objectsCollected = static_cast<int>(memoryReclaimed / 1024); // Rough estimate
    auto activity = std::make_shared<GcActivity>(
        activityId, selectedAlgorithm->getId(), endTime, duration, memoryReclaimed, objectsCollected, cpuImpact);
    
    // Add activity
    {
        std::lock_guard<std::mutex> activitiesLock(activitiesMutex);
        activities.insert(activities.begin(), activity);
        
        // Keep only the last 1000 activities
        if (activities.size() > 1000) {
            activities.resize(1000);
        }
    }
    
    // Create memory record
    int recordId = memoryRecords.size() + 1;
    auto record = std::make_shared<MemoryRecord>(
        recordId, endTime, totalMemory, usedMemory, freeMemory, fragmentation);
    
    // Add memory record
    {
        std::lock_guard<std::mutex> recordsLock(recordsMutex);
        memoryRecords.insert(memoryRecords.begin(), record);
        
        // Keep only the last 1000 records
        if (memoryRecords.size() > 1000) {
            memoryRecords.resize(1000);
        }
    }
    
    return memoryReclaimed;
}

size_t MemoryManager::optimizeMemory() {
    std::lock_guard<std::mutex> lock(memoryMutex);
    
    // Simulate memory optimization
    size_t memoryReclaimed = 0;
    
    // Find fragmented blocks and consolidate them
    for (auto& block : memoryBlocks) {
        if (block->getStatus() == BlockStatus::FRAGMENTED) {
            memoryReclaimed += block->getSize();
            block->setStatus(BlockStatus::FREE);
        }
    }
    
    // Update memory usage
    usedMemory -= memoryReclaimed;
    freeMemory += memoryReclaimed;
    
    // Update fragmentation
    updateFragmentation();
    
    return memoryReclaimed;
}

size_t MemoryManager::defragmentMemory() {
    std::lock_guard<std::mutex> lock(memoryMutex);
    
    // Simulate memory defragmentation
    size_t memoryReclaimed = 0;
    
    // Consolidate free blocks
    for (auto& block : memoryBlocks) {
        if (block->getStatus() == BlockStatus::FREE) {
            // Find adjacent free blocks and merge them
            for (auto& otherBlock : memoryBlocks) {
                if (otherBlock != block && otherBlock->getStatus() == BlockStatus::FREE) {
                    block->setStatus(BlockStatus::ACTIVE); // Mark as active temporarily
                    otherBlock->setStatus(BlockStatus::ACTIVE); // Mark as active temporarily
                    memoryReclaimed += otherBlock->getSize();
                    otherBlock->setStatus(BlockStatus::FREE); // Mark as free again
                    block->setStatus(BlockStatus::FREE); // Mark as free again
                }
            }
        }
    }
    
    // Update memory usage
    usedMemory -= memoryReclaimed;
    freeMemory += memoryReclaimed;
    
    // Update fragmentation
    updateFragmentation();
    
    return memoryReclaimed;
}

// Algorithm operations
std::vector<std::shared_ptr<GcAlgorithm>> MemoryManager::getAllAlgorithms() const {
    return algorithms;
}

std::shared_ptr<GcAlgorithm> MemoryManager::getAlgorithm(int id) const {
    for (const auto& algorithm : algorithms) {
        if (algorithm->getId() == id) {
            return algorithm;
        }
    }
    return nullptr;
}

bool MemoryManager::updateAlgorithm(int id, bool enabled, int performanceScore) {
    for (auto& algorithm : algorithms) {
        if (algorithm->getId() == id) {
            algorithm->setEnabled(enabled);
            algorithm->setPerformanceScore(performanceScore);
            return true;
        }
    }
    return false;
}

// Activity operations
std::vector<std::shared_ptr<GcActivity>> MemoryManager::getRecentActivities(int limit) const {
    std::lock_guard<std::mutex> lock(activitiesMutex);
    
    std::vector<std::shared_ptr<GcActivity>> result;
    result.reserve(std::min(limit, static_cast<int>(activities.size())));
    
    for (size_t i = 0; i < std::min(static_cast<size_t>(limit), activities.size()); ++i) {
        result.push_back(activities[i]);
    }
    
    return result;
}

// Memory record operations
std::vector<std::shared_ptr<MemoryRecord>> MemoryManager::getRecentMemoryRecords(int limit) const {
    std::lock_guard<std::mutex> lock(recordsMutex);
    
    std::vector<std::shared_ptr<MemoryRecord>> result;
    result.reserve(std::min(limit, static_cast<int>(memoryRecords.size())));
    
    for (size_t i = 0; i < std::min(static_cast<size_t>(limit), memoryRecords.size()); ++i) {
        result.push_back(memoryRecords[i]);
    }
    
    return result;
}

// Memory block operations
std::vector<std::shared_ptr<MemoryBlock>> MemoryManager::getAllBlocks() const {
    std::lock_guard<std::mutex> lock(memoryMutex);
    return memoryBlocks;
}

// Settings operations
std::shared_ptr<GcSettings> MemoryManager::getSettings() const {
    return settings;
}

bool MemoryManager::updateSettings(const GcSettings& newSettings) {
    *settings = newSettings;
    return true;
}

// Stats operations
int MemoryManager::getGcRunsToday() const {
    return gcRunsToday;
}

std::chrono::system_clock::time_point MemoryManager::getLastGcRun() const {
    return lastGcRun;
}

int MemoryManager::getAverageGcDuration() const {
    return averageGcDuration;
}

float MemoryManager::getCpuImpact() const {
    return cpuImpact;
}

// WebSocket interface
void MemoryManager::startWebSocketServer(int port) {
    // In a real implementation, this would start a WebSocket server
    // For simulation, we'll just print a message
    std::cout << "WebSocket server started on port " << port << std::endl;
}

void MemoryManager::stopWebSocketServer() {
    // In a real implementation, this would stop the WebSocket server
    // For simulation, we'll just print a message
    std::cout << "WebSocket server stopped" << std::endl;
}

// Private methods
void MemoryManager::initializeMemory(size_t totalMemory) {
    this->totalMemory = totalMemory;
    this->usedMemory = static_cast<size_t>(totalMemory * 0.42); // 42% used initially
    this->freeMemory = totalMemory - this->usedMemory;
    
    // Create initial memory blocks
    memoryBlocks.clear();
    
    // Create some random blocks
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> sizeDis(1024, 1024 * 1024); // 1 KB to 1 MB
    std::uniform_real_distribution<> statusDis(0.0, 1.0);
    
    size_t remainingSize = totalMemory;
    int blockId = 1;
    
    while (remainingSize > 0) {
        size_t blockSize = std::min(static_cast<size_t>(sizeDis(gen)), remainingSize);
        
        BlockStatus status;
        if (statusDis(gen) < 0.3) {
            status = BlockStatus::FREE;
        } else if (statusDis(gen) < 0.8) {
            status = BlockStatus::ACTIVE;
        } else {
            status = BlockStatus::FRAGMENTED;
        }
        
        auto block = std::make_shared<MemoryBlock>(blockId++, blockSize, status);
        memoryBlocks.push_back(block);
        
        remainingSize -= blockSize;
    }
    
    // Update fragmentation
    updateFragmentation();
}

void MemoryManager::allocateMemory(size_t size) {
    // In a real implementation, this would allocate memory
    // For simulation, we'll just update the counters
    usedMemory += size;
    freeMemory -= size;
    
    // Update memory usage
    updateMemoryUsage();
}

void MemoryManager::freeMemory(size_t size) {
    // In a real implementation, this would free memory
    // For simulation, we'll just update the counters
    usedMemory -= size;
    freeMemory += size;
    
    // Update memory usage
    updateMemoryUsage();
}

void MemoryManager::updateMemoryUsage() {
    // Create memory record
    int recordId = memoryRecords.size() + 1;
    auto record = std::make_shared<MemoryRecord>(
        recordId, std::chrono::system_clock::now(), totalMemory, usedMemory, freeMemory, fragmentation);
    
    // Add memory record
    {
        std::lock_guard<std::mutex> recordsLock(recordsMutex);
        memoryRecords.insert(memoryRecords.begin(), record);
        
        // Keep only the last 1000 records
        if (memoryRecords.size() > 1000) {
            memoryRecords.resize(1000);
        }
    }
}

void MemoryManager::updateFragmentation() {
    // Calculate fragmentation
    size_t fragmentedSize = 0;
    
    for (const auto& block : memoryBlocks) {
        if (block->getStatus() == BlockStatus::FRAGMENTED) {
            fragmentedSize += block->getSize();
        }
    }
    
    fragmentation = static_cast<float>(fragmentedSize) / totalMemory * 100.0f;
}

void MemoryManager::initializeAlgorithms() {
    algorithms.clear();
    
    // Add default algorithms
    algorithms.push_back(std::make_shared<MarkSweepAlgorithm>(1));
    algorithms.push_back(std::make_shared<GenerationalAlgorithm>(2));
    algorithms.push_back(std::make_shared<ReferenceCountingAlgorithm>(3));
    algorithms.push_back(std::make_shared<ConcurrentGcAlgorithm>(4));
}

void MemoryManager::startBackgroundGc() {
    running = true;
    backgroundGcThreadObj = std::thread(&MemoryManager::backgroundGcThread, this);
}

void MemoryManager::stopBackgroundGc() {
    running = false;
    gcCondition.notify_one();
    
    if (backgroundGcThreadObj.joinable()) {
        backgroundGcThreadObj.join();
    }
}

void MemoryManager::backgroundGcThread() {
    while (running) {
        // Check if auto collection is enabled
        if (settings->isAutoCollection()) {
            // Check if memory usage is above threshold
            float memoryUsagePercent = static_cast<float>(usedMemory) / totalMemory * 100.0f;
            
            if (memoryUsagePercent > settings->getMemoryThreshold()) {
                // Run garbage collection
                runGarbageCollection();
            }
        }
        
        // Wait for the next check
        std::unique_lock<std::mutex> lock(memoryMutex);
        gcCondition.wait_for(lock, std::chrono::minutes(settings->getTimeInterval()));
    }
}

void MemoryManager::handleWebSocketMessage(const std::string& message) {
    // In a real implementation, this would handle WebSocket messages
    // For simulation, we'll just print the message
    std::cout << "Received WebSocket message: " << message << std::endl;
    
    // Parse JSON message
    Json::Value root;
    Json::Reader reader;
    
    if (reader.parse(message, root)) {
        std::string command = root["command"].asString();
        
        if (command == "runGc") {
            runGarbageCollection();
        } else if (command == "optimizeMemory") {
            optimizeMemory();
        } else if (command == "defragmentMemory") {
            defragmentMemory();
        } else if (command == "updateSettings") {
            // Parse settings
            bool autoCollection = root["settings"]["autoCollection"].asBool();
            int memoryThreshold = root["settings"]["memoryThreshold"].asInt();
            int timeInterval = root["settings"]["timeInterval"].asInt();
            bool backgroundCollection = root["settings"]["backgroundCollection"].asBool();
            int cpuLimit = root["settings"]["cpuLimit"].asInt();
            
            std::string priorityStr = root["settings"]["collectionPriority"].asString();
            CollectionPriority collectionPriority = CollectionPriority::BALANCED;
            
            if (priorityStr == "speed") {
                collectionPriority = CollectionPriority::SPEED;
            } else if (priorityStr == "memory") {
                collectionPriority = CollectionPriority::MEMORY;
            }
            
            // Update settings
            GcSettings newSettings(autoCollection, memoryThreshold, timeInterval,
                                  backgroundCollection, cpuLimit, collectionPriority);
            updateSettings(newSettings);
            
            // Restart background GC if needed
            if (backgroundCollection != settings->isBackgroundCollection()) {
                if (backgroundCollection) {
                    startBackgroundGc();
                } else {
                    stopBackgroundGc();
                }
            }
        }
    }
}

void MemoryManager::sendWebSocketMessage(const std::string& message) {
    // In a real implementation, this would send a WebSocket message
    // For simulation, we'll just print the message
    std::cout << "Sending WebSocket message: " << message << std::endl;
} 