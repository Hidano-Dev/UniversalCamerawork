using UCAPI4Unity.Core;
using UnityEngine;
using uOSC;

namespace UCAPI4Unity.Sample
{
    public class UcApiSender : MonoBehaviour
    {
        [SerializeField] private uOscClient client;
        [SerializeField] private new Camera camera;

        private void Update()
        {
            var ucapi = UcApiForUnity.SerializeFromCamera(camera);
            client.Send("/ucapi/data", ucapi);
        }
    }
}