using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;
using CFG;
using Protocol;
using ACT;

public class UIHome : GTWindow
{
    class ItemFunc
    {
        public string    name;
        public string    icon;
        public UIEventListener.VoidDelegate onClick;
    }

    class ItemSkill
    {
        public GameObject btn;
        public UISprite   maskSprite;
        public UISprite   lockSprite;
        public UITexture  icon;
    }

    class ItemBuff
    {
        public GameObject item;
        public UISprite   mask;
        public UISprite   icon;
        public UILabel    ovelayNum;
    }

    class ItemPartner
    {
        public GameObject heroGo;
        public UISprite   heroIcon;
        public UISprite   heroQuality;
        public UILabel    heroLevel;
        public UILabel    heroName;
        public UISlider   heroHPSlider;
        public UILabel    heroHPPercent;
    }

    private List<ItemFunc>                   mTopBtnTemps;
    private List<GameObject>                 mBottomButtons = new List<GameObject>();
    private List<GameObject>                 mTopButtons    = new List<GameObject>();
    private List<ItemBuff>                   mBuffItems     = new List<ItemBuff>();
    private Dictionary<ESkillPos, ItemSkill> mSkillBtns     = new Dictionary<ESkillPos, ItemSkill>();
    private List<ItemPartner>                mPartners      = new List<ItemPartner>();

    private Transform                        mBottomRight;
    private Transform                        mBottomLeft;
    private Transform                        mTopLeft;
    private Transform                        mTopRight;
    private Transform                        mTopRightGrid;
    private Transform                        mTop;
    private Transform                        mBottom;

    private EJoystick                        mNGUIJoystick;

    private UISlider                         mBar1;
    private UISlider                         mBar2;
    private UISlider                         mBar3;
    private UILabel                          mBarValue1;
    private UILabel                          mBarValue2;
    private UISprite                         mPlayerIcon;
    private UILabel                          mPlayerLevel;
    private UILabel                          mPlayerName;

    private GameObject                       btnMenu;
    private GameObject                       btnTemp;
    private GameObject                       btnHero;
    private GameObject                       btnJump;
    private GameObject                       btnBag;
    private GameObject                       btnRide;
    private GameObject                       btnMap;

    private UIToggle                         menuTask;
    private UIToggle                         menuPartner;
    private UIToggle                         menuAutoFight;

    private Transform                        contentTask;
    private Transform                        contentPartner;
    private UILabel                          mTaskContentType1;
    private UILabel                          mTaskContentType2;
    private UITexture                        mMapTexture;

    public UIHome()
    {
        Path = "Home/UIHome";
        Resident = false;
        Type = EWindowType.Window;
        MaskType = EWindowMaskType.None;
        ShowMode = EWindowShowMode.HideOther;
        mTopBtnTemps = new List<ItemFunc>
        {

           new ItemFunc {name="副本",icon="427",onClick=OnRaidClick},
           new ItemFunc {name="商店",icon="425",onClick=OnShopClick},
           new ItemFunc {name="冒险",icon="428",onClick=OnAdventureClick},
           new ItemFunc {name="任务",icon="426",onClick=OnTaskClick},
           new ItemFunc {name="技能",icon="431",onClick=OnSkillClick},
           new ItemFunc {name="神器",icon="432",onClick=OnRelicsClick},
           new ItemFunc {name="坐骑",icon="422",onClick=OnMountClick},
           new ItemFunc {name="伙伴",icon="430",onClick=OnPartnerClick},
           new ItemFunc {name="宠物",icon="424",onClick=OnPetClick},
           new ItemFunc {name="设置",icon="432",onClick=OnSettingClick},
        };
    }

    protected override void OnAwake()
    {
        mTop = transform.Find("Top");
        mTopLeft = transform.Find("TopLeft");
        mTopRight = transform.Find("TopRight");
        mBottom = transform.Find("Bottom");
        mBottomRight = transform.Find("BottomRight");
        mBottomLeft = transform.Find("BottomLeft");
        mTopRightGrid = mTopRight.Find("Grid");

        btnMenu = mTopRight.Find("Pivot/Btn_Menu").gameObject;
        btnBag  = mTopRight.Find("Pivot/Btn_Bag").gameObject;
 
        for (int i = 0; i < 6; i++)
        {
            ItemSkill tab = new ItemSkill();
            Transform btn = mBottomRight.Find(i.ToString());
            tab.btn = btn.gameObject;
            tab.lockSprite = btn.transform.Find("Lock").GetComponent<UISprite>();
            tab.maskSprite = btn.transform.Find("Mask").GetComponent<UISprite>();
            tab.icon = btn.transform.Find("Icon").GetComponent<UITexture>();
            tab.maskSprite.gameObject.SetActive(false);
            mSkillBtns.Add((ESkillPos)i, tab);
        }
        mNGUIJoystick = mBottomLeft.Find("Joystick").GetComponent<EJoystick>();
        mMapTexture = mTopRight.Find("Map/Texture").GetComponent<UITexture>();

        mBar1 = mTopLeft.Find("Bar_1").GetComponent<UISlider>();
        mBar2 = mTopLeft.Find("Bar_2").GetComponent<UISlider>();
        mBar3 = mTopLeft.Find("Bar_3").GetComponent<UISlider>();
        mBarValue1 = mBar1.transform.Find("Num").GetComponent<UILabel>();
        mBarValue2 = mBar2.transform.Find("Num").GetComponent<UILabel>();
        mPlayerIcon = mTopLeft.Find("Hero/Icon").GetComponent<UISprite>();
        mPlayerLevel = mTopLeft.Find("Hero/Level").GetComponent<UILabel>();
        mPlayerName = mTopLeft.Find("Hero/Name").GetComponent<UILabel>();

        btnHero = mTopLeft.Find("Hero").gameObject;
        btnJump = mBottomRight.Find("Btn_Jump").gameObject;
        btnRide = mBottomRight.Find("Btn_Ride").gameObject;
        btnTemp = transform.Find("BtnTemplate").gameObject;
        btnTemp.SetActive(false);
  
        for (int i = 1; i <= 4; i++)
        {
            ItemBuff tab = new ItemBuff();
            Transform trans = mTopLeft.transform.Find("Buff/" + i);
            tab.item = trans.gameObject;
            tab.item.SetActive(false);
            tab.icon = trans.Find("Icon").GetComponent<UISprite>();
            tab.mask = trans.Find("Mask").GetComponent<UISprite>();
            tab.ovelayNum = trans.Find("Num").GetComponent <UILabel>();
            mBuffItems.Add(tab);
        }

        menuTask = mTopLeft.Find("Menus/Menu_Task").GetComponent<UIToggle>();
        menuPartner = mTopLeft.Find("Menus/Menu_Partner").GetComponent<UIToggle>();
        menuAutoFight = mBottomRight.Find("Btn_Auto").GetComponent<UIToggle>();
        contentPartner = mTopLeft.Find("Menus/Content_Partner");
        contentTask = mTopLeft.Find("Menus/Content_Task");

        int group = GTWindowManager.Instance.GetToggleGroupId();
        menuTask.group = group;
        menuPartner.group = group;
        switch(GTLauncher.Instance.CurrCopyType)
        {
            case ECopyType.TYPE_CITY:
                menuTask.value = true;
                break;
            case ECopyType.TYPE_WORLD:
                menuTask.value = true;
                break;
            case ECopyType.TYPE_PVE:
                menuPartner.value = true;
                break;
        }
        mTaskContentType1 = contentTask.Find("Type1").GetComponent<UILabel>();
        mTaskContentType2 = contentTask.Find("Type2").GetComponent<UILabel>();

        for (int i = 1; i <= 2; i++)
        {
            Transform trans = contentPartner.Find(i.ToString());
            ItemPartner tab = new ItemPartner();
            tab.heroGo = trans.gameObject;
            tab.heroIcon = trans.Find("Icon").GetComponent<UISprite>();
            tab.heroQuality = trans.Find("Quality").GetComponent<UISprite>();
            tab.heroName = trans.Find("Name").GetComponent<UILabel>();
            tab.heroLevel = trans.Find("Level").GetComponent<UILabel>();
            tab.heroHPSlider = trans.Find("HP").GetComponent<UISlider>();
            tab.heroHPPercent = trans.Find("HP/Num").GetComponent<UILabel>();
            tab.heroGo.SetActive(false);
            mPartners.Add(tab);
        }
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnMenu).                 onClick = OnMenuClick;
        UIEventListener.Get(btnHero).                 onClick = OnHeroClick;
        for (int i = 0; i < mSkillBtns.Count; i++)
        {
            GameObject btn = mSkillBtns[(ESkillPos)i].btn;
            UIEventListener.Get(btn).                 onClick = OnCastSkillClick;
        }
        UIEventListener.Get(btnJump).                 onClick = OnJumpClick;
        UIEventListener.Get(btnRide).                 onClick = OnRideClick;
        UIEventListener.Get(btnBag).                  onClick = OnBagClick;
        UIEventListener.Get(menuAutoFight.gameObject).onClick = OnAutoClick;
    }

    protected override void OnAddHandler()
    {
        mNGUIJoystick.On_JoystickMove += OnJoystickMove;
        mNGUIJoystick.On_JoystickMoveEnd += OnJoystickMoveEnd;

        GTEventCenter.AddHandler(GTEventID.TYPE_UPDATE_AVATAR_HP,         OnUpdateAvatarHealth);
        GTEventCenter.AddHandler(GTEventID.TYPE_UPDATE_AVATAR_MP,         OnUpdateAvatarPower);
        GTEventCenter.AddHandler(GTEventID.TYPE_UPDATE_AVATAR_ATTR,       OnUpdateAttr);
        GTEventCenter.AddHandler(GTEventID.TYPE_UPDATE_PARTNER_HP,        OnRefreshPartner);
        GTEventCenter.AddHandler(GTEventID.TYPE_UPDATE_AVATAR_BUFF,       OnRefreshBuffItems);
        GTEventCenter.AddHandler(GTEventID.TYPE_CHANGE_HEROLEVEL,         OnUpdateAvatarLevel);
        GTEventCenter.AddHandler(GTEventID.TYPE_CHANGE_HERONAME,          OnUpdateAvatarName);
        GTEventCenter.AddHandler(GTEventID.TYPE_CHANGE_HEROHEAD,          OnUpdateAvatarIcon);
        GTEventCenter.AddHandler<int, int>(GTEventID.TYPE_PARTNER_CHANGE, OnRefreshPartner);
        GTEventCenter.AddHandler(GTEventID.TYPE_PARTNER_ADVANCE,          OnRefreshPartner);
        GTEventCenter.AddHandler(GTEventID.TYPE_PARTNER_UPGRADE,          OnRefreshPartner);
        GTEventCenter.AddHandler(GTEventID.TYPE_TASK_THREAD_STATE,        OnUpdateThreadTask);
        GTEventCenter.AddHandler(GTEventID.TYPE_CHANGE_HEROEXP,           OnUpdateAvatarExp);
        GTEventCenter.AddHandler(GTEventID.TYPE_FORCE_STOPJOYSTICK,       OnForceToStopJoystick);
        GTUpdate.Instance.AddListener(OnUpdate);
    }

    protected override void OnDelHandler()
    {
        mNGUIJoystick.On_JoystickMove -= OnJoystickMove;
        mNGUIJoystick.On_JoystickMoveEnd -= OnJoystickMoveEnd;
        GTEventCenter.DelHandler(GTEventID.TYPE_UPDATE_AVATAR_HP,         OnUpdateAvatarHealth);
        GTEventCenter.DelHandler(GTEventID.TYPE_UPDATE_AVATAR_MP,         OnUpdateAvatarPower);
        GTEventCenter.DelHandler(GTEventID.TYPE_UPDATE_PARTNER_HP,        OnRefreshPartner);
        GTEventCenter.DelHandler(GTEventID.TYPE_UPDATE_AVATAR_ATTR,       OnUpdateAttr);
        GTEventCenter.DelHandler(GTEventID.TYPE_UPDATE_AVATAR_BUFF,       OnRefreshBuffItems);
        GTEventCenter.DelHandler(GTEventID.TYPE_CHANGE_HEROLEVEL,         OnUpdateAvatarLevel);
        GTEventCenter.DelHandler(GTEventID.TYPE_CHANGE_HERONAME,          OnUpdateAvatarName);
        GTEventCenter.DelHandler(GTEventID.TYPE_CHANGE_HEROHEAD,          OnUpdateAvatarIcon);
        GTEventCenter.DelHandler<int, int>(GTEventID.TYPE_PARTNER_CHANGE, OnRefreshPartner);
        GTEventCenter.DelHandler(GTEventID.TYPE_PARTNER_ADVANCE,          OnRefreshPartner);
        GTEventCenter.DelHandler(GTEventID.TYPE_PARTNER_UPGRADE,          OnRefreshPartner);
        GTEventCenter.DelHandler(GTEventID.TYPE_TASK_THREAD_STATE,        OnUpdateThreadTask);
        GTEventCenter.DelHandler(GTEventID.TYPE_CHANGE_HEROEXP,           OnUpdateAvatarExp);
        GTEventCenter.DelHandler(GTEventID.TYPE_FORCE_STOPJOYSTICK,       OnForceToStopJoystick);
        GTUpdate.Instance.DelListener(OnUpdate);
    }

    protected override void OnEnable()
    {
        InitButtons();
        ShowMenus();
        Character player = GTWorld.Main;
        if (player == null || player.CacheTransform == null)
        {
            return;
        }
        OnUpdateMapTexture();
        OnRefreshSkillItems();
        OnRefreshSkillIcons();
        OnRefreshBuffItems();
        OnRefreshPartner();
        OnUpdateAvatarHealth();
        OnUpdateAvatarPower();
        OnUpdateAvatarLevel();
        OnUpdateAvatarExp();
        OnUpdateAvatarName();
        OnUpdateThreadTask();
        OnUpdateBranchTask();
        OnUpdateAutoFightState();
        OnUpdateRideState();
    }

    protected override void OnClose()
    {
        mBottomButtons.Clear();
        mTopButtons.Clear();
        mSkillBtns.Clear();
        mBuffItems.Clear();
        mPartners.Clear();
    }

    private void InitButtons()
    {
        UIGrid grid = mTopRightGrid.GetComponent<UIGrid>();
        for (int i = 0; i < mTopBtnTemps.Count; i++)
        {
            ItemFunc data = mTopBtnTemps[i];
            GameObject item = NGUITools.AddChild(mTopRightGrid.gameObject, btnTemp);
            grid.AddChild(item.transform);
            item.SetActive(true);
            item.transform.Find("Name").GetComponent<UILabel>().text = data.name.ToString();
            item.transform.Find("Icon").GetComponent<UISprite>().spriteName = data.icon.ToString();
            UIEventListener.Get(item).onClick = data.onClick;
            mTopButtons.Add(item);
        }
    }

    private void HideMenus()
    {
        UIToggle toggle = btnMenu.GetComponent<UIToggle>();
        if (toggle.value)
        {
            toggle.value = false;
        }
        ShowMenus();
    }

    private void ShowMenus()
    {
        UIToggle toggle = btnMenu.GetComponent<UIToggle>();
        mTopRightGrid.gameObject.SetActive(toggle.value);
    }

    private void OnSettingClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTWindowManager.Instance.OpenWindow(EWindowID.UISetting);
    }

    private void OnAdventureClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTWindowManager.Instance.OpenWindow(EWindowID.UIAdventure);
    }

    private void OnRaidClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTWindowManager.Instance.OpenWindow(EWindowID.UIMainRaid);
    }

    private void OnShopClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTWindowManager.Instance.OpenWindow(EWindowID.UIStore);
    }

    private void OnMenuClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        UIToggle toggle = go.GetComponent<UIToggle>();
        ShowMenus();
    }

    private void OnRideClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        Character mainPlayer = GTWorld.Main;
        if (mainPlayer.IsRide)
        {
            GTNetworkSend.Instance.TryLeaveMount();
        }
        else
        {
            GTNetworkSend.Instance.TryStartMount();
        }
        OnUpdateRideState();
    }

    private void OnTaskClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTWindowManager.Instance.OpenWindow(EWindowID.UITask);
    }

    private void OnPartnerClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTWindowManager.Instance.OpenWindow(EWindowID.UIPartner);
    }

    private void OnPetClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTWindowManager.Instance.OpenWindow(EWindowID.UIPet);
    }

    private void OnRelicsClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTWindowManager.Instance.OpenWindow(EWindowID.UIRelics);
    }

    private void OnBagClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTWindowManager.Instance.OpenWindow(EWindowID.UIBag);
    }

    private void OnSkillClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTWindowManager.Instance.OpenWindow(EWindowID.UISkill);
    }

    private void OnMountClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTWindowManager.Instance.OpenWindow(EWindowID.UIMount);
    }

    private void OnHeroClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTWindowManager.Instance.OpenWindow(EWindowID.UIHeroInfo);
    }

    private void OnJumpClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTNetworkSend.Instance.TryJump();
    }

    private void OnAutoClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTWorld.Main.EnableAI(!GTWorld.Main.IsAIEnabled);
        OnUpdateAutoFightState();
    }

    private void OnPauseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTLauncher.Instance.LoadScene(GTGlobal.LastCityID);
    }

    private void OnCastSkillClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_SKILLCLICK, 0.2f);
        int pos = go.name.ToInt32();
        ESkillPos skillPos = (ESkillPos)pos;
        GTEventCenter.FireEvent(GTEventID.TYPE_ACTOR_CAST_SKILL, skillPos);
    }

    private void OnUpdate()
    {
        Character player = GTWorld.Main;
        if (player == null || player.CacheTransform == null)
        {
            return;
        }
        OnRefreshSkillItems();
        OnRefreshBuffItems();
    }

    private void OnUpdateMapTexture()
    {
        Camera cam = GTCameraManager.Instance.MinimapCamera;
        if (cam == null)
        {
            return;
        }
        mMapTexture.mainTexture = cam.targetTexture;
    }

    private void OnUpdateAvatarPower()
    {
        Character player = GTWorld.Main;
        int maxMP = player.CurrAttr.MaxMP;
        int curMP = player.CurrAttr.MP;
        mBar2.value = curMP / (maxMP * 1f);
        mBarValue2.text = GTTools.Format("{0}/{1}", curMP, maxMP);
    }

    private void OnUpdateAvatarHealth()
    {
        Character player = GTWorld.Main;
        int maxHP = player.CurrAttr.MaxHP;
        int curHP = player.CurrAttr.HP;
        mBar1.value = curHP / (maxHP * 1f);
        mBarValue1.text = GTTools.Format("{0}/{1}", curHP, maxHP);
    }

    private void OnUpdateAvatarName()
    {
        Character player = GTWorld.Main;
        mPlayerName.text = player.Name;
    }

    private void OnUpdateAvatarLevel()
    {
        Character player = GTWorld.Main;
        mPlayerLevel.text = player.Level.ToString();
    }

    private void OnUpdateAvatarExp()
    {
        XCharacter role = GTGlobal.Main;
        int maxExp = ReadCfgRoleLevel.GetDataById(role.Level).RequireExp;
        mBar3.value = (role.CurExp * 1f) / maxExp;
    }

    private void OnUpdateAvatarIcon()
    {
        Character player = GTWorld.Main;
        DActor db = ReadCfgActor.GetDataById(player.ID);
        mPlayerIcon.spriteName = db.Icon;
    }

    private void OnUpdateAttr()
    {
        OnUpdateAvatarHealth();
        OnUpdateAvatarPower();
    }

    private void OnForceToStopJoystick()
    {
        mNGUIJoystick.ForceToEnd();
    }

    private void OnUpdateThreadTask()
    {

    }

    private void OnUpdateBranchTask()
    {
        string format = GTItemHelper.GetText("[00ffff]【支线】[-]{0}");
        string content = GTItemHelper.GetText("没有发现新任务");
        mTaskContentType2.text = GTTools.Format(format, content);
    }

    private void OnRefreshSkillItems()
    {
        foreach(var current in mSkillBtns)
        {
            ESkillPos pos = current.Key;
            ActSkill skill = GTWorld.Main.Skill.GetSkill(pos);
            ItemSkill tab = current.Value;
            if (skill == null)
            {
                tab.maskSprite.gameObject.SetActive(true);
                tab.lockSprite.gameObject.SetActive(true);
            }
            else
            {
                tab.lockSprite.gameObject.SetActive(false);
                if(skill.IsCD())
                {
                    tab.maskSprite.gameObject.SetActive(true);
                    tab.maskSprite.fillAmount = skill.GetLeftTime() / skill.CD;
                }
                else
                {
                    tab.maskSprite.gameObject.SetActive(false);
                }
            }
        }
    }

    private void OnRefreshSkillIcons()
    {
        foreach (var current in mSkillBtns)
        {
            ESkillPos pos = current.Key;
            ActSkill skill = GTWorld.Main.Skill.GetSkill(pos);
            ItemSkill tab = current.Value;
            if (skill != null)
            {
                DSkill db = ReadCfgSkill.GetDataById(skill.ID);
                if (db != null)
                {
                    GTItemHelper.ShowTexture(tab.icon, db.Icon);
                }
            }
        }
    }

    private void OnRefreshBuffItems()
    {
        Dictionary<int, ActBuff> buffs = GTWorld.Main.Skill.GetBuffs();
        Dictionary<int, ActBuff>.Enumerator em = buffs.GetEnumerator();
        int index = 0;
        while(em.MoveNext())
        {
            if (index > mBuffItems.Count - 1)
            {
                break;
            }
            ActBuff current = em.Current.Value;
            ItemBuff tab = mBuffItems[index];
            tab.item.SetActive(true);
            tab.icon.spriteName = current.Icon;
            switch(current.OverlayType)
            {
                case EBuffOverlayType.Overlay:
                case EBuffOverlayType.OverlayAndRefresh:
                    tab.ovelayNum.text = (current.MaxOverlayNum > 0 && current.CurOverlayNum > 1) ?
                        current.CurOverlayNum.ToString() : string.Empty;
                    break;
                case EBuffOverlayType.Refresh:
                case EBuffOverlayType.UnChange:
                    tab.ovelayNum.text = string.Empty;
                    break;
            }
            if (current.Duration > 0)
            {
                tab.mask.fillAmount = current.LeftTime / current.Duration;
            }
            else
            {
                tab.mask.fillAmount = 0;
            }
            index++;
        }
        em.Dispose();
        for (int i = index; i < mBuffItems.Count; i++)
        {
            ItemBuff tab = mBuffItems[index];
            tab.item.SetActive(false);
        }
    }

    private void OnJoystickMoveEnd(EJoystick move)
    {
        GTNetworkSend.Instance.TryStopJoystick();
    }

    private void OnJoystickMove(EJoystick move)
    {
        float x = move.joystickAxis.x;
        float y = move.joystickAxis.y;
        if (Math.Abs(x) > 0.1f || Math.Abs(y) > 0.1f)
        {
            GTNetworkSend.Instance.TryMoveJoystick(x, y);
        }
    }

    private void OnRefreshPartner()
    {
        Character[] partners = { GTWorld.Main.Partner1, GTWorld.Main.Partner2};
        for (int i = 0; i < partners.Length; i++)
        {
            Character c = partners[i];
            ItemPartner tab = mPartners[i];
            if (c != null && c.ID > 0)
            {
                XPartner data = DataDBSPartner.GetDataById(c.ID);
                tab.heroGo.SetActive(true);
                DActor db = ReadCfgActor.GetDataById(c.ID);
                int level = data == null ? db.Level : data.Level;
                tab.heroLevel.text = GTTools.Format("Lv.{0}", level);
                GTItemHelper.ShowQualityText(tab.heroName, db.Name, db.Quality);
                tab.heroIcon.spriteName = db.Icon;
                tab.heroQuality.spriteName = "Q" + (db.Quality);
                if (c.CurrAttr.HP <= 0)
                {
                    tab.heroHPSlider.value = 0;
                    tab.heroHPPercent.text = "0%";
                    GTItemHelper.ShowImageBlack(tab.heroIcon, true);
                    GTItemHelper.ShowImageBlack(tab.heroQuality, true);
                }
                else
                {
                    tab.heroHPSlider.value = (c.CurrAttr.HP * 1f) / c.CurrAttr.MaxHP;
                    tab.heroHPPercent.text = tab.heroHPSlider.value.ToPercent();
                    GTItemHelper.ShowImageBlack(tab.heroIcon, false);
                    GTItemHelper.ShowImageBlack(tab.heroQuality, false);
                }
            }
            else
            {
                tab.heroGo.SetActive(false);
            }
        }
    }

    private void OnRefreshPartner(int arg1,int arg2)
    {
        OnRefreshPartner();
    }

    private void OnUpdateAutoFightState()
    {
        menuAutoFight.value = GTWorld.Main.IsAIEnabled;
    }

    private void OnUpdateRideState()
    {
        GameObject highLight = btnRide.transform.Find("HighLight").gameObject;
        highLight.SetActive(GTWorld.Main.IsRide);
    }
}