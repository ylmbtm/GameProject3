#include "stdafx.h"
#include "CharObject.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"



CCharObject::CCharObject()
{
	memset(m_szObjectName, 0, 1024);
}

CCharObject::~CCharObject()
{

}

