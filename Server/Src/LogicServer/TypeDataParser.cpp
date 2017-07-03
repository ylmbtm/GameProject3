#include <stdafx.h>
#include "TypeDataParser.h"

CGloalData::CGloalData()
{

}

CGloalData::~CGloalData()
{

}

CGloalData* CGloalData::GetInstancePtr()
{
	static CGloalData _StaticMgr;

	return &_StaticMgr;
}

BOOL CGloalData::InitDataParser()
{
	m_vtDataFuncList.push_back(DataFuncNode("sss", &CGloalData::ReadRoleType));
	return TRUE;
}

BOOL CGloalData::ReadRoleType(RecordSet *pRdSet)
{
	return TRUE;
}

BOOL CGloalData::ReadTypeData()
{
	for(std::vector<DataFuncNode>::iterator itor = m_vtDataFuncList.begin(); itor != m_vtDataFuncList.end(); itor++)
	{
		RecordSet rs;

		DataFunc pFun = (*itor).m_pDataFunc;

		(this->*pFun)(&rs);
	}

	return TRUE;
}
