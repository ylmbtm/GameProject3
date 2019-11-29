#include "stdafx.h"
#include "DataBuffer.h"


CBufferAllocator::CBufferAllocator()
{
	m_BufferManagerAny.SetEnablePool(FALSE);
}

CBufferAllocator::~CBufferAllocator()
{

}

CBufferAllocator* CBufferAllocator::GetInstancePtr()
{
	static CBufferAllocator BufferManagerAll;

	return &BufferManagerAll;
}

IDataBuffer* CBufferAllocator::AllocDataBuff( int nSize )
{
	if(nSize < 64)
	{
		return m_BufferManager64B.AllocDataBuff();
	}
	if(nSize < 128)
	{
		return m_BufferManager128B.AllocDataBuff();
	}
	if(nSize < 256)
	{
		return m_BufferManager256B.AllocDataBuff();
	}
	if(nSize < 512)
	{
		return m_BufferManager512B.AllocDataBuff();
	}
	if(nSize < 1024)
	{
		return m_BufferManager1K.AllocDataBuff();
	}
	else if(nSize < 2048)
	{
		return m_BufferManager2K.AllocDataBuff();
	}
	else if(nSize < 4096)
	{
		return m_BufferManager4K.AllocDataBuff();
	}
	else if(nSize < 8192)
	{
		return m_BufferManager8K.AllocDataBuff();
	}
	else if(nSize < 16384)
	{
		return m_BufferManager16K.AllocDataBuff();
	}
	else if(nSize < 32768)
	{
		return m_BufferManager32K.AllocDataBuff();
	}
	else if(nSize < 65536)
	{
		return m_BufferManager64K.AllocDataBuff();
	}

	return m_BufferManagerAny.AllocDataBuff();
}
