using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using Protocol;

public class UIRole : GTWindow
{
    private UILabel         labHeroName;
    private UIInput         iptHeroName;
    private GameObject      btnEnterGame;
    private GameObject      btnCreateRole;
    private GameObject      btnRoll;
    private GameObject      btnHero;

    private GameObject      roleTemplate;
    private UIGrid          roleGrid;

    private int             mRoleIndex      = 0;
    private Character       mPlayer         = null;
    private Vector3         mRoleModelPos   = new Vector3(-44.17f, 2.473f, -4.41f);
    private Vector3         mRoleModelEuler = new Vector3(0, 180, 0);
    private List<DRole>     mRoleDBList;

    private DRole           CurRole
    {
        get { return mRoleDBList[mRoleIndex]; }
    }

    public UIRole()
    {
        Type     = EWindowType.Window;
        Resident = false;
        Path     = "Role/UIRole";
        ShowMode = EWindowShowMode.HideOther;
    }

    protected override void OnAwake()
    {
        labHeroName = transform.Find("HeroName/Label").GetComponent<UILabel>();
        Transform left = transform.Find("Left");
        Transform right = transform.Find("Right");

        iptHeroName = transform.Find("Bottom/NameInput").GetComponent<UIInput>();
        btnCreateRole = transform.Find("Bottom/Btn_CreateRole").gameObject;
        btnRoll = transform.Find("Bottom/Btn_Roll").gameObject;
        btnEnterGame = transform.Find("Bottom/Btn_EnterGame").gameObject;
        btnHero = transform.Find("HeroTexture").gameObject;
        roleTemplate = transform.Find("Template").gameObject;
        roleTemplate.SetActive(false);
        roleGrid = transform.Find("View/Grid").GetComponent<UIGrid>();
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnCreateRole).onClick = OnCreateRoleClick;
        UIEventListener.Get(btnEnterGame).onClick =OnEnterGameClick;
        UIEventListener.Get(btnRoll).onClick = OnRollClick;
        UIEventListener.Get(btnHero.gameObject).onDrag = OnHeroTextureDrag;
    }

    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler(GTEventID.TYPE_LOGIN_ROLECRATE, OnRecvCreateRole);
        GTEventCenter.AddHandler(GTEventID.TYPE_LOGIN_ROLELOGIN, OnRecvEnterGame);
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler(GTEventID.TYPE_LOGIN_ROLECRATE, OnRecvCreateRole);
        GTEventCenter.DelHandler(GTEventID.TYPE_LOGIN_ROLELOGIN, OnRecvEnterGame);
    }

    protected override void OnEnable()
    {
        mRoleDBList = new List<DRole>(ReadCfgRole.Dict.Values);
        InitView();
        ShowView();
        ShowModel();
        ShowRandomName();
    }

    protected override void OnClose()
    {
        mRoleIndex = 0;
        GTWorld.Instance.DelActor(mPlayer);
        mPlayer = null;
    }

    private void OnHeroTextureDrag(GameObject go, Vector2 delta)
    {
        if (mPlayer == null)
        {
            return;
        }
        ESpin.Get(mPlayer.Obj).OnSpin(delta, 2);
    }

    private void OnCreateRoleClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        string name = iptHeroName.value.Trim();
        GTNetworkSend.Instance.TryCreateRole(name, CurRole.Carrer, MLLogin.Instance.LastAccountID);
    }

    private void OnEnterGameClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        int carrerID = mRoleDBList[mRoleIndex].Carrer;
        XCharacter c = DataDBSCharacter.GetDataById(carrerID);
        if(c == null)
        {
            GTItemHelper.ShowTip("你还没有创建这个角色");
            return;
        }
        GTNetworkSend.Instance.TryRoleLogin(c.GUID);
    }

    private void OnRollClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        ShowRandomName();
    }

    private void OnRecvEnterGame()
    {
        Hide();
    }

    private void OnRecvCreateRole()
    {
        ShowView();
    }

    private void ShowView()
    {
        DRole db = mRoleDBList[mRoleIndex];
        XCharacter role = DataDBSCharacter.GetDataById(db.Carrer);
        DActor actorDB = ReadCfgActor.GetDataById(db.ActorID);
        btnCreateRole.SetActive(role == null);
        btnRoll.SetActive(role == null);
        iptHeroName.gameObject.SetActive(role == null);
        btnEnterGame.SetActive(role != null);
        labHeroName.text = role != null ? GTTools.Format("Lv.{0} {1}", role.Level, role.Name != null ? role.Name : actorDB.Name) : "未创建";
    }

    private void InitView()
    {
        int group = GTWindowManager.Instance.GetToggleGroupId();
        for (int i = 0; i < mRoleDBList.Count; i++)
        {
            int index = i;
            DActor roleDB = ReadCfgActor.GetDataById(mRoleDBList[i].Carrer);
            GameObject item = NGUITools.AddChild(roleGrid.gameObject, roleTemplate);
            item.SetActive(true);
            roleGrid.AddChild(item.transform);
            UISprite roleIcon = item.transform.Find("Icon").GetComponent<UISprite>();
            roleIcon.spriteName = roleDB.Icon;

            UIToggle toggle = item.GetComponent<UIToggle>();
            toggle.group = group;
            toggle.value = 0 == mRoleIndex;
            UIEventListener.Get(item).onClick = delegate
            {
                GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
                if (mRoleIndex == index)
                {
                    return;
                }
                mRoleIndex = index;
                ShowView();
                ShowModel();
                ShowRandomName();
            };
        }
    }

    private void ShowModel()
    {
        if (mPlayer != null)
        {
            GTWorld.Instance.DelActor(mPlayer);
        }
        GTTransform param = GTTransform.Default;
        DRole roleDB = mRoleDBList[mRoleIndex];
        mPlayer = GTWorld.Instance.AddRole(roleDB.ActorID, param);
        mPlayer.EnableCharacter(false);
        mPlayer.EnableRootMotion(false);
        mPlayer.Action.Play("idle");
        if (roleDB.DisplayWeapon > 0)
        {
            mPlayer.Avatar.ChangeAvatar(8, roleDB.DisplayWeapon);
        }
        mPlayer.CacheTransform.localPosition = mRoleModelPos;
        mPlayer.CacheTransform.localEulerAngles = mRoleModelEuler;
        GameObject go = mPlayer.Obj;
        go.transform.position = mRoleModelPos;
        go.transform.eulerAngles = mRoleModelEuler;
        go.SetActive(true);
    }
    
    private void ShowRandomName()
    {
        XCharacter role = DataDBSCharacter.GetDataById(CurRole.Carrer);
        if (role == null)
        {
            int sex = CurRole.Carrer == 2 || CurRole.Carrer == 3 ? 1 : 0;
            string randomName = GetRandomName(sex);
            iptHeroName.value = randomName;
        }
    }

    private string ShowStar(int star)
    {
        string s = string.Empty;
        for (int i = 0; i < star; i++)
        {
            s = GTTools.Format("{0}{1}", s, "★");
        }
        return s;
    }

    private string GetRandomName(int sex)
    {
        int r = UnityEngine.Random.Range(0, ReadCfgRandomName.Dict.Count);
        DRandomName db =  ReadCfgRandomName.Dict[r + 1];
        if (db.Sex != sex || iptHeroName.value == db.Name)
        {
            return GetRandomName(sex);
        }
        else
        {
            return db.Name;
        }
    }
}
