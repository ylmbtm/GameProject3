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
	UINT32 dwTypeID;
};


struct StLocalString
{
	StLocalString()
	{

	}
	UINT32  dwID;
	std::string strID;
	std::string Language[15];
};

#endif //__CONFIG_STRUCT_H__