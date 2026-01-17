#pragma once
#include "include/ucapi_message_serializer_factory.h"
#include <memory>
#include <atomic>
#include <mutex>

namespace ucapi {
    class Config {
    public:
        static IMessageSerializerFactory& GetSerializerFactory();

        static void Initialize();
        static void Shutdown();

    private:
        static std::unique_ptr<IMessageSerializerFactory> s_factory;
        static std::atomic<bool> s_initialized;
        static std::once_flag s_init_flag;
    };
}
