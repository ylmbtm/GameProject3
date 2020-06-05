#ifndef __GAME_LOG_MANAGER_H__
#define __GAME_LOG_MANAGER_H__

class CGameLogManager
{
public:
	CGameLogManager();
	~CGameLogManager();

public:
	static CGameLogManager* GetInstancePtr();
};

#endif //__GAME_LOG_MANAGER_H__
