using System.Runtime.InteropServices;
using UnityEngine;

namespace UCAPI4Unity.Core
{
    public static class UcApiForUnity
    {
        public static void ApplyToCamera(byte[] raw, Camera camera)
        {
            var ucApiRecords = UcApiCore.DeserializeInternal(raw, 1);
            var rec = ucApiRecords[0];
            rec.ApplyToCamera(camera);
        }
        
        public static byte[] SerializeFromCamera(Camera cam)
        {
            var payload = new UcApiRecord(cam);
            var payloadPtr = Marshal.AllocHGlobal(Marshal.SizeOf(payload));
            Marshal.StructureToPtr(payload, payloadPtr, false);
            
            var obj = new UcApiObject
            {
                Magic = 0xAA55,
                Version = 0,
                NumPayload = 1,
                PayloadLength = 128,
                CRC16 = 0, // CRC16 placeholder 
                Payloads = payloadPtr
            };
            
            var serializedData = UcApiCore.SerializeInternal(obj);

            Marshal.FreeHGlobal(payloadPtr);

            return serializedData;
        }
    }
}
