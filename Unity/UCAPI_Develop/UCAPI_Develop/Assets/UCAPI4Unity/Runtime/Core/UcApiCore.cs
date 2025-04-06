using System;
using System.Runtime.InteropServices;
using UCAPI4Unity.Core;

namespace UCAPI4Unity.Runtime.Core
{
    public static class UcApiCore
    {
        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr UCAPI_Deserialize(byte[] buffer, UIntPtr payloadCount);

        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern int UCAPI_Serialize(IntPtr obj, out IntPtr outBuffer, out UIntPtr outSize);

        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern void UCAPI_FreeBuffer(IntPtr buffer);

        public static UcApiRecord[] DeserializeToRecord(byte[] buffer, int payloadCount)
        {
            var ucApiObject = DeserializeToObject(buffer, payloadCount);
            if (ucApiObject.NumPayload == 0)
            {
                throw new Exception("No payloads found.");
            }
            var payloads = new UcApiRecord[ucApiObject.NumPayload];
            var payloadPtr = ucApiObject.Payloads;
            for (var i = 0; i < ucApiObject.NumPayload; i++)
            {
                var payload = Marshal.PtrToStructure<UcApiRecord>(payloadPtr);
                payloads[i] = payload;
                payloadPtr += Marshal.SizeOf(payload);
            }
            
            return payloads;
        }
        
        public static byte[] SerializeFromObject(UcApiObject ucApiObject)
        {
            var ucApiObjectPtr = Marshal.AllocHGlobal(Marshal.SizeOf(ucApiObject));
            Marshal.StructureToPtr(ucApiObject, ucApiObjectPtr, false);
            var result = UCAPI_Serialize(ucApiObjectPtr, out var buffer, out var size);
            if (result != 0 || buffer == IntPtr.Zero)
            {
                throw new Exception("Serialization to MessagePack failed.");
            }

            var managedBuffer = new byte[(int)size];
            Marshal.Copy(buffer, managedBuffer, 0, (int)size);
            UCAPI_FreeBuffer(buffer);

            Marshal.FreeHGlobal(ucApiObjectPtr);
            return managedBuffer;
        }

        internal static UcApiObject DeserializeToObject(byte[] buffer, int payloadCount)
        {
            var ucApiObjPtr = UCAPI_Deserialize(buffer, (UIntPtr)payloadCount);
            if (ucApiObjPtr == IntPtr.Zero)
            {
                throw new Exception("Deserialization failed.");
            }
            var ucApiObject = Marshal.PtrToStructure<UcApiObject>(ucApiObjPtr);
            if (ucApiObject.NumPayload == 0)
            {
                throw new Exception("No payloads found.");
            }
            
            UCAPI_FreeBuffer(ucApiObjPtr);
            return ucApiObject;
        }
    }
}