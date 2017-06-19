// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#ifndef __STDAFX_H__
#define __STDAFX_H__

#include <stdio.h>

#include <map>
#include <hash_map>
#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include <assert.h>
#include <set>

#define ASSERT_FAIELD (assert(false))
#define ASSERT(x)     (assert(x))

#include "Platform.h"

extern "C" 
{ 
#include "lua.h"  
#include "lualib.h"  
#include "lauxlib.h"  
}  

#pragma comment(lib, "lua51.lib")

#endif