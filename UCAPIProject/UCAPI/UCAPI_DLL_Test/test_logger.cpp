#include "pch.h"
#include "ucapi_dll.h"
#include "ucapi_logger.h"
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <cstring>

// =============================================================================
// Logger Core Functionality Tests
// =============================================================================

// Test: Logger initialization and IsInitialized
TEST(Logger, InitializationAndStatus) {
    // Logger should be initialized by DllMain
    EXPECT_TRUE(ucapi::Logger::IsInitialized());
    
    // Should be able to get instance after initialization
    EXPECT_NO_THROW({
        ucapi::Logger& logger = ucapi::Logger::GetInstance();
        (void)logger;  // Suppress unused variable warning
    });
}

// Test: Logger singleton behavior - same instance returned
TEST(Logger, SingletonBehavior) {
    ucapi::Logger& instance1 = ucapi::Logger::GetInstance();
    ucapi::Logger& instance2 = ucapi::Logger::GetInstance();
    
    // Both references should point to the same instance
    EXPECT_EQ(&instance1, &instance2);
}

// Test: Basic log entry addition
TEST(Logger, BasicLogEntry) {
    ucapi::Logger& logger = ucapi::Logger::GetInstance();
    logger.ClearLogs();
    
    logger.Error("TestFunction", "Test error message");
    
    EXPECT_EQ(logger.GetLogCount(), 1);
    
    ucapi::LogEntry entry;
    EXPECT_TRUE(logger.GetLogEntry(0, entry));
    EXPECT_EQ(entry.level, ucapi::LogLevel::Error);
    EXPECT_EQ(entry.function, "TestFunction");
    EXPECT_EQ(entry.message, "Test error message");
}

// Test: Multiple log levels
TEST(Logger, MultipleLevels) {
    ucapi::Logger& logger = ucapi::Logger::GetInstance();
    logger.ClearLogs();
    logger.SetLogLevel(ucapi::LogLevel::Debug);  // Enable all levels
    
    logger.Error("Func1", "Error msg");
    logger.Warning("Func2", "Warning msg");
    logger.Info("Func3", "Info msg");
    logger.Debug("Func4", "Debug msg");
    
    EXPECT_EQ(logger.GetLogCount(), 4);
    
    ucapi::LogEntry entry;
    EXPECT_TRUE(logger.GetLogEntry(0, entry));
    EXPECT_EQ(entry.level, ucapi::LogLevel::Error);
    
    EXPECT_TRUE(logger.GetLogEntry(1, entry));
    EXPECT_EQ(entry.level, ucapi::LogLevel::Warning);
    
    EXPECT_TRUE(logger.GetLogEntry(2, entry));
    EXPECT_EQ(entry.level, ucapi::LogLevel::Info);
    
    EXPECT_TRUE(logger.GetLogEntry(3, entry));
    EXPECT_EQ(entry.level, ucapi::LogLevel::Debug);
}

// Test: Log level filtering
TEST(Logger, LogLevelFiltering) {
    ucapi::Logger& logger = ucapi::Logger::GetInstance();
    logger.ClearLogs();
    
    // Set to Error level - should only log errors
    logger.SetLogLevel(ucapi::LogLevel::Error);
    
    logger.Error("Func1", "Error msg");
    logger.Warning("Func2", "Warning msg");  // Should be filtered out
    logger.Info("Func3", "Info msg");        // Should be filtered out
    logger.Debug("Func4", "Debug msg");      // Should be filtered out
    
    EXPECT_EQ(logger.GetLogCount(), 1);
    
    // Clear and test Warning level
    logger.ClearLogs();
    logger.SetLogLevel(ucapi::LogLevel::Warning);
    
    logger.Error("Func1", "Error msg");
    logger.Warning("Func2", "Warning msg");
    logger.Info("Func3", "Info msg");    // Should be filtered out
    logger.Debug("Func4", "Debug msg");  // Should be filtered out
    
    EXPECT_EQ(logger.GetLogCount(), 2);
}

// Test: Get/Set log level
TEST(Logger, GetSetLogLevel) {
    ucapi::Logger& logger = ucapi::Logger::GetInstance();
    
    logger.SetLogLevel(ucapi::LogLevel::Error);
    EXPECT_EQ(logger.GetLogLevel(), ucapi::LogLevel::Error);
    
    logger.SetLogLevel(ucapi::LogLevel::Warning);
    EXPECT_EQ(logger.GetLogLevel(), ucapi::LogLevel::Warning);
    
    logger.SetLogLevel(ucapi::LogLevel::Info);
    EXPECT_EQ(logger.GetLogLevel(), ucapi::LogLevel::Info);
    
    logger.SetLogLevel(ucapi::LogLevel::Debug);
    EXPECT_EQ(logger.GetLogLevel(), ucapi::LogLevel::Debug);
}

// Test: Clear logs
TEST(Logger, ClearLogs) {
    ucapi::Logger& logger = ucapi::Logger::GetInstance();
    logger.ClearLogs();
    logger.SetLogLevel(ucapi::LogLevel::Debug);
    
    logger.Error("Func1", "Msg1");
    logger.Warning("Func2", "Msg2");
    logger.Info("Func3", "Msg3");
    
    EXPECT_EQ(logger.GetLogCount(), 3);
    
    logger.ClearLogs();
    EXPECT_EQ(logger.GetLogCount(), 0);
}

// Test: GetLogEntry with invalid index
TEST(Logger, GetLogEntryInvalidIndex) {
    ucapi::Logger& logger = ucapi::Logger::GetInstance();
    logger.ClearLogs();
    
    logger.Error("Func1", "Msg1");
    
    ucapi::LogEntry entry;
    EXPECT_TRUE(logger.GetLogEntry(0, entry));
    EXPECT_FALSE(logger.GetLogEntry(1, entry));  // Out of range
    EXPECT_FALSE(logger.GetLogEntry(100, entry));  // Way out of range
}

// Test: Timestamp is set correctly
TEST(Logger, TimestampIsSet) {
    ucapi::Logger& logger = ucapi::Logger::GetInstance();
    logger.ClearLogs();
    
    auto before = std::chrono::system_clock::now();
    logger.Error("TestFunc", "Test message");
    auto after = std::chrono::system_clock::now();
    
    ucapi::LogEntry entry;
    EXPECT_TRUE(logger.GetLogEntry(0, entry));
    
    auto beforeMs = std::chrono::duration_cast<std::chrono::milliseconds>(before.time_since_epoch()).count();
    auto afterMs = std::chrono::duration_cast<std::chrono::milliseconds>(after.time_since_epoch()).count();
    
    EXPECT_GE(entry.timestamp, static_cast<uint64_t>(beforeMs));
    EXPECT_LE(entry.timestamp, static_cast<uint64_t>(afterMs));
}

// =============================================================================
// Circular Buffer Tests
// =============================================================================

// Test: Circular buffer wraparound behavior
TEST(Logger, CircularBufferWraparound) {
    ucapi::Logger& logger = ucapi::Logger::GetInstance();
    logger.ClearLogs();
    logger.SetLogLevel(ucapi::LogLevel::Debug);
    
    // Fill the circular buffer (256 entries)
    for (size_t i = 0; i < ucapi::UCAPI_MAX_LOG_ENTRIES; ++i) {
        std::string msg = "Message " + std::to_string(i);
        logger.Info("TestFunc", msg.c_str());
    }
    
    EXPECT_EQ(logger.GetLogCount(), ucapi::UCAPI_MAX_LOG_ENTRIES);
    
    // Verify first entry
    ucapi::LogEntry entry;
    EXPECT_TRUE(logger.GetLogEntry(0, entry));
    EXPECT_EQ(entry.message, "Message 0");
    
    // Add one more entry - should overwrite the oldest
    logger.Info("TestFunc", "Message 256");
    
    EXPECT_EQ(logger.GetLogCount(), ucapi::UCAPI_MAX_LOG_ENTRIES);
    
    // First entry should now be "Message 1" (oldest was overwritten)
    EXPECT_TRUE(logger.GetLogEntry(0, entry));
    EXPECT_EQ(entry.message, "Message 1");
    
    // Last entry should be "Message 256"
    EXPECT_TRUE(logger.GetLogEntry(ucapi::UCAPI_MAX_LOG_ENTRIES - 1, entry));
    EXPECT_EQ(entry.message, "Message 256");
}

// Test: Circular buffer after multiple wraparounds
TEST(Logger, CircularBufferMultipleWraparounds) {
    ucapi::Logger& logger = ucapi::Logger::GetInstance();
    logger.ClearLogs();
    logger.SetLogLevel(ucapi::LogLevel::Debug);
    
    // Add 2.5x buffer size entries to test multiple wraparounds
    // This ensures the circular buffer logic works correctly beyond a single wraparound
    constexpr size_t totalEntries = ucapi::UCAPI_MAX_LOG_ENTRIES * 2 + ucapi::UCAPI_MAX_LOG_ENTRIES / 2;
    for (size_t i = 0; i < totalEntries; ++i) {
        std::string msg = "Entry " + std::to_string(i);
        logger.Info("TestFunc", msg.c_str());
    }
    
    EXPECT_EQ(logger.GetLogCount(), ucapi::UCAPI_MAX_LOG_ENTRIES);
    
    // First entry should be from the last 256 entries
    size_t expectedFirstIndex = totalEntries - ucapi::UCAPI_MAX_LOG_ENTRIES;
    ucapi::LogEntry entry;
    EXPECT_TRUE(logger.GetLogEntry(0, entry));
    EXPECT_EQ(entry.message, "Entry " + std::to_string(expectedFirstIndex));
    
    // Last entry should be the most recent
    EXPECT_TRUE(logger.GetLogEntry(ucapi::UCAPI_MAX_LOG_ENTRIES - 1, entry));
    EXPECT_EQ(entry.message, "Entry " + std::to_string(totalEntries - 1));
}

// =============================================================================
// Thread Safety Tests
// =============================================================================

// Test: Concurrent logging from multiple threads
TEST(Logger, ConcurrentLogging) {
    ucapi::Logger& logger = ucapi::Logger::GetInstance();
    logger.ClearLogs();
    logger.SetLogLevel(ucapi::LogLevel::Debug);
    
    // Use 8 threads with 50 logs each to stress-test thread safety
    // Total: 400 logs, which exceeds circular buffer size (256) to also test wraparound under concurrent access
    constexpr int NUM_THREADS = 8;
    constexpr int LOGS_PER_THREAD = 50;
    std::atomic<int> successCount{0};
    
    auto worker = [&](int threadId) {
        for (int i = 0; i < LOGS_PER_THREAD; ++i) {
            std::string msg = "Thread" + std::to_string(threadId) + "_Msg" + std::to_string(i);
            logger.Info("ConcurrentTest", msg.c_str());
            ++successCount;
        }
    };
    
    std::vector<std::thread> threads;
    threads.reserve(NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(worker, i);
    }
    for (auto& t : threads) {
        t.join();
    }
    
    EXPECT_EQ(successCount.load(), NUM_THREADS * LOGS_PER_THREAD);
    
    // Should have logged all entries (or MAX if exceeded)
    size_t expectedCount = std::min(static_cast<size_t>(NUM_THREADS * LOGS_PER_THREAD), ucapi::UCAPI_MAX_LOG_ENTRIES);
    EXPECT_EQ(logger.GetLogCount(), expectedCount);
}

// Test: Concurrent GetLogEntry and AddEntry
TEST(Logger, ConcurrentReadWrite) {
    ucapi::Logger& logger = ucapi::Logger::GetInstance();
    logger.ClearLogs();
    logger.SetLogLevel(ucapi::LogLevel::Debug);
    
    // Add some initial entries
    for (int i = 0; i < 50; ++i) {
        logger.Info("InitFunc", ("Init " + std::to_string(i)).c_str());
    }
    
    std::atomic<bool> stopFlag{false};
    std::atomic<int> writeCount{0};
    std::atomic<int> readCount{0};
    
    // Writer threads
    auto writer = [&]() {
        while (!stopFlag.load()) {
            logger.Info("Writer", "Writing...");
            ++writeCount;
            std::this_thread::yield();  // Cooperative multitasking
        }
    };
    
    // Reader threads
    auto reader = [&]() {
        while (!stopFlag.load()) {
            size_t count = logger.GetLogCount();
            if (count > 0) {
                ucapi::LogEntry entry;
                logger.GetLogEntry(0, entry);
                ++readCount;
            }
            std::this_thread::yield();  // Cooperative multitasking
        }
    };
    
    std::vector<std::thread> threads;
    threads.emplace_back(writer);
    threads.emplace_back(writer);
    threads.emplace_back(reader);
    threads.emplace_back(reader);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    stopFlag.store(true);
    
    for (auto& t : threads) {
        t.join();
    }
    
    // Should have performed some reads and writes without crashing
    EXPECT_GT(writeCount.load(), 0);
    EXPECT_GT(readCount.load(), 0);
}

// Test: Concurrent ClearLogs and AddEntry
TEST(Logger, ConcurrentClearAndAdd) {
    ucapi::Logger& logger = ucapi::Logger::GetInstance();
    logger.SetLogLevel(ucapi::LogLevel::Debug);
    
    std::atomic<bool> stopFlag{false};
    std::atomic<int> clearCount{0};
    std::atomic<int> addCount{0};
    
    auto clearer = [&]() {
        while (!stopFlag.load()) {
            logger.ClearLogs();
            ++clearCount;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    };
    
    auto adder = [&]() {
        while (!stopFlag.load()) {
            logger.Info("Adder", "Adding entry");
            ++addCount;
            std::this_thread::yield();  // Cooperative multitasking
        }
    };
    
    std::vector<std::thread> threads;
    threads.emplace_back(clearer);
    threads.emplace_back(adder);
    threads.emplace_back(adder);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    stopFlag.store(true);
    
    for (auto& t : threads) {
        t.join();
    }
    
    // Should complete without crashing
    EXPECT_GT(clearCount.load(), 0);
    EXPECT_GT(addCount.load(), 0);
}

// =============================================================================
// DLL API Tests
// =============================================================================

// Test: UCAPI_GetLogCount
TEST(LoggerDLL, GetLogCount) {
    UCAPI_ClearLogs();
    UCAPI_SetLogLevel(4);  // Debug level
    
    EXPECT_EQ(UCAPI_GetLogCount(), 0);
    
    ucapi::Logger::GetInstance().Error("TestFunc", "Msg1");
    EXPECT_EQ(UCAPI_GetLogCount(), 1);
    
    ucapi::Logger::GetInstance().Warning("TestFunc", "Msg2");
    EXPECT_EQ(UCAPI_GetLogCount(), 2);
}

// Test: UCAPI_GetLog with valid parameters
TEST(LoggerDLL, GetLogValid) {
    UCAPI_ClearLogs();
    UCAPI_SetLogLevel(4);
    
    ucapi::Logger::GetInstance().Error("MyFunction", "My error message");
    
    uint8_t level = 0;
    uint64_t timestamp = 0;
    char function[256] = {0};
    char message[256] = {0};
    
    int result = UCAPI_GetLog(0, &level, &timestamp, function, sizeof(function), message, sizeof(message));
    
    EXPECT_EQ(result, 0);
    EXPECT_EQ(level, 1);  // Error level
    EXPECT_GT(timestamp, 0u);
    EXPECT_STREQ(function, "MyFunction");
    EXPECT_STREQ(message, "My error message");
}

// Test: UCAPI_GetLog with out of range index
TEST(LoggerDLL, GetLogOutOfRange) {
    UCAPI_ClearLogs();
    UCAPI_SetLogLevel(4);
    
    ucapi::Logger::GetInstance().Error("TestFunc", "Test message");
    
    uint8_t level = 0;
    uint64_t timestamp = 0;
    char function[256] = {0};
    char message[256] = {0};
    
    int result = UCAPI_GetLog(10, &level, &timestamp, function, sizeof(function), message, sizeof(message));
    
    EXPECT_EQ(result, -1);  // Index out of range
}

// Test: UCAPI_GetLog with buffer too small
TEST(LoggerDLL, GetLogBufferTooSmall) {
    UCAPI_ClearLogs();
    UCAPI_SetLogLevel(4);
    
    ucapi::Logger::GetInstance().Error("TestFunc", "This is a long message that will not fit");
    
    uint8_t level = 0;
    uint64_t timestamp = 0;
    char function[256] = {0};
    char message[10] = {0};  // Too small buffer
    
    int result = UCAPI_GetLog(0, &level, &timestamp, function, sizeof(function), message, sizeof(message));
    
    EXPECT_EQ(result, -2);  // Buffer too small
}

// Test: UCAPI_GetLog with null pointers (should work for optional params)
TEST(LoggerDLL, GetLogNullPointers) {
    UCAPI_ClearLogs();
    UCAPI_SetLogLevel(4);
    
    ucapi::Logger::GetInstance().Error("TestFunc", "Test message");
    
    char function[256] = {0};
    char message[256] = {0};
    
    // Should succeed with null level and timestamp
    int result = UCAPI_GetLog(0, nullptr, nullptr, function, sizeof(function), message, sizeof(message));
    
    EXPECT_EQ(result, 0);
    EXPECT_STREQ(function, "TestFunc");
}

// Test: UCAPI_GetLogMessage
TEST(LoggerDLL, GetLogMessage) {
    UCAPI_ClearLogs();
    UCAPI_SetLogLevel(4);
    
    ucapi::Logger::GetInstance().Error("MyFunc", "Error occurred");
    ucapi::Logger::GetInstance().Warning("MyFunc", "Warning issued");
    
    char buffer[512] = {0};
    
    int result1 = UCAPI_GetLogMessage(0, buffer, sizeof(buffer));
    EXPECT_EQ(result1, 0);
    EXPECT_TRUE(std::strstr(buffer, "[ERROR]") != nullptr);
    EXPECT_TRUE(std::strstr(buffer, "[MyFunc]") != nullptr);
    EXPECT_TRUE(std::strstr(buffer, "Error occurred") != nullptr);
    
    int result2 = UCAPI_GetLogMessage(1, buffer, sizeof(buffer));
    EXPECT_EQ(result2, 0);
    EXPECT_TRUE(std::strstr(buffer, "[WARNING]") != nullptr);
}

// Test: UCAPI_GetLogMessage with invalid index
TEST(LoggerDLL, GetLogMessageInvalidIndex) {
    UCAPI_ClearLogs();
    
    char buffer[256] = {0};
    int result = UCAPI_GetLogMessage(0, buffer, sizeof(buffer));
    
    EXPECT_EQ(result, -1);  // No logs available
}

// Test: UCAPI_GetLogMessage with null buffer
TEST(LoggerDLL, GetLogMessageNullBuffer) {
    UCAPI_ClearLogs();
    UCAPI_SetLogLevel(4);
    
    ucapi::Logger::GetInstance().Error("TestFunc", "Test");
    
    int result = UCAPI_GetLogMessage(0, nullptr, 256);
    
    EXPECT_EQ(result, -1);
}

// Test: UCAPI_GetLogMessage with buffer too small
TEST(LoggerDLL, GetLogMessageBufferTooSmall) {
    UCAPI_ClearLogs();
    UCAPI_SetLogLevel(4);
    
    ucapi::Logger::GetInstance().Error("TestFunc", "This is a very long message");
    
    char buffer[10] = {0};
    int result = UCAPI_GetLogMessage(0, buffer, sizeof(buffer));
    
    EXPECT_EQ(result, -2);  // Buffer too small
}

// Test: UCAPI_ClearLogs
TEST(LoggerDLL, ClearLogs) {
    UCAPI_SetLogLevel(4);
    
    ucapi::Logger::GetInstance().Error("Func1", "Msg1");
    ucapi::Logger::GetInstance().Warning("Func2", "Msg2");
    
    EXPECT_GT(UCAPI_GetLogCount(), 0u);
    
    UCAPI_ClearLogs();
    
    EXPECT_EQ(UCAPI_GetLogCount(), 0u);
}

// Test: UCAPI_SetLogLevel and UCAPI_GetLogLevel
TEST(LoggerDLL, SetGetLogLevel) {
    UCAPI_SetLogLevel(1);  // Error
    EXPECT_EQ(UCAPI_GetLogLevel(), 1);
    
    UCAPI_SetLogLevel(2);  // Warning
    EXPECT_EQ(UCAPI_GetLogLevel(), 2);
    
    UCAPI_SetLogLevel(3);  // Info
    EXPECT_EQ(UCAPI_GetLogLevel(), 3);
    
    UCAPI_SetLogLevel(4);  // Debug
    EXPECT_EQ(UCAPI_GetLogLevel(), 4);
}

// Test: UCAPI_SetLogLevel with invalid value (should be ignored)
TEST(LoggerDLL, SetLogLevelInvalid) {
    UCAPI_SetLogLevel(3);  // Set to valid value first
    uint8_t before = UCAPI_GetLogLevel();
    
    UCAPI_SetLogLevel(99);  // Invalid value
    
    // Should not change (or default to something safe)
    uint8_t after = UCAPI_GetLogLevel();
    // The implementation ignores invalid values, so it should remain unchanged
    EXPECT_EQ(after, before);
}

// =============================================================================
// Edge Cases and Error Handling
// =============================================================================

// Test: Empty function and message strings
TEST(Logger, EmptyStrings) {
    ucapi::Logger& logger = ucapi::Logger::GetInstance();
    logger.ClearLogs();
    
    logger.Error("", "");
    
    EXPECT_EQ(logger.GetLogCount(), 1);
    
    ucapi::LogEntry entry;
    EXPECT_TRUE(logger.GetLogEntry(0, entry));
    EXPECT_EQ(entry.function, "");
    EXPECT_EQ(entry.message, "");
}

// Test: Null pointers for function and message
TEST(Logger, NullPointers) {
    ucapi::Logger& logger = ucapi::Logger::GetInstance();
    logger.ClearLogs();
    
    // Should handle null pointers gracefully
    logger.Error(nullptr, nullptr);
    
    EXPECT_EQ(logger.GetLogCount(), 1);
    
    ucapi::LogEntry entry;
    EXPECT_TRUE(logger.GetLogEntry(0, entry));
    EXPECT_EQ(entry.function, "");
    EXPECT_EQ(entry.message, "");
}

// Test: Very long strings
TEST(Logger, VeryLongStrings) {
    ucapi::Logger& logger = ucapi::Logger::GetInstance();
    logger.ClearLogs();
    
    // Test with realistically large strings (1000 chars for function, 2000 for message)
    // This validates the logger handles large strings without truncation or crashes
    std::string longFunction(1000, 'F');
    std::string longMessage(2000, 'M');
    
    logger.Error(longFunction.c_str(), longMessage.c_str());
    
    EXPECT_EQ(logger.GetLogCount(), 1);
    
    ucapi::LogEntry entry;
    EXPECT_TRUE(logger.GetLogEntry(0, entry));
    EXPECT_EQ(entry.function, longFunction);
    EXPECT_EQ(entry.message, longMessage);
}

// Test: LogLevel::None filtering
TEST(Logger, LogLevelNone) {
    ucapi::Logger& logger = ucapi::Logger::GetInstance();
    logger.ClearLogs();
    
    // Set to None - should not log anything
    logger.SetLogLevel(ucapi::LogLevel::None);
    
    logger.Error("Func1", "Msg1");
    logger.Warning("Func2", "Msg2");
    logger.Info("Func3", "Msg3");
    logger.Debug("Func4", "Msg4");
    
    EXPECT_EQ(logger.GetLogCount(), 0);
}

// Test: Logging with LogLevel::None directly
TEST(Logger, LogWithNoneLevel) {
    ucapi::Logger& logger = ucapi::Logger::GetInstance();
    logger.ClearLogs();
    logger.SetLogLevel(ucapi::LogLevel::Debug);
    
    // Try to log with None level directly
    logger.Log(ucapi::LogLevel::None, "TestFunc", "This should not be logged");
    
    EXPECT_EQ(logger.GetLogCount(), 0);
}

// Test: Macro usage
TEST(Logger, MacroUsage) {
    ucapi::Logger& logger = ucapi::Logger::GetInstance();
    logger.ClearLogs();
    logger.SetLogLevel(ucapi::LogLevel::Debug);
    
    UCAPI_LOG_ERROR("Error via macro");
    UCAPI_LOG_WARNING("Warning via macro");
    UCAPI_LOG_INFO("Info via macro");
    UCAPI_LOG_DEBUG("Debug via macro");
    
    EXPECT_EQ(logger.GetLogCount(), 4);
    
    ucapi::LogEntry entry;
    EXPECT_TRUE(logger.GetLogEntry(0, entry));
    EXPECT_EQ(entry.message, "Error via macro");
}
