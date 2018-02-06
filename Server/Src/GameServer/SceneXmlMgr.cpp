#include "stdafx.h"
#include "SceneXmlMgr.h"

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
	rapidxml::xml_document<char>* pXMLDoc = NULL;
	std::map<std::string, rapidxml::xml_document<char>*>::iterator itor = m_mapDocument.find(strXmlName);
	if (itor != m_mapDocument.end())
	{
		pXMLDoc = itor->second;
	}
	else
	{
		std::string strPath = "Map/" + strXmlName;
		FILE* pFile = fopen(strPath.c_str(), "rb");
		ERROR_RETURN_NULL(pFile != NULL);
		fseek(pFile, 0, SEEK_END);
		INT32 nSize = ftell(pFile);
		rewind(pFile);
		char* pBuff = (char*)malloc(nSize + 1);
		memset(pBuff, 0, nSize + 1);
		fread(pBuff, nSize, 1, pFile);
		fclose(pFile);
		pXMLDoc = new rapidxml::xml_document<char>();
		pXMLDoc->parse<0>(pBuff);
		m_mapDocument.insert(std::make_pair(strXmlName, pXMLDoc));
		m_vtDocuBuff.push_back(pBuff);
	}

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

	for (auto itor = m_vtDocuBuff.begin(); itor != m_vtDocuBuff.end(); ++itor)
	{
		char* pBuff = *itor;
		free(pBuff);
	}

	m_vtDocuBuff.clear();

	return TRUE;
}

