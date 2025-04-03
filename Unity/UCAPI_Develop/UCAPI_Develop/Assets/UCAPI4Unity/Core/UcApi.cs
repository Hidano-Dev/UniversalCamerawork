using System;
using System.IO;
using System.Runtime.InteropServices;
using UnityEngine;

namespace UCAPI4Unity.Core
{
    public class UcApi
    {
        public ushort Magic;
        public ushort Version;
        public ushort NumPayload;
        public ushort PayloadLength;
        public ushort CRC16;
        public UcApiRecord[] Payloads;
        
        private const int HeaderSize = 10;

        public UcApi(IntPtr dataPtr)
        {
            var objectPtr = GetObjectPtr(dataPtr);
            GetHeader(objectPtr);
        }

        private static IntPtr GetObjectPtr(IntPtr dataPtr)
        {
            // 先頭4バイトをUcApiDllObjectに変換
            var ucApiDllObject = Marshal.PtrToStructure<UcApiDllObject>(dataPtr);
            // 先頭4バイトのポインタを取得
            return ucApiDllObject.Obj;
        }
        
        private void GetHeader(IntPtr dataPtr)
        {
            // 8バイトアライメントに足りないサイズを計算
            int append = 8 - HeaderSize % 8;
            if (append == 8)
            {
                append = 0;
            }
            
            var dataSize = HeaderSize + append + 8;
            
            var buffer = new byte[dataSize];
            Marshal.Copy(dataPtr, buffer, 0, dataSize);

            using var ms = new MemoryStream(buffer);
            using var br = new BinaryReader(ms);
            // Magic Numberを読み取る
            Magic = br.ReadUInt16();
            // Versionを読み取る
            Version = br.ReadUInt16();
            // Payloadの数を読み取る
            NumPayload = br.ReadUInt16();
            // Payloadのサイズを読み取る
            PayloadLength = br.ReadUInt16();
            // CRC16を読み取る
            CRC16 = br.ReadUInt16();
                
            for (var i = 0; i < append; i++)
            {
                // 8バイトアライメントにするため、データポインタを調整
                br.ReadByte();
            }

            // Magicのチェック
            if (Magic != 0x5543)
            {
                throw new Exception($"Magic check failed. Expected: 0x5543, Actual: {Magic}");
            }

            var ptr = br.ReadUInt64();
            Payloads = GetPayloads((IntPtr)ptr, NumPayload, PayloadLength);
        }

        private UcApiRecord[] GetPayloads(IntPtr dataPtr, int count, int size)
        {
            var records = new UcApiRecord[count];
            for (var i = 0; i < count; i++)
            {
                records[i] = GetPayload(dataPtr, size);
                // 次のPayloadのポインタを取得
                dataPtr += size;
            }
            return records;
        }

        private UcApiRecord GetPayload(IntPtr objectPtr, int size)
        {
            var payload = GetPayloadBuffer(objectPtr, size);
            Debug.Log("Payload: " + BitConverter.ToString(payload));
            var payloadPtr = Marshal.AllocHGlobal(size);
            Marshal.Copy(payload, 0, payloadPtr, size);
            try
            {
                // Payloadのデータを読み取る
                for (var j = 0; j < NumPayload; j++)
                {
                    var record = new UcApiRecord(payloadPtr + j * PayloadLength, PayloadLength);
                    Debug.Log(record.ToString());
                }
            }
            finally
            {
                Marshal.FreeHGlobal(payloadPtr);
            }
            
            return new UcApiRecord(objectPtr, size);
        }

        private static byte[] GetPayloadBuffer(IntPtr dataPtr, int size)
        {
            var buffer = new byte[size];
            Marshal.Copy(dataPtr, buffer, 0, size);
            return buffer;
        }
    }
}