using System;

namespace UCAPI4Unity.Core
{
    public class UcApiDllObject
    {
        public IntPtr NativePtr;
        
        internal UcApiDllObject(IntPtr ptr)
        {
            NativePtr = ptr;
        }

        internal void Invalidate()
        {
            NativePtr = IntPtr.Zero;
        }
    }
}