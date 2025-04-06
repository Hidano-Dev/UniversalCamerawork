using System;
using System.Runtime.InteropServices;
using UnityEngine;

namespace UCAPI4Unity.Runtime.Core
{
    public static class UcApiCore
    {
        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr UCAPI_Deserialize(byte[] buffer, UIntPtr payloadCount);

        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern int UCAPI_Serialize(IntPtr obj, out IntPtr outBuffer, out UIntPtr outSize);

        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern void UCAPI_FreeBuffer(IntPtr buffer);

        public static byte[] SerializeFromRecord(UcApiRecord record)
        {
            var payloadPtr = Marshal.AllocHGlobal(Marshal.SizeOf(record));
            Marshal.StructureToPtr(record, payloadPtr, false);
            
            var obj = new UcApiObject
            {
                Magic = 0xAA55,
                Version = 0,
                NumPayload = 1,
                CRC16 = ComputeChecksum(record),
                Payloads = payloadPtr
            };
            
            Debug.Log("Serializing TimeCode: " + UcApiTimeCode.FromRaw(record.TimeCode));
            DumpRecord(record);
            
            var serializedData = SerializeFromObject(obj);
            // serializedDataにコピーした後、ポインタを解放
            Marshal.FreeHGlobal(payloadPtr);
            return serializedData;
        }
        
        public static UcApiRecord[] DeserializeToRecord(byte[] buffer, int payloadCount)
        {
            var ucApiObject = DeserializeToObject(buffer, payloadCount);
            if (ucApiObject.NumPayload == 0)
            {
                throw new Exception("No payloads found.");
            }
            var payloads = new UcApiRecord[ucApiObject.NumPayload];
            var payloadPtr = ucApiObject.Payloads;
            for (var i = 0; i < ucApiObject.NumPayload; i++)
            {
                var payload = Marshal.PtrToStructure<UcApiRecord>(payloadPtr);
                payloads[i] = payload;
                payloadPtr += Marshal.SizeOf(payload);
            }
            
            var crc = ComputeChecksum(payloads[0]);
            Debug.Log("Deserializing TimeCode: " + UcApiTimeCode.FromRaw(payloads[0].TimeCode));
            DumpRecord(payloads[0]);
            if(ucApiObject.CRC16 != crc)
            {
                throw new Exception("CRC16 checksum mismatch. Header: " + ucApiObject.CRC16 + ", Payload: " + crc);
            }
            
            return payloads;
        }
        
        internal static byte[] SerializeFromObject(UcApiObject ucApiObject)
        {
            var ucApiObjectPtr = Marshal.AllocHGlobal(Marshal.SizeOf(ucApiObject));
            Marshal.StructureToPtr(ucApiObject, ucApiObjectPtr, false);
            var result = UCAPI_Serialize(ucApiObjectPtr, out var buffer, out var size);
            if (result != 0 || buffer == IntPtr.Zero)
            {
                throw new Exception("Serialization to MessagePack failed.");
            }

            var managedBuffer = new byte[(int)size];
            Marshal.Copy(buffer, managedBuffer, 0, (int)size);
            UCAPI_FreeBuffer(buffer);

            Marshal.FreeHGlobal(ucApiObjectPtr);
            return managedBuffer;
        }

        internal static UcApiObject DeserializeToObject(byte[] buffer, int payloadCount)
        {
            var ucApiObjPtr = UCAPI_Deserialize(buffer, (UIntPtr)payloadCount);
            if (ucApiObjPtr == IntPtr.Zero)
            {
                throw new Exception("Deserialization failed.");
            }
            var ucApiObject = Marshal.PtrToStructure<UcApiObject>(ucApiObjPtr);
            if (ucApiObject.NumPayload == 0)
            {
                throw new Exception("No payloads found.");
            }
            
            UCAPI_FreeBuffer(ucApiObjPtr);
            return ucApiObject;
        }
        
        private static ushort ComputeChecksum(UcApiRecord record, ushort poly = 0x1021, ushort initValue = 0xFFFF)
        {
            var data = new byte[Marshal.SizeOf(record)];
            var ptr = Marshal.AllocHGlobal(Marshal.SizeOf(record));
            Marshal.StructureToPtr(record, ptr, false);
            Marshal.Copy(ptr, data, 0, data.Length);
            Marshal.FreeHGlobal(ptr);
            
            var crc = initValue;
            foreach (var b in data)
            {
                crc ^= (ushort)(b << 8);
                for (var i = 0; i < 8; i++)
                {
                    if ((crc & 0x8000) != 0)
                        crc = (ushort)((crc << 1) ^ poly);
                    else
                        crc <<= 1;
                }
            }
            return crc;
        }

        /// <summary>
        /// デバッグ用にレコードの内容を出力します。
        /// </summary>
        /// <param name="record"></param>
        private static void DumpRecord(UcApiRecord record)
        {
            Debug.Log("Record Dump:"
            + $"\n  CameraNo: {record.CameraNo}"
            + $"\n  Commands: {record.Commands}"
            + $"\n  TimeCode: {record.TimeCode}"
            + $"\n  PacketNo: {record.PacketNo}"
            + $"\n  EyePositionRightM: {record.EyePositionRightM}"
            + $"\n  EyePositionUpM: {record.EyePositionUpM}"
            + $"\n  EyePositionForwardM: {record.EyePositionForwardM}"
            + $"\n  LookVectorRightM: {record.LookVectorRightM}"
            + $"\n  LookVectorUpM: {record.LookVectorUpM}"
            + $"\n  LookVectorForwardM: {record.LookVectorForwardM}"
            + $"\n  UpVectorRightM: {record.UpVectorRightM}"
            + $"\n  UpVectorUpM: {record.UpVectorUpM}"
            + $"\n  UpVectorForwardM: {record.UpVectorForwardM}"
            + $"\n  FocalLengthMm: {record.FocalLengthMm}"
            + $"\n  AspectRatio: {record.AspectRatio}"
            + $"\n  FocusDistanceM: {record.FocusDistanceM}"
            + $"\n  Aperture: {record.Aperture}"
            + $"\n  SensorSizeWidthMm: {record.SensorSizeWidthMm}"
            + $"\n  SensorSizeHeightMm: {record.SensorSizeHeightMm}"
            + $"\n  NearClipM: {record.NearClipM}"
            + $"\n  FarClipM: {record.FarClipM}"
            + $"\n  LensShiftHorizontalRatio: {record.LensShiftHorizontalRatio}"
            + $"\n  LensShiftVerticalRatio: {record.LensShiftVerticalRatio}"
            + $"\n  LensDistortionRadialCoefficientsK1: {record.LensDistortionRadialCoefficientsK1}"
            + $"\n  LensDistortionRadialCoefficientsK2: {record.LensDistortionRadialCoefficientsK2}"
            + $"\n  LensDistortionCenterPointRightMm: {record.LensDistortionCenterPointRightMm}"
            + $"\n  LensDistortionCenterPointUpMm: {record.LensDistortionCenterPointUpMm}"
            + $"\n  CRC16: {ComputeChecksum(record)}");
        }
    }
}