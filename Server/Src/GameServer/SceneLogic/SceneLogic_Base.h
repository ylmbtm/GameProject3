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

	}

	virtual BOOL OnCreateMonster()
	{

	}

	virtual BOOL OnCreatePet()
	{

	}

	virtual BOOL OnPlayerDie()
	{

	}

	virtual BOOL OnMonsterDie()
	{

	}

	virtual BOOL OnPetDie()
	{

	}

	virtual BOOL Update()
	{

	}

public:
	CScene *m_pScene;

};

#endif //_SCENELOGIC_BASE_H_


