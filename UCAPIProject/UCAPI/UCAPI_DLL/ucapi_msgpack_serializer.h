#pragma once
#include "include/ucapi_message_serializer.h"
#include "ucapi_msgpack_types.h"

namespace ucapi {
    class MessagePackSerializer : public IMessageSerializer {
    public:
        virtual ~MessagePackSerializer() = default;
        
        virtual HRESULT Serialize(const CameraState& src, std::vector<uint8_t>& out) noexcept override;
        
        virtual HRESULT Deserialize(const uint8_t* data, size_t size, CameraState& out) noexcept override;
        
    private:
        ucapi_msgpack_record_t ConvertToMsgPack(const CameraState& src);
        CameraState ConvertFromMsgPack(const ucapi_msgpack_record_t& msgpack_record);
    };
}
