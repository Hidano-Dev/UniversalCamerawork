#pragma once
#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>
#include <memory>
#include <atomic>
#include <mutex>

namespace ucapi {

/// Log level enumeration for UCAPI logging system
enum class LogLevel : uint8_t {
    None    = 0,  ///< No logging
    Error   = 1,  ///< Error messages only (default)
    Warning = 2,  ///< Warnings and errors
    Info    = 3,  ///< Informational messages, warnings, and errors
    Debug   = 4   ///< All messages including debug
};

/// Single log entry structure
struct LogEntry {
    LogLevel level;           ///< Log level of this entry
    uint64_t timestamp;       ///< Timestamp in milliseconds since epoch
    std::string function;     ///< Function name where log was generated
    std::string message;      ///< Log message content

    LogEntry();
    LogEntry(LogLevel lvl, const char* func, const char* msg);
};

/// Maximum number of log entries to retain (circular buffer behavior)
constexpr size_t UCAPI_MAX_LOG_ENTRIES = 256;

/// Singleton logger class for UCAPI
class Logger {
public:
    /// Get the singleton instance (throws if not initialized)
    static Logger& GetInstance();

    /// Initialize the logger (called from DllMain)
    static void Initialize();

    /// Shutdown the logger (called from DllMain)
    static void Shutdown();

    /// Check if logger is initialized
    static bool IsInitialized();

    /// Log an error message
    void Error(const char* function, const char* message);

    /// Log a warning message
    void Warning(const char* function, const char* message);

    /// Log an info message
    void Info(const char* function, const char* message);

    /// Log a debug message
    void Debug(const char* function, const char* message);

    /// Generic log method
    void Log(LogLevel level, const char* function, const char* message);

    /// Get the current log level
    LogLevel GetLogLevel() const;

    /// Set the log level (messages below this level are ignored)
    void SetLogLevel(LogLevel level);

    /// Get the total number of log entries currently stored
    size_t GetLogCount() const;

    /// Get a log entry by index (0 = oldest)
    /// Returns false if index is out of range
    bool GetLogEntry(size_t index, LogEntry& outEntry) const;

    /// Clear all log entries
    void ClearLogs();

    // Public constructor/destructor for unique_ptr compatibility
    // Use Initialize()/GetInstance() for singleton access
    Logger();
    ~Logger();

    // Prevent copying and moving
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

private:
    void AddEntry(LogLevel level, const char* function, const char* message);
    static uint64_t GetTimestamp();

    // Static members for singleton pattern (matching Config class)
    static std::unique_ptr<Logger> s_instance;
    static std::atomic<bool> s_initialized;
    static std::once_flag s_init_flag;

    // Instance members
    mutable std::mutex m_mutex;               ///< Thread-safety mutex
    std::vector<LogEntry> m_entries;          ///< Log entry storage
    size_t m_writeIndex;                      ///< Circular buffer write index
    size_t m_count;                           ///< Current number of entries
    std::atomic<LogLevel> m_logLevel;         ///< Current log level filter
};

} // namespace ucapi

// Convenience macros for logging with automatic function name
#define UCAPI_LOG_ERROR(msg) \
    do { if (ucapi::Logger::IsInitialized()) ucapi::Logger::GetInstance().Error(__FUNCTION__, msg); } while(0)

#define UCAPI_LOG_WARNING(msg) \
    do { if (ucapi::Logger::IsInitialized()) ucapi::Logger::GetInstance().Warning(__FUNCTION__, msg); } while(0)

#define UCAPI_LOG_INFO(msg) \
    do { if (ucapi::Logger::IsInitialized()) ucapi::Logger::GetInstance().Info(__FUNCTION__, msg); } while(0)

#define UCAPI_LOG_DEBUG(msg) \
    do { if (ucapi::Logger::IsInitialized()) ucapi::Logger::GetInstance().Debug(__FUNCTION__, msg); } while(0)
