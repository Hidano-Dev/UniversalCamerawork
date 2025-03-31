using System.Collections;
using System.Runtime.InteropServices;
using NUnit.Framework;
using UCAPI4Unity.Core;

namespace UCAPI4Unity.Tests
{
    public class UcApiCoreTest
    {
        [Test(ExpectedResult = null)]
        public IEnumerator UcApiCoreTest_CreateDefault()
        {
            var ucApi = Core.UcApiForUnity.CreateDefault();
            Assert.IsNotNull(ucApi);
            Assert.AreEqual(0, ucApi.Magic);
            Assert.AreEqual(0, ucApi.Version);
            Assert.AreEqual(0, ucApi.NumPayload);
            Assert.AreEqual(0, ucApi.PayloadLength);
            Assert.AreEqual(0, ucApi.CRC16);
            yield return null;
        }
        
        [Test(ExpectedResult = null)]
        public IEnumerator UcApiCoreTest_Serialize()
        {
            var ucApi = Core.UcApiForUnity.CreateDefault();
            Assert.IsNotNull(ucApi);
            Assert.AreEqual(0, ucApi.Magic);
            Assert.AreEqual(0, ucApi.Version);
            Assert.AreEqual(0, ucApi.NumPayload);
            Assert.AreEqual(0, ucApi.PayloadLength);
            Assert.AreEqual(0, ucApi.CRC16);

            var result = Core.UcApiForUnity.Serialize(ucApi, out var outBuffer, out var outSize);
            Assert.AreEqual(0, result);
            Assert.IsTrue(outSize.ToUInt32() > 0);
            yield return null;
        }
        
        [Test(ExpectedResult = null)]
        public IEnumerator UcApiCoreTest_Deserialize()
        {
            var ucApi = UcApiForUnity.CreateDefault();
            Assert.IsNotNull(ucApi);
            Assert.AreEqual(0, ucApi.Magic);
            Assert.AreEqual(0, ucApi.Version);
            Assert.AreEqual(0, ucApi.NumPayload);
            Assert.AreEqual(0, ucApi.PayloadLength);
            Assert.AreEqual(0, ucApi.CRC16);

            var result = UcApiForUnity.Serialize(ucApi, out var outBuffer, out var outSize);
            Assert.AreEqual(0, result);
            Assert.IsTrue(outSize.ToUInt32() > 0);

            var managedBuffer = new byte[outSize.ToUInt32()];
            Marshal.Copy(outBuffer, managedBuffer, 0, (int)outSize.ToUInt32());
            
            var deserializedUcApi = UcApiForUnity.Deserialize(managedBuffer);
            Assert.IsNotNull(deserializedUcApi);
            Assert.AreEqual(ucApi.Magic, deserializedUcApi.Magic);
            Assert.AreEqual(ucApi.Version, deserializedUcApi.Version);
            Assert.AreEqual(ucApi.NumPayload, deserializedUcApi.NumPayload);
            Assert.AreEqual(ucApi.PayloadLength, deserializedUcApi.PayloadLength);
            Assert.AreEqual(ucApi.CRC16, deserializedUcApi.CRC16);
            Assert.AreEqual(ucApi.Payloads.Length, deserializedUcApi.Payloads.Length);
            for (var i = 0; i < ucApi.Payloads.Length; i++)
            {
                Assert.AreEqual(ucApi.Payloads[i].CameraNo, deserializedUcApi.Payloads[i].CameraNo);
                Assert.AreEqual(ucApi.Payloads[i].Commands, deserializedUcApi.Payloads[i].Commands);
                Assert.AreEqual(ucApi.Payloads[i].TimeCode.ToString(), deserializedUcApi.Payloads[i].TimeCode.ToString());
                Assert.AreEqual(ucApi.Payloads[i].PacketNo, deserializedUcApi.Payloads[i].PacketNo);
                Assert.AreEqual(ucApi.Payloads[i].EyePositionRightM, deserializedUcApi.Payloads[i].EyePositionRightM);
                Assert.AreEqual(ucApi.Payloads[i].EyePositionUpM, deserializedUcApi.Payloads[i].EyePositionUpM);
                Assert.AreEqual(ucApi.Payloads[i].EyePositionForwardM, deserializedUcApi.Payloads[i].EyePositionForwardM);
                Assert.AreEqual(ucApi.Payloads[i].LookVectorRightM, deserializedUcApi.Payloads[i].LookVectorRightM);
                Assert.AreEqual(ucApi.Payloads[i].LookVectorUpM, deserializedUcApi.Payloads[i].LookVectorUpM);
                Assert.AreEqual(ucApi.Payloads[i].LookVectorForwardM, deserializedUcApi.Payloads[i].LookVectorForwardM);
                Assert.AreEqual(ucApi.Payloads[i].UpVectorRightM, deserializedUcApi.Payloads[i].UpVectorRightM);
                Assert.AreEqual(ucApi.Payloads[i].UpVectorUpM, deserializedUcApi.Payloads[i].UpVectorUpM);
                Assert.AreEqual(ucApi.Payloads[i].UpVectorForwardM, deserializedUcApi.Payloads[i].UpVectorForwardM);
                Assert.AreEqual(ucApi.Payloads[i].FocalLengthMm, deserializedUcApi.Payloads[i].FocalLengthMm);
                Assert.AreEqual(ucApi.Payloads[i].AspectRatio, deserializedUcApi.Payloads[i].AspectRatio);
                Assert.AreEqual(ucApi.Payloads[i].FocusDistanceM, deserializedUcApi.Payloads[i].FocusDistanceM);
                Assert.AreEqual(ucApi.Payloads[i].Aperture, deserializedUcApi.Payloads[i].Aperture);
                Assert.AreEqual(ucApi.Payloads[i].SensorSizeWidthMm, deserializedUcApi.Payloads[i].SensorSizeWidthMm);
                Assert.AreEqual(ucApi.Payloads[i].SensorSizeHeightMm, deserializedUcApi.Payloads[i].SensorSizeHeightMm);
                Assert.AreEqual(ucApi.Payloads[i].NearClipM, deserializedUcApi.Payloads[i].NearClipM);
                Assert.AreEqual(ucApi.Payloads[i].FarClipM, deserializedUcApi.Payloads[i].FarClipM);
                Assert.AreEqual(ucApi.Payloads[i].LensShiftHorizontalRatio, deserializedUcApi.Payloads[i].LensShiftHorizontalRatio);
                Assert.AreEqual(ucApi.Payloads[i].LensShiftVerticalRatio, deserializedUcApi.Payloads[i].LensShiftVerticalRatio);
                Assert.AreEqual(ucApi.Payloads[i].LensDistortionRadialCoefficientsK1, deserializedUcApi.Payloads[i].LensDistortionRadialCoefficientsK1);
                Assert.AreEqual(ucApi.Payloads[i].LensDistortionRadialCoefficientsK2, deserializedUcApi.Payloads[i].LensDistortionRadialCoefficientsK2);
                Assert.AreEqual(ucApi.Payloads[i].LensDistortionCenterPointRightMm, deserializedUcApi.Payloads[i].LensDistortionCenterPointRightMm);
                Assert.AreEqual(ucApi.Payloads[i].LensDistortionCenterPointUpMm, deserializedUcApi.Payloads[i].LensDistortionCenterPointUpMm);
            }
            yield return null;
        }
    }
}
