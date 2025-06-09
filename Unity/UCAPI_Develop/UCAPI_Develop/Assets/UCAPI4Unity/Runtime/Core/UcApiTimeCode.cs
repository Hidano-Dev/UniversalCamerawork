using System;
using System.Runtime.InteropServices;

namespace UCAPI4Unity.Runtime.Core
{
    /// <summary>
    /// SMPTE 12M Linear Timecode (LTC) 80-bit structure for Unity
    /// </summary>
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct UcApiSmpteTimecode
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 10)]
        public byte[] Data;  // 80 bits = 10 bytes

        public UcApiSmpteTimecode(byte[] data = null)
        {
            Data = data ?? new byte[10];
            if (Data.Length != 10)
            {
                Array.Resize(ref Data, 10);
            }
        }

        // Get/Set frame number (0-29)
        public byte Frame
        {
            get => GetBcdValue(0, 0x3F);
            set => SetBcdValue(0, value, 29);
        }

        // Get/Set second (0-59)
        public byte Second
        {
            get => GetBcdValue(2, 0x7F);
            set => SetBcdValue(2, value, 59);
        }

        // Get/Set minute (0-59)
        public byte Minute
        {
            get => GetBcdValue(4, 0x7F);
            set => SetBcdValue(4, value, 59);
        }

        // Get/Set hour (0-23)
        public byte Hour
        {
            get => GetBcdValue(6, 0x3F);
            set => SetBcdValue(6, value, 23);
        }

        // Get/Set drop frame flag
        public bool DropFrame
        {
            get => GetBit(10);
            set => SetBit(10, value);
        }

        // Get/Set color frame flag
        public bool ColorFrame
        {
            get => GetBit(11);
            set => SetBit(11, value);
        }

        // Get/Set user bits (field: 0-7, value: 0-15)
        public byte GetUserBits(int field)
        {
            if (field < 0 || field > 7) return 0;
            
            int[] bitPositions = { 4, 12, 20, 28, 36, 44, 52, 60 };
            int startBit = bitPositions[field];
            
            byte value = 0;
            for (int i = 0; i < 4; i++)
            {
                if (GetBit(startBit + i))
                {
                    value |= (byte)(1 << i);
                }
            }
            return value;
        }

        public void SetUserBits(int field, byte value)
        {
            if (field < 0 || field > 7 || value > 15) return;
            
            int[] bitPositions = { 4, 12, 20, 28, 36, 44, 52, 60 };
            int startBit = bitPositions[field];
            
            for (int i = 0; i < 4; i++)
            {
                SetBit(startBit + i, (value & (1 << i)) != 0);
            }
        }

        // Validate sync word
        public bool IsValidSync()
        {
            ushort syncWord = 0;
            for (int i = 0; i < 16; i++)
            {
                if (GetBit(64 + i))
                {
                    syncWord |= (ushort)(1 << i);
                }
            }
            return syncWord == 0x3FFD;
        }

        // Set sync word
        public void SetSyncWord()
        {
            ushort syncWord = 0x3FFD;
            for (int i = 0; i < 16; i++)
            {
                SetBit(64 + i, (syncWord & (1 << i)) != 0);
            }
        }

        // Clear all data
        public void Clear()
        {
            if (Data == null) Data = new byte[10];
            Array.Clear(Data, 0, 10);
            SetSyncWord();
        }

        private bool GetBit(int bitPosition)
        {
            if (bitPosition >= 80 || Data == null) return false;
            
            int byteIndex = bitPosition / 8;
            int bitIndex = bitPosition % 8;
            
            return (Data[byteIndex] & (1 << bitIndex)) != 0;
        }

        private void SetBit(int bitPosition, bool value)
        {
            if (bitPosition >= 80 || Data == null) return;
            
            int byteIndex = bitPosition / 8;
            int bitIndex = bitPosition % 8;
            
            if (value)
            {
                Data[byteIndex] |= (byte)(1 << bitIndex);
            }
            else
            {
                Data[byteIndex] &= (byte)~(1 << bitIndex);
            }
        }

        private byte GetBcdValue(int byteOffset, byte mask)
        {
            if (Data == null) return 0;
            
            int bitOffset = byteOffset * 8;
            byte units = 0;
            byte tens = 0;
            
            // Get units (4 bits)
            for (int i = 0; i < 4; i++)
            {
                if (GetBit(bitOffset + i))
                {
                    units |= (byte)(1 << i);
                }
            }
            
            // Get tens (variable bits based on field)
            int tensBits = 4;
            if (byteOffset == 2 || byteOffset == 4) tensBits = 3;  // seconds/minutes tens (0-5)
            if (byteOffset == 6) tensBits = 2;  // hours tens (0-2)
            if (byteOffset == 0) tensBits = 2;  // frame tens (0-2)
            
            for (int i = 0; i < tensBits && (bitOffset + 8 + i) < 80; i++)
            {
                if (GetBit(bitOffset + 8 + i))
                {
                    tens |= (byte)(1 << i);
                }
            }
            
            return (byte)(tens * 10 + units);
        }

        private void SetBcdValue(int byteOffset, byte value, byte maxValue)
        {
            if (Data == null) return;
            if (value > maxValue) value = maxValue;
            
            int bitOffset = byteOffset * 8;
            byte units = (byte)(value % 10);
            byte tens = (byte)(value / 10);
            
            // Set units (4 bits)
            for (int i = 0; i < 4; i++)
            {
                SetBit(bitOffset + i, (units & (1 << i)) != 0);
            }
            
            // Set tens (variable bits based on field)
            int tensBits = 4;
            if (byteOffset == 2 || byteOffset == 4) tensBits = 3;  // seconds/minutes tens (0-5)
            if (byteOffset == 6) tensBits = 2;  // hours tens (0-2)
            if (byteOffset == 0) tensBits = 2;  // frame tens (0-2)
            
            for (int i = 0; i < tensBits && (bitOffset + 8 + i) < 80; i++)
            {
                SetBit(bitOffset + 8 + i, (tens & (1 << i)) != 0);
            }
        }

        public override string ToString()
        {
            return $"{Hour:D2}:{Minute:D2}:{Second:D2}:{Frame:D2} " +
                   $"{(DropFrame ? "Drop" : "Non-Drop")} " +
                   $"{(ColorFrame ? "Color" : "Mono")} " +
                   $"Sync: {(IsValidSync() ? "OK" : "NG")}";
        }
    }

    /// <summary>
    /// Legacy timecode structure for backward compatibility
    /// 8bit...Frame Number
    /// 6bit...Second
    /// 6bit...Minute
    /// 5bit...Hour
    /// 4bit...Frame Rate
    /// 1bit...Drop Frame
    /// 2bit...Reserved
    /// Total 32bit
    /// </summary>
    public struct UcApiTimeCode
    {
        public uint FrameNumber;
        public uint Second;
        public uint Minute;
        public uint Hour;
        public FrameRate FrameRate;
        public bool DropFrame;
        private uint _reserved;

        public static UcApiTimeCode FromRaw(uint raw)
        {
            var timeCode = new UcApiTimeCode
            {
                FrameNumber = raw & 0xFF,
                Second = (raw >> 8) & 0x3F,
                Minute = (raw >> 14) & 0x3F,
                Hour = (raw >> 20) & 0x1F,
                FrameRate = (FrameRate)((raw >> 25) & 0xF),
                DropFrame = ((raw >> 29) & 0x1) == 1,
                _reserved = (raw >> 30) & 0x3
            };
            
            return timeCode;
        }

        public override string ToString()
        {
            return $"{Hour:D2}:{Minute:D2}:{Second:D2}:{FrameNumber:D2} " +
                   $"({(int)FrameRate}) " +
                   $"{(DropFrame ? "Drop" : "Non-Drop")} " +
                   $"Reserved: {_reserved}";
        }
        
        public static uint ToRaw(UcApiTimeCode timeCode)
        {
            var raw = timeCode.FrameNumber;
            raw |= timeCode.Second << 8;
            raw |= timeCode.Minute << 14;
            raw |= timeCode.Hour << 20;
            raw |= (uint)((int)timeCode.FrameRate << 25);
            raw |= (uint)(timeCode.DropFrame ? 1 : 0) << 29;
            raw |= timeCode._reserved << 30;
            return raw;
        }

        // Convert to SMPTE LTC format
        public UcApiSmpteTimecode ToSmpte()
        {
            var smpte = new UcApiSmpteTimecode();
            smpte.Clear();
            smpte.Frame = (byte)FrameNumber;
            smpte.Second = (byte)Second;
            smpte.Minute = (byte)Minute;
            smpte.Hour = (byte)Hour;
            smpte.DropFrame = DropFrame;
            smpte.ColorFrame = false;  // Default
            smpte.SetSyncWord();
            return smpte;
        }

        // Convert from SMPTE LTC format
        public static UcApiTimeCode FromSmpte(UcApiSmpteTimecode smpte)
        {
            return new UcApiTimeCode
            {
                FrameNumber = smpte.Frame,
                Second = smpte.Second,
                Minute = smpte.Minute,
                Hour = smpte.Hour,
                DropFrame = smpte.DropFrame,
                FrameRate = FrameRate.FrameRate30,  // Default
                _reserved = 0
            };
        }
    }
}

