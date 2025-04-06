using System;

namespace UCAPI4Unity.Core
{
    public struct UcApiObject
    {
        public ushort Magic;
        public ushort Version;
        public ushort NumPayload;
        public ushort PayloadLength;
        public ushort CRC16;
        public IntPtr Payloads;
    }
}