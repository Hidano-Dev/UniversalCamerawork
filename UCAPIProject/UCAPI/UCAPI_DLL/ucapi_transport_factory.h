#ifndef UCAPI_TRANSPORT_FACTORY_H
#define UCAPI_TRANSPORT_FACTORY_H

#include "include/ucapi_transport.h"

namespace ucapi {
namespace transport {

class TransportFactory : public ITransportFactory {
public:
    std::unique_ptr<ITransport> CreateTransport(const std::string& type) override;
    
private:
    TransportFactory() = default;
    friend ITransportFactory& ITransportFactory::GetInstance();
};

} // namespace transport
} // namespace ucapi

#endif // UCAPI_TRANSPORT_FACTORY_H
