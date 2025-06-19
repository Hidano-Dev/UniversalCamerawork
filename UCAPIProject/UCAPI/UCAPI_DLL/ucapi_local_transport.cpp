#include "pch.h"
#include "ucapi_local_transport.h"

namespace ucapi {
namespace transport {

HRESULT LocalTransport::Connect(const std::string& endpoint) noexcept {
    if (endpoint.empty()) {
        return E_INVALIDARG;
    }
    
    if (m_connected) {
        return E_FAIL;
    }
    
    m_endpoint = endpoint;
    m_connected = true;
    return S_OK;
}

HRESULT LocalTransport::Disconnect() noexcept {
    if (!m_connected) {
        return E_FAIL;
    }
    
    m_connected = false;
    m_endpoint.clear();
    return S_OK;
}

HRESULT LocalTransport::Send(const uint8_t* data, size_t size) noexcept {
    if (!m_connected) {
        return E_FAIL;
    }
    
    if (data == nullptr || size == 0) {
        return E_INVALIDARG;
    }
    
    return S_OK;
}

HRESULT LocalTransport::Receive(uint8_t* buffer, size_t bufferSize, size_t& outSize) noexcept {
    if (!m_connected) {
        return E_FAIL;
    }
    
    if (buffer == nullptr || bufferSize == 0) {
        return E_INVALIDARG;
    }
    
    outSize = 0;
    return S_OK;
}

} // namespace transport
} // namespace ucapi
