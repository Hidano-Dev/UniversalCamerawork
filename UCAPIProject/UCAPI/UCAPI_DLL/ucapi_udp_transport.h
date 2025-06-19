#ifndef UCAPI_UDP_TRANSPORT_H
#define UCAPI_UDP_TRANSPORT_H

#include "include/ucapi_transport.h"

namespace ucapi {
namespace transport {

class UdpTransport : public ITransport {
public:
    UdpTransport() = default;
    virtual ~UdpTransport() = default;

    HRESULT Connect(const std::string& endpoint) noexcept override;
    HRESULT Disconnect() noexcept override;
    HRESULT Send(const uint8_t* data, size_t size) noexcept override;
    HRESULT Receive(uint8_t* buffer, size_t bufferSize, size_t& outSize) noexcept override;

private:
    bool m_connected = false;
    std::string m_endpoint;
};

} // namespace transport
} // namespace ucapi

#endif // UCAPI_UDP_TRANSPORT_H
