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
        public  int FrameNumber { get; }
        public int Second { get; }
        public int Minute { get; }
        public int Hour { get; }
        
        public FrameRate FrameRate { get; }
        
        public bool DropFrame { get; }
        
        private int _reserved;

        
        /// <summary>
        /// 32bitのデータをTimeCodeに変換する
        /// </summary>
        /// <param name="data"></param>
        public UcApiTimeCode(int data)
        {
            FrameNumber = GetFrameNumber(data);
            Second = GetSecond(data);
            Minute = GetMinute(data);
            Hour = GetHour(data);
            FrameRate = GetFrameRate(data);
            DropFrame = GetDropFrame(data);
            _reserved = GetReserved(data);
        }
        
        private static int GetFrameNumber(int data)
        {
            return data & 0xFF;
        }
        
        private static int GetSecond(int data)
        {
            return (data >> 8) & 0x3F;
        }
        
        private static int GetMinute(int data)
        {
            return (data >> 14) & 0x3F;
        }
        
        private static int GetHour(int data)
        {
            return (data >> 20) & 0x1F;
        }
        
        private static FrameRate GetFrameRate(int data)
        {
            return (FrameRate)((data >> 25) & 0xF);
        }
        
        private static bool GetDropFrame(int data)
        {
            return ((data >> 29) & 0x1) == 1;
        }
        
        private static int GetReserved(int data)
        {
            return (data >> 30) & 0x3;
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