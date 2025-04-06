using System;
using System.Runtime.InteropServices;
using UnityEngine;

namespace UCAPI4Unity.Core
{
    public static class UcApiForUnity
    {
        // Deserialize from MessagePack
        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr UCAPI_Deserialize(byte[] buffer, ulong size);

        // Serialize to MessagePack
        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern int UCAPI_Serialize(IntPtr obj, out IntPtr outBuffer, out UIntPtr outSize);

        // Encode to binary
        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr UCAPI_EncodeToBinary(IntPtr obj, out UIntPtr outSize);

        // Decode from binary
        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr UCAPI_DecodeFromBinary(byte[] buffer, UIntPtr size);

        // Free buffer
        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern void UCAPI_FreeBuffer(IntPtr buffer);

        // Free object
        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern void UCAPI_FreeObject(IntPtr obj);


        public static void ApplyToCamera(byte[] raw, Camera camera)
        {
            // DLL内でsize_t型として受け取る
            var ucApiObjPtr = UCAPI_Deserialize(raw, 138);
            if (ucApiObjPtr == IntPtr.Zero)
            {
                throw new Exception("Deserialization failed.");
            }
            var ucApiObject = Marshal.PtrToStructure<UcApiObject>(ucApiObjPtr);
            if (ucApiObject.NumPayload == 0)
            {
                throw new Exception("No payloads found.");
            }
            var payloads = new UcApiRecord[ucApiObject.NumPayload];
            var payloadPtr = ucApiObject.Payloads;
            for (var i = 0; i < ucApiObject.NumPayload; i++)
            {
                var payload = Marshal.PtrToStructure<UcApiRecord>(payloadPtr);
                payloads[i] = payload;
                payloadPtr += Marshal.SizeOf(payload);
            }
            
            var rec = payloads[0];
            
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
            
            UCAPI_FreeBuffer(ucApiObjPtr);
        }
        
        public static byte[] SerializeFromCamera(Camera cam)
        {
            var payload = new UcApiRecord
            {
                CameraNo = 1,
                Commands = 0x0B, // DOF_ENABLE | LENS_DISTORTION_ENABLE 仮
                PacketNo = 1,
                TimeCode = new UcApiTimeCode
                {
                    FrameNumber = 12,
                    Second = 34,
                    Minute = 56,
                    Hour = 78,
                    FrameRate = (byte)FrameRate.FrameRate60,
                    DropFrame = 0
                },
                EyePositionRightM = cam.transform.position.x,
                EyePositionUpM = cam.transform.position.y,
                EyePositionForwardM = cam.transform.position.z,
                LookVectorRightM = cam.transform.forward.x,
                LookVectorUpM = cam.transform.forward.y,
                LookVectorForwardM = cam.transform.forward.z,
                UpVectorRightM = cam.transform.up.x,
                UpVectorUpM = cam.transform.up.y,
                UpVectorForwardM = cam.transform.up.z,
                FocalLengthMm = cam.focalLength,
                AspectRatio = cam.aspect,
                FocusDistanceM = cam.focusDistance,
                Aperture = cam.aperture,
                SensorSizeWidthMm = cam.sensorSize.x,
                SensorSizeHeightMm = cam.sensorSize.y,
                NearClipM = cam.nearClipPlane,
                FarClipM = cam.farClipPlane,
                LensShiftHorizontalRatio = cam.lensShift.x,
                LensShiftVerticalRatio = cam.lensShift.y,
                // Lens distortion と center point は 0 にしておく
                LensDistortionRadialCoefficientsK1 = 0f,
                LensDistortionRadialCoefficientsK2 = 0f,
                LensDistortionCenterPointRightMm = 0f,
                LensDistortionCenterPointUpMm = 0f
            };
            var payloadPtr = Marshal.AllocHGlobal(Marshal.SizeOf(payload));
            Marshal.StructureToPtr(payload, payloadPtr, false);
            
            var obj = new UcApiObject
            {
                Magic = 0xAA55,
                Version = 0,
                NumPayload = 1,
                PayloadLength = 128,
                CRC16 = 0, // CRC16 placeholder 
                Payloads = payloadPtr
            };
            
            var serializedData = SerializeInternal(obj);

            Marshal.FreeHGlobal(payloadPtr);

            return serializedData;
        }
        
        private static byte[] SerializeInternal(UcApiObject ucApiObject)
        {
            var ucApiObjectPtr = Marshal.AllocHGlobal(Marshal.SizeOf(ucApiObject));
            Marshal.StructureToPtr(ucApiObject, ucApiObjectPtr, false);
            var result = UCAPI_Serialize(ucApiObjectPtr, out var buffer, out var size);
            if (result != 0 || buffer == IntPtr.Zero)
            {
                throw new Exception("Serialization to MessagePack failed.");
            }

            var managedBuffer = new byte[(int)size];
            Marshal.Copy(buffer, managedBuffer, 0, (int)size);
            UCAPI_FreeBuffer(buffer);

            Marshal.FreeHGlobal(ucApiObjectPtr);
            return managedBuffer;
        }
    }
}
