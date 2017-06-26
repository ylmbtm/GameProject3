using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

public class UIWorldMap : GTWindow
{
    private GameObject      btnBackCity;
    private List<WorldArea> mAreas = new List<WorldArea>();

    private UITexture resItemTexture1;
    private UITexture resItemTexture2;
    private UITexture resItemTexture3;
    private UITexture resItemTexture4;

    private UILabel resItemNum1;
    private UILabel resItemNum2;
    private UILabel resItemNum3;
    private UILabel resItemNum4;

    private UITexture   mapColor;
    private Texture2D   worldMapTex;
    private BoxCollider worldCollider;

    private int      mAreaIndex = -1;
    private Color32  mWhiteColor = new Color(255, 255, 255, 0);
    private UISprite background;
    private float    mTime;

    class WorldArea
    {
        public Transform area;
        public UISprite  areaIcon1;
        public UISprite  areaIcon2;
        public UILabel   areaName;
        public UILabel   areaChapter;
        public TweenColor tweenColor;
        public Color32 areaMapColor;
        public bool isOpen = false;
    }

    public UIWorldMap()
    {
        mResPath = "World/UIWorldMap";
        Type = EWindowType.WINDOW;
    }

    protected override void OnAwake()
    {
        Transform top = transform.Find("Top");
        Transform center = transform.Find("Center");

        background = transform.Find("Background").GetComponent<UISprite>();
        btnBackCity = top.Find("Btn_BackCity").gameObject;
        mapColor = transform.Find("MapColor").GetComponent<UITexture>();
        resItemTexture1 = top.Find("Res/1/Icon").GetComponent<UITexture>();
        resItemTexture2 = top.Find("Res/2/Icon").GetComponent<UITexture>();
        resItemTexture3 = top.Find("Res/3/Icon").GetComponent<UITexture>();
        resItemTexture4 = top.Find("Res/4/Icon").GetComponent<UITexture>();
        resItemNum1 = top.Find("Res/1/Num").GetComponent<UILabel>();
        resItemNum2 = top.Find("Res/2/Num").GetComponent<UILabel>();
        resItemNum3 = top.Find("Res/3/Num").GetComponent<UILabel>();
        resItemNum4 = top.Find("Res/4/Num").GetComponent<UILabel>();

        Transform areaTrans = center.Find("Area");

        int pChapter = RaidModule.Instance.GetMaxChapterByCopyType(ECopyType.TYPE_WORLD);
        for (int i=0;i<13;i++)
        {
            WorldArea tab = new WorldArea();
            tab.area = areaTrans.Find((i+1).ToString());
            tab.areaIcon1 = tab.area.Find("1").GetComponent<UISprite>();
            tab.areaIcon2 = tab.area.Find("2").GetComponent<UISprite>();
            tab.areaName = tab.area.Find("Name").GetComponent<UILabel>();
            tab.areaChapter = tab.area.Find("Chapter").GetComponent<UILabel>();
            tab.tweenColor = tab.areaIcon1.transform.GetComponent<TweenColor>();
            tab.isOpen= i <= pChapter - 1;
            mAreas.Add(tab);
        }
        InitAreasColor();

        if(mapColor.mainTexture==null)
        { 
            GTItemHelper.ShowTexture(mapColor, "Textures/Other/MapColor");
        }
        worldMapTex = (Texture2D)mapColor.mainTexture;
        worldCollider = mapColor.GetComponent<BoxCollider>();
    } 

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnBackCity).onClick = OnBackCityClick;
    }

    private void OnBackCityClick(GameObject go)
    {
        Hide();
    }

    private void ShowView()
    {
        ShowInitView();
        ShowResView();
    }


    private void ShowInitView()
    {
        transform.Find("Center").gameObject.SetActive(true);
        mapColor.gameObject.SetActive(true);
        background.gameObject.SetActive(true);
        int pChapter = RaidModule.Instance.GetMaxChapterByCopyType(ECopyType.TYPE_WORLD);
        Color pOpenNameColor = new Color32(246, 139, 4,255);
        Color pOpenChapterColor = new Color32(246, 209, 4,255);
        for (int i=0;i<mAreas.Count;i++)
        {
            WorldArea tab = mAreas[i];
            tab.areaIcon1.gameObject.SetActive(true);
            GTItemHelper.ShowImageBlack(tab.areaIcon1, !tab.isOpen);
            int chapter = DWorld.WORLD_ST_CHAPTER_ID + i;
            DWorld db = ReadCfgWorld.GetDataById(chapter);
            tab.areaName.text = db.Name;
            tab.areaName.color = tab.isOpen ? pOpenNameColor : Color.gray;
            tab.areaChapter.text = GTTools.Format("第 {0} 章", chapter);
            tab.areaChapter.color = tab.isOpen ? pOpenChapterColor : Color.gray;
        }
    }

    private void ShowResView()
    {
        GTItemHelper.ShowItemTexture(resItemTexture1, 1);
        GTItemHelper.ShowItemTexture(resItemTexture2, 2);
        GTItemHelper.ShowItemTexture(resItemTexture3, 101);
        GTItemHelper.ShowItemTexture(resItemTexture4, 102);
        resItemNum1.text = DataManager.Instance.GetItemCountById(1).ToString();
        resItemNum2.text = DataManager.Instance.GetItemCountById(2).ToString();
        resItemNum3.text = DataManager.Instance.GetItemCountById(101).ToString();
        resItemNum4.text = DataManager.Instance.GetItemCountById(102).ToString();
    }

    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler(GTEventID.TYPE_MOUSE_BUTTON,    OnMouseButton);
        GTEventCenter.AddHandler(GTEventID.TYPE_MOUSE_BUTTON_UP, OnMouseButtonUp);
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler(GTEventID.TYPE_MOUSE_BUTTON, OnMouseButton);
        GTEventCenter.DelHandler(GTEventID.TYPE_MOUSE_BUTTON_UP, OnMouseButtonUp);
    }

 

    private void JudgeAreaByColor(Color32 wColor)
    {
        for(int i=0;i<mAreas.Count;i++)
        {
            WorldArea tab = mAreas[i];
            if (Math.Abs(wColor.r - tab.areaMapColor.r) < 10 &&
                Math.Abs(wColor.g - tab.areaMapColor.g) < 10 &&
                Math.Abs(wColor.b - tab.areaMapColor.b) < 10 &&
                Math.Abs(wColor.a - tab.areaMapColor.a) < 10)
            {
                mAreaIndex = i;
            }
        }
    }

    private void OnMouseButton()
    {
        if (Time.realtimeSinceStartup - mTime < 0.5f)
        {
            return;
        }
        if (!transform.gameObject.activeSelf)
        {
            return;
        }

        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        mTime = Time.realtimeSinceStartup;
        Vector3 pos = Input.mousePosition;
        float rx = background.width * 1f / Screen.width;
        float ry = background.height * 1f / Screen.height;
        int x = (int)(pos.x * rx - (background.width - worldMapTex.width) * 0.5f);
        int y = (int)(pos.y * ry - (background.height - worldMapTex.height) * 0.5f);
        Color32 wColor = worldMapTex.GetPixel(x, y);
        JudgeAreaByColor(wColor);

        if (mAreaIndex < 0 || wColor.a < 120)
        {
            return;
        }
        if (!mAreas[mAreaIndex].isOpen && mAreaIndex > 0)
        {
            string s = GTTools.Format("通关第{0}章：{1} 后解锁", DWorld.WORLD_ST_CHAPTER_ID + mAreaIndex - 1, mAreas[mAreaIndex - 1].areaName.text);
            GTItemHelper.ShowTip(s);
            return;
        }
        mAreas[mAreaIndex].areaIcon1.gameObject.SetActive(true);
        for (int i = 0; i < mAreas.Count; i++)
        {
            if (mAreas[i].isOpen)
            {
                mAreas[i].areaIcon2.gameObject.SetActive(i == mAreaIndex);
            }
        }
    }

    private void OnMouseButtonUp()
    {
        if(mAreaIndex>=0)
        {
            if (!mAreas[mAreaIndex].isOpen)
            {
                return;
            }
            mAreas[mAreaIndex].areaIcon2.gameObject.SetActive(false);
            this.OnSelectMap(mAreaIndex);
            mAreaIndex = -1;
        }
    }

    private void OnSelectMap(int index)
    {
        GTWindowManager.Instance.OpenWindow(EWindowID.UI_MAINRAID);
        UIMainRaid window = (UIMainRaid)GTWindowManager.Instance.GetWindow(EWindowID.UI_MAINRAID);
        window.ShowViewByChapter(DWorld.WORLD_ST_CHAPTER_ID + index);
    }

    private void InitAreasColor()
    {
        mAreas[0].areaMapColor = new Color32(1, 72, 15, 255);
        mAreas[1].areaMapColor = new Color32(255, 255, 0, 255);
        mAreas[2].areaMapColor = new Color32(0, 0, 0, 255);
        mAreas[3].areaMapColor = new Color32(12, 24, 147, 255);
        mAreas[4].areaMapColor = new Color32(0, 255, 90, 255);
        mAreas[5].areaMapColor = new Color32(255, 12, 0, 255);
        mAreas[6].areaMapColor = new Color32(97, 255, 235, 255);
        mAreas[7].areaMapColor = new Color32(123, 0, 255, 255);
        mAreas[8].areaMapColor = new Color32(253, 255, 92, 255);
        mAreas[9].areaMapColor = new Color32(29, 117, 255, 255);
        mAreas[10].areaMapColor = new Color32(222, 0, 255, 255);
        mAreas[11].areaMapColor = new Color32(255, 132, 0, 255);
        mAreas[12].areaMapColor = new Color32(255, 255, 255, 255);
    }

    protected override void OnEnable()
    {
        ShowView();
        GameObject btn1 = mAreas[0].area.gameObject;
        UIEventListener.Get(btn1).onClick = (GameObject go) => { OnGuideClickChapter(0); };
        GuideManager.Instance.AddListener("Btn_Chapter1", btn1.transform);
    }

    private void OnGuideClickChapter(int index)
    {
        BoxCollider pCollider = mAreas[index].area.gameObject.GetComponent<BoxCollider>();
        if (pCollider != null)
        {
            UnityEngine.Object.Destroy(mAreas[index].area.gameObject);
        }
        OnMouseButton();
    }

    protected override void OnClose()
    {
        mAreas.Clear();
        mAreaIndex = -1;
        GuideManager.Instance.DelListener("Btn_Chapter1");
    }
}
