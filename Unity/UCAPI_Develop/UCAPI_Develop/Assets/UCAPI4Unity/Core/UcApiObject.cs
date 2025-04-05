using System;
using System.IO;
using System.Runtime.InteropServices;

namespace UCAPI4Unity.Core
{
    public struct UcApiObject
    {
        public ushort Magic;
        public ushort Version;
        public ushort NumPayload;
        public ushort PayloadLength;
        public ushort CRC16;
        public UcApiRecord[] Payloads;
        
        private const int HeaderSize = 10;

        public UcApiObject(IntPtr dataPtr)
        {
            Magic = 0x55AA;
            Version = 0;
            NumPayload = 1;
            //UcApiRecordのサイズを取得
            PayloadLength = (ushort)((Marshal.SizeOf(typeof(UcApiRecord)) + 7) / 8 * 8);
            CRC16 = 0;
            Payloads = Array.Empty<UcApiRecord>();
            var objectPtr = GetObjectPtr(dataPtr);
            ReadPayloads(objectPtr);
        }

        private static IntPtr GetObjectPtr(IntPtr dataPtr)
        {
            // 先頭4バイトをUcApiDllObjectに変換
            var ucApiDllObject = Marshal.PtrToStructure<UcApiDllObject>(dataPtr);
            // 先頭4バイトのポインタを取得
            return ucApiDllObject.Obj;
        }
        
        private void ReadPayloads(IntPtr dataPtr)
        {
            var dataSize = CalculateDataSize(HeaderSize);
            
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
                
            for (var i = 0; i < CalculateAppendSize(HeaderSize); i++)
            {
                // 8バイトアライメントにするため、データポインタを調整
                br.ReadByte();
            }
            // Recordデータのポインタ
            var ptr = br.ReadUInt64();
            
            br.Close();
            ms.Close();

            Payloads = GetPayloads((IntPtr)ptr, NumPayload, PayloadLength);
        }

        private static int CalculateDataSize(int headerSize)
        {
            // HeaderSize + ポインタ1個分 + 8バイトアライメント
            var dataSize = headerSize + 8 + CalculateAppendSize(headerSize);
            return dataSize;
        }

        /// <summary>
        /// 8バイトアライメントに足りないサイズを計算
        /// </summary>
        /// <param name="size"></param>
        /// <returns></returns>
        private static int CalculateAppendSize(int size)
        {
            var append = 8 - size % 8;
            if (append == 8)
            {
                append = 0;
            }
            return append;
        }

        private static UcApiRecord[] GetPayloads(IntPtr dataPtr, int count, int size)
        {
            var records = new UcApiRecord[count];
            for (var i = 0; i < count; i++)
            {
                records[i] = new UcApiRecord(dataPtr, size);
                // 次のPayloadのポインタを取得
                dataPtr += size;
            }
            return records;
        }
    }
}