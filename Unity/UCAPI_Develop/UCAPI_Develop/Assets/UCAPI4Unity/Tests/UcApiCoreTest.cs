using System.Collections;
using NUnit.Framework;
using UCAPI4Unity.Core;

namespace UCAPI4Unity.Tests
{
    public class UcApiCoreTest
    {
        private static UcApiDllObject CreateDefaultMock()
        {
            var binary = new byte[10 + 128];
            binary[0] = 0xAA;
            binary[1] = 0x55;
            binary[2] = 0x00;
            binary[3] = 0x00;
            binary[4] = 0x01;
            binary[5] = 0x00;
            binary[6] = 0x80;
            binary[7] = 0x00;
            binary[8] = 0x00;
            binary[9] = 0x00;

            // カメラNo (uint32_t) -> 1
            binary[10 + 0] = 0x01;

            // commands (uint16_t) -> 0x0A0B
            binary[10 + 4] = 0x0B;
            binary[10 + 5] = 0x0A;

            // timecode
            binary[10 + 6] = 12;           // frame
            binary[10 + 7] = 34;           // second + (minute lower bits)
            binary[10 + 8] = 56;           // minute + hour lower bits
            binary[10 + 9] = 78;           // hour upper + framerate + drop + reserved

            // packetNo (uint8_t)
            binary[10 + 10] = 0x2A;

            // その他 float 値はすべてゼロのままにする

            return UcApiForUnity.DecodeFromBinary(binary);
        }

        [Test(ExpectedResult = null)]
        public IEnumerator UcApiCoreTest_SerializeDeserialize_CompareFields()
        {
            var original = CreateDefaultMock();

            var msgpack = UcApiForUnity.SerializeToMessagePack(original);
            Assert.IsNotNull(msgpack);
            Assert.IsTrue(msgpack.Length > 0);

            var clone = UcApiForUnity.DeserializeFromMessagePack(msgpack);
            Assert.IsNotNull(clone);

            var data = new UcApiObject(original);

            Assert.AreEqual(data.Magic, clone.Magic);
            Assert.AreEqual(data.Version, clone.Version);
            Assert.AreEqual(data.NumPayload, clone.NumPayload);
            Assert.AreEqual(data.PayloadLength, clone.PayloadLength);
            Assert.AreEqual(data.CRC16, clone.CRC16);

            Assert.IsNotNull(data.Payloads);
            Assert.IsNotNull(clone.Payloads);
            Assert.AreEqual(data.Payloads.Length, clone.Payloads.Length);

            var a = data.Payloads[0];
            var b = clone.Payloads[0];

            Assert.AreEqual(a.CameraNo, b.CameraNo);
            Assert.AreEqual(a.Commands, b.Commands);
            Assert.AreEqual(a.PacketNo, b.PacketNo);

            // Timecode 比較（ToString または各フィールド）
            Assert.AreEqual(a.TimeCode.FrameNumber, b.TimeCode.FrameNumber);
            Assert.AreEqual(a.TimeCode.Second, b.TimeCode.Second);
            Assert.AreEqual(a.TimeCode.Minute, b.TimeCode.Minute);
            Assert.AreEqual(a.TimeCode.Hour, b.TimeCode.Hour);
            Assert.AreEqual(a.TimeCode.FrameRate, b.TimeCode.FrameRate);
            Assert.AreEqual(a.TimeCode.DropFrame, b.TimeCode.DropFrame);

            UcApiForUnity.Free(original);
            yield return null;
        }
        
        [Test(ExpectedResult = null)]
        public IEnumerator UcApiCoreTest_EncodeDecodeBinary()
        {
            var ucApi = CreateDefaultMock();
            Assert.IsNotNull(ucApi);

            var binary = UcApiForUnity.EncodeToBinary(ucApi);
            Assert.IsNotNull(binary);
            Assert.IsTrue(binary.Length >= 138);

            var decoded = UcApiForUnity.DecodeFromBinary(binary);
            Assert.IsNotNull(decoded);
            UcApiForUnity.Free(ucApi);
            UcApiForUnity.Free(decoded);
            yield return null;
        }

        [Test(ExpectedResult = null)]
        public IEnumerator UcApiCoreTest_SerializeDeserializeMessagePack()
        {
            var ucApi = CreateDefaultMock();
            Assert.IsNotNull(ucApi);

            var msgpack = UcApiForUnity.SerializeToMessagePack(ucApi);
            Assert.IsNotNull(msgpack);
            Assert.IsTrue(msgpack.Length > 0);

            var deserialized = UcApiForUnity.DeserializeFromMessagePack(msgpack);
            Assert.IsNotNull(deserialized);

            UcApiForUnity.Free(ucApi);
            yield return null;
        }
    }
}