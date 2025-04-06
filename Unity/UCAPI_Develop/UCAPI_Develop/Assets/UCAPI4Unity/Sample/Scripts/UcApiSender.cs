using UCAPI4Unity.Runtime;
using Unity.Cinemachine;
using UnityEngine;
using uOSC;

namespace UCAPI4Unity.Sample.Scripts
{
    public class UcApiSender : MonoBehaviour
    {
        public enum Mode
        {
            UnityCamera,
            CinemachineCamera,
        }
        
        [SerializeField] private uOscClient client;
        [SerializeField] private Mode mode;
        [SerializeField] private new Camera camera;
        [SerializeField] private ICinemachineCamera virtualCamera;

        private void Update()
        {
            byte[] ucapi = null;
            switch (mode)
            {
                case Mode.UnityCamera:
                    ucapi = UcApi4Unity.SerializeFromCamera(camera);
                    break;
                case Mode.CinemachineCamera:
                    ucapi = UcApi4Unity.SerializeFromVirtualCamera(virtualCamera);
            }
            client.Send("/ucapi/data", ucapi);
        }
    }
}