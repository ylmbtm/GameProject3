using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using Protocol;

public class UIMountTurned : GTWindow
{
    private UIScrollView    view;
    private UIGrid          grid;
    private GameObject      template;
    private List<ItemMount> mMountItems = new List<ItemMount>();
    private int             mSelectIndex = 0;

    class ItemMount
    {
        public int        id;
        public UIToggle   toggle;
        public UISprite   quality;
        public UISprite   icon;
        public GameObject dress;
        public GameObject btn;
    }

    public UIMountTurned()
    {
        MaskType = EWindowMaskType.None;
        Type = EWindowType.Window;
        Resident = false;
        ShowMode = EWindowShowMode.SaveTarget;
        Path = "Mount/UIMountTurned";
    }

    protected override void OnAwake()
    {
        view = transform.Find("View").GetComponent<UIScrollView>();
        grid = transform.Find("View/Grid").GetComponent<UIGrid>();
        template = transform.Find("View/Template").gameObject;
        template.SetActive(false);
    }

    protected override void OnEnable()
    {
        InitMountItems();
        InitView();
    }

    protected override void OnAddButtonListener()
    {

    }

    protected override void OnAddHandler()
    {

    }

    protected override void OnDelHandler()
    {

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
        foreach (var current in mounts)
        {
            GameObject item = NGUITools.AddChild(grid.gameObject, template);
            item.SetActive(true);
            UIToggle toggle = item.GetComponent<UIToggle>();
            toggle.group = group;
            ItemMount tab = new ItemMount();
            tab.id = current.Value.Id;
            tab.toggle = toggle;
            tab.btn = item;
            tab.quality = item.transform.GetComponent<UISprite>();
            tab.icon = item.transform.Find("Icon").GetComponent<UISprite>();
            tab.dress = item.transform.Find("Dress").gameObject;
            int index = mMountItems.Count;
            UIEventListener.Get(tab.btn).onClick = (GameObject go) =>
            {
                if (mSelectIndex == index)
                {
                    return;
                }
                this.mSelectIndex = index;
                UIMount.SetSelectMountID(tab.id);
                this.ShowView();
            };
            mMountItems.Add(tab);
        }
        mMountItems[0].toggle.value = true;
        UIMount.SetSelectMountID(mMountItems[0].id);
    }

    private void InitView()
    {
        XCharacter role = GTGlobal.Main;
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

    }
}
