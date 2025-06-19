#include "pch.h"
#include "ucapi_message_serializer_factory_impl.h"
#include "ucapi_msgpack_serializer.h"
#include <algorithm>

namespace ucapi {

MessageSerializerFactoryImpl::MessageSerializerFactoryImpl() {
    RegisterSerializer("MsgPack", []() {
        return std::make_unique<MessagePackSerializer>();
    });
}

std::unique_ptr<IMessageSerializer> MessageSerializerFactoryImpl::CreateSerializer(const std::string& formatName) noexcept {
    try {
        auto it = m_factories.find(formatName);
        if (it != m_factories.end()) {
            return it->second();
        }
        return nullptr;
    }
    catch (...) {
        return nullptr;
    }
}

HRESULT MessageSerializerFactoryImpl::RegisterSerializer(const std::string& formatName, 
    std::function<std::unique_ptr<IMessageSerializer>()> factory) noexcept {
    try {
        m_factories[formatName] = factory;
        return S_OK;
    }
    catch (...) {
        return E_FAIL;
    }
}

std::vector<std::string> MessageSerializerFactoryImpl::GetSupportedFormats() const noexcept {
    try {
        std::vector<std::string> formats;
        formats.reserve(m_factories.size());
        
        for (const auto& pair : m_factories) {
            formats.push_back(pair.first);
        }
        
        std::sort(formats.begin(), formats.end());
        return formats;
    }
    catch (...) {
        return std::vector<std::string>();
    }
}

}
