#ifndef UCAPI_TRANSPORT_H
#define UCAPI_TRANSPORT_H

#include <windows.h>
#include <string>
#include <cstdint>
#include <cstddef>
#include <memory>

namespace ucapi {
namespace transport {

class ITransport {
public:
    virtual ~ITransport() = default;
    virtual HRESULT Connect(const std::string& endpoint) noexcept = 0;
    virtual HRESULT Disconnect() noexcept = 0;
    virtual HRESULT Send(const uint8_t* data, size_t size) noexcept = 0;
    virtual HRESULT Receive(uint8_t* buffer, size_t bufferSize, size_t& outSize) noexcept = 0;
};

class ITransportFactory {
public:
    virtual ~ITransportFactory() = default;
    virtual std::unique_ptr<ITransport> CreateTransport(const std::string& type) = 0;
    static ITransportFactory& GetInstance();
};

} // namespace transport
} // namespace ucapi

#endif // UCAPI_TRANSPORT_H
