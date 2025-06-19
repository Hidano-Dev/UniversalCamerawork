#pragma once
#include "ucapi_message_serializer.h"

namespace ucapi {
    class DummySerializer : public IMessageSerializer {
    public:
        DummySerializer() = default;
        virtual ~DummySerializer() = default;

        HRESULT Serialize(const CameraState& src, std::vector<uint8_t>& out) noexcept override;

        HRESULT Deserialize(const uint8_t* data, size_t size, CameraState& out) noexcept override;
    };
}
