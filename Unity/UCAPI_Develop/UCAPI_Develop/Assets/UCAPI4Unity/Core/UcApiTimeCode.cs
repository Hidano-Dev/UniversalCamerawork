using System;

namespace UCAPI4Unity.Core
{
    /// <summary>
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
        public int FrameNumber { get; }
        public int Second { get; }
        public int Minute { get; }
        public int Hour { get; }
        public FrameRate FrameRate { get; }
        public bool DropFrame { get; }

        private readonly int _reserved;

        public UcApiTimeCode(byte[] data)
        {
            if (data == null || data.Length != 4)
                throw new ArgumentException("Timecode data must be 4 bytes.");

            // Little-endianでintに変換
            int raw = BitConverter.ToInt32(data, 0);

            FrameNumber = raw & 0xFF;
            Second = (raw >> 8) & 0x3F;
            Minute = (raw >> 14) & 0x3F;
            Hour = (raw >> 20) & 0x1F;
            FrameRate = (FrameRate)((raw >> 25) & 0xF);
            DropFrame = ((raw >> 29) & 0x1) == 1;
            _reserved = (raw >> 30) & 0x3;
        }

        public override string ToString()
        {
            return $"{Hour:D2}:{Minute:D2}:{Second:D2}:{FrameNumber:D2} " +
                   $"({(int)FrameRate}) " +
                   $"{(DropFrame ? "Drop" : "Non-Drop")} " +
                   $"Reserved: {_reserved}";
        }
    }
}