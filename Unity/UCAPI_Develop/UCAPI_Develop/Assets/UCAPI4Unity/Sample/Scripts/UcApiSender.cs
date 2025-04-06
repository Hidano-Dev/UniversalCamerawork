using UCAPI4Unity.Runtime;
using UnityEngine;
using uOSC;

namespace UCAPI4Unity.Sample.Scripts
{
    public class UcApiSender : MonoBehaviour
    {
        [SerializeField] private uOscClient client;
        [SerializeField] private new Camera camera;

        private void Update()
        {
            var ucapi = UcApi4Unity.SerializeFromCamera(camera);
            client.Send("/ucapi/data", ucapi);
        }
    }
}