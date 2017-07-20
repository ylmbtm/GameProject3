#ifndef __CONFIG_STRUCT_H__
#define __CONFIG_STRUCT_H__

struct  StConstantValue
{
	StConstantValue(){}

	UINT32 xxxx;
};


struct StCopyBase
{
	StCopyBase()
	{

	}

    UINT32 GetKey()
    {
        return dwTypeID;
    }

	UINT32 dwTypeID;
    UINT32 dwLogicType;
    UINT32 dwCostActID;
    UINT32 dwCostActNum;
    UINT32 dwAwardID;
    UINT32 dwBattleTimes;
};


struct StLocalString
{
	StLocalString()
	{

	}
	UINT32  dwID;
	std::string Language[15];
};

#endif //__CONFIG_STRUCT_H__