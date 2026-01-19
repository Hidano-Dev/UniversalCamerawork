#include "pch.h"
#include "ucapi_logger.h"
#include <chrono>
#include <stdexcept>

namespace ucapi {

// Static member initialization
std::unique_ptr<Logger> Logger::s_instance = nullptr;
std::atomic<bool> Logger::s_initialized{false};
std::once_flag Logger::s_init_flag;

// LogEntry implementation
LogEntry::LogEntry()
    : level(LogLevel::None)
    , timestamp(0)
    , function()
    , message()
{
}

LogEntry::LogEntry(LogLevel lvl, const char* func, const char* msg)
    : level(lvl)
    , timestamp(0)
    , function(func ? func : "")
    , message(msg ? msg : "")
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    timestamp = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
}

// Logger implementation
Logger::Logger()
    : m_mutex()
    , m_entries()
    , m_writeIndex(0)
    , m_count(0)
    , m_logLevel(LogLevel::Error)
{
    m_entries.reserve(UCAPI_MAX_LOG_ENTRIES);
}

Logger::~Logger()
{
    // No special cleanup needed - vector handles memory
}

Logger& Logger::GetInstance()
{
    // memory_order_acquire ensures s_instance is visible
    if (!s_initialized.load(std::memory_order_acquire) || !s_instance) {
        throw std::runtime_error("Logger not initialized. Call Logger::Initialize() first.");
    }
    return *s_instance;
}

void Logger::Initialize()
{
    std::call_once(s_init_flag, []() {
        s_instance.reset(new Logger());
        // memory_order_release ensures s_instance write is visible before s_initialized
        s_initialized.store(true, std::memory_order_release);
    });
}

void Logger::Shutdown()
{
    // DLL_PROCESS_DETACH: other threads should not be accessing
    if (s_initialized.load(std::memory_order_acquire)) {
        s_instance.reset();
        s_initialized.store(false, std::memory_order_release);
    }
}

bool Logger::IsInitialized()
{
    return s_initialized.load(std::memory_order_acquire) && s_instance != nullptr;
}

void Logger::Error(const char* function, const char* message)
{
    Log(LogLevel::Error, function, message);
}

void Logger::Warning(const char* function, const char* message)
{
    Log(LogLevel::Warning, function, message);
}

void Logger::Info(const char* function, const char* message)
{
    Log(LogLevel::Info, function, message);
}

void Logger::Debug(const char* function, const char* message)
{
    Log(LogLevel::Debug, function, message);
}

void Logger::Log(LogLevel level, const char* function, const char* message)
{
    AddEntry(level, function, message);
}

LogLevel Logger::GetLogLevel() const
{
    return m_logLevel.load(std::memory_order_relaxed);
}

void Logger::SetLogLevel(LogLevel level)
{
    m_logLevel.store(level, std::memory_order_relaxed);
}

size_t Logger::GetLogCount() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_count;
}

bool Logger::GetLogEntry(size_t index, LogEntry& outEntry) const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (index >= m_count) {
        return false;
    }

    // Calculate actual index in circular buffer
    size_t actualIndex;
    if (m_count < UCAPI_MAX_LOG_ENTRIES) {
        actualIndex = index;
    } else {
        actualIndex = (m_writeIndex + index) % UCAPI_MAX_LOG_ENTRIES;
    }

    outEntry = m_entries[actualIndex];
    return true;
}

void Logger::ClearLogs()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_entries.clear();
    m_writeIndex = 0;
    m_count = 0;
}

void Logger::AddEntry(LogLevel level, const char* function, const char* message)
{
    // Early exit if level is below threshold (atomic read, no lock needed)
    LogLevel currentLevel = m_logLevel.load(std::memory_order_relaxed);
    if (level == LogLevel::None || static_cast<uint8_t>(level) > static_cast<uint8_t>(currentLevel)) {
        return;
    }

    std::lock_guard<std::mutex> lock(m_mutex);

    // Circular buffer implementation
    if (m_entries.size() < UCAPI_MAX_LOG_ENTRIES) {
        m_entries.emplace_back(level, function, message);
        m_count = m_entries.size();
    } else {
        // Overwrite oldest entry
        m_entries[m_writeIndex] = LogEntry(level, function, message);
        m_writeIndex = (m_writeIndex + 1) % UCAPI_MAX_LOG_ENTRIES;
        m_count = UCAPI_MAX_LOG_ENTRIES;
    }
}


} // namespace ucapi
