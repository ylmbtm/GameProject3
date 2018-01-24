#ifndef _SCENE_XML_MANAGER_H_
#define _SCENE_XML_MANAGER_H_

#include "RapidXml.h"

class CSceneXmlManager
{
	CSceneXmlManager();
	~CSceneXmlManager();

public:
	static CSceneXmlManager* GetInstancePtr();

	BOOL		Init();

	BOOL		Uninit();

	rapidxml::xml_document<char>*  GetXmlDocument(std::string strXmlName);
public:



	std::vector<char*>								     m_vtDocuBuff;
	std::map<std::string, rapidxml::xml_document<char>*> m_mapDocument;
};

#endif //_SCENE_MANAGER_H_
