using System;
using System.Runtime.InteropServices;
using UnityEngine;

namespace UCAPI4Unity.Core
{
    public static class UcApiForUnity
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

        public static UcApi CreateDefault()
        {
            // Create a default UCAPI object using the C++ factory function.
            var ucapiObject = UCAPI_CreateDefault();
            if (ucapiObject == IntPtr.Zero)
            {
                Debug.Assert(false, "UCAPI_CreateDefault failed.");
            }
            Debug.Log("UCAPI object created successfully.");
            var defaultUcapi = new UcApi(ucapiObject);
            return defaultUcapi;
        }
        
        public static int Serialize(UcApi ucApi, out IntPtr outBuffer, out UIntPtr outSize)
        {
            var ucApiPtr = GetUcApiPointer(ucApi);
            // Serialize the UCAPI object to a native buffer.
            var result = UCAPI_Serialize(ucApiPtr, out outBuffer, out outSize);
            if (result != 0)
            {
                Debug.LogError("UCAPI_Serialize failed.");
                UCAPI_FreeObject(ucApiPtr);
                return result;
            }
            Debug.Log("Serialization succeeded. Buffer size: " + outSize.ToUInt32());
            return 0;
        }

        public static UcApi Deserialize(byte[] managedBuffer)
        {
            // Now, attempt to deserialize the byte array back to a UCAPI object.
            var dllObjectPtr = UCAPI_Deserialize(managedBuffer, (UIntPtr)managedBuffer.Length);
            if (dllObjectPtr == IntPtr.Zero)
            {
                Debug.LogError("UCAPI_Deserialize failed.");
                return null;
            }
            Debug.Log("Deserialization succeeded.");

            var ucapi = new UcApi(dllObjectPtr);
            return ucapi;
        }
        
        private static IntPtr GetUcApiPointer(UcApi ucApi)
        {
            // Get the pointer to the UCAPI object.
            var ptr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(UcApi)));
            Marshal.StructureToPtr(ucApi, ptr, false);
            return ptr;
        }
    }
}
