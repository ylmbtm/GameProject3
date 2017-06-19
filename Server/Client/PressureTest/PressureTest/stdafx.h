// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <string>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <assert.h>

//#define ASSERT_FAIELD (ASSERT(false))

#define ASSERT_FAIELD (assert(false))

#define ASSERT assert

#pragma  comment(lib,"ws2_32")
#pragma  comment(lib, "Mswsock") 

// TODO: 在此处引用程序需要的其他头文件
