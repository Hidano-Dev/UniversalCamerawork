using UCAPI4Unity.Core;
using UnityEngine;
using uOSC;

namespace UCAPI4Unity.Sample
{
    public class UcApiReceiver : MonoBehaviour
    {
        [SerializeField] private new Camera camera;

        public void OnDataReceived(Message message)
        {
            if (message.address != "/ucapi/data") return;

            var raw = (byte[])message.values[0];
            UcApiForUnity.ApplyToCamera(raw, camera);
        }
    }
}