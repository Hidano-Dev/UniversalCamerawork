using System;

namespace UCAPI4Unity.Runtime.Core
{
    public struct UcApiObject
    {
        public ushort Magic;
        public ushort Version;
        public ushort NumPayload;
        public ushort CRC16;
        public IntPtr Payloads;
    }
}