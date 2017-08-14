using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using DG.Tweening;
using Protocol;
using ACT;
using BVT;
using BVT.AI;

public class Character : ICharacter
{  
    public DActor                              Cfg            { get; set; }
    public DActorModel                         CfgModel       { get; set; }
    public GameObject                          Obj            { get; set; }
    public Transform                           ObjTrans       { get; set; }
    public Transform                           CacheFixEuler  { get; set; }
    public Transform                           CacheTransform { get; set; }

    public ulong                               GUID           { get; set; }
    public int                                 ID             { get; set; }
    public int                                 Carrer         { get; set; }
    public EBattleCamp                         Camp           { get; private set; }

    public EActorType                          Type           { get; private set; }
    public EActorSort                          Sort           { get; private set; }
    public EActorRace                          Race           { get; private set; }
    public EActorSex                           Sex            { get; private set; }
    public EActorUnit                          Unit           { get; private set; }
    public string                              Name           { get; private set; }
    public Int32                               Title          { get; private set; }
    public Int32                               Level          { get; private set; }
    public Int32                               Group          { get; private set; }

    public CharacterCommand                    Command        { get; private set; }
    public CharacterView                       View           { get; private set; }
    public CharacterSkill                      Skill          { get; private set; }
    public CharacterMove                       Move           { get; private set; }
    public CharacterAvatar                     Avatar         { get; private set; }
    public CharacterHUD                        HUD            { get; private set; }
    public CharacterNetwork                    Sync           { get; private set; }

    public Character                           Pet            { get; set; }
    public Character                           Host           { get; set; }
    public Character                           Target         { get; set; }
    public Character                           Vehicle        { get; set; }
    public Character                           Mount          { get; set; }
    public Character                           Partner1       { get; set; }
    public Character                           Partner2       { get; set; }

    public Callback<EDeadReason>               OnDead         { get; set; }
    public Callback                            OnDeadFinish   { get; set; }
    
    public Animator                            Anim           { get; private set; }
    public CharacterController                 CC             { get; private set; }
    public IStateMachine<Character, FSMState>  Machine        { get; private set; }

    public GTAction                            Action         { get; private set; }
    public GTCallback                          TailFunc       { get; private set; }
    public GTTransform                         BornData       { get; private set; }
    public CharacterAttr                       CurrAttr       { get; private set; }
    public NodeTree                            AI             { get; private set; }
    public GameObject                          MiniTag        { get; private set; }

    public bool                                IsMain         { get; private set; }
    public bool                                IsLoad         { get; private set; }
    public bool                                IsPause        { get; private set; }
    public bool                                IsStealth      { get; private set; }
    public bool                                IsRide         { get; private set; }
    public bool                                IsFixBody      { get; private set; }
    public bool                                IsSilent       { get; set; }
    public bool                                IsDivive       { get; set; }
    public bool                                IsAIEnabled    { get; private set; }
    public bool                                IsAIPaused     { get; private set; }
    public bool                                IsRagdoll      { get; private set; }
    public bool                                IsRemoteActor  { get; set; }

    private List<Character>                    mEnemys       = new List<Character>();
    private List<Character>                    mAllys        = new List<Character>();
    private Dictionary<EActorNature, bool>     mActorNatures = new Dictionary<EActorNature, bool>();
    private List<CharacterTaunt>               mThreatList   = new List<CharacterTaunt>();
    private List<float>                        mAttrPercents = new List<float>();

    public FSMState                            FSM
    {
        get { return Machine == null ? FSMState.FSM_IDLE : Machine.GetCurStateID(); }
    }

    public FSMState                            FSMPrev
    {
        get { return Machine == null ? FSMState.FSM_IDLE : Machine.GetPreStateID(); }
    }

    public EAIState                            AIState
    {
        get; set;
    }

    public float                               Height
    {
        get { return CC.height * CacheTransform.localScale.x; }
    }
 
    public float                               Radius
    {
        get { return CC.radius * CacheTransform.localScale.x; }
    }

    public float                               CurrSpeed
    {
        get { return CurrAttr == null ? 0 : CurrAttr.SPEED / 100f; }
    }

    public float                               BaseSpeed
    {
        get { return Cfg.Attrs[EAttr.SPEED] / 100f; }
    }

    public Vector3                             Dir
    {
        get { return CacheTransform.forward;  }
        set { CacheTransform.forward = value; }
    }

    public Vector3                             Euler
    {
        get { return CacheTransform.eulerAngles; }
        set { CacheTransform.eulerAngles = value; }
    }

    public Vector3                             Pos
    {
        get { return CacheTransform.position; }
        set { CacheTransform.position = value;}
    }

    public Vector3                             Scale
    {
        get { return CacheTransform.localScale; }
        set { CacheTransform.localScale = value; }
    }

    public Vector3                             FollowOffset
    {
        get;
        set;
    }

    public Character(int id, ulong guid, EActorType type, EBattleCamp camp, bool isMain)
    {
        this.ID           = id;
        this.GUID         = guid;
        this.Type         = type;
        this.Camp         = camp;
        this.IsMain       = isMain;
        this.Cfg          = ReadCfgActor.GetDataById(ID);
        this.CfgModel     = ReadCfgActorModel.GetDataById(Cfg.Model);
        this.CurrAttr     = new CharacterAttr();
        this.Command      = new CharacterCommand();
        this.Skill        = new CharacterSkill(this);
        this.TailFunc     = new GTCallback();
        this.Sync         = new CharacterNetwork(this);
    }

    public void Load(GTTransform bornData)
    {
        this.BornData = bornData;
        this.InitRoot();
        this.InitObj(CfgModel);;
        this.InitActorNature();
        this.InitLayer();
        this.InitMiniTag();
        this.InitAction();
        this.InitBoard();
        this.InitMove();
        this.InitCommands();
        this.InitAvatar();
        this.InitFSM();
        this.InitAI();
    }

    void LoadObj(ref GameObject go, DActorModel db)
    {
        if (db != null)
        {
            go = GTPoolManager.Instance.GetObject(db.Model);
        }
    }

    void InitRoot()
    {
        this.CacheTransform = new GameObject(ID.ToString()).transform;
        this.CacheFixEuler  = new GameObject(string.Format("Character_FixEuler_{0}_{1}", GUID, ID)).transform;
        this.Pos            = BornData.Pos;
        this.Euler          = BornData.Euler;
        this.View           = CacheTransform.gameObject.GET<CharacterView>();
        this.View.Owner     = this;
    }

    void InitObj(DActorModel db)
    {
        GameObject go = null;
        this.LoadObj(ref go, db);
        this.Obj  = go;
        this.ObjTrans = this.Obj.transform;
        this.ObjTrans.parent = CacheTransform;
        this.ObjTrans.localPosition = Vector3.zero;
        this.ObjTrans.localEulerAngles = Vector3.zero;
        this.ObjTrans.localScale = Vector3.one;
        CharacterController cc = Obj.GetComponent<CharacterController>();
        this.CC = this.CacheTransform.gameObject.GET<CharacterController>();
        this.CC.center = cc.center;
        this.CC.radius = cc.radius;
        this.CC.height = cc.height;
        this.CC.enabled = true;
        cc.enabled = false;
        this.IsLoad = true;
        this.CacheTransform.localScale = Vector3.one * CfgModel.ModelScale;
    }

    void InitActorNature()
    {
        this.mActorNatures = Cfg.Natures;
    }

    void InitActorEffect()
    {
        this.IsStealth  = false;
        this.IsRide     = false;
    }

    void InitAction()
    {
        this.Anim = Obj.GetComponent<Animator>();
        this.Anim.enabled = true;
        this.Anim.applyRootMotion = false;
        this.Anim.cullingMode = AnimatorCullingMode.CullUpdateTransforms;
        this.Action = new GTAction(Anim);
    }

    void InitFSM()
    {
        Machine = new IStateMachine<Character, FSMState>(this);
        Machine.AddState(FSMState.FSM_EMPTY,       new CharacterEmptyFSM());
        Machine.AddState(FSMState.FSM_IDLE,        new CharacterIdleFSM());
        Machine.AddState(FSMState.FSM_RUN,         new CharacterRunFSM());
        Machine.AddState(FSMState.FSM_WALK,        new CharacterWalkFSM());
        Machine.AddState(FSMState.FSM_JUMP,        new CharacterJumpFSM());
        Machine.AddState(FSMState.FSM_TALK,        new CharacterTalkFSM());

        Machine.AddState(FSMState.FSM_REBORN,      new CharacterRebornFSM());
        Machine.AddState(FSMState.FSM_MINE,        new CharacterMineFSM());
        Machine.AddState(FSMState.FSM_INTERACTIVE, new CharacterInterActiveFSM());

        Machine.AddState(FSMState.FSM_SKILL,       new CharacterSkillFSM());
        Machine.AddState(FSMState.FSM_HOOK,        new CharacterHookFSM());
        Machine.AddState(FSMState.FSM_GRAB,        new CharacterGrabFSM());
        Machine.AddState(FSMState.FSM_ROLL,        new CharacterRollFSM());
        Machine.AddState(FSMState.FSM_DANCE,       new CharacterDanceFSM());

        Machine.AddState(FSMState.FSM_FROST,       new CharacterFrostFSM());
        Machine.AddState(FSMState.FSM_FLOATING,    new CharacterFloatFSM());
        Machine.AddState(FSMState.FSM_FEAR,        new CharacterFearFSM());
        Machine.AddState(FSMState.FSM_STUN,        new CharacterStunFSM());
        Machine.AddState(FSMState.FSM_WOUND,       new CharacterWoundFSM());
        Machine.AddState(FSMState.FSM_BLIND,       new CharacterBlindFSM());
        Machine.AddState(FSMState.FSM_PARALY,      new CharacterParalyFSM());
        Machine.AddState(FSMState.FSM_SLEEP,       new CharacterSleepFSM());

        Machine.AddState(FSMState.FSM_BEATBACK,    new CharacterBeatBackFSM());
        Machine.AddState(FSMState.FSM_BEATDOWN,    new CharacterBeatDownFSM());
        Machine.AddState(FSMState.FSM_BEATFLY,     new CharacterBeatFlyFSM());
        Machine.AddState(FSMState.FSM_DEAD,        new CharacterDeadFSM());

        Machine.AddState(FSMState.FSM_RIDEMOVE,    new CharacterRideMoveFSM());
        Machine.AddState(FSMState.FSM_RIDEIDLE,    new CharacterRideIdleFSM());

        Machine.SetCurStateID(FSMState.FSM_EMPTY);
        Machine.Startup();
    }

    void InitLayer()
    {
        switch (Type)
        {
            case EActorType.PLAYER:
                NGUITools.SetLayer(CacheTransform.gameObject, IsMain ? GTLayer.LAYER_AVATAR : GTLayer.LAYER_PLAYER);
                break;
            case EActorType.MONSTER:
                NGUITools.SetLayer(CacheTransform.gameObject, GTLayer.LAYER_MONSTER);
                break;
            case EActorType.NPC:
                NGUITools.SetLayer(CacheTransform.gameObject, GTLayer.LAYER_NPC);
                break;
            case EActorType.PET:
                NGUITools.SetLayer(CacheTransform.gameObject, GTLayer.LAYER_PET);
                break;
            case EActorType.MOUNT:
                NGUITools.SetLayer(CacheTransform.gameObject, GTLayer.LAYER_MOUNT);
                break;
            case EActorType.PARTNER:
                NGUITools.SetLayer(CacheTransform.gameObject, GTLayer.LAYER_PARTNER);
                break;
        }
    }

    void InitBoard()
    {
        this.HUD = new CharacterHUD(this);
        this.HUD.SetHeight(Height * 1.0f);
    }

    void InitCommands()
    {
        this.Command.Add<CommandIdle>(Check);
        this.Command.Add<CommandMove>(Check);
        this.Command.Add<CommandUseSkill>(Check);
        this.Command.Add<CommandDead>(Check);
        this.Command.Add<CommandTalk>(Check);
        this.Command.Add<CommandFrost>(Check);
        this.Command.Add<CommandStun>(Check);
        this.Command.Add<CommandParaly>(Check);
        this.Command.Add<CommandSleep>(Check);
        this.Command.Add<CommandBlind>(Check);
        this.Command.Add<CommandFear>(Check);
        this.Command.Add<CommandFixBodyBegin>(Check);
        this.Command.Add<CommandFixBodyLeave>(Check);
        this.Command.Add<CommandWound>(Check);
        this.Command.Add<CommandBeatBack>(Check);
        this.Command.Add<CommandBeatDown>(Check);
        this.Command.Add<CommandBeatFly>(Check);
        this.Command.Add<CommandFloat>(Check);
        this.Command.Add<CommandHook>(Check);
        this.Command.Add<CommandGrab>(Check);
        this.Command.Add<CommandVariation>(Check);
        this.Command.Add<CommandRideBegin>(Check);
        this.Command.Add<CommandRideLeave>(Check);
        this.Command.Add<CommandJump>(Check);
        this.Command.Add<CommandStealthBegin>(Check);
        this.Command.Add<CommandStealthLeave>(Check);
        this.Command.Add<CommandReborn>(Check);
        this.Command.Add<CommandMine>(Check);
        this.Command.Add<CommandInterActive>(Check);
        this.Command.Add<CommandDance>(Check);
        this.Command.Add<CommandRoll>(Check);
    }

    void InitAvatar()
    {
        this.Avatar = new CharacterAvatar(this.ObjTrans);
    }

    void InitMove()
    {
        this.Move = new CharacterMove(this.CacheTransform);
        this.Move.Radius = Radius;
        this.Move.Height = Height;
        this.Move.Speed  = CurrSpeed;
    }

    void InitAI()
    {
        GameObject go = null;
        switch(Type)
        {
            case EActorType.MONSTER:
                go = GTResourceManager.Instance.Load<GameObject>("AI/AI1001", true);
                break;
            case EActorType.PLAYER:
                go = GTResourceManager.Instance.Load<GameObject>("AI/AI1002", true);
                break;
            case EActorType.PARTNER:
                go = GTResourceManager.Instance.Load<GameObject>("AI/AI1003", true);
                break;
        }
        if (go == null)
        {
            return;
        }
        go.transform.parent = CacheTransform;
        if (go != null)
        {
            AI = go.GetComponent<NodeTree>();            
        }
        if (AI != null)
        {
            AI.SetAgent(CacheTransform);
            AI.Blackboard.SetData(AIDefine.KEY_ATTACK_DIST, Skill.AttackDist);
            AI.Blackboard.SetData(AIDefine.KEY_WARNER_DIST, Skill.WarnerDist);
            AI.Blackboard.SetData(AIDefine.KEY_GOHOME_DIST, Skill.GoHomeDist);
        }
        this.DoExecuteAI();
        this.AIState = EAIState.IDLE;
    }

    void InitMiniTag()
    {
        GameObject go = null;
        if(IsMain)
        {
            go = GTResourceManager.Instance.Load<GameObject>("Model/Other/Sphere_Blue", true);
        }
        else
        {
            ETargetRelationShip bc = GetTargetCampRelationShip(GTWorld.Main);
            switch(bc)
            {
                case ETargetRelationShip.TYPE_ALLY:
                    go = GTResourceManager.Instance.Load<GameObject>("Model/Other/Sphere_Green", true);
                    break;
                case ETargetRelationShip.TYPE_ENEMY:
                    go = GTResourceManager.Instance.Load<GameObject>("Model/Other/Sphere_Red", true);
                    break;
                case ETargetRelationShip.TYPE_NEUTRAL:
                    go = GTResourceManager.Instance.Load<GameObject>("Model/Other/Sphere_Yellow", true);
                    break;
            }
        }
        if (go != null)
        {
            go.layer = GTLayer.LAYER_MINI;
            GTTools.ResetLocalTransform(go.transform, CacheTransform);
        }
    }

    public void Destroy()
    {

    }

    public void Execute()
    {
        if (CacheTransform == null || IsLoad == false || Machine == null)
        {
            return;
        }
        if (CacheFixEuler != null)
        {
            CacheFixEuler.position = this.CacheTransform.position;
        }
        if (Machine != null)
        {
            Machine.Execute();
        }
        if (Move != null)
        {
            Move.   Execute();
        }
        if (Skill != null)
        {
            Skill.  Execute();
        }
        if (TailFunc.CanInvoke())
        {
            TailFunc.Invoke();
        }
        if (Sync != null)
        {
            Sync.   Execute();
        }
    }

    public void Release()
    {
        this.Action. Release();
        this.Skill.  Release();
        this.Avatar. Release();
        this.Move.   Release();
        this.HUD.    Release();
        GTPoolManager.Instance.ReleaseGo(CfgModel.Model, Obj);
        UnityEngine.Object.Destroy(CacheTransform.gameObject);
    }

    public void Pause(bool pause)
    {
        if (pause == IsPause)
        {
            return;
        }
        Anim.enabled = !pause;
        View.enabled = !pause;
        Move.SetAgentActive(!pause);
        IsPause = pause;
    }

    Resp Check(CommandIdle cmd)
    {
        if (IsFSMLayer3() || IsFSMLayer2())
        {
            return Resp.TYPE_NO;
        }
        if (FSM == FSMState.FSM_SKILL)
        {
            return Resp.TYPE_NO;
        }
        ChangeFSM(FSMState.FSM_IDLE, cmd);      
        return Resp.TYPE_YES;
    }

    Resp Check(CommandMove cmd)
    {
        if (IsFSMLayer3() || IsFSMLayer2())
        {
            return Resp.TYPE_NO;
        }
        if (GetActorNature(EActorNature.CAN_MOVE) == false)
        {
            return Resp.TYPE_NO;
        }
        if (cmd.MoveType != EMoveType.MoveForce && Move.IsCanReachPosition(cmd.DestPosition) == false)
        {
            return Resp.TYPE_CANNOT_MOVETODEST;
        }
        if (FSM == FSMState.FSM_SKILL)
        {
            ActSkill skill = Skill.GetCurrent();
            if (skill == null)
            {
                return Resp.TYPE_NO;
            }
            else
            {
                if(skill.CanMove)
                {
                    DoNoAnimRun(cmd);
                    return Resp.TYPE_YES;
                }
                else
                {
                    return Resp.TYPE_NO;
                }
            }
        }
        if (IsFixBody == false)
        {
            if (CurrSpeed >= BaseSpeed * 0.65f)
            {
                ChangeFSM(FSMState.FSM_RUN, cmd);
            }
            else
            {
                ChangeFSM(FSMState.FSM_WALK, cmd);
            }
        }
        else
        {
            DoRun(cmd);
        }
        return Resp.TYPE_YES;
    }

    Resp Check(CommandUseSkill cmd)
    {
        if (IsSilent)
        {
            return Resp.TYPE_SKILL_SILENT;
        }
        if (IsFSMLayer3() || IsFSMLayer2())
        {
            return Resp.TYPE_NO;
        }
        if (FSM == FSMState.FSM_SKILL)
        {
            return Resp.TYPE_NO;
        }
        if(IsRide)
        {
            return Resp.TYPE_NO;
        }
        ActSkill s = Skill.GetSkill(cmd.ID);
        if (s == null)
        {
            return Resp.TYPE_SKILL_NOTFIND;
        }
        if (s.IsCD())
        {
            return Resp.TYPE_NO;
        }
        Resp resp = CanUsePowerByCostType(s.CostType, s.CostNum);
        if (resp != Resp.TYPE_YES)
        {
            return resp;
        }
        cmd.ExitCallback = Skill.ExitCurrentSkill;
        cmd.StopCallback = Skill.StopCurrentSkill;
        Machine.GetState(FSMState.FSM_SKILL).SetFSMTime(s.Duration);
        ChangeFSM(FSMState.FSM_SKILL, cmd);
        return Resp.TYPE_YES;
    }

    Resp Check(CommandJump cmd)
    {
        if (IsRide)
        {
            return Resp.TYPE_RIDE_ING;
        }
        if (IsFSMLayer3())
        {
            return Resp.TYPE_CANNOT_CONTROLSELF;
        }
        if (IsFSMLayer2())
        {
            return Resp.TYPE_NO;
        }
        if (FSM == FSMState.FSM_SKILL)
        {
            return Resp.TYPE_SKILL_CASTING;
        }
        if (FSM == FSMState.FSM_RUN || FSM == FSMState.FSM_WALK)
        {
            cmd.Direction = Dir ;
            cmd.Direction.Normalize();
        }
        else
        {
            cmd.Direction = Vector3.zero;
        }
        ChangeFSM(FSMState.FSM_JUMP, cmd);
        return Resp.TYPE_YES;
    }

    Resp Check(CommandDead cmd)
    {
        if (IsFSMSkill())
        {
            Skill.StopCurrentSkill();
        }
        ChangeFSM(FSMState.FSM_DEAD, cmd);
        return Resp.TYPE_YES;
    }

    Resp Check(CommandTalk cmd)
    {
        if (IsRide)
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMLayer3())
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMLayer2())
        {
            return Resp.TYPE_NO;
        }
        if (FSM == FSMState.FSM_SKILL)
        {
            return Resp.TYPE_NO;
        }
        ChangeFSM(FSMState.FSM_TALK, cmd);
        return Resp.TYPE_YES;
    }

    Resp Check(CommandWound cmd)
    {
        if (IsRide)
        {
            return Resp.TYPE_NO;
        }
        if (IsDivive)
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMLayer3())
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMSkill())
        {
            Skill.StopCurrentSkill();
        }
        ChangeFSM(FSMState.FSM_WOUND, cmd);
        return Resp.TYPE_YES;
    }

    Resp Check(CommandBeatBack cmd)
    {
        if (IsRide)
        {
            return Resp.TYPE_NO;
        }
        if (IsDivive)
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMLayer3())
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMSkill())
        {
            Skill.StopCurrentSkill();
        }
        ChangeFSM(FSMState.FSM_BEATBACK, cmd);
        return Resp.TYPE_YES;
    }

    Resp Check(CommandBeatFly cmd)
    {
        if (IsRide)
        {
            return Resp.TYPE_NO;
        }
        if (IsDivive)
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMLayer3())
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMSkill())
        {
            Skill.StopCurrentSkill();
        }
        ChangeFSM(FSMState.FSM_BEATFLY, cmd);
        return Resp.TYPE_YES;
    }

    Resp Check(CommandBeatDown cmd)
    {
        if (IsRide)
        {
            return Resp.TYPE_NO;
        }
        if (IsDivive)
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMLayer3())
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMSkill())
        {
            Skill.StopCurrentSkill();
        }
        ChangeFSM(FSMState.FSM_BEATDOWN, cmd);
        return Resp.TYPE_YES;
    }

    Resp Check(CommandFloat cmd)
    {
        if (IsRide)
        {
            return Resp.TYPE_NO;
        }
        if (IsDivive)
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMLayer3())
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMSkill())
        {
            Skill.StopCurrentSkill();
        }
        ChangeFSM(FSMState.FSM_FLOATING, cmd);
        return Resp.TYPE_YES;
    }

    Resp Check(CommandHook cmd)
    {
        if (IsRide)
        {
            return Resp.TYPE_NO;
        }
        if (IsBoss())
        {
            return Resp.TYPE_NO;
        }
        if (IsDivive)
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMSkill())
        {
            Skill.StopCurrentSkill();
        }
        return Resp.TYPE_YES;
    }

    Resp Check(CommandGrab cmd)
    {
        if (IsRide)
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMLayer3())
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMLayer2())
        {
            return Resp.TYPE_NO;
        }
        ChangeFSM(FSMState.FSM_GRAB, cmd);
        return Resp.TYPE_YES;
    }

    Resp Check(CommandFrost cmd)
    {
        if (IsRide)
        {
            return Resp.TYPE_NO;
        }
        if (IsDivive)
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMLayer3())
        {
            return Resp.TYPE_NO;
        }
        Machine.GetState(FSMState.FSM_FROST).SetFSMTime(cmd.LastTime);
        ChangeFSM(FSMState.FSM_FROST, cmd);
        return Resp.TYPE_YES;
    }

    Resp Check(CommandStun cmd)
    {
        if (IsRide)
        {
            return Resp.TYPE_NO;
        }
        if (IsDivive)
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMLayer3())
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMSkill())
        {
            Skill.StopCurrentSkill();
        }
        Machine.GetState(FSMState.FSM_STUN).SetFSMTime(cmd.LastTime);
        ChangeFSM(FSMState.FSM_STUN, cmd);
        return Resp.TYPE_YES;
    }

    Resp Check(CommandParaly cmd)
    {
        if (IsRide)
        {
            return Resp.TYPE_NO;
        }
        if (IsDivive)
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMLayer3())
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMSkill())
        {
            Skill.StopCurrentSkill();
        }
        Machine.GetState(FSMState.FSM_PARALY).SetFSMTime(cmd.LastTime);
        ChangeFSM(FSMState.FSM_PARALY, cmd);
        return Resp.TYPE_YES;
    }

    Resp Check(CommandSleep cmd)
    {
        if (IsRide)
        {
            return Resp.TYPE_NO;
        }
        if (IsDivive)
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMLayer3())
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMSkill())
        {
            Skill.StopCurrentSkill();
        }
        Machine.GetState(FSMState.FSM_SLEEP).SetFSMTime(cmd.LastTime);
        ChangeFSM(FSMState.FSM_SLEEP, cmd);
        return Resp.TYPE_YES;
    }

    Resp Check(CommandBlind cmd)
    {
        if (IsRide)
        {
            return Resp.TYPE_NO;
        }
        if (IsDivive)
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMLayer3())
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMSkill())
        {
            Skill.StopCurrentSkill();
        }
        Machine.GetState(FSMState.FSM_BLIND).SetFSMTime(cmd.LastTime);
        ChangeFSM(FSMState.FSM_BLIND, cmd);
        return Resp.TYPE_YES;
    }

    Resp Check(CommandFear cmd)
    {
        if (IsRide)
        {
            return Resp.TYPE_NO;
        }
        if (IsDivive)
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMLayer3())
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMSkill())
        {
            Skill.StopCurrentSkill();
        }
        Machine.GetState(FSMState.FSM_FEAR).SetFSMTime(cmd.LastTime);
        ChangeFSM(FSMState.FSM_FEAR, cmd);
        return Resp.TYPE_YES;
    }

    Resp Check(CommandFixBodyBegin cmd)
    {
        if (IsRide)
        {
            return Resp.TYPE_NO;
        }
        if (IsDivive)
        {
            return Resp.TYPE_NO;
        }
        if (FSM == FSMState.FSM_RUN || FSM == FSMState.FSM_WALK)
        {
            ChangeEmptyFSM();
        }
        this.IsFixBody = true;
        return Resp.TYPE_YES;
    }

    Resp Check(CommandFixBodyLeave cmd)
    {
        this.IsFixBody = false;
        return Resp.TYPE_YES;
    }

    Resp Check(CommandRideBegin cmd)
    {
        DoRideStart(cmd);
        IsRide = true;
        return Resp.TYPE_YES;
    }

    Resp Check(CommandRideLeave cmd)
    {
        IsRide = false;
        DoRideLeave(cmd);
        return Resp.TYPE_YES;
    }

    Resp Check(CommandReborn cmd)
    {
        if (FSM != FSMState.FSM_DEAD)
        {
            return Resp.TYPE_NO;
        }
        if (FSM == FSMState.FSM_REBORN)
        {
            return Resp.TYPE_NO;
        }
        ChangeFSM(FSMState.FSM_REBORN, cmd);
        return Resp.TYPE_YES;
    }

    Resp Check(CommandInterActive cmd)
    {
        if (IsRide)
        {
            return Resp.TYPE_RIDE_ING;
        }
        if (IsFSMLayer3())
        {
            return Resp.TYPE_CANNOT_CONTROLSELF;
        }
        if (IsFSMLayer2())
        {
            return Resp.TYPE_RIDE_NOTDOATFSM;
        }
        if (FSM == FSMState.FSM_SKILL)
        {
            return Resp.TYPE_SKILL_CASTING;
        }
        if (FSM == FSMState.FSM_INTERACTIVE)
        {
            return Resp.TYPE_NO;
        }
        ChangeFSM(FSMState.FSM_INTERACTIVE, cmd);
        return Resp.TYPE_YES;
    }

    Resp Check(CommandMine cmd)
    {
        if (IsRide)
        {
            return Resp.TYPE_RIDE_ING;
        }
        if (IsFSMLayer3())
        {
            return Resp.TYPE_CANNOT_CONTROLSELF;
        }
        if (IsFSMLayer2())
        {
            return Resp.TYPE_RIDE_NOTDOATFSM;
        }
        if (FSM == FSMState.FSM_SKILL)
        {
            return Resp.TYPE_SKILL_CASTING;
        }
        if(FSM==FSMState.FSM_MINE)
        {
            return Resp.TYPE_NO;
        }
        ChangeFSM(FSMState.FSM_MINE, cmd);
        return Resp.TYPE_YES;
    }

    Resp Check(CommandRoll cmd)
    {
        if (IsRide)
        {
            return Resp.TYPE_RIDE_ING;
        }
        if (IsFSMLayer3())
        {
            return Resp.TYPE_CANNOT_CONTROLSELF;
        }
        if (IsFSMLayer2())
        {
            return Resp.TYPE_NO;
        }
        if (FSM == FSMState.FSM_SKILL)
        {
            return Resp.TYPE_SKILL_CASTING;
        }
        ChangeFSM(FSMState.FSM_ROLL, cmd);
        return Resp.TYPE_YES;
    }

    Resp Check(CommandDance cmd)
    {
        if (IsRide)
        {
            return Resp.TYPE_RIDE_ING;
        }
        if (IsFSMLayer3())
        {
            return Resp.TYPE_CANNOT_CONTROLSELF;
        }
        if (IsFSMLayer2())
        {
            return Resp.TYPE_NO;
        }
        if (FSM == FSMState.FSM_SKILL)
        {
            return Resp.TYPE_SKILL_CASTING;
        }
        if (FSM == FSMState.FSM_DANCE)
        {
            return Resp.TYPE_NO;
        }
        ChangeFSM(FSMState.FSM_DANCE, cmd);
        return Resp.TYPE_YES;
    }

    Resp Check(CommandStealthBegin cmd)
    {
        if (IsRide)
        {
            return Resp.TYPE_RIDE_ING;
        }
        if(IsStealth)
        {
            return Resp.TYPE_NO;
        }
        if (IsFSMLayer3())
        {
            return Resp.TYPE_CANNOT_CONTROLSELF;
        }
        if (IsFSMLayer2())
        {
            return Resp.TYPE_NO;
        }
        IsStealth = true;
        DoBeginStealth(cmd);
        return Resp.TYPE_YES;
    }

    Resp Check(CommandStealthLeave cmd)
    {
        if(!IsStealth)
        {
            return Resp.TYPE_NO;
        }
        IsStealth = false;
        DoLeaveStealth(cmd);
        return Resp.TYPE_YES;
    }

    Resp Check(CommandVariation cmd)
    {
        return Resp.TYPE_NO;
    }

    public void DoEmpty()
    {
        if (IsRide)
        {
            ChangeFSM(FSMState.FSM_RIDEIDLE, null);
        }
        else
        {
            ChangeFSM(FSMState.FSM_IDLE,     null);
        }
    }

    public void DoRun(CommandMove cmd)
    {
        if (IsFixBody)
        {
            CacheTransform.LookAt(Pos + cmd.Motion);

            Move.Stop();
            return;
        }
        if (cmd.MoveType == EMoveType.MoveForce)
        {
            Move.Stop();
            CacheTransform.DOLookAt(Pos + cmd.Motion, 0.5f);
            CC.SimpleMove(Dir * CurrSpeed);
            Action.Play("run", null, true);
        }
        else
        {
            Move.AddListener(DoEmpty);
            Move.AddListener(cmd.OnFinish);
            Move.Speed = CurrSpeed;
            Move.SetDestination(cmd.DestPosition);
            Action.Play("run", null, true);
        }
        if (Type == EActorType.MOUNT && Host != null)
        {
            Host.ChangeFSM(FSMState.FSM_RIDEMOVE, null);
        }
    }

    public void DoNoAnimRun(CommandMove cmd)
    {
        if (IsFixBody)
        {
            CacheTransform.LookAt(Pos + cmd.Motion);
            Move.Stop();
            return;
        }
        if (cmd.MoveType == EMoveType.MoveForce)
        {
            Move.Stop();
            CacheTransform.LookAt(Pos + cmd.Motion);
            CC.SimpleMove(Dir * CurrSpeed);
        }
        else
        {
            Move.AddListener(DoEmpty);
            Move.AddListener(cmd.OnFinish);
            Move.Speed = CurrSpeed;
            Move.SetDestination(cmd.DestPosition);
        }
    }

    public void DoWalk(CommandMove cmd)
    {
        if (cmd.MoveType == EMoveType.MoveForce)
        {
            Move.Stop();
            CacheTransform.LookAt(Pos + cmd.Motion);
            CC.SimpleMove(Dir * CurrSpeed);
            Action.Play("walk", null, true);
        }
        else
        {
            Move.AddListener(DoEmpty);
            Move.AddListener(cmd.OnFinish);
            Move.Speed = CurrSpeed;
            Move.SetDestination(cmd.DestPosition);
            Action.Play("walk", null, true);
        }
        if (Type == EActorType.MOUNT && Host != null)
        {
            Host.ChangeFSM(FSMState.FSM_RIDEMOVE, null);
        }
    }

    public void DoIdle(CommandIdle cmd)
    {
        Move.Stop();
        Action.Play("idle", null, true);
        if (Type == EActorType.MOUNT && Host != null)
        {
            Host.ChangeFSM(FSMState.FSM_RIDEIDLE, null);
        }
    }

    public void DoJump(CommandJump cmd)
    {
        Move.Stop();
        Action.Play("jump", ChangeEmptyFSM);
        if (cmd.Direction != Vector3.zero)
        {
            float duration = Action.GetLength("jump");
            float maxDir = CurrSpeed * duration;
            Vector3  endValue = Pos + cmd.Direction * maxDir;
            CheckMoveEndPointInFront(ref endValue, maxDir);
            CacheTransform.DOMove(endValue, duration);
        }
    }

    public void DoDead(CommandDead cmd)
    {
        this.AIState = EAIState.DEAD;
        if (OnDead != null)
        {
            OnDead.Invoke(EDeadReason.Normal);
        }
        if (Move != null)
        {
            Move.Release();
        }
        if (AI != null)
        {
            AI.enabled = false;
        }
        if (CC != null)
        {
            CC.enabled = false;
        }
        TailFunc.AddListener(Skill.Release);
        Action.Play("die", OnDeadFinish);
        Avatar.SetShadowActive(false);
    }

    public void DoTalk(CommandTalk cmd)
    {
        Move.Stop();
        Action.Play("badao", ChangeEmptyFSM);
    }

    public void DoUseSkill(CommandUseSkill cmd)
    {
        Skill.UseSkillById(cmd.ID);      
    }

    public void DoBeatDown(CommandBeatDown cmd)
    {
        Move.Stop();
        Action.Play("down", ChangeEmptyFSM);
    }

    public void DoBeatBack(CommandBeatBack cmd)
    {
        Move.Stop();
        Action.Play("hit", ChangeEmptyFSM);
        Vector3 endValue = Pos - Dir.normalized * cmd.MaxDis;
        CheckMoveEndPointInBack(ref endValue, cmd.MaxDis);
        CacheTransform.DOMove(endValue, 0.3f);
    }

    public void DoBeatFly(CommandBeatFly cmd)
    {
        Move.Stop();
        Action.Play("fly", ChangeEmptyFSM);
    }

    public void DoBlind(CommandBlind cmd)
    {
        Move.Stop();
        Action.Play("yun", null, true);
    }

    public void DoFear(CommandFear cmd)
    {
        Move.Stop();
        Action.Play("yun", null, true);
    }

    public void DoWound(CommandWound cmd)
    {
        Move.Stop();
        Action.Play("hit", ChangeEmptyFSM);
    }

    public void DoFloatStart(CommandFloat cmd)
    {
        Move.Stop();
        Action.Play("getup", ChangeEmptyFSM);
    }

    public void DoFloatLeave()
    {

    }

    public void DoReborn(CommandReborn cmd)
    {
        Move.Stop();
        Action.Play("fuhuo", ChangeEmptyFSM);
    }

    public void DoSleep(CommandSleep cmd)
    {
        Move.Stop();
        Action.Play("yun", null, true);
    }

    public void DoParaly(CommandParaly cmd)
    {
        Move.Stop();
        Action.Play("yun", null, true);
    }

    public void DoMine(CommandMine cmd)
    {
        Move.Stop();
        Action.Play(new List<string>{"xuli_01", "xuli_02" }, ChangeEmptyFSM);
    }

    public void DoInterActive(CommandInterActive cmd)
    {
        Move.Stop();
        Action.Play("badao", ChangeEmptyFSM);
    }

    public void DoStun(CommandStun cmd)
    {
        Move.Stop();
        Action.Play("yun", null, true);
    }

    public void DoRideStart(CommandRideBegin cmd)
    {
        Move.Stop();
        CacheTransform.parent = Mount.Avatar.GetRidePoint();
        CacheTransform.localPosition = Vector3.zero;
        CacheTransform.localEulerAngles = Vector3.zero;
        ChangeFSM(FSMState.FSM_RIDEIDLE, null);
        HUD.SetHeight(Height * 0.7f);
        GTAudioManager.Instance.PlaySound(GTAudioKey.SOUND_RIDE_START);
    }

    public void DoRideLeave(CommandRideLeave cmd)
    {
        Move.Stop();
        Vector3 pos = GTTools.NavSamplePosition(Pos);
        this.Pos   = pos;
        this.Euler = Mount.Euler;
        Action.Play("idle", null, true);
        HUD.SetHeight(Height * 1.0f);
        GTAudioManager.Instance.PlaySound(GTAudioKey.SOUND_RIDE_LEAVE);
    }

    public void DoRideIdle()
    {
        Move.Stop();
        Action.Play("qicheng", null, true);
    }

    public void DoRideMove()
    {
        Move.Stop();
        Action.Play("qicheng_run", null, true);
    }

    public void DoUpDown()
    {
        Move.Stop();
        Action.Play("up_down", ChangeEmptyFSM);
    }

    public void DoDance(CommandDance cmd)
    {
        Move.Stop();
        int r = UnityEngine.Random.Range(0, 2);
        if (r == 0)
        {

            Action.Play("denglu_pose", ChangeEmptyFSM);
        }
        else
        {
            Action.Play("denglu_huiwei", ChangeEmptyFSM);
        }
    }

    public void DoRoll(CommandRoll cmd)
    {
        Move.Stop();
        Action.Play("miss", ChangeEmptyFSM);
        CacheTransform.DOMove(Pos + Dir.normalized * 20, 0.5f);
    }

    public void DoGrab(CommandGrab cmd)
    {
        Move.Stop();
        Action.Play(new List<string> { "zhua_01", "zhua_02", "zhua_a_01", "zhua_a_02"}, ChangeEmptyFSM);
    }

    public void DoHook(CommandHook cmd)
    {

    }

    public void DoBeginStealth(CommandStealthBegin cmd)
    {
        Avatar.SetTransparentVertLitOn();
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_STEALTH_START);
    }

    public void DoLeaveStealth(CommandStealthLeave cmd)
    {
        Avatar.SetTransparentVertLitOff();
        Avatar.SetShadowActive(true);
        HUD.   SetActive(true);
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_STEALTH_LEAVE);
    }

    public void DoExecuteAI()
    {
        if (AI == null)
        {
            return;
        }
        if (IsAIEnabled)
        {
            if(IsAIPaused)
            {
                this.AI.enabled = false;
                this.AI.Release();
            }
            else
            {
                this.AI.enabled = true;
            }
        }
        else
        {
            this.AI.enabled = false;
            this.AI.Release();
        }
    }

    public void EnableAnimator(bool enabled)
    {
        if (Anim != null)
        {
            Anim.enabled = enabled;
        }
    }

    public void EnableCharacter(bool enabled)
    {
        if (CC != null)
        {
            CC.enabled = enabled;
        }
    }

    public void EnableRootMotion(bool enabled)
    {
        if (Anim != null)
        {
            Anim.applyRootMotion = enabled;
        }
    }

    public void ChangeFSM(FSMState newFSM, ICommand cmd)
    {
        if (Machine == null || CacheTransform == null)
        {
            return;
        }
        if (FSM == FSMState.FSM_DEAD && newFSM != FSMState.FSM_REBORN)
        {
            return;
        }
        IState<Character, FSMState> state = Machine.GetState(newFSM);
        if (state != null)
        {
            state.SetCommand(cmd);
            Machine.ChangeState(newFSM);
        }
    }

    public void ChangeEmptyFSM()
    {
        ChangeFSM(FSMState.FSM_EMPTY, null);
    }

    public void EnableAI(bool isEnableAI)
    {
        if (this.IsAIEnabled == isEnableAI)
        {
            return;
        }
        this.IsAIEnabled = isEnableAI;
        this.DoExecuteAI();
    }

    public void PausedAI(bool isPausedAI)
    {
        if (this.IsAIPaused == isPausedAI)
        {
            return;
        }
        this.IsAIPaused = isPausedAI;
        this.DoExecuteAI();
    }

    public bool IsEnemy(Character actor)
    {
        return false;
    }

    public bool IsAlly(Character actor)
    {
        return false;
    }

    public bool IsBoss()
    {
        return Sort == EActorSort.Boss;
    }

    public bool IsPlayer()
    {
        return Type == EActorType.PLAYER;
    }

    public bool IsFullHP()
    {
        return false;
    }

    public bool IsFullMP()
    {
        return false;
    }

    public bool IsFSMLayer0()
    {
        switch (FSM)
        {
            case FSMState.FSM_RUN:
            case FSMState.FSM_IDLE:
            case FSMState.FSM_WALK:
                return true;
            default:
                return false;
        }
    }

    public bool IsFSMLayer1()
    {
        switch (FSM)
        {
            case FSMState.FSM_DANCE:
            case FSMState.FSM_MINE:
            case FSMState.FSM_INTERACTIVE:
            case FSMState.FSM_TALK:
                return true;
            default:
                return false;
        }
    }

    public bool IsFSMLayer2()
    {
        switch (FSM)
        {
            case FSMState.FSM_REBORN:
            case FSMState.FSM_JUMP:
            case FSMState.FSM_ROLL:
            case FSMState.FSM_GRAB:
                return true;
            default:
                return false;
        }
    }

    public bool IsFSMLayer3()
    {
        switch (FSM)
        {
            case FSMState.FSM_STUN:
            case FSMState.FSM_FROST:
            case FSMState.FSM_FEAR:
            case FSMState.FSM_BEATFLY:
            case FSMState.FSM_BEATDOWN:
            case FSMState.FSM_BEATBACK:
            case FSMState.FSM_DEAD:
            case FSMState.FSM_FLOATING:
            case FSMState.FSM_VARIATION:
            case FSMState.FSM_WOUND:
            case FSMState.FSM_SLEEP:
            case FSMState.FSM_PARALY:
            case FSMState.FSM_BLIND:
            case FSMState.FSM_GRAB:
                return true;
            default:
                return false;
        }
    }

    public bool IsFSMSkill()
    {
        return FSM == FSMState.FSM_SKILL;
    }

    public bool IsDead()
    {
        return FSM == FSMState.FSM_DEAD;
    }

    public bool IsDestroy()
    {
        return CacheTransform == null && IsLoad == true;
    }

    public bool CanBeAttack(bool includeSteal)
    {
        if (IsDead())
        {
            return false;
        }
        if (IsStealth && includeSteal == false)
        {
            return false;
        }
        if (GetActorNature(EActorNature.CAN_KILL))
        {
            return false;
        }
        return true;
    }

    public bool CanUseMP(int useNum)
    {
        return CurrAttr.MP >= useNum;
    }

    public bool CanUseHP(int useNum)
    {
        return CurrAttr.HP > useNum;
    }

    public bool CanUseSP(int useNum)
    {
        return CurrAttr.SP >= useNum;
    }

    public bool CanUseXP(int useNum)
    {
        return false;
    }

    public Resp CanUsePowerByCostType(ESkillCostType costType, int useNum)
    {
        if (useNum <= 0)
        {
            return Resp.TYPE_YES;
        }
        switch (costType)
        {
            case ESkillCostType.HP:
                return CanUseHP(useNum) ? Resp.TYPE_YES : Resp.TYPE_SKILL_LACKHP;
            case ESkillCostType.MP:
                return CanUseMP(useNum) ? Resp.TYPE_YES : Resp.TYPE_SKILL_LACKMP;
            case ESkillCostType.SP:
                return CanUseSP(useNum) ? Resp.TYPE_YES : Resp.TYPE_SKILL_LACKSP;
            case ESkillCostType.XP:
                return CanUseXP(useNum) ? Resp.TYPE_YES : Resp.TYPE_SKILL_LACKXP;
            default:
                return Resp.TYPE_YES;
        }
    }

    public void UsePowerByCostType(ESkillCostType costType, int useNum)
    {
        if (useNum <= 0)
        {
            return;
        }
        switch (costType)
        {
            case ESkillCostType.MP:
                this.CurrAttr.MP -= useNum;
                break;
            case ESkillCostType.HP:
                this.CurrAttr.HP -= useNum;
                break;
            case ESkillCostType.SP:
                this.CurrAttr.SP -= useNum;
                break;
            case ESkillCostType.XP:
                break;
        }
    }

    public void KillMe()
    {
        this.CanUseHP(CurrAttr.HP);
        this.Command.Get<CommandDead>().Do();
    }

    public void StopMove()
    {
        Command.Get<CommandIdle>().Do();
    }

    public void FaceTarget(float duration)
    {
        if (Target != null)
        {
            if (duration > 0)
            {
                Vector3 towards = Target.Pos - Pos;
                towards.Normalize();
                CacheTransform.transform.DOLookAt(towards, duration, AxisConstraint.Y);
            }
            else
            {
                Vector3 point = new Vector3(Target.Pos.x, Pos.y, Target.Pos.z);
                CacheTransform.transform.LookAt(point);
            }
        }
    }

    public void TauntBy(Character taunter, int threatValue)
    {
        CharacterTaunt item = null;
        for (int i = 0; i < mThreatList.Count; i++)
        {
            if (mThreatList[i].CC == taunter)
            {
                item = mThreatList[i];
            }
        }
        if (item == null)
        {
            item = new CharacterTaunt();
            item.CC = taunter;
        }
        item.ThreatValue += threatValue;
    }

    public void CheckDead()
    {
        if (CurrAttr.HP <= 0)
        {
            this.Command.Get<CommandDead>().Do();
        }
    }

    public int AddHP(int hp)
    {
        if (IsDead())
        {
            return 0;
        }
        int oldHP = CurrAttr.HP;
        int newHP = oldHP + hp > CurrAttr.MaxHP ? CurrAttr.MaxHP : oldHP + hp;
        CurrAttr.Update(EAttr.HP, newHP);
        return newHP - oldHP;
    }

    public int AddMP(int mp)
    {
        if (IsDead())
        {
            return 0;
        }
        int oldMP = CurrAttr.MP;
        int newMP = oldMP + mp > CurrAttr.MaxMP ? CurrAttr.MaxMP : oldMP + mp;
        CurrAttr.Update(EAttr.MP, newMP);
        return newMP - oldMP;
    }

    public int BeDamage(int damageValue)
    {
        int oldHP = CurrAttr.HP;
        if (oldHP > damageValue)
        {
            this.CurrAttr.HP = oldHP - damageValue;
            return damageValue;
        }
        else
        {
            this.CurrAttr.HP = 0;
            this.CheckDead();
            return oldHP;
        }
    }

    public List<Character> GetAllys()
    {
        mAllys.Clear();
        GetActorsByTargetRelationShip(ETargetRelationShip.TYPE_ALLY, ref mAllys);
        return mAllys;
    }

    public List<Character> GetEnemys(bool ignoreStealth = false)
    {
        mEnemys.Clear();
        GetActorsByTargetRelationShip(ETargetRelationShip.TYPE_ENEMY, ref mEnemys, ignoreStealth);
        for (int i = mEnemys.Count - 1; i >= 0; i--)
        {
            if(mEnemys[i].GetActorNature(EActorNature.CAN_KILL) == false)
            {
                mEnemys.RemoveAt(i);
            }
        }
        return mEnemys;
    }

    public List<Character> GetAffectCharacters(EAffect affect, bool ignoreStealth)
    {
        switch (affect)
        {
            case EAffect.Self:
                return new List<Character>() { this };
            case EAffect.Enem:
                return GetEnemys(ignoreStealth);
            case EAffect.Boss:
                return GTWorld.Boss == null ? null : new List<Character>() { GTWorld.Boss };
            case EAffect.Each:
                return GTWorld.Characters;
            case EAffect.Host:
                return Host == null ? null : new List<Character>() { Host };
            case EAffect.Ally:
                return GetAllys();
            default:
                return null;
        }
    }

    public bool Match(EAffect affect, Character actor, bool includeDead = false )
    {
        if (actor == null)
        {
            return false;
        }
        if (actor.IsDead() && includeDead == false)
        {
            return false;
        }
        switch (affect)
        {
            case EAffect.Ally:
                return GetTargetCampRelationShip(actor) == ETargetRelationShip.TYPE_ALLY;
            case EAffect.Boss:
                return actor.IsBoss();
            case EAffect.Each:
                return true;
            case EAffect.Host:
                return actor == Host;
            case EAffect.Self:
                return actor == this;
            case EAffect.Enem:
                return GetTargetCampRelationShip(actor) == ETargetRelationShip.TYPE_ENEMY;
            default:
                return false;
        }
    }

    public ETargetRelationShip GetTargetCampRelationShip(Character actor)
    {
        if (Camp == EBattleCamp.CT_NONE || actor.Camp == EBattleCamp.CT_NONE)
        {
            return ETargetRelationShip.TYPE_NONE;
        }
        if (actor.Camp == Camp)
        {
            return ETargetRelationShip.TYPE_ALLY;
        }
        switch (actor.Camp)
        {
            case EBattleCamp.CT_PVE_PLAYER:
                return Camp == EBattleCamp.CT_PVE_ENEMY ? ETargetRelationShip.TYPE_ENEMY : ETargetRelationShip.TYPE_NEUTRAL;
            case EBattleCamp.CT_PVE_ENEMY:
                return Camp == EBattleCamp.CT_PVE_PLAYER ? ETargetRelationShip.TYPE_ENEMY : ETargetRelationShip.TYPE_NEUTRAL;
            case EBattleCamp.CT_PVE_NEUTRAL:
                return ETargetRelationShip.TYPE_NEUTRAL;
            default:
                return ETargetRelationShip.TYPE_NONE;
        }
    }

    public bool GetActorNature(EActorNature type)
    {
        bool yes = false;
        mActorNatures.TryGetValue(type, out yes);
        return yes;
    }

    public void SetActorNature(EActorNature type, bool flag)
    {
        mActorNatures[type] = flag;
    }

    public void GetActorsByTargetRelationShip(ETargetRelationShip relationShip, ref List<Character> list, bool ignoreStealth = false)
    {
        for (int i = 0; i < GTWorld.Characters.Count; i++)
        {
            Character actor = GTWorld.Characters[i];
            if (GetTargetCampRelationShip(actor) == relationShip && actor.IsDead() == false)
            {
                if (ignoreStealth == false)
                {
                    list.Add(actor);
                }
                else
                {
                    if (!actor.IsStealth)
                    {
                        list.Add(actor);
                    }
                }
            }
        }
    }

    public Character FindEnemyNearest(float maxJudgeDistance = 10000)
    {
        this.GetEnemys();
        float dist = maxJudgeDistance;
        Character tar = null;
        for (int i = 0; i < mEnemys.Count; i++)
        {
            Character cc = mEnemys[i];
            float d = GTTools.GetHorizontalDistance(cc.Pos, this.Pos);
            if (d < dist)
            {
                tar = cc;
                dist = d;
            }
        }
        return tar;
    }

    public void SyncData(XCharacter data, ESyncDataType type)
    {
        switch (type)
        {
            case ESyncDataType.TYPE_ALL:
                SyncBase(data);
                SyncAvatar(data);
                SyncCurrAttr(data);
                break;
            case ESyncDataType.TYPE_BASEATTR:
                break;
            case ESyncDataType.TYPE_CURRATTR:
                SyncCurrAttr(data);
                break;
            case ESyncDataType.TYPE_AOI:
                break;
            case ESyncDataType.TYPE_EQUIP:
                SyncAvatar(data);
                SyncCurrAttr(data);
                break;
            case ESyncDataType.TYPE_LEVEL:
                SyncLevel(data);
                SyncCurrAttr(data);
                break;
            case ESyncDataType.TYPE_NAME:
                SyncName(data);
                break;
            case ESyncDataType.TYPE_TITLE:
                SyncTitle(data);
                SyncCurrAttr(data);
                break;
        }
    }

    void SyncBase(XCharacter data)
    {
        this.Sex   = data == null ? Cfg.Sex :  (EActorSex)data.Sex;
        this.Sort  = data == null ? Cfg.Sort : (EActorSort)data.Sort;
        this.Race  = data == null ? Cfg.Race : (EActorRace)data.Race;
        this.Unit  = data == null ? Cfg.Unit : (EActorUnit)data.Unit;
        this.Group = data == null ? Cfg.Group : data.Group;
        this.Level = data == null ? Cfg.Level : data.Level;
        this.Name  = data == null ? Cfg.Name : data.Name;
        this.Title = data == null ? Cfg.Title : data.Title;
        this.HUD.Show();
    }

    void SyncCurrAttr(XCharacter data)
    {
        if (data == null || data.CurAttrs.Count == 0)
        {
            CurrAttr.CopyFrom(Cfg.Attrs);
        }
        else
        {
            CurrAttr.CopyFrom(data.CurAttrs);
        }
    }

    void SyncAvatar(XCharacter data)
    {
        if (data == null)
        {
            return;
        }
        if (data.CurEquips.Count < 8)
        {
            return;
        }
        for (int i = 0; i < 8; i++)
        {
            int pos = i + 1;
            int equipID = data.CurEquips[i];
            Avatar.ChangeAvatar(pos, equipID);
        }
    }

    void SyncLevel(XCharacter data)
    {
        this.Level = data == null ? Cfg.Level : data.Level;
        this.HUD.Show();
    }

    void SyncName(XCharacter data)
    {
        this.Name = data == null ? Cfg.Name : data.Name;
        this.HUD.Show();
    }

    void SyncTitle(XCharacter data)
    {
        this.Title = data == null ? Cfg.Title : data.Title;
        this.HUD.Show();
    }

    public void CheckMoveEndPointInFront(ref Vector3 endValue, float maxDir)
    {
        Vector3 rayStartPoint = Pos + new Vector3(0, Height / 2, 0);
        RaycastHit hitInfo;
        if (Physics.Raycast(rayStartPoint, Dir, out hitInfo, maxDir, LayerMask.GetMask("Default")))
        {
            endValue = hitInfo.point;
            endValue.y = Pos.y;
            endValue -= Radius * Dir;
        }
    }

    public void CheckMoveEndPointInBack(ref Vector3 endValue, float maxDir)
    {
        Vector3 rayStartPoint = Pos + new Vector3(0, Height / 2, 0);
        RaycastHit hitInfo;
        if (Physics.Raycast(rayStartPoint, Dir, out hitInfo, maxDir, LayerMask.GetMask("Default")))
        {
            endValue = hitInfo.point;
            endValue.y = Pos.y;
            endValue += Radius * Dir;
        }
    }
}