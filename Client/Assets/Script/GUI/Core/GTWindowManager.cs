using System;
using System.Collections.Generic;
using System.Collections;
using System.Text;
using UnityEngine;


public class GTWindowManager : GTSingleton<GTWindowManager>
{
    private Dictionary<EWindowID, GTWindow>         mAllWindows = new Dictionary<EWindowID, GTWindow>();
    private Dictionary<EWindowType, List<GTWindow>> mOpenWindows = new Dictionary<EWindowType, List<GTWindow>>();
    private Dictionary<EWindowType, int>            mMinDepths = new Dictionary<EWindowType, int>();
    private List<GTWindow>                          mOpenWinStack = new List<GTWindow>();
    private int mToggleGroupId = 1;

    public GTWindowManager()
    {
        this.SetAllDepth();
        this.RegisterWindows();
    }

    void SetAllDepth()
    {
        mMinDepths[EWindowType.BOTTOM] = 100;
        mMinDepths[EWindowType.FLYWORD] = 200;
        mMinDepths[EWindowType.FIXED]  = 500;
        mMinDepths[EWindowType.WINDOW] = 700;
        mMinDepths[EWindowType.DIALOG] = 900;
        mMinDepths[EWindowType.MSGTIP] = 2000;
        mMinDepths[EWindowType.LOADED] = 4000;
        mMinDepths[EWindowType.MASKED] = 5000;
    }

    void RegisterWindows()
    {
        RegisterWindow(EWindowID.UI_LOGIN, new UILogin());
        RegisterWindow(EWindowID.UI_LOADING,  new UILoading());
        //RegisterWindow(EWindowID.UI_CREATEROLE, new UIRole());
        //RegisterWindow(EWindowID.UI_HOME, new UIHome());
        //RegisterWindow(EWindowID.UI_HEROINFO, new UIHeroInfo());
        //RegisterWindow(EWindowID.UI_SETTING, new UISetting());

        //RegisterWindow(EWindowID.UI_BAG, new UIBag());
        //RegisterWindow(EWindowID.UI_ROLEEQUIP, new UIRoleEquip());
        //RegisterWindow(EWindowID.UI_ROLEGEM, new UIRoleGem());
        //RegisterWindow(EWindowID.UI_ROLEFASHION, new UIRoleFashion());
        //RegisterWindow(EWindowID.UI_ROLERUNE, new UIRoleRune());
        //RegisterWindow(EWindowID.UI_ROLEFETTER, new UIRoleFetter());

        //RegisterWindow(EWindowID.UI_ITEMINFO, new UIItemInfo());
        //RegisterWindow(EWindowID.UI_ITEMUSE, new UIItemUse());
        //RegisterWindow(EWindowID.UI_EQUIPINFO, new UIEquipInfo());
        //RegisterWindow(EWindowID.UI_GEMINFO, new UIGemInfo());
        //RegisterWindow(EWindowID.UI_CHIPINFO, new UIChipInfo());
        //RegisterWindow(EWindowID.UI_EQUIP, new UIEquip());
        //RegisterWindow(EWindowID.UI_GEM, new UIGem());

        //RegisterWindow(EWindowID.UI_MESSAGETIP, new UIMessageTip());
        //RegisterWindow(EWindowID.UI_AWARDTIP, new UIAwardTip());

        //RegisterWindow(EWindowID.UI_MAINRAID, new UIMainRaid());
        //RegisterWindow(EWindowID.UI_MAINGATE, new UIMainGate());
        //RegisterWindow(EWindowID.UI_MAINRESULT, new UIMainResult());

        //RegisterWindow(EWindowID.UI_AWARDBOX, new UIAwardBox());
        

        //RegisterWindow(EWindowID.UI_MOUNT, new UIMount());
        //RegisterWindow(EWindowID.UI_MOUNTLIBRARY, new UIMountLibrary());
        //RegisterWindow(EWindowID.UI_MOUNTBLOOD, new UIMountBlood());
        //RegisterWindow(EWindowID.UI_MOUNTTAME, new UIMountTame());
        //RegisterWindow(EWindowID.UI_MOUNTTURNED, new UIMountTurned());

        //RegisterWindow(EWindowID.UI_RELICS, new UIRelics());
        //RegisterWindow(EWindowID.UI_STORE, new UIStore());
        //RegisterWindow(EWindowID.UI_PET, new UIPet());

        //RegisterWindow(EWindowID.UI_PARTNER, new UIPartner());
        //RegisterWindow(EWindowID.UI_PARTNERBATTLE, new UIPartnerBattle());
        //RegisterWindow(EWindowID.UI_PARTNERSTRENGH, new UIPartnerStrength());
        //RegisterWindow(EWindowID.UI_PARTNERADVANCE, new UIPartnerAdvance());
        //RegisterWindow(EWindowID.UI_PARTNERFETTER, new UIPartnerFetter());
        //RegisterWindow(EWindowID.UI_PARTNERFPROPERTY, new UIPartnerProperty());
        //RegisterWindow(EWindowID.UI_PARTNERSKILL, new UIPartnerSkill());
        //RegisterWindow(EWindowID.UI_PARTNERSTAR, new UIPartnerStar());
        //RegisterWindow(EWindowID.UI_PARTNERWAKE, new UIPartnerWake());
        //RegisterWindow(EWindowID.UI_PARTNERWASH, new UIPartnerWash());

        //RegisterWindow(EWindowID.UI_TASK, new UITask());
        //RegisterWindow(EWindowID.UI_TASKTALK, new UITaskTalk());
        //RegisterWindow(EWindowID.UI_WORLDMAP, new UIWorldMap());

        //RegisterWindow(EWindowID.UI_PLOTCUTSCENE, new UIPlotCutscene());
        //RegisterWindow(EWindowID.UI_GUIDE, new UIGuide());

        //RegisterWindow(EWindowID.UI_ADVENTURE, new UIAdventure());
        //RegisterWindow(EWindowID.UI_SKILL, new UISkill());
        //RegisterWindow(EWindowID.UI_REBORN, new UIReborn());
        //RegisterWindow(EWindowID.UI_MAINBOSSHP, new UIMainBossHP());
    }

    void RegisterWindow(EWindowID id,GTWindow win)
    { 
        mAllWindows[id] = win;
        win.ID = id;
    }

    void FindPanels(GTWindow window,ref List<UIPanel> panels)
    {
        if (window == null||window.Root==null)
        {
            return;
        }
        panels.AddRange(window.Root.GetComponents<UIPanel>());
        panels.AddRange(window.Root.GetComponentsInChildren<UIPanel>());
    }

    void RefreshDepth(GTWindow window)
    {
        EWindowType type = window.Type;
        List<UIPanel> pList = new List<UIPanel>();
        FindPanels(window, ref pList);

        List<UIPanel> aList = new List<UIPanel>();
        List<GTWindow> windows = mOpenWindows[type];
        for(int i=0;i<windows.Count;i++)
        {
            FindPanels(windows[i], ref aList);
        }
        for (int i = aList.Count - 1; i >= 0; i--)
        {
            if (aList[i] == null || aList[i].transform == null)
            {
                aList.RemoveAt(i);
            }
        }
        if (pList.Count >= 2)
        {
            pList.Sort(UIPanel.CompareFunc);
        }
        int stDepth = mMinDepths[type];
        aList.ForEach(item => { stDepth = item.depth > stDepth ? item.depth : stDepth; });
        pList.ForEach(item => { stDepth += 2; item.depth = stDepth; });
        UIPanel.list.Sort(UIPanel.CompareFunc);
    }

    void DealWindowStack(GTWindow win,bool open)
    {
        if (win.Type != EWindowType.WINDOW)
        {
            return;
        }
        if (open)
        {
            for (int i = 0; i < mOpenWinStack.Count; i++)
            {
                if (mOpenWinStack[i] != win)
                {
                    mOpenWinStack[i].CacheTransform.gameObject.SetActive(false);
                }
            }
            mOpenWinStack.Add(win);
        }
        else
        {
            mOpenWinStack.Remove(win);
            if (mOpenWinStack.Count > 0)
            {
                GTWindow last = mOpenWinStack[mOpenWinStack.Count - 1];
                last.CacheTransform.gameObject.SetActive(true);
            }
        }
    }

    void CloseAllWindowByType(EWindowType type)
    {
        List<GTWindow> list = null;
        mOpenWindows.TryGetValue(type, out list);
        if (list == null) return;
        for (int i = 0; i < list.Count; i++)
        {
            if (!list[i].HasParentWindow)
            {
                list[i].Hide();
            }
        }
    }

    public GTWindow OpenWindow(EWindowID windowID)
    {
        if (!mAllWindows.ContainsKey(windowID))
        {
            return null;
        }
        GTWindow window = mAllWindows[windowID];
        DealWindowStack(window, true);
        window.Show();
        Transform trans = window.CacheTransform;
        if (trans == null)
            return null;
        if (window.Type == EWindowType.WINDOW)
        {
            CloseAllWindowByType(EWindowType.DIALOG);
        }
        if (window.Type == EWindowType.WINDOW || window.Type == EWindowType.DIALOG)
        {
            GTEventCenter.FireEvent(GTEventID.TYPE_STOP_JOYSTICK);
        }
        GTCameraManager.Instance.AddUI(trans.gameObject);
        List<GTWindow> list = null;
        mOpenWindows.TryGetValue(window.Type, out list);
        if (list == null)
        {
            list = new List<GTWindow>();
            mOpenWindows[window.Type] = list;
        }
        list.Add(window);
        RefreshDepth(window);
        return window;
    }

    public void     CloseWindow(EWindowID windowID)
    {
        GTWindow window = mAllWindows[windowID];
        if (window == null) return;
        EWindowType type = window.Type;
        window.Close();
        List<GTWindow> list = null;
        mOpenWindows.TryGetValue(type, out list);
        if (list != null)
        {
            list.Remove(window);
        }
        DealWindowStack(window, false);
    }

    public GTWindow GetWindow(EWindowID windowID)
    {
        GTWindow window = null;
        mAllWindows.TryGetValue(windowID, out window);
        return window;
    }

    public T GetWindow<T>(EWindowID windowID) where T : GTWindow
    {
        GTWindow baseWindow = null;
        mAllWindows.TryGetValue(windowID, out baseWindow);
        T window = (T)baseWindow;
        return window;
    }

    public int GetToggleGroupId()
    {
        mToggleGroupId++;
        return mToggleGroupId;
    }

    public void Release()
    {
        mToggleGroupId = 1;
        foreach (KeyValuePair<EWindowID, GTWindow> pair in mAllWindows)
        {
            if (pair.Key != EWindowID.UI_LOADING)
            {
                pair.Value.Close();
            }
        }
        mOpenWindows.Clear();
        mOpenWinStack.Clear();
    }
}