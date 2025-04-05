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

        public static UcApiObject CreateDefault()
        {
            // Create a default UCAPI object using the C++ factory function.
            var ucapiObject = UCAPI_CreateDefault();
            Debug.Assert(ucapiObject != IntPtr.Zero);
            Debug.Log("UCAPI object created successfully.");
            return new UcApiObject(ucapiObject);
        }
        
        public static int Serialize(UcApiObject ucApiObject, out IntPtr outBuffer, out UIntPtr outSize)
        {
            var ucApiPtr = GetUcApiPointer(ucApiObject);
            try
            {
                var result = UCAPI_Serialize(ucApiPtr, out outBuffer, out outSize);
                Debug.Log("Serialization succeeded. Buffer size: " + outSize.ToUInt32());
                return result;
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                throw;
            }
            finally
            {
                UCAPI_FreeObject(ucApiPtr);
            }
        }

        public static UcApiObject Deserialize(byte[] managedBuffer)
        {
            // Now, attempt to deserialize the byte array back to a UCAPI object.
            var dllObjectPtr = UCAPI_Deserialize(managedBuffer, (UIntPtr)managedBuffer.Length);
            Debug.Assert(dllObjectPtr != IntPtr.Zero);
            Debug.Log("Deserialization succeeded.");

            return new UcApiObject(dllObjectPtr);
        }
        
        private static IntPtr GetUcApiPointer(UcApiObject ucApiObject)
        {
            // Get the pointer to the UCAPI object.
            var ptr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(UcApiObject)));
            Marshal.StructureToPtr(ucApiObject, ptr, false);
            return ptr;
        }
    }
}
