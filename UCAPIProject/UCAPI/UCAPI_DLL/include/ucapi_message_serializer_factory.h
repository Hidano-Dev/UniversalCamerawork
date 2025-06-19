#pragma once
#include "ucapi_message_serializer.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

namespace ucapi {
    class IMessageSerializerFactory {
    public:
        virtual ~IMessageSerializerFactory() = default;

        virtual std::unique_ptr<IMessageSerializer> CreateSerializer(const std::string& formatName) noexcept = 0;

        virtual HRESULT RegisterSerializer(const std::string& formatName, 
            std::function<std::unique_ptr<IMessageSerializer>()> factory) noexcept = 0;

        virtual std::vector<std::string> GetSupportedFormats() const noexcept = 0;
    };
}
