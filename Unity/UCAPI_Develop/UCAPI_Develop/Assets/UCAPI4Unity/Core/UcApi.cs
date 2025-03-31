using System;
using System.Collections.Generic;
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
        public ushort NumPayloadLength;
        public ushort CRC16;
        public List<UcApiRecord> Payload = new();
        
        private const int HeaderSize = 10;

        public UcApi(IntPtr dataPtr)
        {
            var objectPtr = GetObjectPtr(dataPtr);
            GetHeader(objectPtr);
            GetPayload(objectPtr, NumPayloadLength);
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
            // 全体のバイト数を把握するため、最初の10バイトを読み取る
            var buffer = new byte[HeaderSize];
            Marshal.Copy(dataPtr, buffer, 0, HeaderSize);
            
            using (var ms = new MemoryStream(buffer))
            using (var br = new BinaryReader(ms))
            {
                // Magic Numberを読み取る
                Magic = br.ReadUInt16();
                // Versionを読み取る
                Version = br.ReadUInt16();
                // Payloadの数を読み取る
                NumPayload = br.ReadUInt16();
                // Payloadのサイズを読み取る
                NumPayloadLength = br.ReadUInt16();
                // CRC16を読み取る
                CRC16 = br.ReadUInt16();
            }
            
            // Magicのチェック
            if (Magic != 0x5543)
            {
                throw new Exception($"Magic check failed. Expected: 0x5543, Actual: {Magic}");
            }
        }

        private void GetPayload(IntPtr dataPtr, int size)
        {
            var payload = GetPayloadBuffer(dataPtr, size);
            Debug.Log("Payload: " + BitConverter.ToString(payload));
            var payloadPtr = Marshal.AllocHGlobal(size);
            Marshal.Copy(payload, 0, payloadPtr, size);
            try
            {
                // Payloadのデータを読み取る
                for (var i = 0; i < NumPayload; i++)
                {
                    var record = new UcApiRecord(payloadPtr + i * NumPayloadLength, NumPayloadLength);
                    Payload.Add(record);
                }
            }
            finally
            {
                Marshal.FreeHGlobal(payloadPtr);
            }
        }

        private static byte[] GetPayloadBuffer(IntPtr dataPtr, int size)
        {
            var buffer = new byte[size];
            Marshal.Copy(dataPtr, buffer, 0, size);
            return buffer;
        }
    }
}