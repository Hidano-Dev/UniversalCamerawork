namespace UCAPI4Unity.Runtime.Core
{
    /// <summary>
    /// Total 80 bits
    /// </summary>
    public struct UcApiTimeCode
    {
        public uint FrameNumber;
        public uint Second;
        public uint Minute;
        public uint Hour;
        public FrameRate FrameRate;
        public bool DropFrame;
        public bool ColorFrame;
        public byte[] UserBits;

        public static UcApiTimeCode FromSmpteData(byte[] smpteData)
        {
            if (smpteData == null || smpteData.Length < 10)
            {
                return new UcApiTimeCode
                {
                    FrameNumber = 0,
                    Second = 0,
                    Minute = 0,
                    Hour = 0,
                    FrameRate = FrameRate.FrameRate60,
                    DropFrame = false,
                    ColorFrame = false,
                    UserBits = new byte[4]
                };
            }

            byte frameUnits = (byte)(smpteData[0] & 0x0F);
            byte frameTens = (byte)(smpteData[1] & 0x03);
            byte secUnits = (byte)(smpteData[2] & 0x0F);
            byte secTens = (byte)(smpteData[3] & 0x07);
            byte minUnits = (byte)(smpteData[4] & 0x0F);
            byte minTens = (byte)(smpteData[5] & 0x07);
            byte hourUnits = (byte)(smpteData[6] & 0x0F);
            byte hourTens = (byte)(smpteData[7] & 0x03);

            return new UcApiTimeCode
            {
                FrameNumber = (uint)(frameTens * 10 + frameUnits),
                Second = (uint)(secTens * 10 + secUnits),
                Minute = (uint)(minTens * 10 + minUnits),
                Hour = (uint)(hourTens * 10 + hourUnits),
                FrameRate = FrameRate.FrameRate60,
                DropFrame = (smpteData[1] & 0x04) != 0,
                ColorFrame = (smpteData[1] & 0x08) != 0,
                UserBits = new byte[]
                {
                    (byte)((smpteData[0] >> 4) & 0x0F),
                    (byte)((smpteData[1] >> 4) & 0x0F),
                    (byte)((smpteData[2] >> 4) & 0x0F),
                    (byte)((smpteData[3] >> 4) & 0x0F)
                }
            };
        }

        public byte[] ToSmpteData()
        {
            var smpteData = new byte[10];
            
            byte frameUnits = (byte)(FrameNumber % 10);
            byte frameTens = (byte)(FrameNumber / 10);
            byte secUnits = (byte)(Second % 10);
            byte secTens = (byte)(Second / 10);
            byte minUnits = (byte)(Minute % 10);
            byte minTens = (byte)(Minute / 10);
            byte hourUnits = (byte)(Hour % 10);
            byte hourTens = (byte)(Hour / 10);

            var userBits = UserBits ?? new byte[4];
            
            smpteData[0] = (byte)((frameUnits & 0x0F) | ((userBits[0] & 0x0F) << 4));
            smpteData[1] = (byte)(((frameTens & 0x03) | (DropFrame ? 0x04 : 0x00) | (ColorFrame ? 0x08 : 0x00)) | ((userBits[1] & 0x0F) << 4));
            smpteData[2] = (byte)((secUnits & 0x0F) | ((userBits[2] & 0x0F) << 4));
            smpteData[3] = (byte)((secTens & 0x07) | ((userBits[3] & 0x0F) << 4));
            smpteData[4] = (byte)((minUnits & 0x0F) | ((userBits[0] & 0x0F) << 4));
            smpteData[5] = (byte)((minTens & 0x07) | ((userBits[1] & 0x0F) << 4));
            smpteData[6] = (byte)((hourUnits & 0x0F) | ((userBits[2] & 0x0F) << 4));
            smpteData[7] = (byte)((hourTens & 0x03) | ((userBits[3] & 0x0F) << 4));
            
            smpteData[8] = 0x3F;
            smpteData[9] = 0xFD;
            
            return smpteData;
        }

        public static UcApiTimeCode FromRaw(uint raw)
        {
            return new UcApiTimeCode
            {
                FrameNumber = raw & 0xFF,
                Second = (raw >> 8) & 0x3F,
                Minute = (raw >> 14) & 0x3F,
                Hour = (raw >> 20) & 0x1F,
                FrameRate = (FrameRate)((raw >> 25) & 0xF),
                DropFrame = ((raw >> 29) & 0x1) == 1,
                ColorFrame = ((raw >> 30) & 0x1) == 1,
                UserBits = new byte[4]
            };
        }

        public override string ToString()
        {
            return $"{Hour:D2}:{Minute:D2}:{Second:D2}:{FrameNumber:D2} " +
                   $"({(int)FrameRate}) " +
                   $"{(DropFrame ? "Drop" : "Non-Drop")} " +
                   $"{(ColorFrame ? "Color" : "B&W")}";
        }
        
        public static uint ToRaw(UcApiTimeCode timeCode)
        {
            var raw = timeCode.FrameNumber;
            raw |= timeCode.Second << 8;
            raw |= timeCode.Minute << 14;
            raw |= timeCode.Hour << 20;
            raw |= (uint)((int)timeCode.FrameRate << 25);
            raw |= (uint)(timeCode.DropFrame ? 1 : 0) << 29;
            raw |= (uint)(timeCode.ColorFrame ? 1 : 0) << 30;
            return raw;
        }
    }
}
