#include "pch.h"
#include "ucapi_config.h"
#include "ucapi_message_serializer_factory_impl.h"
#include <stdexcept>

namespace ucapi {

std::unique_ptr<IMessageSerializerFactory> Config::s_factory = nullptr;
bool Config::s_initialized = false;

IMessageSerializerFactory& Config::GetSerializerFactory() {
    if (!s_initialized || !s_factory) {
        throw std::runtime_error("Config not initialized. Call Config::Initialize() first.");
    }
    return *s_factory;
}

void Config::Initialize() {
    if (!s_initialized) {
        s_factory = std::make_unique<MessageSerializerFactoryImpl>();
        s_initialized = true;
    }
}

void Config::Shutdown() {
    if (s_initialized) {
        s_factory.reset();
        s_initialized = false;
    }
}

}
