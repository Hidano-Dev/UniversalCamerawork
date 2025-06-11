using System;
using System.Runtime.InteropServices;

public static class NativeMethods
{
    [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
    public static extern ushort UCAPI_CalcCRC16(IntPtr recordPtr, UIntPtr length, ushort poly, ushort initValue);
}
