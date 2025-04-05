using System;
using System.Runtime.InteropServices;
using MessagePack;
using UnityEngine;

namespace UCAPI4Unity.Core
{
    public static class UcApiForUnity
    {
        // Deserialize from MessagePack
        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr UCAPI_DeserializeMessagePack(byte[] buffer, UIntPtr size);

        // Serialize to MessagePack
        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern int UCAPI_SerializeMessagePack(IntPtr obj, out IntPtr outBuffer, out UIntPtr outSize);

        // Encode to binary
        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr UCAPI_EncodeToBinary(IntPtr obj, out UIntPtr outSize);

        // Decode from binary
        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr UCAPI_DecodeFromBinary(byte[] buffer, UIntPtr size);

        // Free buffer
        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern void UCAPI_FreeBuffer(IntPtr buffer);

        // Free object
        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern void UCAPI_FreeObject(IntPtr obj);

        public static UcApiObject DeserializeFromMessagePack(byte[] messagePackData)
        {
            
            var ucApiObject = MessagePackSerializer.Deserialize<UcApiObject>(messagePackData);
            return ucApiObject;
        }

        public static byte[] SerializeToMessagePack(UcApiDllObject ucApiObject)
        {
            var result = UCAPI_SerializeMessagePack(ucApiObject.NativePtr, out var buffer, out var size);
            if (result != 0 || buffer == IntPtr.Zero)
            {
                throw new Exception("Serialization to MessagePack failed.");
            }

            var managedBuffer = new byte[(int)size];
            Marshal.Copy(buffer, managedBuffer, 0, (int)size);
            UCAPI_FreeBuffer(buffer);

            return managedBuffer;
        }

        public static UcApiDllObject DecodeFromBinary(byte[] binaryData)
        {
            var nativePtr = UCAPI_DecodeFromBinary(binaryData, (UIntPtr)binaryData.Length);
            Debug.Assert(nativePtr != IntPtr.Zero, "Binary decode failed.");
            return new UcApiDllObject(nativePtr);
        }

        public static byte[] EncodeToBinary(UcApiDllObject ucApiObject)
        {
            var buffer = UCAPI_EncodeToBinary(ucApiObject.NativePtr, out var size);

            if (buffer == IntPtr.Zero || size == UIntPtr.Zero)
            {
                throw new Exception("Binary encode failed.");
            }

            var managedBuffer = new byte[(int)size];
            Marshal.Copy(buffer, managedBuffer, 0, (int)size);
            UCAPI_FreeBuffer(buffer);

            return managedBuffer;
        }

        public static void Free(UcApiDllObject obj)
        {
            if (obj != null && obj.NativePtr != IntPtr.Zero)
            {
                UCAPI_FreeObject(obj.NativePtr);
                obj.Invalidate();
            }
        }
    }
}
