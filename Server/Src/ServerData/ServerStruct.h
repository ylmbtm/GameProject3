#ifndef __SERVER_STRUCT_H__
#define __SERVER_STRUCT_H__

struct IDataBase
{
	virtual BOOL Execut(std::string sql) = 0;
};



#endif //__SERVER_STRUCT_H__