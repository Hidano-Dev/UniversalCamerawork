using System;
using System.Collections;
using NUnit.Framework;
using UnityEngine.TestTools;
using UCAPI4Unity.Runtime.Core;

namespace UCAPI4Unity.Tests
{
    public class UcApiCoreTest
    {
        // 正常なレコードを用いたシリアライズ／デシリアライズのラウンドトリップテスト
        [UnityTest]
        public IEnumerator SerializeAndDeserialize_ValidData()
        {
            // テスト用レコードを作成
            var original = new UcApiRecord
            {
                CameraNo = 1,
                Commands = 0x0001,
                TimeCode = 12345,
                SubFrame = 0.5f,
                PacketNo = 1,
                EyePositionRightM = 1.0f,
                EyePositionUpM = 2.0f,
                EyePositionForwardM = 3.0f,
                LookVectorRightM = 0.0f,
                LookVectorUpM = 0.0f,
                LookVectorForwardM = 1.0f,
                UpVectorRightM = 0.0f,
                UpVectorUpM = 1.0f,
                UpVectorForwardM = 0.0f,
                FocalLengthMm = 50.0f,
                AspectRatio = 1.777f,
                FocusDistanceM = 10.0f,
                Aperture = 2.8f,
                SensorSizeWidthMm = 36.0f,
                SensorSizeHeightMm = 24.0f,
                NearClipM = 0.1f,
                FarClipM = 1000.0f,
                LensShiftHorizontalRatio = 0.0f,
                LensShiftVerticalRatio = 0.0f,
                LensDistortionRadialCoefficientsK1 = 0.0f,
                LensDistortionRadialCoefficientsK2 = 0.0f,
                LensDistortionCenterPointRightMm = 0.0f,
                LensDistortionCenterPointUpMm = 0.0f
            };

            // レコードをシリアライズ
            var buffer = UcApiCore.SerializeFromRecord(original);
            Assert.IsNotNull(buffer);
            Assert.Greater(buffer.Length, 0);

            // シリアライズしたバッファをデシリアライズ
            var deserialized = UcApiCore.DeserializeToRecord(buffer, 1);
            Assert.IsNotNull(deserialized);
            Assert.AreEqual(1, deserialized.Length);

            var record = deserialized[0];
            Assert.AreEqual(original.CameraNo, record.CameraNo);
            Assert.AreEqual(original.Commands, record.Commands);
            Assert.AreEqual(original.TimeCode, record.TimeCode);
            Assert.AreEqual(original.SubFrame, record.SubFrame, 0.001f);
            Assert.AreEqual(original.PacketNo, record.PacketNo);
            Assert.AreEqual(original.EyePositionRightM, record.EyePositionRightM, 0.001f);
            Assert.AreEqual(original.EyePositionUpM, record.EyePositionUpM, 0.001f);
            Assert.AreEqual(original.EyePositionForwardM, record.EyePositionForwardM, 0.001f);
            Assert.AreEqual(original.FocalLengthMm, record.FocalLengthMm, 0.001f);
            Assert.AreEqual(original.AspectRatio, record.AspectRatio, 0.001f);

            yield return null;
        }

        // 異常な入力（nullや短いバッファ）によるデシリアライズテスト
        [Test]
        public void Deserialize_InvalidData_ThrowsException()
        {
            // データが不足しているバッファ
            var shortBuffer = new byte[2];
            Assert.Throws<Exception>(() => UcApiCore.DeserializeToRecord(shortBuffer, 1));
        }

        // 境界値（最小サイズ、最大サイズ）を用いたテスト例
        [UnityTest]
        public IEnumerator Serialize_Deserialize_BoundaryValues()
        {
            // 最小値設定のレコード（すべてゼロ）
            var minRecord = new UcApiRecord();

            var minBuffer = UcApiCore.SerializeFromRecord(minRecord);
            var minDeserialized = UcApiCore.DeserializeToRecord(minBuffer, 1);
            Assert.AreEqual(1, minDeserialized.Length);
            Assert.AreEqual(minRecord.CameraNo, minDeserialized[0].CameraNo);
            Assert.AreEqual(minRecord.Commands, minDeserialized[0].Commands);

            // 最大値設定のレコード（例として上限値を設定）
            var maxRecord = new UcApiRecord
            {
                CameraNo = uint.MaxValue,
                Commands = ushort.MaxValue,
                TimeCode = uint.MaxValue,
                SubFrame = float.MaxValue,
                PacketNo = byte.MaxValue,
                EyePositionRightM = float.MaxValue,
                EyePositionUpM = float.MaxValue,
                EyePositionForwardM = float.MaxValue
            };

            var maxBuffer = UcApiCore.SerializeFromRecord(maxRecord);
            var maxDeserialized = UcApiCore.DeserializeToRecord(maxBuffer, 1);
            Assert.AreEqual(1, maxDeserialized.Length);
            Assert.AreEqual(maxRecord.CameraNo, maxDeserialized[0].CameraNo);
            Assert.AreEqual(maxRecord.Commands, maxDeserialized[0].Commands);
            Assert.AreEqual(maxRecord.TimeCode, maxDeserialized[0].TimeCode);
            Assert.AreEqual(maxRecord.PacketNo, maxDeserialized[0].PacketNo);

            yield return null;
        }
    }
}