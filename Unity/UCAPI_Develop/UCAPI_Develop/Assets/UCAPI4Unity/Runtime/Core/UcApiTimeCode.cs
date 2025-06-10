namespace UCAPI4Unity.Runtime.Core
{
    /// <summary>
    /// SMPTE 80bit (10byte) + subframe (2byte) タイムコード
    /// </summary>
    public struct UcApiTimeCode
    {
        public byte Hour;      // BCD
        public byte Minute;    // BCD
        public byte Second;    // BCD
        public byte Frame;     // BCD
        public byte[] UserBits; // 4バイト
        public ushort Subframe; // サブフレーム（ゲームエンジン拡張）

        public static UcApiTimeCode FromBytes(byte[] data)
        {
            if (data == null || data.Length < 12) throw new System.ArgumentException("TimeCode data must be 12 bytes");
            return new UcApiTimeCode
            {
                Frame = FromBcd(data[0]),
                Second = FromBcd(data[1]),
                Minute = FromBcd(data[2]),
                Hour = FromBcd(data[3]),
                UserBits = new byte[] { data[4], data[5], data[6], data[7] },
                Subframe = (ushort)((data[8] << 8) | data[9])
            };
        }

        public byte[] ToBytes()
        {
            var data = new byte[12];
            data[0] = ToBcd(Frame);
            data[1] = ToBcd(Second);
            data[2] = ToBcd(Minute);
            data[3] = ToBcd(Hour);
            for (int i = 0; i < 4; i++) data[4 + i] = (UserBits != null && UserBits.Length > i) ? UserBits[i] : (byte)0;
            data[8] = (byte)((Subframe >> 8) & 0xFF);
            data[9] = (byte)(Subframe & 0xFF);
            // 10,11バイト目は将来拡張用に0
            return data;
        }

        public override string ToString()
        {
            return $"{Hour:D2}:{Minute:D2}:{Second:D2}:{Frame:D2} Sub:{Subframe} UserBits:[{string.Join(",", UserBits ?? new byte[4])}]";
        }

        private static byte ToBcd(byte val) => (byte)(((val / 10) << 4) | (val % 10));
        private static byte FromBcd(byte bcd) => (byte)(((bcd >> 4) * 10) + (bcd & 0x0F));
    }
}