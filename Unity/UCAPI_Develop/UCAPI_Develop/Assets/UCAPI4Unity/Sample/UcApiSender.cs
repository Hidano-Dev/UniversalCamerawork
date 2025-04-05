using UCAPI4Unity.Core;
using UnityEngine;
using uOSC;

namespace UCAPI4Unity.Sample
{
    public class UcApiSender : MonoBehaviour
    {
        [SerializeField] private uOscClient client;
        [SerializeField] private new Camera camera;

        private void Start()
        {
            var mock = CreateDefaultMock();
            var msgpack = UcApiForUnity.SerializeToMessagePack(mock);
            UcApiForUnity.Free(mock);
            client.Send("/ucapi/data", msgpack);
        }
        
        private static UcApiDllObject CreateDefaultMock()
        {
            var binary = new byte[10 + 128];
            binary[0] = 0xAA;
            binary[1] = 0x55;
            binary[4] = 0x01;
            binary[6] = 0x80;

            // 任意のカメラNo
            binary[10 + 0] = 0x01;

            // timeCode適当
            binary[10 + 6] = 1;
            binary[10 + 7] = 2;
            binary[10 + 8] = 3;
            binary[10 + 9] = 4;

            return UcApiForUnity.DecodeFromBinary(binary);
        }
    }
}