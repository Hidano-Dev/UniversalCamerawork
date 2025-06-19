#pragma once
#include "include/ucapi_message_serializer_factory.h"
#include <unordered_map>

namespace ucapi {
    class MessageSerializerFactoryImpl : public IMessageSerializerFactory {
    public:
        MessageSerializerFactoryImpl();
        virtual ~MessageSerializerFactoryImpl() = default;
        
        virtual std::unique_ptr<IMessageSerializer> CreateSerializer(const std::string& formatName) noexcept override;
        
        virtual HRESULT RegisterSerializer(const std::string& formatName, 
            std::function<std::unique_ptr<IMessageSerializer>()> factory) noexcept override;
        
        virtual std::vector<std::string> GetSupportedFormats() const noexcept override;
        
    private:
        std::unordered_map<std::string, std::function<std::unique_ptr<IMessageSerializer>()>> m_factories;
    };
}
