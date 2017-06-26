using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

struct PacketHeader
{
    public UInt32 CheckCode;
    public UInt32 dwMsgID;
    public UInt32 dwSize;       //总长度header + body
    public UInt32 dwPacketNo;	//生成序号 = wCommandID^dwSize+index(每个包自动增长索引); 还原序号 = pHeader->dwPacketNo - pHeader->wCommandID^pHeader->dwSize;
    public UInt64 u64TargetID;
    public UInt32 dwUserData;
};
