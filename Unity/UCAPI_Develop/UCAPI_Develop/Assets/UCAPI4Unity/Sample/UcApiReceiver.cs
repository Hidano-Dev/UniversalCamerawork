using UCAPI4Unity.Core;
using UnityEngine;
using uOSC;

namespace UCAPI4Unity.Sample
{
    public class UcApiReceiver : MonoBehaviour
    {
        public void OnDataReceived(Message message)
        {
            if (message.address != "/ucapi/data") return;

            var raw = (byte[])message.values[0];
            var dllObj = UcApiForUnity.DeserializeFromMessagePack(raw);
            var obj = new UcApiObject(dllObj);

            Debug.Log($"Received UCAPI data: Magic=0x{obj.Magic:X}, Payloads={obj.Payloads.Length}");
            if (obj.Payloads.Length > 0)
            {
                var rec = obj.Payloads[0];
                Debug.Log($"Camera={rec.CameraNo}, Timecode={rec.TimeCode}");
            }

            UcApiForUnity.Free(dllObj);
        }

    }
}