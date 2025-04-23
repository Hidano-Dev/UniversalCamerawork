using Unity.Cinemachine;
using UnityEngine;
using UnityEngine.Rendering.Universal;

namespace UCAPI4Unity.Sample.Scripts.CinemachineSupporter
{
    public class CinemachineVolumeTracer : MonoBehaviour
    {
        [SerializeField] private CinemachineCamera virtualCamera;
        [SerializeField] private CinemachineVolumeSettings volumeSettings;
        
        private DepthOfField _depthOfField;
        
        // Start is called once before the first execution of Update after the MonoBehaviour is created
        private void Start()
        {
            volumeSettings.Profile.TryGet(out _depthOfField);
        }

        // Update is called once per frame
        private void Update()
        {
            if (virtualCamera == null || _depthOfField == null)
            {
                return;
            }
            
            _depthOfField.aperture.Override(virtualCamera.State.Lens.PhysicalProperties.Aperture);
            _depthOfField.focusDistance.Override(virtualCamera.State.Lens.PhysicalProperties.FocusDistance);
            var focalLength = Camera.FieldOfViewToFocalLength(virtualCamera.State.Lens.FieldOfView, virtualCamera.State.Lens.PhysicalProperties.SensorSize.y);
            _depthOfField.focalLength.Override(focalLength);
            
            // 更新を適用
            volumeSettings.Profile.isDirty = true;
        }
    }
}
