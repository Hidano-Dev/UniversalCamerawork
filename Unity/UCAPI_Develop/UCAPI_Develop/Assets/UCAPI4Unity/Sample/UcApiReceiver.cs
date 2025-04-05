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
            var dllObj = UcApiForUnity.DeserializeFromMessagePack(raw);
            if (dllObj.Payloads.Length > 0)
            {
                ApplyToCamera(dllObj.Payloads[0]);
            }
        }

        private void ApplyToCamera(UcApiRecord rec)
        {
            // カメラ位置・回転を反映
            var position = new Vector3(
                rec.EyePositionRightM,
                rec.EyePositionUpM,
                rec.EyePositionForwardM
            );

            var forward = new Vector3(
                rec.LookVectorRightM,
                rec.LookVectorUpM,
                rec.LookVectorForwardM
            );

            var up = new Vector3(
                rec.UpVectorRightM,
                rec.UpVectorUpM,
                rec.UpVectorForwardM
            );

            camera.transform.position = position;
            camera.transform.rotation = Quaternion.LookRotation(forward, up);
            
            // レンズ・クリップなどを反映
            camera.focalLength = rec.FocalLengthMm;
            camera.aspect = rec.AspectRatio;
            camera.nearClipPlane = rec.NearClipM;
            camera.farClipPlane = rec.FarClipM;
            camera.sensorSize = new Vector2(rec.SensorSizeWidthMm, rec.SensorSizeHeightMm);
            camera.lensShift = new Vector2(rec.LensShiftHorizontalRatio, rec.LensShiftVerticalRatio);

            // 被写界深度など（コマンドビットによる有効化が必要なら拡張可）
            var dofEnabled = (rec.Commands & 0x04) != 0;
            if (dofEnabled)
            {
                camera.usePhysicalProperties = true;
                camera.focusDistance = rec.FocusDistanceM;
                camera.aperture = rec.Aperture;
            }
        }
    }
}