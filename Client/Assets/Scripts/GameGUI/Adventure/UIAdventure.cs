using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;

public class UIAdventure : GTWindow
{
    private GameObject    btnClose;
    private UIScrollView  scrollView;
    private UICycleGrid   cycleGrid;

    public UIAdventure()
    {
        MaskType = EWindowMaskType.Black;
        Type = EWindowType.Window;
        Path = "Adventure/UIAdventure";
        Resident = false;
        ShowMode = EWindowShowMode.HideOther;
    }

    protected override void OnAwake()
    {
        Transform pivot = transform.Find("Pivot");
        btnClose = pivot.Find("BtnClose").gameObject;
        scrollView = pivot.Find("View").GetComponent<UIScrollView>();
        cycleGrid = pivot.Find("View/Grid").GetComponent<UICycleGrid>();
    }

    protected override void OnAddHandler()
    {

    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick = OnReturnClick;
    }

    protected override void OnEnable()
    {
        this.InitView();
    }

    protected override void OnDelHandler()
    {

    }

    protected override void OnClose()
    {

    }

    private void InitView()
    {
        Dictionary<int, DAdventure> dict = ReadCfgAdventure.Dict;
        cycleGrid.onInitializeItemCallback = OnInitItem;
        cycleGrid.Init(dict.Count, 1);
    }

    private void OnInitItem(GameObject go, int index)
    {
        int id = index + 1;
        DAdventure db = ReadCfgAdventure.GetDataById(id);
        go.transform.Find("Name").GetComponent<UILabel>().text = db.Name;
        go.transform.Find("Icon").GetComponent<UISprite>().spriteName = db.Icon;
        if (db.Times <= 0)
        {
            go.transform.Find("Times").GetComponent<UILabel>().text = string.Empty;
        }
        else
        {
            string localString = GTItemHelper.GetText("次数：{0}/{1}");
            go.transform.Find("Times").GetComponent<UILabel>().text = GTTools.Format(localString, db.Times, db.Times);
        }

        UIEventListener.Get(go).onClick = delegate (GameObject btn)
        {
            GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        };
    }

    private void OnReturnClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
        Hide();
    }
}
