using System;
using System.IO;
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

        private const int HeaderSize = 10;

        public UcApiObject(UcApiDllObject dllObj)
        {
            Magic = 0;
            Version = 0;
            NumPayload = 0;
            PayloadLength = 0;
            CRC16 = 0;
            Payloads = Array.Empty<UcApiRecord>();

            if (dllObj == null || dllObj.NativePtr == IntPtr.Zero)
                throw new ArgumentException("Invalid UCAPI DLL object.");

            // DLL からバイナリを取得
            var binary = UcApiForUnity.EncodeToBinary(dllObj);

            if (binary == null || binary.Length < HeaderSize)
                throw new InvalidDataException("Failed to read binary data from UCAPI DLL object.");
            
            using var ms = new MemoryStream(binary);
            using var br = new BinaryReader(ms);

            Magic = br.ReadUInt16();
            Version = br.ReadUInt16();
            NumPayload = br.ReadUInt16();
            PayloadLength = br.ReadUInt16();
            CRC16 = br.ReadUInt16();

            Payloads = new UcApiRecord[NumPayload];
            for (var i = 0; i < NumPayload; i++)
            {
                var recordBytes = br.ReadBytes(PayloadLength);
                Payloads[i] = new UcApiRecord(recordBytes);
            }
        }
    }
}