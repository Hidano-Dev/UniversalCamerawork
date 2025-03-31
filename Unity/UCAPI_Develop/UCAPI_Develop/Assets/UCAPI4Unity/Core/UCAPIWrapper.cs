using System;
using System.Runtime.InteropServices;
using UnityEngine;

namespace UCAPI4Unity.Core
{
    public class UcApiWrapper : MonoBehaviour
    {
        // Import the UCAPI_CreateDefault function from the DLL.
        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr UCAPI_CreateDefault();

        // Import the UCAPI_Serialize function from the DLL.
        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern int UCAPI_Serialize(IntPtr ucapi, out IntPtr outBuffer, out UIntPtr outSize);

        // Import the UCAPI_FreeBuffer function from the DLL.
        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern void UCAPI_FreeBuffer(IntPtr buffer);

        // Import the UCAPI_Deserialize function from the DLL.
        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr UCAPI_Deserialize(byte[] inBuffer, UIntPtr inSize);

        // Import the UCAPI_FreeObject function from the DLL.
        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern void UCAPI_FreeObject(IntPtr obj);

        // Example usage: Create a default UCAPI object, serialize and then deserialize it.
        void Start()
        {
            // Create a default UCAPI object using the C++ factory function.
            IntPtr ucapiObject = UCAPI_CreateDefault();
            if (ucapiObject == IntPtr.Zero)
            {
                Debug.LogError("UCAPI_CreateDefault failed.");
                return;
            }
            Debug.Log("UCAPI object created successfully.");
            var defaultUcapi = new UcApi(ucapiObject);

            // Serialize the UCAPI object.
            IntPtr nativeBuffer;
            UIntPtr nativeSize;
            int result = UCAPI_Serialize(ucapiObject, out nativeBuffer, out nativeSize);
            if (result != 0)
            {
                Debug.LogError("UCAPI_Serialize failed.");
                UCAPI_FreeObject(ucapiObject);
                return;
            }

            int size = (int)nativeSize;
            byte[] managedBuffer = new byte[size];
            Marshal.Copy(nativeBuffer, managedBuffer, 0, size);
            Debug.Log("Serialization succeeded. Buffer size: " + size);

            // Free the native buffer allocated by UCAPI_Serialize.
            UCAPI_FreeBuffer(nativeBuffer);

            // Now, attempt to deserialize the byte array back to a UCAPI object.
            IntPtr dllObjectPtr = UCAPI_Deserialize(managedBuffer, (UIntPtr)managedBuffer.Length);
            if (dllObjectPtr == IntPtr.Zero)
            {
                Debug.LogError("UCAPI_Deserialize failed.");
                UCAPI_FreeObject(ucapiObject);
                return;
            }
            else
            {
                Debug.Log("Deserialization succeeded.");
            }

            // ここでdllObjectPtrを用いて必要な処理を実施する…
            var ucapi = new UcApi(dllObjectPtr);

            // 使用後、生成したオブジェクトは必ず解放する。
            UCAPI_FreeObject(dllObjectPtr);
            UCAPI_FreeObject(ucapiObject);
        }
    }
}
