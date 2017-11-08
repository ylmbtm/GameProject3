#include "stdafx.h"
#include "SharedMemory.h"
#include "CommonConvert.h"
#include "CommonFunc.h"

ShareObject::ShareObject()
{
	m_dwCheckCode = BLOCK_CHECK_CODE;
	m_State = SMS_USE;
	m_updatetime = 0;
}

void ShareObject::lock()
{
	m_State = SMS_LOCK;
}

BOOL ShareObject::islock() const
{
	return m_State == SMS_LOCK;
}

void ShareObject::unlock()
{
	m_updatetime = time(NULL);
	m_State = SMS_USE;
}

void ShareObject::useit()
{
	m_State = SMS_USE;
}

void ShareObject::release()
{
	m_State = SMS_RELEASE;
}

void ShareObject::destroy()
{
	m_State = SMS_DELETE;
}

BOOL ShareObject::isDestroy() const
{
	return (m_State == SMS_DELETE);
}

BOOL ShareObject::isRelease() const
{
	return (m_State == SMS_RELEASE);
}

time_t ShareObject::getLastMotifyTime()
{
	return m_updatetime;
}

BOOL ShareObject::isUse() const
{
	return m_State != SMS_NONE;
}

void ShareObject::reset()
{
	m_State = SMS_NONE;
	m_updatetime = time(NULL);
}

/******************************************************************
每块共享内存都是由数据体和数据头组成,数据体在前面，数据头在后面T为体H为头
TTTTTTTTTTTTTTTTTTTTTTHHHHHHHHHHHHHHHHHHHHH
数据体和数据块的数量是一样的
********************************************************************/


BOOL SharedMemoryBase::NewPage()
{
	unsigned int size = m_countperPage * (m_space);
	std::string pagename = std::string(m_modulename) + CommonConvert::IntToString(m_pageCount);

	shareMemoryPage newpage;

	//newpage.m_shm = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, pagename.c_str());
	newpage.m_shm = CommonFunc::CreateShareMemory(pagename, size);
	if(newpage.m_shm == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	//newpage.m_pdata = (CHAR*)MapViewOfFile(newpage.m_shm, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
	newpage.m_pdata = (CHAR*)CommonFunc::GetShareMemory(newpage.m_shm);
	if(newpage.m_pdata == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	newpage.m_pBlock = (_SMBlock*)(newpage.m_pdata + m_rawblockSize * m_countperPage);
	m_pageCount++;
	m_count += m_countperPage;
	InitPage(newpage);
	m_ShareMemoryPageMapping.push_back(newpage);
	return TRUE;
}


//----------------------------------------------
void SharedMemoryBase::InitPage(shareMemoryPage& rPage)
{
	UINT32 size = 0;
	size = m_countperPage * m_rawblockSize;
	char* pdata = rPage.m_pdata;
	memset(pdata, 0, size);

	//设置防护区域
	for (INT32 i = 0; i != m_countperPage; ++i)
	{
		*(pdata + (m_rawblockSize)*i) = BLOCK_CHECK_CODE;
	}

	//m_pageCount已经包含当前页码，故需要减1
	UINT32 startindex = m_countperPage * (m_pageCount - 1);
	for (UINT32 i = startindex; i < m_count; ++i)
	{
		_SMBlock* ptem = &(rPage.m_pBlock[i - startindex]);
		new(ptem)(_SMBlock);
		ptem->m_dwIndex = i;
		m_mapSMBlock.insert(std::make_pair(i, ptem));
		m_mapFreeSMBlock.insert(std::make_pair(i, ptem));
	}
}



void SharedMemoryBase::InitToMap()
{
	if (!m_bEmpty)
	{
		for (UINT32 i = 0; i < m_count; ++i)
		{
			_SMBlock* ptem = GetSMBbyRawIndex(i);
			ShareObject* ptemdata = GetObjectByRawindex(i);
			if (ptem->m_bUse && (ptemdata->m_State == SMS_USE || ptemdata->m_State == SMS_LOCK))
			{
				m_mapUsedSMBlock.insert(std::make_pair(ptemdata, ptem));
			}
			else
			{
				m_mapFreeSMBlock.insert(std::make_pair(i, ptem));
			}
			m_mapSMBlock.insert(std::make_pair(i, ptem));
		}
	}

	else
	{
		if (m_ShareMemoryPageMapping.size() < 1)
		{
			m_count = 0;
			return;
		}

		shareMemoryPage& page = m_ShareMemoryPageMapping[0];

		InitPage(page);
	}
}

BOOL SharedMemoryBase::IsFirstCreated()
{
	return m_bEmpty;
}

void SharedMemoryBase::ImportOtherPage()
{
	while (1)
	{
		std::string pagename = std::string(m_modulename) + CommonConvert::IntToString(m_pageCount);

		shareMemoryPage page;
		//page.m_shm = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, pagename.c_str());
		page.m_shm = CommonFunc::OpenShareMemory(pagename);
		if(page.m_shm == NULL)
		{
			break;
		}
		//page.m_pdata = (CHAR*)MapViewOfFile(page.m_shm, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
		page.m_pdata = (CHAR*)CommonFunc::GetShareMemory(page.m_shm);
		if(page.m_pdata == NULL)
		{
			break;
		}
		///如果原来就已经存过，初始化数据
		///找到头数据块的头
		page.m_pBlock = (_SMBlock*)(page.m_pdata + m_rawblockSize * m_countperPage);
		m_ShareMemoryPageMapping.push_back(page);
		m_count += m_countperPage;
		m_pageCount++;
	}
}

/**@param name 共享内存名字，可以通过名字找回,暂时如果有只打开内存中已经有的。
*@param count  T的个数
*@param noCreate 不允许创建
*/
SharedMemoryBase::SharedMemoryBase(const std::string& name, UINT32 rawblockSize, UINT32 count, BOOL noCreate/*=false*/)
	: m_rawblockSize(rawblockSize), m_count(count), m_space(rawblockSize + sizeof(_SMBlock)), m_modulename(name)

{
	m_countperPage = m_count;
	UINT32 size = m_countperPage * (m_space);
	m_pageCount = 1;
	std::string pagename = std::string(m_modulename) + CommonConvert::IntToString(0);

	shareMemoryPage firstpage;
	//firstpage.m_shm = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, pagename.c_str());
	firstpage.m_shm = CommonFunc::OpenShareMemory(pagename);
	if(firstpage.m_shm != NULL)
	{
		//firstpage.m_pdata = (CHAR*)MapViewOfFile(firstpage.m_shm, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
		firstpage.m_pdata = (CHAR*)CommonFunc::GetShareMemory(firstpage.m_shm);
		if(firstpage.m_pdata != NULL)
		{
			firstpage.m_pBlock = (_SMBlock*)(firstpage.m_pdata + m_rawblockSize * m_countperPage);
			m_ShareMemoryPageMapping.push_back(firstpage);
			ImportOtherPage();
			m_bEmpty = FALSE;
		}
		else
		{
			UINT32 dwError = CommonFunc::GetLastError();
			printf("---error---:%d", dwError);
		}
	}
	else
	{
		if (!noCreate)
		{
			shareMemoryPage firstpage;

			//firstpage.m_shm = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, pagename.c_str());
			firstpage.m_shm = CommonFunc::CreateShareMemory(pagename, size);
			if(firstpage.m_shm == NULL)
			{
				printf("CreateFileMapping Failed!!!!");
			}
			//firstpage.m_pdata = (CHAR*)MapViewOfFile(firstpage.m_shm, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
			firstpage.m_pdata = (CHAR*)CommonFunc::GetShareMemory(firstpage.m_shm);

			///找到头数据块的头
			//#ifdef SHARED_BLOCK_CHECK
			//			firstpage.m_pBlock=(_SMBlock*)(firstpage.m_pdata+(m_rawblockSize+BLOCK_FLAG_SIZE)*m_countperPage);
			//#else
			firstpage.m_pBlock = (_SMBlock*)(firstpage.m_pdata + m_rawblockSize * m_countperPage);
			//#endif
			///清空所有内存;
			memset(firstpage.m_pdata, 0, size);
			InitPage(firstpage);
			m_ShareMemoryPageMapping.push_back(firstpage);
		}

		m_bEmpty = TRUE;
	}
}

SharedMemoryBase::~SharedMemoryBase()
{
	for (UINT32 r = 0; r < (UINT32)m_ShareMemoryPageMapping.size(); r++)
	{
		CommonFunc::ReleaseShareMemory(m_ShareMemoryPageMapping[r].m_pdata);
		CommonFunc::CloseShareMemory(m_ShareMemoryPageMapping[r].m_shm);
		m_ShareMemoryPageMapping[r].m_shm = INVALID_HANDLE_VALUE;
	}
}

const UINT32 SharedMemoryBase::GetCount() const
{
	return m_count;
}

UINT32 SharedMemoryBase::GetFreeCount() const
{
	return (UINT32)m_mapFreeSMBlock.size();
}

UINT32 SharedMemoryBase::GetUseCount() const
{
	return (UINT32)m_mapUsedSMBlock.size();
}

_SMBlock* SharedMemoryBase::GetSMBbyRawIndex(INT32 index)
{
	if (index < (INT32)m_count)
	{
		UINT32 whichpage = index / m_countperPage;
		UINT32 pageindex = index % m_countperPage;
		shareMemoryPage& page = m_ShareMemoryPageMapping[whichpage];
		return &(page.m_pBlock[pageindex]);
	}
	return NULL;
}

ShareObject* SharedMemoryBase::GetObjectByRawindex(UINT32 index)
{
	if (index < m_count)
	{
		UINT32 whichpage = index / m_countperPage;
		UINT32 pageindex = index % m_countperPage;
		shareMemoryPage& page = m_ShareMemoryPageMapping[whichpage];
		return reinterpret_cast<ShareObject*>(page.m_pdata + m_rawblockSize * pageindex);
	}
	else
	{
// 		std::string str = "index<count,get sharObject error,index:" + Helper::CommonConvert(index) + "count:" + Helper::CommonConvert(m_count);
// 		str += "shareName:" + m_modulename;
// 		xLogMessager::getSingleton().logMessage(str, Log_ErrorLevel);///add by dsq
	}
	return NULL;
}


const UINT32 SharedMemoryBase::GetRawMemoryBlockSize()
{
	ImportOtherPage();
	return m_count;
}

void SharedMemoryBase::ProcessCleanDirtyData()
{
	mapUsedSMBlock::iterator it = m_mapUsedSMBlock.begin();
	while (it != m_mapUsedSMBlock.end())
	{
		_SMBlock* pBlock = it->second;
		ShareObject* pobject = static_cast<ShareObject*>(it->first);
		if (!pobject->isUse())
		{
			pobject->reset();
			_SMBlock* pblock = it->second;
			pblock->m_bUse = FALSE;
			m_mapFreeSMBlock.insert(std::make_pair(pblock->m_dwIndex, pblock));
			it = m_mapUsedSMBlock.erase(it);
		}
		else
		{
			it++;
		}
	}
}

ShareObject* SharedMemoryBase::NewObject(BOOL isNewBlock/*=false*/)
{
	///如果未分配内存没有了,则开始处理脏数据
	if (m_mapFreeSMBlock.size() == 0)
	{
		ProcessCleanDirtyData();
	}
	if (m_mapFreeSMBlock.size() == 0)
	{
		if (NewPage())
		{
			return NewObject(isNewBlock);
		}
		else
		{
			return NULL;
		}
	}
	///从未分配的队列中拿出，加入到已分配队列当中
	mapFreeSMBlock::iterator it = m_mapFreeSMBlock.begin();
	while (it != m_mapFreeSMBlock.end())
	{
		_SMBlock* pBlock = it->second;;
		ShareObject* pobject = GetObjectByRawindex(pBlock->m_dwIndex);
		if (pobject == NULL)///add by dsq
		{
			++it;
			continue;
		}
		///判断是否是删除状态
		if (!pobject->isDestroy())
		{
			m_mapUsedSMBlock.insert(std::make_pair(pobject, pBlock));
			m_mapFreeSMBlock.erase(it);
			pBlock->m_bUse = TRUE;
			pBlock->m_bNewBlock = isNewBlock;

			pobject->useit();

			return pobject;
		}
		it++;
	}

	////以下为新加的代码 zm
	ProcessCleanDirtyData();

	if (NewPage())
	{
		return NewObject(isNewBlock);
	}

	return NULL;
}

BOOL SharedMemoryBase::DestoryObject(ShareObject* pobject)
{
	if (pobject == NULL)
	{
		return FALSE;
	}
	pobject->reset();
	mapUsedSMBlock::iterator it = m_mapUsedSMBlock.find(pobject);
	if (it == m_mapUsedSMBlock.end())
	{
		return FALSE;
	}
	_SMBlock* pblock = it->second;
	pblock->m_bUse = FALSE;
	m_mapUsedSMBlock.erase(it);
	m_mapFreeSMBlock.insert(std::make_pair(pblock->m_dwIndex, pblock));
	return TRUE;

}

