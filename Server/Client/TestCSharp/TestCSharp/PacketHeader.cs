using System;
public class TransferHeader
{
	public Byte CheckCode;
	public UInt16 dwSize;
	public void Read(ReadBufferHelper bh)
	{
		CheckCode = bh.ReadUint8();
		dwSize = bh.ReadUint16();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint8(CheckCode);
		bh.WriteUint16(dwSize);
		return ;
	}
};
public class CommandHeader
{
	public Byte CmdHandleID;
	public UInt16 wCommandID;
	public UInt32 dwSceneID;
	public UInt64 u64CharID;
	public UInt32 dwPacketNo;
	public void Read(ReadBufferHelper bh)
	{
		CmdHandleID = bh.ReadUint8();
		wCommandID = bh.ReadUint16();
		dwSceneID = bh.ReadUint32();
		u64CharID = bh.ReadUint64();
		dwPacketNo = bh.ReadUint32();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint8(CmdHandleID);
		bh.WriteUint16(wCommandID);
		bh.WriteUint32(dwSceneID);
		bh.WriteUint64(u64CharID);
		bh.WriteUint32(dwPacketNo);
		return ;
	}
};
