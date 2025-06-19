#include "pch.h"
#include "include/ucapi_transport.h"
#include "ucapi_transport_factory.h"

using namespace ucapi::transport;

TEST(TransportFactory, Singleton) {
    ITransportFactory& factory1 = ITransportFactory::GetInstance();
    ITransportFactory& factory2 = ITransportFactory::GetInstance();
    EXPECT_EQ(&factory1, &factory2);
}

TEST(TransportFactory, CreateLocalTransport) {
    ITransportFactory& factory = ITransportFactory::GetInstance();
    auto transport = factory.CreateTransport("Local");
    EXPECT_NE(transport, nullptr);
}

TEST(TransportFactory, CreateUdpTransport) {
    ITransportFactory& factory = ITransportFactory::GetInstance();
    auto transport = factory.CreateTransport("UDP");
    EXPECT_NE(transport, nullptr);
}

TEST(TransportFactory, CreateTcpTransport) {
    ITransportFactory& factory = ITransportFactory::GetInstance();
    auto transport = factory.CreateTransport("TCP");
    EXPECT_EQ(transport, nullptr);
}

TEST(TransportFactory, CreateInvalidTransport) {
    ITransportFactory& factory = ITransportFactory::GetInstance();
    auto transport = factory.CreateTransport("Invalid");
    EXPECT_EQ(transport, nullptr);
}

TEST(LocalTransport, ConnectValidEndpoint) {
    ITransportFactory& factory = ITransportFactory::GetInstance();
    auto transport = factory.CreateTransport("Local");
    ASSERT_NE(transport, nullptr);
    
    HRESULT result = transport->Connect("localhost:8080");
    EXPECT_EQ(result, S_OK);
}

TEST(LocalTransport, ConnectEmptyEndpoint) {
    ITransportFactory& factory = ITransportFactory::GetInstance();
    auto transport = factory.CreateTransport("Local");
    ASSERT_NE(transport, nullptr);
    
    HRESULT result = transport->Connect("");
    EXPECT_EQ(result, E_INVALIDARG);
}

TEST(LocalTransport, ConnectTwice) {
    ITransportFactory& factory = ITransportFactory::GetInstance();
    auto transport = factory.CreateTransport("Local");
    ASSERT_NE(transport, nullptr);
    
    HRESULT result1 = transport->Connect("localhost:8080");
    EXPECT_EQ(result1, S_OK);
    
    HRESULT result2 = transport->Connect("localhost:8081");
    EXPECT_EQ(result2, E_FAIL);
}

TEST(LocalTransport, DisconnectAfterConnect) {
    ITransportFactory& factory = ITransportFactory::GetInstance();
    auto transport = factory.CreateTransport("Local");
    ASSERT_NE(transport, nullptr);
    
    HRESULT connectResult = transport->Connect("localhost:8080");
    EXPECT_EQ(connectResult, S_OK);
    
    HRESULT disconnectResult = transport->Disconnect();
    EXPECT_EQ(disconnectResult, S_OK);
}

TEST(LocalTransport, DisconnectWithoutConnect) {
    ITransportFactory& factory = ITransportFactory::GetInstance();
    auto transport = factory.CreateTransport("Local");
    ASSERT_NE(transport, nullptr);
    
    HRESULT result = transport->Disconnect();
    EXPECT_EQ(result, E_FAIL);
}

TEST(LocalTransport, SendValidData) {
    ITransportFactory& factory = ITransportFactory::GetInstance();
    auto transport = factory.CreateTransport("Local");
    ASSERT_NE(transport, nullptr);
    
    HRESULT connectResult = transport->Connect("localhost:8080");
    EXPECT_EQ(connectResult, S_OK);
    
    uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
    HRESULT sendResult = transport->Send(data, sizeof(data));
    EXPECT_EQ(sendResult, S_OK);
}

TEST(LocalTransport, SendWithoutConnect) {
    ITransportFactory& factory = ITransportFactory::GetInstance();
    auto transport = factory.CreateTransport("Local");
    ASSERT_NE(transport, nullptr);
    
    uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
    HRESULT result = transport->Send(data, sizeof(data));
    EXPECT_EQ(result, E_FAIL);
}

TEST(LocalTransport, SendNullData) {
    ITransportFactory& factory = ITransportFactory::GetInstance();
    auto transport = factory.CreateTransport("Local");
    ASSERT_NE(transport, nullptr);
    
    HRESULT connectResult = transport->Connect("localhost:8080");
    EXPECT_EQ(connectResult, S_OK);
    
    HRESULT sendResult = transport->Send(nullptr, 10);
    EXPECT_EQ(sendResult, E_INVALIDARG);
}

TEST(LocalTransport, SendZeroSize) {
    ITransportFactory& factory = ITransportFactory::GetInstance();
    auto transport = factory.CreateTransport("Local");
    ASSERT_NE(transport, nullptr);
    
    HRESULT connectResult = transport->Connect("localhost:8080");
    EXPECT_EQ(connectResult, S_OK);
    
    uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
    HRESULT sendResult = transport->Send(data, 0);
    EXPECT_EQ(sendResult, E_INVALIDARG);
}

TEST(LocalTransport, ReceiveValidBuffer) {
    ITransportFactory& factory = ITransportFactory::GetInstance();
    auto transport = factory.CreateTransport("Local");
    ASSERT_NE(transport, nullptr);
    
    HRESULT connectResult = transport->Connect("localhost:8080");
    EXPECT_EQ(connectResult, S_OK);
    
    uint8_t buffer[1024];
    size_t outSize = 0;
    HRESULT receiveResult = transport->Receive(buffer, sizeof(buffer), outSize);
    EXPECT_EQ(receiveResult, S_OK);
    EXPECT_EQ(outSize, 0u);
}

TEST(LocalTransport, ReceiveWithoutConnect) {
    ITransportFactory& factory = ITransportFactory::GetInstance();
    auto transport = factory.CreateTransport("Local");
    ASSERT_NE(transport, nullptr);
    
    uint8_t buffer[1024];
    size_t outSize = 0;
    HRESULT result = transport->Receive(buffer, sizeof(buffer), outSize);
    EXPECT_EQ(result, E_FAIL);
}

TEST(LocalTransport, ReceiveNullBuffer) {
    ITransportFactory& factory = ITransportFactory::GetInstance();
    auto transport = factory.CreateTransport("Local");
    ASSERT_NE(transport, nullptr);
    
    HRESULT connectResult = transport->Connect("localhost:8080");
    EXPECT_EQ(connectResult, S_OK);
    
    size_t outSize = 0;
    HRESULT receiveResult = transport->Receive(nullptr, 1024, outSize);
    EXPECT_EQ(receiveResult, E_INVALIDARG);
}

TEST(LocalTransport, ReceiveZeroBufferSize) {
    ITransportFactory& factory = ITransportFactory::GetInstance();
    auto transport = factory.CreateTransport("Local");
    ASSERT_NE(transport, nullptr);
    
    HRESULT connectResult = transport->Connect("localhost:8080");
    EXPECT_EQ(connectResult, S_OK);
    
    uint8_t buffer[1024];
    size_t outSize = 0;
    HRESULT receiveResult = transport->Receive(buffer, 0, outSize);
    EXPECT_EQ(receiveResult, E_INVALIDARG);
}

TEST(UdpTransport, BasicOperations) {
    ITransportFactory& factory = ITransportFactory::GetInstance();
    auto transport = factory.CreateTransport("UDP");
    ASSERT_NE(transport, nullptr);
    
    HRESULT connectResult = transport->Connect("192.168.1.100:9000");
    EXPECT_EQ(connectResult, S_OK);
    
    uint8_t data[] = {0xAA, 0xBB, 0xCC, 0xDD};
    HRESULT sendResult = transport->Send(data, sizeof(data));
    EXPECT_EQ(sendResult, S_OK);
    
    uint8_t buffer[1024];
    size_t outSize = 0;
    HRESULT receiveResult = transport->Receive(buffer, sizeof(buffer), outSize);
    EXPECT_EQ(receiveResult, S_OK);
    EXPECT_EQ(outSize, 0u);
    
    HRESULT disconnectResult = transport->Disconnect();
    EXPECT_EQ(disconnectResult, S_OK);
}
