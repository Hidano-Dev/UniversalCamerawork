#pragma once
#include "include/ucapi_message_serializer_factory.h"
#include <memory>

namespace ucapi {
    class Config {
    public:
        static IMessageSerializerFactory& GetSerializerFactory();
        
        static void Initialize();
        static void Shutdown();
        
    private:
        static std::unique_ptr<IMessageSerializerFactory> s_factory;
        static bool s_initialized;
    };
}
