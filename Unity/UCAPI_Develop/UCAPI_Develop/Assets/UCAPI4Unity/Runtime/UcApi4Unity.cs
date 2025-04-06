using System.Runtime.InteropServices;
using UCAPI4Unity.Runtime.Core;
using UCAPI4Unity.Runtime.UnityCamera;
using UnityEngine;

namespace UCAPI4Unity.Runtime
{
    public static class UcApi4Unity
    {
        public static void ApplyToCamera(byte[] raw, Camera camera)
        {
            var ucApiRecords = UcApiCore.DeserializeToRecord(raw, 1);
            var rec = ucApiRecords[0];
            UcApiRecordParser.ToCamera(rec, camera);
        }
        
        public static byte[] SerializeFromCamera(Camera cam)
        {
            var payload = UcApiRecordParser.FromCamera(cam);
            var payloadPtr = Marshal.AllocHGlobal(Marshal.SizeOf(payload));
            Marshal.StructureToPtr(payload, payloadPtr, false);
            
            var obj = new UcApiObject
            {
                Magic = 0xAA55,
                Version = 0,
                NumPayload = 1,
                CRC16 = 0, // CRC16 placeholder 
                Payloads = payloadPtr
            };
            
            var serializedData = UcApiCore.SerializeFromObject(obj);

            Marshal.FreeHGlobal(payloadPtr);

            return serializedData;
        }
    }
}
