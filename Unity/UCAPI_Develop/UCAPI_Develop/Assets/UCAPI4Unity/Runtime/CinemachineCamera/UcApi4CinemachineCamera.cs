using UCAPI4Unity.Runtime.Core;
using Unity.Cinemachine;

namespace UCAPI4Unity.Runtime.CinemachineCamera
{
    public class UcApi4CinemachineCamera
    {
        public static void ApplyToVirtualCamera(byte[] raw, Unity.Cinemachine.CinemachineCamera virtualCamera, CinemachineVolumeSettings volumeSettings)
        {
            var ucApiRecords = UcApiCore.DeserializeToRecord(raw, 1);
            var rec = ucApiRecords[0];
            UcApiRecordParser.ToVirtualCamera(rec, virtualCamera, volumeSettings);
        }
        
        public static byte[] SerializeFromVirtualCamera(ICinemachineCamera cam)
        {
            var payload = UcApiRecordParser.FromVirtualCamera(cam);
            return UcApiCore.SerializeFromRecord(payload);
        }
    }
}
