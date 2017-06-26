using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using System.Linq;
using Protocol;

public class UIMountLibrary : GTWindow
{
    private GameObject      btnFight;
    private UIScrollView    view;
    private UIGrid          grid;
    private GameObject      template;

    private List<ItemMount> mMountItems = new List<ItemMount>();
    private int             mSelectIndex = 0;    
    private UILabel         mMountSpeed;
    private UILabel         mMountQuality;

    class ItemMount
    {
        public int        id;
        public UIToggle   toggle;
        public UISprite   quality;
        public UISprite   icon;
        public GameObject dress;
        public GameObject btn;
    }

    public UIMountLibrary()
    {
        Type = EWindowType.DIALOG;
        mResident = false;
        mResPath = "Mount/UIMountLibrary";
    }

    protected override void OnAwake()
    {
        btnFight = transform.Find("Content/Btn_Fight").gameObject;
        view = transform.Find("View").GetComponent<UIScrollView>();
        grid = transform.Find("View/Grid").GetComponent<UIGrid>();
        template = transform.Find("View/Template").gameObject;
        template.SetActive(false);
        mMountSpeed = transform.Find("Content/Base/MountSpeed/Value").GetComponent<UILabel>();
        mMountQuality= transform.Find("Content/Base/MountQuality/Value").GetComponent<UILabel>();
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnFight).onClick = OnFightClick;
    }

    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler(GTEventID.TYPE_DRESS_MOUNT, OnRecvSetMount);
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler(GTEventID.TYPE_DRESS_MOUNT, OnRecvSetMount);
    }

    protected override void OnEnable()
    {
        InitMountItems();
        InitView();
        ShowView();
    }

    protected override void OnClose()
    {
        mSelectIndex = 0;
        mMountItems.Clear();
    }

    private void InitMountItems()
    {
        Dictionary<int, DActor> mounts = new Dictionary<int, DActor>();
        foreach (var current in ReadCfgActor.Dict)
        {
            if (current.Value.Type != EActorType.MOUNT)
            {
                continue;
            }
            mounts.Add(current.Key, current.Value);
        }
        int group = GTWindowManager.Instance.GetToggleGroupId();

        foreach (KeyValuePair<int, DActor> pair in mounts)
        {
            GameObject item = NGUITools.AddChild(grid.gameObject, template);
            item.SetActive(true);
            UIToggle toggle = item.GetComponent<UIToggle>();
            toggle.group = group;

            ItemMount tab = new ItemMount();
            tab.id = pair.Value.Id;
            tab.toggle = toggle;
            tab.btn = item;
            tab.quality = item.transform.GetComponent<UISprite>();
            tab.icon = item.transform.Find("Icon").GetComponent<UISprite>();
            tab.dress = item.transform.Find("Dress").gameObject;
            int index = mMountItems.Count;

            UIEventListener.Get(tab.btn).onClick = (GameObject go) =>
            {
                GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
                if (mSelectIndex == index)
                {
                    return;
                }
                mSelectIndex = index;
                UIMount.SetSelectMountID(tab.id) ;
                ShowView();
            };

            mMountItems.Add(tab);
        }
        mMountItems[0].toggle.value = true;
        UIMount.SetSelectMountID(mMountItems[0].id);
    }

    private void InitView()
    {
        XCharacter role = RoleModule.Instance.GetCurPlayer();
        for (int i = 0; i < mMountItems.Count; i++)
        {
            ItemMount tab = mMountItems[i];
            tab.dress.SetActive(role.Mount == tab.id);
            GTItemHelper.ShowActorQuality(tab.quality, tab.id);
            DActor db = ReadCfgActor.GetDataById(tab.id);
            tab.icon.spriteName = db.Icon;
        }
    }

    private void ShowView()
    {
        ItemMount tab = mMountItems[mSelectIndex];
        DActor db = ReadCfgActor.GetDataById(tab.id);
        mMountSpeed.text = db.RSpeed.ToPoint();
        GTItemHelper.ShowItemQuality(mMountQuality, db.Quality);
        XCharacter role = RoleModule.Instance.GetCurPlayer();
        btnFight.SetActive(role.Mount != tab.id);
    }

    private void OnRecvSetMount()
    {
        InitView();
        ShowView();
    }

    private void OnFightClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        MountService.Instance.TryChangeMount(UIMount.GetSelectMountID());
    }
}
