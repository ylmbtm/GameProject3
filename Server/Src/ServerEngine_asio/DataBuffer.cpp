#include "stdafx.h"
#include "DataBuffer.h"


CBufferManagerAll::CBufferManagerAll()
{

}

CBufferManagerAll::~CBufferManagerAll()
{

}

CBufferManagerAll* CBufferManagerAll::GetInstancePtr()
{
	static CBufferManagerAll BufferManagerAll;

	return &BufferManagerAll;
}

IDataBuffer* CBufferManagerAll::AllocDataBuff( int nSize )
{
	if(nSize < 1024)
	{
		return g_BufferManager1K.AllocDataBuff();
	}
	else if(nSize <2048)
	{
		return g_BufferManager2K.AllocDataBuff();
	}
	else if(nSize < 4096)
	{
		return g_BufferManager4K.AllocDataBuff();
	}
	else if(nSize < 8192)
	{
		return g_BufferManager8K.AllocDataBuff();
	}
	else if(nSize < 16384)
	{
		return g_BufferManager16K.AllocDataBuff();
	}
	else if(nSize < 32768)
	{
		return g_BufferManager32K.AllocDataBuff();
	}
	else if(nSize < 65536)
	{
		return g_BufferManager64K.AllocDataBuff();
	}

	return NULL;
}
