using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using UnityEngine.SceneManagement;
using System.Xml;
using System.IO;
using System.Reflection;

public class GTLauncher : MonoBehaviour
{
    public static GTLauncher  Instance;
    public static int LAST_CITY_ID = GTSceneKey.SCENE_CITY_4;

    [HideInInspector]
    public bool          ShowFPS;
    [HideInInspector]
    public bool          TestScene;
    [HideInInspector]
    public string        CurrSceneName;
    [HideInInspector]
    public ESceneType    CurrSceneType;
    [HideInInspector]
    public ESceneType    NextSceneType;
    [HideInInspector]
    public Int32         TestActorID;
    [HideInInspector]
    public bool          MusicDisable = true;
   [HideInInspector]
    public bool          UseGuide = true;

    private IStateMachine<GTLauncher, ESceneType> mStateMachine;

    void Awake()
    {
        Application.runInBackground = true;
        Instance = this;
        DontDestroyOnLoad(gameObject);
        //PlayCGMovie();
    }

    void Start()
    {
        //TryShowFPS();
        //IgnorePhysicsLayer();
        AddFSM();
        LoadManager();
        OpenTag();
        StartGame();
    }

    void OpenTag()
    {
        GTLog.Open(GTLogTag.TAG_ACTOR);
    }

    void LoadManager()
    {
        NetworkManager.       Instance.Init();
        NetworkManager.       Instance.Start("127.0.0.1", 3001);

        GTCoroutinueManager.  Instance.SetDontDestroyOnLoad(transform);
        GTResourceManager.    Instance.SetDontDestroyOnLoad(transform);
        GTAudioManager.       Instance.SetDontDestroyOnLoad(transform);
        GTCameraManager.      Instance.SetDontDestroyOnLoad(transform);
        GTConfigManager.      Instance.Init();

        GTWindowManager.      Instance.Init();
        GTInputManager.       Instance.SetDontDestroyOnLoad(transform);
        GTWorld.              Instance.SetDontDestroyOnLoad(transform);
        GTWorld.              Instance.EnterWorld(GTSceneKey.SCENE_LAUNCHER);
        GTTouchEffect.        Instance.SetDontDestroyOnLoad(transform);

        GTTimerManager.       Instance.AddListener(1, SecondTick, 0);
        GTCtrl.               Instance.AddLoginCtrl();
    }

    void AddFSM()
    {
        this.mStateMachine = new IStateMachine<GTLauncher, ESceneType>(this);
        this.mStateMachine.AddState(ESceneType.TYPE_INIT,  new SceneInit());
        this.mStateMachine.AddState(ESceneType.TYPE_LOGIN, new SceneLogin());
        this.mStateMachine.AddState(ESceneType.TYPE_LOAD,  new SceneLoading());
        this.mStateMachine.AddState(ESceneType.TYPE_ROLE,  new SceneRole());
        this.mStateMachine.AddState(ESceneType.TYPE_CITY,  new SceneCity());
        this.mStateMachine.AddState(ESceneType.TYPE_PVE,   new ScenePVE());
        this.mStateMachine.AddState(ESceneType.TYPE_WORLD, new SceneWorld());
        this.mStateMachine.SetCurState(this.mStateMachine.GetState(ESceneType.TYPE_INIT));
        CurScene = (IScene)this.mStateMachine.GetState(ESceneType.TYPE_INIT);
    }

    void SecondTick()
    {
        GTEventCenter.FireEvent(GTEventID.TYPE_SECOND_TICK);
    }

    void IgnorePhysicsLayer()
    {
        Physics.IgnoreLayerCollision(GTLayer.LAYER_AVATAR,  GTLayer.LAYER_PARTNER);
        Physics.IgnoreLayerCollision(GTLayer.LAYER_AVATAR,  GTLayer.LAYER_PET);
        Physics.IgnoreLayerCollision(GTLayer.LAYER_AVATAR,  GTLayer.LAYER_MONSTER);
        Physics.IgnoreLayerCollision(GTLayer.LAYER_AVATAR,  GTLayer.LAYER_NPC);
        Physics.IgnoreLayerCollision(GTLayer.LAYER_AVATAR,  GTLayer.LAYER_PLAYER);
        Physics.IgnoreLayerCollision(GTLayer.LAYER_MOUNT,   GTLayer.LAYER_PARTNER);
        Physics.IgnoreLayerCollision(GTLayer.LAYER_MOUNT,   GTLayer.LAYER_PET);
        Physics.IgnoreLayerCollision(GTLayer.LAYER_MOUNT,   GTLayer.LAYER_MONSTER);
        Physics.IgnoreLayerCollision(GTLayer.LAYER_MOUNT,   GTLayer.LAYER_NPC);
        Physics.IgnoreLayerCollision(GTLayer.LAYER_PARTNER, GTLayer.LAYER_BARRER);
    }

    public void StartGame()
    {
        if (TestScene)
        {
            CurPlayerID = TestActorID <= 0 ? 1 : TestActorID;
            DataManager.Instance.LoadCommonData();
            DataManager.Instance.LoadRoleData(CurPlayerID);
            LoadScene(LAST_CITY_ID);
        }
        else
        {
            LoadScene(GTSceneKey.SCENE_LOGIN);
        }
    }

    public void ChangeState(ESceneType state, ICommand ev)
    {
        if (CurrSceneType == state)
        {
            return;
        }
        this.mStateMachine.GetState(state).SetCommand(ev);
        this.mStateMachine.ChangeState(state);
        this.CurrSceneType = state;
        this.CurrSceneName = GTLauncher.LoadedLevelName;
        CurScene = (IScene)this.mStateMachine.GetState(state);
    }

    public void LoadScene(int sceneId)
    {
        DScene db = ReadCfgScene.GetDataById(sceneId);
        switch(db.SceneType)
        {
            case ESceneType.TYPE_LOGIN:
                {
                    this.NextSceneType = ESceneType.TYPE_LOGIN;
                }
                break;
            case ESceneType.TYPE_ROLE:
                {
                    this.NextSceneType = ESceneType.TYPE_ROLE;
                    DataManager.Instance.LoadCommonData();                  
                }
                break;
            case ESceneType.TYPE_CITY:
                {
                    this.NextSceneType = ESceneType.TYPE_CITY;
                    if (this.CurrSceneType == ESceneType.TYPE_ROLE)
                    {
                        GTCtrl.Instance.AddAllCtrls();
                        DataManager.Instance.LoadRoleData(CurPlayerID);
                        DataTimer.Instance.Init();
                    }
                }
                break;
            case ESceneType.TYPE_WORLD:
                {
                    this.NextSceneType = ESceneType.TYPE_WORLD;
                    if (this.CurrSceneType == ESceneType.TYPE_ROLE)
                    {
                        DataManager.Instance.LoadRoleData(CurPlayerID);
                        DataTimer.Instance.Init();
                    }
                }
                break;
            case ESceneType.TYPE_PVE:
                {
                    this.NextSceneType = ESceneType.TYPE_PVE;
                }
                break;
        }
        CommandLoadScene cmd = new CommandLoadScene();
        cmd.SceneID = sceneId;
        ChangeState(ESceneType.TYPE_LOAD, cmd);
    }

//     public void TryShowFPS()
//     {
//         if (ShowFPS)
//         {
//             GameObject go = new GameObject("FPS");
//             go.AddComponent<EFPS>();
//             go.transform.parent = transform;
//         }
//     }

    public AsyncOperation LoadLevelById(int id)
    {
        DScene db = ReadCfgScene.GetDataById(id);
        CurSceneID = db.Id;
        if (string.IsNullOrEmpty(db.SceneName))
        {
            return null;
        }
        ReleaseResource();
        return SceneManager.LoadSceneAsync(db.SceneName);
    }

    public void ReleaseResource()
    {
        Resources.UnloadUnusedAssets();
        GC.Collect();
    }

    public void PlayCGMovie()
    {
        Screen.sleepTimeout = SleepTimeout.NeverSleep;
        Handheld.PlayFullScreenMovie("CG.mp4", Color.black, FullScreenMovieControlMode.CancelOnInput, FullScreenMovieScalingMode.AspectFit);
    }

    void Update()
    {
        if (this.mStateMachine != null)
        {
            this.mStateMachine.Execute();
        }
        GTTimerManager.  Instance.Execute();
        GTUpdate.        Instance.Execute();
        NetworkManager.  Instance.Execute();
    }

    void FixedUpdate()
    {
        GTAction.Update();
    }

    void OnApplicationQuit()
    {
        GTTimerManager.Instance.DelListener(SecondTick);
        DataTimer.Instance.Exit();
        ReleaseResource();
    }

    public static ELanguage Language
    {
        get; set;
    }

    public static float     TimeScale
    {
        get { return Time.timeScale; }
        set { Time.timeScale = value; }
    }

    public static IScene    CurScene
    {
        get;
        private set;
    }

    public static Int32     CurSceneID
    {
        get;
        private set;
    }

    public static Int32     CurPlayerID
    {
        get;
        set;
    }

    public static string    LoadedLevelName
    {
        get { return SceneManager.GetActiveScene().name; }
    }
}
