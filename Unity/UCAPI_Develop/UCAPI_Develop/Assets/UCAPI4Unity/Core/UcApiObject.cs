using MessagePack;

namespace UCAPI4Unity.Core
{
    [MessagePackObject]
    public struct UcApiObject
    {
        [Key(0)] public ushort Magic;
        [Key(1)] public ushort Version;
        [Key(2)] public ushort NumPayload;
        [Key(3)] public ushort PayloadLength;
        [Key(4)] public ushort CRC16;
        [Key(5)] public UcApiRecord[] Payloads;
    }
}