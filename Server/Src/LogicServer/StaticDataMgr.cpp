#include "stdafx.h"
#include "StaticDataMgr.h"

CStaticDataMgr::CStaticDataMgr(void)
{
}

CStaticDataMgr::~CStaticDataMgr(void)
{
	Release();
}

BOOL CStaticDataMgr::ProcessDBFile( std::string strDbFile )
{
	try
	{
		m_DBConnection.open(strDbFile.c_str());
	}
	catch(CppSQLite3Exception& e)  
	{  
		printf("%s",e.errorMessage());  
		return FALSE;
	}  

	CppSQLite3Query TableNames = m_DBConnection.execQuery("SELECT name FROM sqlite_master WHERE type='table' ORDER BY name");
	while(!TableNames.eof())
	{
		std::string strSql ="SELECT * from ";
		std::string strTbName = TableNames.getStringField("name", NULL);
		strSql += strTbName;

		CppSQLite3Query Tabledatas = m_DBConnection.execQuery(strSql.c_str());

		ProcessTable(strTbName, Tabledatas);
		
		TableNames.nextRow();
	}
	
	m_DBConnection.close();

	return TRUE;
}


BOOL CStaticDataMgr::GetFileList(std::string strDbDir, std::vector<std::string> &vtFileList)
{
	vtFileList.clear();
#ifdef WIN32
	WIN32_FIND_DATAA findData;
	HANDLE hFindHandle		= INVALID_HANDLE_VALUE;
	std::string findName	= strDbDir + "\\*";
	hFindHandle = FindFirstFileA(findName.c_str(), &findData);

	if( INVALID_HANDLE_VALUE == hFindHandle )
	{
		return FALSE;
	}

	do 
	{
		if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			continue;
		}

		vtFileList.push_back(findData.cFileName);
	} 
	while (FindNextFileA(hFindHandle, &findData));

	FindClose(hFindHandle);
	hFindHandle = INVALID_HANDLE_VALUE;
#else
	DIR *pDirHandle = opendir(strDbDir.c_str());
	if( NULL == pDirHandle )
	{
		return FALSE;
	}

	struct dirent *pItem = NULL;
	struct stat statbuf;
	while( (pItem = readdir(pDirHandle)) != NULL )
	{

		lstat(pItem->d_name, &statbuf);
		if(S_ISDIR(statbuf.st_mode))
		{
			continue;
		}

		vtFileList.push_back(findData.cFileName);
	}

	closedir(dirHandle);
	dirHandle = NULL;
#endif
	return TRUE;
}


BOOL CStaticDataMgr::Load(std::string strDbDir)
{
	std::vector<std::string> vtDbFileList;
	if(!GetFileList(strDbDir, vtDbFileList))
	{
		return FALSE;
	}

	for(size_t i = 0; i < vtDbFileList.size(); i++)
	{
		std::string strFileName = vtDbFileList[i];

		ProcessDBFile(strDbDir + "\\"+strFileName);
	}

	return TRUE;
}

BOOL CStaticDataMgr::Release()
{

	return TRUE;
}



//BOOL CStaticDataMgr::LoadAccountData( CppSQLite3Query &QueryData )
//{
//	QueryData.getStringField()
//
//	return TRUE;
//}

