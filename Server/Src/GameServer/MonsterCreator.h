#ifndef _GAME_MONSTER_CREATOR_H_
#define _GAME_MONSTER_CREATOR_H_













class monsterCreator
{
public:
	monsterCreator(xBattleScene* scene);
	~monsterCreator() {};

	//解析波信息
	bool parserWaveData(rapidxml::xml_node<>* xmlFTGroup, const Point2d& bondcenter, const Point2d& bondsize, uint32 hardlevel, bool bLoadItemDrop = true);

	//解析机关
	bool parserTrapMachine(rapidxml::xml_node<>* xmlFTGroup, const Point2d& bondcenter, const Point2d& bondsize, uint32 hardlevel);

	//解析路障
	bool parserObstale(rapidxml::xml_node<>* xmlFTGroup, const Point2d& bondcenter, const Point2d& bondsize);

	///解析矿石;
	bool parseOreData(rapidxml::xml_node<>* xmlFTGroup, const Point2d& bondcenter, const Point2d& bondsize);

	///解析宝箱：
	bool parseTreasure(rapidxml::xml_node<>* xmlFTGroup, const Point2d& bondcenter, const Point2d& bondsize);

	///解析友方单位
	bool parseFriendData(rapidxml::xml_node<>* xmlFTGroup, const Point2d& bondcenter, const Point2d& bondsize);

	///解析怪物数量限制
	bool parseMonsterLimit(rapidxml::xml_node<>* xmlFTGroup);


	//解析拉福尔
	bool parserLoverData(rapidxml::xml_node<>* xmlFTGroup, const Point2d& bondcenter, const Point2d& bondsize, uint32 hardlevel, uint32& loverID );

	//是否所有波都结束了
	bool getisWaveFinish()const;

	/// 是否为最后一波
	bool isLasterWave();

	///获得第一个大波小波ID
	uint32 getFirstBigWaveSmallWaveID();

	//玩家移动
	void onPlayerMove(uint32 roleID, const Point2d& pos);

	//两点之间是否经过触发点
	void onPlayerMovePoint(uint32 roleID, const Point2d& stPos, const Point2d& enPos);

	//激活一波
	void onActiveCurrentWave(SM_BigWave& bigwave, uint32 roleID);

	//怪物死亡
	void onMonsterDie(uint32 id);

	//出怪
	int32 onPushMonsters(uint32 roleID, uint32 monsterID, uint32 count, uint32 type, uint32 camps, SM_WaveInfo* waveinfo);

	/// 计算随机怪
	bool onCalcRandPos();

	//获得当前波内容,可能存在一波里面有N小波同时触发
	bool getCurrentWaveData(SM_BigWave& bigwave, vecpsmallwave& data);

	//完成当前波
	void finishcurrentwave(SM_BigWave& bigwave);

	void update();

	bool isBossAllDie();

	void onPushFixMonsters();

	void onPushFixMonstersEx(bool bFlag);  //骑士团boss使用 bFlag 为true时刷怪
	//立即出当前等待倒计时出的波
	void onActiveWaitWaveNow();

	const int32 getFirstSmallWaveID() const;

	///死亡之塔专用,获得第一大波的当前小波数
	uint32 getCurrentFloor();

	//杀死机关,传入id,传出typeid
	bool killObstacle(uint32 id, SM_ObstaleData& data);

	///矿石采集完毕;传入id,传出typeid
	bool killOre(LONG64 id, SM_OreData& data);

	///得到矿石数据;
	void getOreMap(map_OreData& oreMap);

	///得到宝箱数据;
	void getAllTreasure(map_TreasureData& treasureMap);

	///客户端激活一大波怪物
	void activeBigWave(int32 bigwaveNum, int32 roleid);

	///获取副本怪物信息
	void getVecSamllSave(vecsmallwave& monsterInfo);

	//boss来临动画播完 标记开始刷怪
	void onBossComing(int32 bigwaveid, int32 smallwaveid);


	// 获取当前怪物的掉落
	void getDropItemFromMonster(vector<ObjCountDrop>& droplist, uint32 monsterID);

	/// 全部掉落
	void getAllDropItemFromMonster(vector<ObjCountDrop>& droplist);

	/// 获取掉落的物品
	void getDropItems(vector<ObjCountDrop>& droplist, uint32 monsterSceneID, bool bFlag = false);
	/// 获取出判定的物品
	void getDecideDropItems(vector<ObjCountDrop>& droplist, bool bFlag = false, int32 nWave = -1);

	/// 移动已经掉落的物品
	void removeDropItems(vector<ObjCountDrop>& droplist);

	// 判断怪物是否打完
	bool isKillAllMonster();

	// 当前场景有几只怪
	int32 getMonsterCount();

	// 怪物数据信息
	void getMonsterList(GM_Monster_list& pMonsterList);

	// 当前场景中杀死了几只怪
	int32 getKillMonsterCount();

	// 扫荡掉落
	void sweepFBDropItem(int32 nProfession, vector<ObjCountDrop>& vecDropItems);

	SM_BossData* getBossData() { return &m_bossdata; }

	//是否为友方单位
	bool IsFriendId(int32 id);

	/// 总权重计算
	int32 calcTotalWeight();
	/// 计算某一波的权重
	void calcWaveTotalWeight();
	int32 getWaveTotalWeight(int32 nWave);
	int32 getCurWaveWeight(int32 nWave);
	void setWaveTotalWeight(int32 nWave, int32 nMonsterWeight, bool bFlag = true);

	/// 当前怪物掉落的金钱
	void monsterDropCoin(const int32& nMonsterID, int32& nGold, int32& nDiamond);

	/// 波利副本的百分比掉落
	void poleyMonsterDrop(const int32& nMonsterID, int32& nDiamond);

	/// 当前怪物掉落堆
	void monsterDropStack(const int32& nMonsterID, int32& nStackGold, int32& nStackDiamond);

	/// 怪物数据
	mapdibm::iterator getMapdropItemsBindMonsterBegin() { return m_mapdropItemsBindMonster.begin(); }
	mapdibm::iterator getMapdropItemsBindMonsterEnd() { return m_mapdropItemsBindMonster.end(); }
	//得到友军单位最大索引值的ID
	int32 GetMaxIndexFriendId();
	//得到友军单位的loverid index在100-200之间
	int32 getFriendLoverId();

	///通知所有玩家wave信息,bool 是是否是开始
	void notifyWave(bool isstart, const SM_BigWave& bigwave, bool isbigwavestartorend);

	void notifywaveStart(xBattleObject* obj, void* pData);

	void notifywaveEnd(xBattleObject* obj, void* pData);

	///定量出怪
	void PushMonster(SM_WaveInfo* waveinfo);
	//刷新机关
	void PushTrapMachine();
	//是否有怪物活着
	bool HaveMonsterAlive(SM_WaveInfo* info);
	//剩余需要刷新怪物数量
	uint32 GetMonsterLeftCount(SM_WaveInfo* info);
	void insertoneWave(const SM_WaveInfo& data);
	bool isspecialbigwave(const SM_BigWave& bigwave);
	bool CheckAllFinished(const SM_BigWave& bigwave);

	//获取大波的信息
	SM_BigWave* getbigWave(uint32 bigwaveid);
	//获取小波是否为boss波
	bool isBossSmallWave(int32 bigwaveid, int32 smallwaveid);
	//获取小波中的延迟信息
	float GetSmallWaveDelayTime(int32 bigwaveid, int32 smallwaveid);
	//设置小波中boss是否刷出
	void setBossStartRefresh(int32 bigwaveid, int32 smallwaveid);

	/// 通过怪物的ID获取怪物的波数
	int32 getMonsterWaveFromMonsterSceneID(const uint32& monstersceneid);

	// 把场景中掉落的物品与怪物绑定
	void sceneDropItemBind(FirstDrop nFlag = FirstDrop::FD_FB_STATUS);

	// 把场景中掉落的物品与怪物绑定
	void sceneDropItemBind(int32 professional, int32 nFlag = FirstDrop::FD_FB_STATUS);
	// 记录当前场景中所有种类的怪
	void recordMonster();
	void recordMonster_fix();

	// 怪物数记录
	void recordMonsterNum(int32 nMonsterID, int32 nCount, int32 nMonsterWave, int32& nFluence);

	// 根据怪物类型定义物品的掉落概率
	void calcItemsDropRateFromMonsterType(vec_scene& vecSceneDrop);
	// 获取掉落物品对应的怪物类型
	bool getDropItemMonsterType(MonsterType& mtData, scene_drop vecSceneDrop, int32 nItemID, int32 nMonsterWave);
	// 掉落物品绑定到怪物身上
	void dropItemBindMonster(MonsterType& mtTmp, scene_drop sdData);

	void sortBigWave();

	void sortSmallWave();

	void takesort();

	void onStartBigWave(SM_BigWave& bigwave, uint32 roleID);

	//记录棋盘小波对应的怪物总数
	void RecordChessSmallWaveCount(int32 smallwave, int32 count);

	//得到棋盘的怪物数量和波数信息
	monsterWaveCount& GetChessWaveCount();
	///获取当前大波激活点
	bool getCurrentWaveActiveWaveRect(vecpsmallwave vecpwavs, std::vector<Rect2d>& vecRect);

	bool hasnextwave(SM_BigWave& bigwave);

	//木桶出怪逻辑
	void onPushObstaleMonster(xBattleObject* obj, const SM_ObstaleData& obstaleData);

	void onPushOreMonster(const SM_OreData& oreData);

	LONG64 m_oreGmid;

	//void notifyLoverReward(xBattleObject* obj,void* pData);
	//得到同屏怪物的最大数量
	uint32 GetMaxMonsterTotalCount() { return m_MaxMonsterTotal; }
	//得到同屏一类怪物的最大数量
	uint32 GetMaxMonsterSingleCount(int32 monsterId);
	//设置同屏最大的怪物数量
	void SetMaxMonsterCount(int32 count);
	//添加单个怪物数量限制
	void AddSingleMonster(int32 monsterid, int32 maxcount);

	/// 掉落组选择判定
	void selDropGroup(vec_scene& vecscenedrop);

	/// 掉落组选择判定
	void selDropGroup(vec_scene& vecscenedrop, int32 professional);
	/// 保存掉落物品
	void saveDropGroupItem(const scene_drop& scenedrop);

	/// 保存掉落物品
	void saveDropGroupItem(const scene_drop& scenedrop, int32 professional);

	/// 记录怪物死亡
	void recordMonsterDie(const uint32& monsterSceneID);

	/// 判断当前波的怪物是否全部死亡
	int32 getFinishDieWave(const uint32& monsterSceneID);

	/// 判断当前波的怪物是否全部死亡
	bool isWaveFinishDie();

	int32 getTotalWeight() { return m_nTotalWeight; }

	/// 怪物权重
	void setMonsterWeight(int32 nParam1, bool bAdd = true);
	int32 getMonsterWeight();

	/// 怪物类型特殊处理
	bool dropMonsterType(uint32 monsterid);

	/// 测试 B================================================================================
	/// 测试数据保存到日志
	void saveDropTestData(const DropItems& dItem);
	/// 打印
	void printfLogMessage();
	/// 测试 E================================================================================

protected:

	mapdibm			m_mapdropItemsBindMonster;	// 把要掉落的物品放到怪物身上
	mapDropItems	m_mapdropItemsBindWave;		// 把要掉落的物品绑定到波数上
	mapMonsterWave	m_mapMonsterWave;	/// 当前波的怪物死亡状态

	vecbigwave		m_wavedata;

	vecsmallwave	m_fixwaveinfo;//一出来就有的怪物信息
	vecsmallwave	m_trapwaveinfo;//机关信息

	SM_BossData		m_bossdata;

	xBattleScene*	m_scene;

	vecActiveBigWave m_vecActiveBigWave;

	map_obstaleData m_map_obstaleData;

	map_OreData m_map_oreData;
	map_TreasureData m_map_Treasure;
	xHashMap<int32, int32> m_friendMap;	//友方单位ID
	map<int32, int32> m_mapMonsterWeight;	/// 怪物类型对应的权重
	int32 m_nTotalWeight;					/// 总权重
	mapMonsterWaveWeight m_nWaveWeight;		/// 某一波的权重
	int32 m_nMonsterWeight;					/// 怪物权重
	uint32 m_MaxMonsterTotal;				//  同屏怪物的总数量
	map<int32, int32> m_MaxMonsterSingle;	 //  同屏一类怪物的数量
	monsterWaveCount m_MapMonsterWaveCount;	//怪物波数对应怪物总数量
	map_wave_rate every_wave_rate;			/// 每一个波的怪物掉落
	map<int32, int32> m_mapItem;
};

#endif