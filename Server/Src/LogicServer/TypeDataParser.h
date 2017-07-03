#ifndef __TYPE_DATA_PARSER_H__
#define __TYPE_DATA_PARSER_H__

struct RecordSet {
    INT32 GetIntField(const char* szField, INT32 nNullValue=0);
    INT64 GetInt64Field(const char* szField, INT64 nNullValue=0);
    FLOAT GetFloatField(const char* szField, FLOAT fNullValue=0.0);
    const char* GetStringField(const char* szField, const char* szNullValue="");
};


class CGloalData;

typedef BOOL(CGloalData::*DataFunc)(RecordSet *pRdSet);

struct DataFuncNode 
{
    std::string m_strTbName;
    DataFunc m_pDataFunc;

    DataFuncNode(std::string sValue, DataFunc func)
    {
        m_strTbName = sValue;
        m_pDataFunc = func;
    }
};

class CGloalData
{
public:
    CGloalData();
	~CGloalData();
public:
	static CGloalData* GetInstancePtr();
	//读表基础方法
    BOOL InitDataParser();
	BOOL ReadTypeData(); 
	std::vector<DataFuncNode> m_vtDataFuncList;


public:
    BOOL ReadRoleType(RecordSet *pRdSet);



public:
};



#endif //__TYPE_DATA_PARSER_H__
