#include "pch.h"
#include "ucapi_config.h"
#include "ucapi_message_serializer_factory_impl.h"
#include <stdexcept>

namespace ucapi {

std::unique_ptr<IMessageSerializerFactory> Config::s_factory = nullptr;
std::atomic<bool> Config::s_initialized{false};
std::once_flag Config::s_init_flag;

IMessageSerializerFactory& Config::GetSerializerFactory() {
    // Use memory_order_acquire to ensure s_factory is visible after s_initialized is true
    if (!s_initialized.load(std::memory_order_acquire) || !s_factory) {
        throw std::runtime_error("Config not initialized. Call Config::Initialize() first.");
    }
    return *s_factory;
}

void Config::Initialize() {
    // Thread-safe initialization using std::call_once
    std::call_once(s_init_flag, []() {
        s_factory = std::make_unique<MessageSerializerFactoryImpl>();
        // Use memory_order_release to ensure s_factory write is visible before s_initialized
        s_initialized.store(true, std::memory_order_release);
    });
}

void Config::Shutdown() {
    // Shutdown is expected to be called only from DllMain DLL_PROCESS_DETACH
    // where no other threads should be accessing Config
    if (s_initialized.load(std::memory_order_acquire)) {
        s_factory.reset();
        s_initialized.store(false, std::memory_order_release);
    }
}

}
