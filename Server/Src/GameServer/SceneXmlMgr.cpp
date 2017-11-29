#include "stdafx.h"
#include "SceneXmlMgr.h"
#include "Log.h"

CSceneXmlManager::CSceneXmlManager()
{

}

CSceneXmlManager::~CSceneXmlManager()
{
	Uninit();
}

CSceneXmlManager* CSceneXmlManager::GetInstancePtr()
{
	static CSceneXmlManager _StaticValue;

	return &_StaticValue;
}

rapidxml::xml_document<char>* CSceneXmlManager::GetXmlDocument(std::string strXmlName)
{
	std::map<std::string, rapidxml::xml_document<char>*>::iterator itor = m_mapDocument.find(strXmlName);
	if (itor != m_mapDocument.end())
	{
		return itor->second;
	}

	std::string strPath = "Map\\";
	strPath += strXmlName;
	FILE* pFile = fopen(strPath.c_str(), "rb");
	ERROR_RETURN_NULL(pFile != NULL);

	char buff[204800] = {0};
	fseek (pFile, 0, SEEK_END);
	INT32 nSize = ftell (pFile);
	rewind (pFile);
	fread(buff, nSize, 1, pFile);
	fclose(pFile);
	buff[nSize + 1] = 0;

	rapidxml::xml_document<char>* pXMLDoc = new rapidxml::xml_document<char>();

	pXMLDoc->parse<0>(buff);

	m_mapDocument.insert(std::make_pair(strXmlName, pXMLDoc));

	return pXMLDoc;
}

BOOL CSceneXmlManager::Init()
{
	return TRUE;
}

BOOL CSceneXmlManager::Uninit()
{
	for (auto itor = m_mapDocument.begin(); itor != m_mapDocument.end(); ++itor)
	{
		rapidxml::xml_document<char>* pXMLDoc = itor->second;

		delete pXMLDoc;
	}

	m_mapDocument.clear();

	return TRUE;
}

