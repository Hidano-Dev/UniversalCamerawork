namespace UCAPI4Unity.Runtime.Core
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
    }
}