#ifndef _SCENELOGIC_BASE_H_
#define _SCENELOGIC_BASE_H_
class CScene;



class SceneLogicBase
{
public:
	SceneLogicBase()
	{
		
	}
	
	~SceneLogicBase()
	{
		
	}
public:
	virtual BOOL OnCreatePlayer()
	{
		return TRUE;
	}

	virtual BOOL OnCreateMonster()
	{
		return TRUE;
	}

	virtual BOOL OnCreatePet()
	{
		return TRUE;
	}

	virtual BOOL OnPlayerDie()
	{
		return TRUE;
	}

	virtual BOOL OnMonsterDie()
	{
		return TRUE;
	}

	virtual BOOL OnPetDie()
	{
		return TRUE;
	}

	virtual BOOL Update()
	{
		return TRUE;
	}

public:
	CScene *m_pScene;

};

#endif //_SCENELOGIC_BASE_H_


