#ifndef __PACKET__HEADER_H__
#define __PACKET__HEADER_H__

#pragma  pack(push)
#pragma  pack(1)

#define CLIENT_CODE 0x88
#define SERVER_CODE 0x99

struct PacketHeader
{
	UINT32	 CheckCode;
	UINT32   dwMsgID;
	UINT32   dwSize;
	UINT32   dwPacketNo;	//生成序号 = wCommandID^dwSize+index(每个包自动增长索引); 还原序号 = pHeader->dwPacketNo - pHeader->wCommandID^pHeader->dwSize;
	UINT64   u64TargetID;
	UINT32   dwUserData;	//客户端在副本中的情况下，dwUserData是copyguid， 在逻辑服的情况下， 部分协议充当客户端连接ID
};

#pragma  pack(pop)



#endif /* __PACKET__HEADER_H__*/
