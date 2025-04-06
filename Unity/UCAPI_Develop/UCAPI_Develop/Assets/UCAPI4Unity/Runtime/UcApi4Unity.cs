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
            return UcApiCore.SerializeFromRecord(payload);
        }
    }
}
