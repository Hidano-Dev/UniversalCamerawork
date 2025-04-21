using UCAPI4Unity.Runtime.CinemachineCamera;
using UCAPI4Unity.Runtime.UnityCamera;
using Unity.Cinemachine;
using UnityEngine;
using uOSC;

namespace UCAPI4Unity.Sample.Scripts
{
    public class UcApiReceiver : MonoBehaviour
    {
        [SerializeField] private new Camera camera;
        [SerializeField] private CinemachineCamera virtualCamera;
        
        private CinemachineVolumeSettings _volumeSettings;

        private void Start()
        {
            _volumeSettings = virtualCamera.GetComponent<CinemachineVolumeSettings>();
        }

        public void OnDataReceived(Message message)
        {
            if (message.address != "/ucapi/data") return;

            var raw = (byte[])message.values[0];
            UcApi4UnityCamera.ApplyToCamera(raw, camera);
            UcApi4CinemachineCamera.ApplyToVirtualCamera(raw, virtualCamera, _volumeSettings);
        }
    }
}