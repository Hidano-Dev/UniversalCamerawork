#include "pch.h"
#include "ucapi_transport_factory.h"
#include "ucapi_local_transport.h"
#include "ucapi_udp_transport.h"

namespace ucapi {
namespace transport {

ITransportFactory& ITransportFactory::GetInstance() {
    static TransportFactory instance;
    return instance;
}

std::unique_ptr<ITransport> TransportFactory::CreateTransport(const std::string& type) {
    if (type == "Local") {
        return std::make_unique<LocalTransport>();
    } else if (type == "UDP") {
        return std::make_unique<UdpTransport>();
    } else if (type == "TCP") {
        return nullptr;
    }
    return nullptr;
}

} // namespace transport
} // namespace ucapi
