using System;
using System.Collections.Generic;
using System.Xml;

public enum EDropType
{
    ONERAN = 1,//随机掉落一定数量的单个物品
    MULFIX = 2,//固定掉落多个物品
    MULRAN = 3,//随机掉落多个物品
}

public enum ERecvType
{
    TYPE_RECV_ALL,//领取所有的
    TYPE_RECV_ONE,//几选一
}


public class ERewardState
{
    public const int NOT_RECEIVE = 0;
    public const int CAN_RECEIVE = 1;
    public const int HAS_RECEIVE = 2;
}

public class DAward : DObj<int>
{
    public int        Id;
    public string     Name;
    public EDropType  DropType;
    public ERecvType  RecvType;
    public string     DropItems;
    public int        MaxDropNum;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id         = element.GetInt("Id");
        this.Name       = element.GetString("Name");
        this.DropType   = (EDropType)element.GetInt("DropType");
        this.RecvType   = (ERecvType)element.GetInt("RecvType");
        this.DropItems  = element.GetString("DropItems");
        this.MaxDropNum = element.GetInt("MaxDropNum");
    }
}

public class ReadCfgAward : DReadBase<int, DAward>
{

}
