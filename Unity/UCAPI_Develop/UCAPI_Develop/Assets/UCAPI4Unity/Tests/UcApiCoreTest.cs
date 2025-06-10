using System;
using System.Collections;
using NUnit.Framework;
using UnityEngine.TestTools;
using UCAPI4Unity.Runtime.Core;

namespace UCAPI4Unity.Tests
{
    public class UcApiCoreTest
    {
        // 正常なオブジェクトを用いたシリアライズ／デシリアライズのラウンドトリップテスト
        [UnityTest]
        public IEnumerator SerializeAndDeserialize_ValidData()
        {
            // 例: テスト用オブジェクトを作成
            var original = new UcApiObject
            {
                Magic = 0x1234,
                Version = 1,
                NumPayload = 2,
                CRC16 = 0xABCD,
                Payloads = IntPtr.Zero
            };

            // オブジェクトをシリアライズする関数（実装済みのシリアライズ処理を呼ぶ）
            var buffer = SerializeObject(original);
            Assert.IsNotNull(buffer);
            Assert.Greater(buffer.Length, 0);

            // シリアライズしたバッファを元にデシリアライズを行う
            var deserialized = DeserializeObject(buffer, original.NumPayload);
            Assert.AreEqual(original.Magic, deserialized.Magic);
            Assert.AreEqual(original.Version, deserialized.Version);
            Assert.AreEqual(original.NumPayload, deserialized.NumPayload);
            Assert.AreEqual(original.CRC16, deserialized.CRC16);

            yield return null;
        }

        // 異常な入力（nullや短いバッファ）によるデシリアライズテスト
        [Test]
        public void Deserialize_InvalidData_ThrowsException()
        {
            // nullバッファ
            byte[] nullBuffer = null;
            Assert.Throws<ArgumentNullException>(() => DeserializeObject(nullBuffer, 0));

            // データが不足しているバッファ
            var shortBuffer = new byte[2];
            Assert.Throws<InvalidOperationException>(() => DeserializeObject(shortBuffer, 0));
        }

        // 境界値（最小サイズ、最大サイズ）を用いたテスト例
        [UnityTest]
        public IEnumerator Serialize_Deserialize_BoundaryValues()
        {
            // 最小値設定のオブジェクト
            var minObj = new UcApiObject
            {
                Magic = 0,
                Version = 0,
                NumPayload = 0,
                CRC16 = 0,
                Payloads = IntPtr.Zero
            };

            var minBuffer = SerializeObject(minObj);
            var minDeserialized = DeserializeObject(minBuffer, minObj.NumPayload);
            Assert.AreEqual(minObj.Magic, minDeserialized.Magic);
            Assert.AreEqual(minObj.Version, minDeserialized.Version);

            // 最大値設定のオブジェクト（例として上限値を設定）
            var maxObj = new UcApiObject
            {
                Magic = ushort.MaxValue,
                Version = ushort.MaxValue,
                NumPayload = ushort.MaxValue,
                CRC16 = ushort.MaxValue,
                Payloads = IntPtr.Zero
            };

            var maxBuffer = SerializeObject(maxObj);
            var maxDeserialized = DeserializeObject(maxBuffer, maxObj.NumPayload);
            Assert.AreEqual(maxObj.Magic, maxDeserialized.Magic);
            Assert.AreEqual(maxObj.Version, maxDeserialized.Version);
            Assert.AreEqual(maxObj.NumPayload, maxDeserialized.NumPayload);
            Assert.AreEqual(maxObj.CRC16, maxDeserialized.CRC16);

            yield return null;
        
        [Test]
        public void TimeCodeTest()
        {
            var timeCode = new UcApiTimeCode
            {
                FrameNumber = 15,
                Second = 30,
                Minute = 45,
                Hour = 12,
                FrameRate = FrameRate.FrameRate60,
                DropFrame = true,
                ColorFrame = false,
                UserBits = new byte[] { 0x1, 0x2, 0x3, 0x4 }
            };

            var smpteData = timeCode.ToSmpteData();
            var decoded = UcApiTimeCode.FromSmpteData(smpteData);

            Assert.AreEqual(timeCode.FrameNumber, decoded.FrameNumber);
            Assert.AreEqual(timeCode.Second, decoded.Second);
            Assert.AreEqual(timeCode.Minute, decoded.Minute);
            Assert.AreEqual(timeCode.Hour, decoded.Hour);
            Assert.AreEqual(timeCode.DropFrame, decoded.DropFrame);
            Assert.AreEqual(timeCode.ColorFrame, decoded.ColorFrame);
            Assert.AreEqual(timeCode.UserBits[0], decoded.UserBits[0]);
            Assert.AreEqual(timeCode.UserBits[1], decoded.UserBits[1]);
            Assert.AreEqual(timeCode.UserBits[2], decoded.UserBits[2]);
            Assert.AreEqual(timeCode.UserBits[3], decoded.UserBits[3]);

            var raw = UcApiTimeCode.ToRaw(timeCode);
            var decodedRaw = UcApiTimeCode.FromRaw(raw);
            Assert.AreEqual(timeCode.FrameNumber, decodedRaw.FrameNumber);
            Assert.AreEqual(timeCode.Second, decodedRaw.Second);
            Assert.AreEqual(timeCode.Minute, decodedRaw.Minute);
            Assert.AreEqual(timeCode.Hour, decodedRaw.Hour);
            Assert.AreEqual(timeCode.FrameRate, decodedRaw.FrameRate);
            Assert.AreEqual(timeCode.DropFrame, decodedRaw.DropFrame);
            Assert.AreEqual(timeCode.ColorFrame, decodedRaw.ColorFrame);
        }

        }

        private static byte[] SerializeObject(UcApiObject obj)
        {
            return UcApiCore.SerializeFromObject(obj);
        }
        
        private static UcApiObject DeserializeObject(byte[] buffer, int payloadCount)
        {
            return UcApiCore.DeserializeToObject(buffer, payloadCount);
        }
    }
}
