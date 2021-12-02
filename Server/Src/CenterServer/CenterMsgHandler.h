#ifndef _CENTER_MSG_HANDLER_H_
#define _CENTER_MSG_HANDLER_H_

class CCenterMsgHandler
{
public:
    CCenterMsgHandler();

    ~CCenterMsgHandler();

    BOOL        Init(INT32 nReserved);

    BOOL        Uninit();

    BOOL        DispatchPacket( NetPacket* pNetPacket);

public:
    //*********************消息处理定义开始******************************
    BOOL        OnMsgLogicSvrRegReq(NetPacket* pPacket);
    //*********************消息处理定义结束******************************
};

#endif //_CENTER_MSG_HANDLER_H_
