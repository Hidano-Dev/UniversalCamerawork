using UCAPI4Unity.Runtime.CinemachineCamera;
using UCAPI4Unity.Runtime.UnityCamera;
using Unity.Cinemachine;
using UnityEngine;
using uOSC;

namespace UCAPI4Unity.Sample.Scripts
{
    public class UcApiSender : MonoBehaviour
    {
        [SerializeField] private CameraMode cameraMode;
        [SerializeField] private uOscClient client;
        [SerializeField] private new Camera camera;
        [SerializeField] private CinemachineCamera virtualCamera;

        private void Update()
        {
            byte[] ucapi = null;
            switch (cameraMode)
            {
                case CameraMode.UnityCamera:
                    ucapi = UcApi4UnityCamera.SerializeFromCamera(camera);
                    break;
                case CameraMode.CinemachineCamera:
                    ucapi = UcApi4CinemachineCamera.SerializeFromVirtualCamera(virtualCamera);
                    break;
            }
            client.Send("/ucapi/data", ucapi);
        }
    }
}