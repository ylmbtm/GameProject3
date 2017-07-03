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
    private int[]           mDisplayWeapons ;
    private Character       mPlayer         = null;
    private Vector3         mRoleModelPos   = new Vector3(-44.17f, 2.473f, -4.41f);
    private Vector3         mRoleModelEuler = new Vector3(0, 180, 0);
    private List<int>       mRoleDBList;

    UInt64 mSelectRoleID;


    public UIRole()
    {
        Type        = EWindowType.WINDOW;
        mResident   = false;
        mResPath    = "Role/UIRole";
        mRoleDBList = new List<int>()
        {
            1, 2, 4,
            90001, 90002, 90005, 90007, 90008, 90009, 90010,
            90013, 90015, 90016, 90017, 90018, 90019, 90020,
            90021
        };

        mDisplayWeapons = new int[3]
        {
            5403, 5406, 5410
        };
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
        GTEventCenter.AddHandler(GTEventID.TYPE_CREATEROLE_CALLBACK, OnRecvCreateRole);
        GTEventCenter.AddHandler(GTEventID.TYPE_ENTERGAME_CALLBACK, OnRecvEnterGame);
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler(GTEventID.TYPE_CREATEROLE_CALLBACK, OnRecvCreateRole);
        GTEventCenter.DelHandler(GTEventID.TYPE_ENTERGAME_CALLBACK, OnRecvEnterGame);
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
        int id = mRoleDBList[mRoleIndex];
        LoginService.Instance.TryCreateRole(name, (uint)id);
    }

    private void OnEnterGameClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        LoginService.Instance.TryEnterGame(mSelectRoleID);
    }

    private void OnRollClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        string randomName = RoleModule.Instance.GetRandomName();
        iptHeroName.value = randomName;
    }

    private void OnRecvEnterGame()
    {
        Hide();
    }

    private void OnRecvCreateRole()
    {
        ShowView();
    }

    protected override void OnEnable()
    {
        InitModel();
        InitView();
        ShowView();
        ShowModel();
    }

    protected override void OnClose()
    {
        mRoleIndex = 0;
        if (mPlayer != null)
        {
            CharacterManager.Instance.DelActor(mPlayer);
            mPlayer = null;
        }
        GTCameraManager.Instance.RevertMainCamera();
    }

    private void ShowView()
    {
        int id = mRoleDBList[mRoleIndex];
        XCharacter role = DataDBSRole.GetDataById(id);
        DActor roleDB = ReadCfgActor.GetDataById(id);
        btnCreateRole.SetActive(role == null);
        btnRoll.SetActive(role == null);
        iptHeroName.gameObject.SetActive(role == null);
        btnEnterGame.SetActive(role != null);
        labHeroName.text = role != null ? GTTools.Format("Lv.{0} {1}", role.Level, role.Name != null ? role.Name : roleDB.Name) : "未创建";
    }

    private void InitModel()
    {
        Camera camara = GTCameraManager.Instance.MainCamera;
        camara.transform.position = new Vector3(-44.05f, 4.55f, -15.2f);
        camara.transform.eulerAngles = new Vector3(6.8639f, -0.4323f, 0.0958f);
        camara.renderingPath = RenderingPath.UsePlayerSettings;
        camara.fieldOfView = 36f;
        camara.clearFlags = CameraClearFlags.Skybox;
    }

    private void InitView()
    {
        int group = GTWindowManager.Instance.GetToggleGroupId();
        for (int i = 0; i < mRoleDBList.Count; i++)
        {
            int id = mRoleDBList[i];
            int index = i;
            DActor roleDB = ReadCfgActor.GetDataById(id);
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
                ShowMask();
                mPlayer.Command.Get<CommandUseSkill>().Update(ESkillPos.Skill_9).Do();
            };
        }
    }

    private void ShowMask()
    {

    }

    private void ShowModel()
    {
        if (mPlayer != null)
        {
            CharacterManager.Instance.DelActor(mPlayer);
        }
        KTransform param = KTransform.Default;
        int id = mRoleDBList[mRoleIndex];
        mPlayer = CharacterManager.Instance.AddRole(id, param);
        mPlayer.EnableCharacter(false);
        mPlayer.EnableRootMotion(false);
        mPlayer.Action.Play("idle");
        if (mRoleIndex < mDisplayWeapons.Length)
        {
            mPlayer.Avatar.ChangeAvatar(8, mDisplayWeapons[mRoleIndex]);
        }
        mPlayer.CacheTransform.localPosition = mRoleModelPos;
        mPlayer.CacheTransform.localEulerAngles = mRoleModelEuler;
        GameObject go = mPlayer.Obj;
        go.transform.position = mRoleModelPos;
        go.transform.eulerAngles = mRoleModelEuler;
        go.SetActive(true);
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
}
