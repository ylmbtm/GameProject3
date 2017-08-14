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

    [HideInInspector]
    public string        CurrSceneName;
    [HideInInspector]
    public ECopyType     CurrCopyType;
    [HideInInspector]
    public ECopyType     NextCopyType;

    [SerializeField]
    public bool          UseFPS;
    [SerializeField]
    public bool          MusicDisable = true;
    [SerializeField]
    public bool          UseGuide = true;
    [SerializeField]
    public IScene        CurScene;
    [SerializeField]
    public Int32         LastCityID;
    [SerializeField]
    public string        LoginIP = "127.0.0.1";
    [SerializeField]
    public int           LoginPort = 5678;

    private IStateMachine<GTLauncher, ECopyType> mStateMachine;

    void Awake()
    {
        Application.runInBackground = true;
        GTGlobal.IsLaunched = true;
        GTGlobal.Read();
        Instance = this;
        DontDestroyOnLoad(gameObject);
        PlayCGMovie();
    }

    void Start()
    {
        TryShowFPS();
        IgnorePhysicsLayer();
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
        NetworkManager.       Instance.SetIPAddress("47.93.31.69", LoginPort);
        GTCoroutinueManager.  Instance.SetRoot(transform);
        GTAudioManager.       Instance.SetRoot(transform);
        GTCameraManager.      Instance.SetRoot(transform);
        GTInputManager.       Instance.SetRoot(transform);
        GTTouch.              Instance.SetRoot(transform);
        GTAudioManager.       Instance.SetMusicActive(!MusicDisable);
        GTResourceManager.    Instance.Init();
        GTConfigManager.      Instance.Init();
        GTWindowManager.      Instance.Init();
        GTWorld.              Instance.SetRoot(transform);
        GTWorld.              Instance.EnterWorld(GTCopyKey.SCENE_LAUNCHER);
        GTTimerManager.       Instance.AddListener(1, SecondTick, 0);
        GTNetworkRecv.        Instance.AddListener();
    }

    void AddFSM()
    {
        this.mStateMachine = new IStateMachine<GTLauncher, ECopyType>(this);
        this.mStateMachine.AddState(ECopyType.TYPE_INIT,  new SceneInit());
        this.mStateMachine.AddState(ECopyType.TYPE_LOGIN, new SceneLogin());
        this.mStateMachine.AddState(ECopyType.TYPE_LOAD,  new SceneLoading());
        this.mStateMachine.AddState(ECopyType.TYPE_ROLE,  new SceneRole());
        this.mStateMachine.AddState(ECopyType.TYPE_CITY,  new SceneCity());
        this.mStateMachine.AddState(ECopyType.TYPE_PVE,   new ScenePVE());
        this.mStateMachine.AddState(ECopyType.TYPE_WORLD, new SceneWorld());
        this.mStateMachine.SetCurState(this.mStateMachine.GetState(ECopyType.TYPE_INIT));
        this.CurScene = (IScene)this.mStateMachine.GetState(ECopyType.TYPE_INIT);
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
        GTGlobal.LastCityID = LastCityID;
        LoadScene(GTCopyKey.SCENE_LOGIN);
    }

    public void ChangeState(ECopyType state, ICommand ev)
    {
        if (CurrCopyType == state)
        {
            return;
        }
        this.mStateMachine.GetState(state).SetCommand(ev);
        this.mStateMachine.ChangeState(state);
        this.CurrCopyType = state;
        this.CurrSceneName = SceneManager.GetActiveScene().name;
        this.CurScene = (IScene)this.mStateMachine.GetState(state);
    }

    public void LoadScene(int sceneId, Callback finishCallback = null)
    {
        DCopy db = ReadCfgCopy.GetDataById(sceneId);
        this.NextCopyType = db.CopyType;
        CommandLoadScene cmd = new CommandLoadScene();
        cmd.SceneID = sceneId;
        SceneLoading s = (SceneLoading)mStateMachine.GetState(ECopyType.TYPE_LOAD);
        s.mCallback = finishCallback;
        ChangeState(ECopyType.TYPE_LOAD, cmd);
    }

    public void TryShowFPS()
    {
        if (UseFPS)
        {
            GameObject go = new GameObject("FPS");
            go.AddComponent<EFPS>();
            go.transform.parent = transform;
        }
    }

    public AsyncOperation LoadLevelById(int id)
    {
        DCopy db = ReadCfgCopy.GetDataById(id);
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
        GTWorld.         Instance.Execute();
        GTAsync.         Instance.Execute();
    }

    void FixedUpdate()
    {
        GTAction.Update();
    }

    void OnApplicationQuit()
    {
        GTTimerManager.Instance.DelListener(SecondTick);
        ReleaseResource();
    }
}
