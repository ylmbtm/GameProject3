using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;

public class UIMount : GTWindow
{
    public UIMount()
    {
        Type = EWindowType.WINDOW;
        mResident = false;
        mResPath = "Mount/UIMount";
    }

    enum Func
    {
        Func_None = -1,
        Func_Library,
        Func_Tame,
        Func_Blood,
        Func_Turned
    }

    private ERender         mRender;
    private CharacterAvatar mAvatar;
    private Transform       mountTrans;
    private UILabel         mountName;
    private GameObject      btnReturn;
    private UITexture       modelTexture;
    private Func            mSelectFunc = Func.Func_None;
    private UIToggle[]      mMenus;

    private Dictionary<Func, EWindowID> mFuncWindows = new Dictionary<Func, EWindowID>
    {
        { Func.Func_Blood,   EWindowID.UI_MOUNTBLOOD },
        { Func.Func_Library, EWindowID.UI_MOUNTLIBRARY },
        { Func.Func_Tame,    EWindowID.UI_MOUNTTAME },
        { Func.Func_Turned,  EWindowID.UI_MOUNTTURNED }
    };


    protected override void OnAwake()
    {
        Transform pivot = transform.Find("Pivot");
        mountTrans = transform.Find("MountTrans");
        mountName = pivot.Find("Top/MountName").GetComponent<UILabel>();
        modelTexture = pivot.Find("ModelTexture").GetComponent<UITexture>();
        btnReturn = pivot.Find("Top/Btn_Return").gameObject;
        mMenus = new UIToggle[4];
        for (int i = 0; i < mMenus.Length; i++)
        {
            mMenus[i] = pivot.Find("Left/Menus/Menu_" + (i + 1)).GetComponent<UIToggle>();
        }
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnReturn).onClick = OnCloseClick;
        UIEventListener.Get(modelTexture.gameObject).onDrag = OnMountDrag;
        for(int i=0;i<mMenus.Length;i++)
        {
            UIToggle menu = mMenus[i];
            Func func = (Func)i;
            UIEventListener.Get(menu.gameObject).onClick = (GameObject go) =>
            {
                GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
                Switch(func);
                ShowView();
            };
        }
    }

    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler(GTEventID.TYPE_SELECT_MOUNT, OnChangeSelectMount);
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler(GTEventID.TYPE_SELECT_MOUNT, OnChangeSelectMount);
    }

    protected override void OnEnable()
    {
        GTCoroutinueManager.Instance.StartCoroutine(FirstOpen());
    }

    protected override void OnClose()
    {
        SetSelectMountID(0);
        mAvatar = CharacterManager.Instance.DelAvatar(mAvatar);
        mRender = ERender.DelRender(mRender);
    }

    private void OnMountDrag(GameObject go, Vector2 delta)
    {
        if (mAvatar == null && mAvatar.GetRootObj() != null)
        {
            return;
        }
        ESpin.Get(mAvatar.GetRootObj()).OnSpin(delta, 2);
    }

    private void OnCloseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        Switch(Func.Func_None);
        Hide();
    }

    private void InitToggles()
    {
        int group = GTWindowManager.Instance.GetToggleGroupId();
        for(int i=0;i<mMenus.Length;i++)
        {
            mMenus[i].group = group;
        }
        mMenus[(int)mSelectFunc].value = true;
    }

    private void ShowView()
    {
        DActor db = ReadCfgActor.GetDataById(mSelectMountID);
        if(db==null)
        {
            return;
        }
        GTItemHelper.ShowItemName(mountName, db.Quality, db.Name);
    }

    private void ShowModel()
    {
        DActor db = ReadCfgActor.GetDataById(mSelectMountID);
        if (db == null)
        {
            mRender = ERender.DelRender(mRender);
            return;
        }
        if (mRender == null)
        {
            mRender = ERender.AddRender(modelTexture);
        }
        mAvatar = CharacterManager.Instance.DelAvatar(mAvatar);
        mRender.DelAllModels();
        mAvatar = CharacterManager.Instance.AddAvatar(db.Model);
        mAvatar.PlayAnim("idle", null);
        GameObject model = mRender.AddModel(mAvatar.GetRootObj());
        model.transform.localPosition = db.StagePos;
        model.transform.localEulerAngles = new Vector3(0, 120, 0);
        model.transform.localScale = new Vector3(db.StageScale, db.StageScale, db.StageScale);
    }

    private void Switch(Func func)
    {
        if (mSelectFunc == func)
            return;
        if (mSelectFunc != Func.Func_None)
        {
            EWindowID id = mFuncWindows[mSelectFunc];
            GTWindowManager.Instance.CloseWindow(id);
        }
        mSelectFunc = func;
        if(mSelectFunc!=Func.Func_None)
        {
            EWindowID newID = mFuncWindows[mSelectFunc];
            GTWindow window = GTWindowManager.Instance.OpenWindow(newID);
            window.SetParent(this);
        }
    }

    private IEnumerator FirstOpen()
    {
        yield return null;
        Switch(Func.Func_Library);
        InitToggles();
    }

    private void OnChangeSelectMount()
    {
        ShowModel();
        ShowView();
    }

    static int mSelectMountID;

    public static int GetSelectMountID()
    {
        return mSelectMountID;
    }

    public static int SetSelectMountID(int value)
    {
        mSelectMountID = value;
        if (value > 0)
        {
            GTEventCenter.FireEvent(GTEventID.TYPE_SELECT_MOUNT);
        }
        return 0;
    }
}
