using UCAPI4Unity.Runtime;
using UCAPI4Unity.Runtime.CinemachineCamera;
using UCAPI4Unity.Runtime.UnityCamera;
using Unity.Cinemachine;
using UnityEngine;
using uOSC;

namespace UCAPI4Unity.Sample.Scripts
{
    public class UcApiSender : MonoBehaviour
    {
        private enum Mode
        {
            UnityCamera,
            CinemachineCamera,
        }
        
        [SerializeField] private uOscClient client;
        [SerializeField] private Mode mode;
        [SerializeField] private new Camera camera;
        [SerializeField] private CinemachineCamera virtualCamera;

        private void Update()
        {
            byte[] ucapi = null;
            switch (mode)
            {
                case Mode.UnityCamera:
                    ucapi = UcApi4UnityCamera.SerializeFromCamera(camera);
                    break;
                case Mode.CinemachineCamera:
                    ucapi = UcApi4CinemachineCamera.SerializeFromVirtualCamera(virtualCamera);
                    break;
            }
            client.Send("/ucapi/data", ucapi);
        }
    }
}