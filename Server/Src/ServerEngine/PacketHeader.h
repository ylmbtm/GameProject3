#ifndef __PACKET__HEADER_H__
#define __PACKET__HEADER_H__

#pragma  pack(push)
#pragma  pack(1)

#define CODE_VALUE 0x88

struct PacketHeader
{
    INT32   CheckCode;
    INT32    nMsgID;
    INT32    nSize;
    INT32    nPacketNo;    //生成序号 = nMsgID^nSize+index(每个包自动增长索引); 还原序号 = pHeader->dwPacketNo - pHeader->nMsgID^pHeader->nSize;
    UINT64   u64TargetID;
    UINT32   dwUserData;    //客户端在副本中的情况下，dwUserData是copyguid， 在逻辑服的情况下， 部分协议充当客户端连接ID
};

struct PacketHeader2
{
    UINT32   CheckCode: 8;
    UINT32   dwSize : 24;
    UINT32   dwPacketNo : 8;    //生成序号 = nMsgID^nSize+index(每个包自动增长索引); 还原序号 = pHeader->dwPacketNo - pHeader->nMsgID^pHeader->nSize;
    UINT32   Compress : 1;
    UINT32   Encrypt : 1;
    UINT32   nMsgID: 22;
    UINT64   u64TargetID;
    UINT32   dwUserData;    //客户端在副本中的情况下，dwUserData是copyguid， 在逻辑服的情况下， 部分协议充当客户端连接ID
};


#pragma  pack(pop)



#endif /* __PACKET__HEADER_H__*/
