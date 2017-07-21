#ifndef _SCENE_XML_MANAGER_H_
#define _SCENE_XML_MANAGER_H_

#include "Utility\RapidXml\rapidxml.h"
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
	

	std::map<string, rapidxml::xml_document<char>*> m_mapDocument;
};

#endif //_SCENE_MANAGER_H_
