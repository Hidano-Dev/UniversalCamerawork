using UCAPI4Unity.Runtime;
using UnityEngine;
using uOSC;

namespace UCAPI4Unity.Sample.Scripts
{
    public class UcApiReceiver : MonoBehaviour
    {
        [SerializeField] private new Camera camera;

        public void OnDataReceived(Message message)
        {
            if (message.address != "/ucapi/data") return;

            var raw = (byte[])message.values[0];
            UcApi4Unity.ApplyToCamera(raw, camera);
        }
    }
}