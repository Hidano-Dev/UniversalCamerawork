namespace UCAPI4Unity.Core
{
    /// <summary>
    /// 7bit...Frame Number
    /// 6bit...Second
    /// 6bit...Minute
    /// 5bit...Hour
    /// 8bit...Reserved
    /// Total 32bit
    /// </summary>
    public class UcApiTimeCode
    {
        public int FrameNumber { get; }
        public int Second { get; }
        public int Minute { get; }
        public int Hour { get; }
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
            _reserved = GetReserved(data);
        }
        
        private static int GetFrameNumber(int data)
        {
            return data & 0x7F;
        }
        
        private static int GetSecond(int data)
        {
            return (data >> 7) & 0x3F;
        }
        
        private static int GetMinute(int data)
        {
            return (data >> 13) & 0x3F;
        }
        
        private static int GetHour(int data)
        {
            return (data >> 19) & 0x1F;
        }
        
        private static int GetReserved(int data)
        {
            return data >> 25;
        }
        
        public override string ToString()
        {
            return $"{Hour:D2}:{Minute:D2}:{Second:D2}:{FrameNumber:D2}";
        }
    }
}