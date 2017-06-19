using System;
public class StConnectNotify
{
	public Byte btConType;
	public UInt64 u64ConnID;
	public void Read(ReadBufferHelper bh)
	{
		btConType = bh.ReadUint8();
		u64ConnID = bh.ReadUint64();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint8(btConType);
		bh.WriteUint64(u64ConnID);
		return ;
	}
};
public class StDisConnectNotify
{
	public Byte btConType;
	public UInt64 u64ConnID;
	public void Read(ReadBufferHelper bh)
	{
		btConType = bh.ReadUint8();
		u64ConnID = bh.ReadUint64();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint8(btConType);
		bh.WriteUint64(u64ConnID);
		return ;
	}
};
public class StCharVerifyVersionReq
{
	public UInt32 dwClientVersion;
	public void Read(ReadBufferHelper bh)
	{
		dwClientVersion = bh.ReadUint32();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint32(dwClientVersion);
		return ;
	}
};
public class StCharVerifyVersionAck
{
	public UInt32 dwVerifyCode;
	public void Read(ReadBufferHelper bh)
	{
		dwVerifyCode = bh.ReadUint32();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint32(dwVerifyCode);
		return ;
	}
};
public class StCharNewAccountReq
{
	public string szAccountName;
	public string szPassword;
	public void Read(ReadBufferHelper bh)
	{
		szAccountName = bh.ReadFixString(32);
		szPassword = bh.ReadFixString(32);
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteFixString(szAccountName,32);
		bh.WriteFixString(szPassword,32);
		return ;
	}
};
public class StCharNewAccountAck
{
	public UInt16 nRetCode;
	public void Read(ReadBufferHelper bh)
	{
		nRetCode = bh.ReadUint16();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint16(nRetCode);
		return ;
	}
};
public class StCharNewCharReq
{
	public UInt32 dwAccountID;
	public string szCharName;
	public UInt32 dwFeature;
	public void Read(ReadBufferHelper bh)
	{
		dwAccountID = bh.ReadUint32();
		szCharName = bh.ReadFixString(32);
		dwFeature = bh.ReadUint32();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint32(dwAccountID);
		bh.WriteFixString(szCharName,32);
		bh.WriteUint32(dwFeature);
		return ;
	}
};
public class StCharDelCharReq
{
	public UInt32 dwAccountID;
	public UInt64 u64CharID;
	public void Read(ReadBufferHelper bh)
	{
		dwAccountID = bh.ReadUint32();
		u64CharID = bh.ReadUint64();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint32(dwAccountID);
		bh.WriteUint64(u64CharID);
		return ;
	}
};
public class StCharDelCharAck
{
	public UInt16 nRetCode;
	public UInt32 dwAccountID;
	public UInt64 u64CharID;
	public void Read(ReadBufferHelper bh)
	{
		nRetCode = bh.ReadUint16();
		dwAccountID = bh.ReadUint32();
		u64CharID = bh.ReadUint64();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint16(nRetCode);
		bh.WriteUint32(dwAccountID);
		bh.WriteUint64(u64CharID);
		return ;
	}
};
public class StCharPickInfo
{
	public UInt64 u64CharID;
	public string szCharName;
	public UInt32 dwLevel;
	public UInt32 dwFeature;
	public void Read(ReadBufferHelper bh)
	{
		u64CharID = bh.ReadUint64();
		szCharName = bh.ReadFixString(32);
		dwLevel = bh.ReadUint32();
		dwFeature = bh.ReadUint32();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint64(u64CharID);
		bh.WriteFixString(szCharName,32);
		bh.WriteUint32(dwLevel);
		bh.WriteUint32(dwFeature);
		return ;
	}
};
public class StCharNewCharAck
{
	public UInt16 nRetCode;
	public StCharPickInfo CharPickInfo= new StCharPickInfo();
	public void Read(ReadBufferHelper bh)
	{
		nRetCode = bh.ReadUint16();
		CharPickInfo.Read(bh);
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint16(nRetCode);
		CharPickInfo.Write(bh);
		return ;
	}
};
public class StCharLoginReq
{
	public string szAccountName;
	public string szPassword;
	public void Read(ReadBufferHelper bh)
	{
		szAccountName = bh.ReadFixString(32);
		szPassword = bh.ReadFixString(32);
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteFixString(szAccountName,32);
		bh.WriteFixString(szPassword,32);
		return ;
	}
};
public class StCharLoginAck
{
	public UInt16 nRetCode;
	public UInt32 dwAccountID;
	public Byte nCount;
	public StCharPickInfo[] CharPickInfo = new StCharPickInfo[4];
	public void Read(ReadBufferHelper bh)
	{
		nRetCode = bh.ReadUint16();
		dwAccountID = bh.ReadUint32();
		nCount = bh.ReadUint8();
        for (int i = 0; i < nCount; i++)
		{
            CharPickInfo[i] = new StCharPickInfo();
			CharPickInfo[i].Read(bh);
		}
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint16(nRetCode);
		bh.WriteUint32(dwAccountID);
		bh.WriteUint8(nCount);
        for (int i = 0; i < nCount; i++)
		{
			CharPickInfo[i].Write(bh);
		}
		return ;
	}
};
public class StCharPickCharReq
{
	public UInt64 u64CharID;
	public void Read(ReadBufferHelper bh)
	{
		u64CharID = bh.ReadUint64();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint64(u64CharID);
		return ;
	}
};
public class StCharPickCharAck
{
	public UInt16 nRetCode;
	public UInt64 u64CharID;
	public UInt32 dwIdentifyCode;
	public UInt16 nProxyPort;
	public string szProxyIpAddr;
	public void Read(ReadBufferHelper bh)
	{
		nRetCode = bh.ReadUint16();
		u64CharID = bh.ReadUint64();
		dwIdentifyCode = bh.ReadUint32();
		nProxyPort = bh.ReadUint16();
		szProxyIpAddr = bh.ReadFixString(32);
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint16(nRetCode);
		bh.WriteUint64(u64CharID);
		bh.WriteUint32(dwIdentifyCode);
		bh.WriteUint16(nProxyPort);
		bh.WriteFixString(szProxyIpAddr,32);
		return ;
	}
};
public class StCharEnterGameReq
{
	public UInt64 u64CharID;
	public UInt32 dwIdentifyCode;
	public void Read(ReadBufferHelper bh)
	{
		u64CharID = bh.ReadUint64();
		dwIdentifyCode = bh.ReadUint32();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint64(u64CharID);
		bh.WriteUint32(dwIdentifyCode);
		return ;
	}
};
public class StCharEnterGameAck
{
	public UInt32 dwSceneID;
	public void Read(ReadBufferHelper bh)
	{
		dwSceneID = bh.ReadUint32();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint32(dwSceneID);
		return ;
	}
};
public class StCharLeaveGameReq
{
	public UInt32 dwLeaveReason;
	public void Read(ReadBufferHelper bh)
	{
		dwLeaveReason = bh.ReadUint32();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint32(dwLeaveReason);
		return ;
	}
};
public class StCharLeaveGameAck
{
	public void Read(ReadBufferHelper bh)
	{
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		return ;
	}
};
public class StCharGmCmdReq
{
	public string szGMCommand;
	public void Read(ReadBufferHelper bh)
	{
		szGMCommand = bh.ReadFixString(32);
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteFixString(szGMCommand,32);
		return ;
	}
};
public class StCharMoveReq
{
	public UInt16 sDir;
	public Single x;
	public Single y;
	public Single z;
	public void Read(ReadBufferHelper bh)
	{
		sDir = bh.ReadUint16();
		x = bh.ReadFloat();
		y = bh.ReadFloat();
		z = bh.ReadFloat();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint16(sDir);
		bh.WriteFloat(x);
		bh.WriteFloat(y);
		bh.WriteFloat(z);
		return ;
	}
};
public class StCharHeartBeatReq
{
	public UInt32 dwReqTimestamp;
	public void Read(ReadBufferHelper bh)
	{
		dwReqTimestamp = bh.ReadUint32();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint32(dwReqTimestamp);
		return ;
	}
};
public class StCharHeartBeatAck
{
	public UInt32 dwReqTimestamp;
	public UInt32 dwServerTime;
	public void Read(ReadBufferHelper bh)
	{
		dwReqTimestamp = bh.ReadUint32();
		dwServerTime = bh.ReadUint32();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint32(dwReqTimestamp);
		bh.WriteUint32(dwServerTime);
		return ;
	}
};
