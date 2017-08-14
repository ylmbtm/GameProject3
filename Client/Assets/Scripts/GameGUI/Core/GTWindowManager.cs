using System;
using System.Collections.Generic;
using System.Collections;
using System.Text;
using UnityEngine;


public class GTWindowManager : GTSingleton<GTWindowManager>
{
    private Dictionary<EWindowID, GTWindow>         m_AllWindows       = new Dictionary<EWindowID, GTWindow>();
    private List<GTWindow>                          m_OpenWindows      = new List<GTWindow>();
    private Dictionary<EWindowType, int>            m_MinDepths        = new Dictionary<EWindowType, int>();
    private List<GTWindow>                          m_MutexStacks     = new List<GTWindow>();
    private int                                     m_ToggleGroupId    = 1;
    private MaskBlack                               m_Black            = null; 
    private MaskBlackTransparent                    m_BlackTransparent = null;
    private MaskWhiteTransparent                    m_WhiteTransparent = null;
    private MaskBlur                                m_Blur             = null;

    public override void Init()
    {
        this.SetAllDepth();
        this.RegisterWindows();
    }

    void SetAllDepth()
    {
        m_MinDepths[EWindowType.Bottom]   = 500;
        m_MinDepths[EWindowType.Window]   = 700;
        m_MinDepths[EWindowType.Message]  = 2000;
        m_MinDepths[EWindowType.Loaded]   = 4000;
        m_MinDepths[EWindowType.NewBie]   = 5000;
    }

    void RegisterWindows()
    {
        RegisterWindow(EWindowID.UILogin, new UILogin());
        RegisterWindow(EWindowID.UIServer, new UIServer());
        RegisterWindow(EWindowID.UINotice, new UINotice());
        RegisterWindow(EWindowID.UIAccount, new UIAccount());
        RegisterWindow(EWindowID.UINetWaiting, new UINetWaiting());

        RegisterWindow(EWindowID.UICreateRole, new UIRole());
        RegisterWindow(EWindowID.UIHome, new UIHome());
        RegisterWindow(EWindowID.UIHeroInfo, new UIHeroInfo());
        RegisterWindow(EWindowID.UISetting, new UISetting());

        RegisterWindow(EWindowID.UIBag, new UIBag());
        RegisterWindow(EWindowID.UIRoleEquip, new UIRoleEquip());
        RegisterWindow(EWindowID.UIRoleGem, new UIRoleGem());
        RegisterWindow(EWindowID.UIRoleFashion, new UIRoleFashion());
        RegisterWindow(EWindowID.UIRoleRune, new UIRoleRune());
        RegisterWindow(EWindowID.UIRoleFetter, new UIRoleFetter());

        RegisterWindow(EWindowID.UIItemInfo, new UIItemInfo());
        RegisterWindow(EWindowID.UIItemUse, new UIItemUse());
        RegisterWindow(EWindowID.UIEquipInfo, new UIEquipInfo());
        RegisterWindow(EWindowID.UIGemInfo, new UIGemInfo());
        RegisterWindow(EWindowID.UIChipInfo, new UIChipInfo());
        RegisterWindow(EWindowID.UIEquip, new UIEquip());
        RegisterWindow(EWindowID.UIGem, new UIGem());

        RegisterWindow(EWindowID.UIMessageTip, new UIMessageTip());
        RegisterWindow(EWindowID.UIMessageBox, new UIMessageBox());
        RegisterWindow(EWindowID.UIMessageBoxForNetwork, new UIMessageBoxForNetwork());
        RegisterWindow(EWindowID.UIAwardTip, new UIAwardTip());
        RegisterWindow(EWindowID.UIDialogue, new UIDialogue());

        RegisterWindow(EWindowID.UIMainRaid, new UIMainRaid());
        RegisterWindow(EWindowID.UIMainCopy, new UIMainCopy());
        RegisterWindow(EWindowID.UIMainResult, new UIMainResult());

        RegisterWindow(EWindowID.UIAwardBox, new UIAwardBox());
        RegisterWindow(EWindowID.UILoading,  new UILoading());

        RegisterWindow(EWindowID.UIMount, new UIMount());
        RegisterWindow(EWindowID.UIMountLibrary, new UIMountLibrary());
        RegisterWindow(EWindowID.UIMountBlood, new UIMountBlood());
        RegisterWindow(EWindowID.UIMountTame, new UIMountTame());
        RegisterWindow(EWindowID.UIMountTurned, new UIMountTurned());

        RegisterWindow(EWindowID.UIRelics, new UIRelics());
        RegisterWindow(EWindowID.UIStore, new UIStore());
        RegisterWindow(EWindowID.UIPet, new UIPet());

        RegisterWindow(EWindowID.UIPartner, new UIPartner());
        RegisterWindow(EWindowID.UIPartnerBattle, new UIPartnerBattle());
        RegisterWindow(EWindowID.UIPartnerStrength, new UIPartnerStrength());
        RegisterWindow(EWindowID.UIPartnerAdvance, new UIPartnerAdvance());
        RegisterWindow(EWindowID.UIPartnerFetter, new UIPartnerFetter());
        RegisterWindow(EWindowID.UIPartnerProperty, new UIPartnerProperty());
        RegisterWindow(EWindowID.UIPartnerSkill, new UIPartnerSkill());
        RegisterWindow(EWindowID.UIPartnerStar, new UIPartnerStar());
        RegisterWindow(EWindowID.UIPartnerWake, new UIPartnerWake());
        RegisterWindow(EWindowID.UIPartnerWash, new UIPartnerWash());

        RegisterWindow(EWindowID.UITask, new UITask());
        RegisterWindow(EWindowID.UITaskTalk, new UITaskTalk());

        RegisterWindow(EWindowID.UIPlotCutscene, new UIPlot());
        RegisterWindow(EWindowID.UIGuide, new UIGuide());

        RegisterWindow(EWindowID.UIAdventure, new UIAdventure());
        RegisterWindow(EWindowID.UISkill, new UISkill());
        RegisterWindow(EWindowID.UIReborn, new UIReborn());
        RegisterWindow(EWindowID.UIMainBossHP, new UIMainBossHP());
    }

    void RegisterWindow(EWindowID id, GTWindow win)
    { 
        m_AllWindows[id] = win;
        win.ID = id;
    }

    void FindPanels(GTWindow window,ref List<UIPanel> panels)
    {
        if (window == null||window.Panel==null)
        {
            return;
        }
        panels.AddRange(window.Panel.GetComponents<UIPanel>());
        panels.AddRange(window.Panel.GetComponentsInChildren<UIPanel>());
    }

    void RefreshDepth(GTWindow window)
    {
        EWindowType type = window.Type;
        List<UIPanel> pList = new List<UIPanel>();
        FindPanels(window, ref pList);

        List<UIPanel> aList = new List<UIPanel>();
        for (int i = 0; i < m_OpenWindows.Count; i++)
        {
            if(m_OpenWindows[i].Type == type)
            {
                FindPanels(m_OpenWindows[i], ref aList);
            }
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
        int stDepth = m_MinDepths[type];
        aList.ForEach(item => { stDepth = item.depth > stDepth ? item.depth : stDepth; });
        pList.ForEach(item => { stDepth += 2; item.depth = stDepth; });
        UIPanel.list.Sort(UIPanel.CompareFunc);
    }

    void DealWindowStack(GTWindow win, bool open)
    {
        if (win.ShowMode != EWindowShowMode.HideOther)
        {
            return;
        }
        if (open)
        {
            if (m_MutexStacks.Count > 0)
            {
                GTWindow w = m_MutexStacks[m_MutexStacks.Count - 1];
                for (int i = 0; i < m_OpenWindows.Count; i++)
                {
                    if (m_OpenWindows[i].ShowMode == EWindowShowMode.SaveTarget &&
                        w.ID == m_OpenWindows[i].TargetID)
                    {
                        m_OpenWindows[i].SetActive(false);
                    }
                    if (m_OpenWindows[i].Type == EWindowType.Window &&
                        m_OpenWindows[i].ShowMode == EWindowShowMode.DoNothing)
                    {
                        m_OpenWindows[i].Hide();
                    }
                }
                w.SetActive(false);
            }
            m_MutexStacks.Add(win);

        }
        else
        {
            m_MutexStacks.Remove(win);
            if (m_MutexStacks.Count > 0)
            {
                GTWindow last = m_MutexStacks[m_MutexStacks.Count - 1];
                last.SetActive(true);
                for (int i = 0; i < m_OpenWindows.Count; i++)
                {
                    if (m_OpenWindows[i].ShowMode == EWindowShowMode.SaveTarget && 
                        m_OpenWindows[i].TargetID == last.ID)
                    {
                        m_OpenWindows[i].SetActive(true);
                    }
                }
            }
        }
    }

    void DealMask()
    {
        GTWindow needBlackWindow = null;
        GTWindow needBlackTransparentWindow = null;
        GTWindow needWhiteTransparentWindow = null;
        GTWindow needBlurWindow = null;
        for (int i = 0; i < m_OpenWindows.Count; i++)
        {
            GTWindow w = m_OpenWindows[i];
            switch(w.MaskType)
            {
                case EWindowMaskType.Black:
                    FindMask(ref needBlackWindow, w);
                    break;
                case EWindowMaskType.BlackTransparent:
                    FindMask(ref needBlackTransparentWindow, w);
                    break;
                case EWindowMaskType.WhiteTransparent:
                    FindMask(ref needWhiteTransparentWindow, w);
                    break;
                case EWindowMaskType.Blur:
                    FindMask(ref needBlurWindow, w);
                    break;
            }
        }
        LoadMask<MaskBlack>(needBlackWindow, "Guis/Mask/MaskBlack", ref m_Black);
        LoadMask<MaskBlackTransparent>(needBlackTransparentWindow, "Guis/Mask/MaskBlackTransparent", ref m_BlackTransparent);
        LoadMask<MaskWhiteTransparent>(needWhiteTransparentWindow, "Guis/Mask/MaskWhiteTransparent", ref m_WhiteTransparent);
        LoadMask<MaskBlur>(needBlurWindow, "Guis/Mask/MaskBlur", ref m_Blur);
    }

    void LoadMask<T>(GTWindow needWindow, string path, ref T mask) where T : MaskGUI
    {
        if (needWindow != null)
        {
            if (mask == null)
            {
                mask = GTResourceManager.Instance.Load<GameObject>(path, true).GetComponent<T>();
                GTCameraManager.Instance.AddUI(mask.gameObject);
            }
            if (mask != null)
            {
                mask.gameObject.SetActive(true);
                mask.SetTargetWindow(needWindow);
            }
        }
        else
        {
            if (mask != null)
            {
                mask.gameObject.SetActive(false);
            }
        }
    }

    void FindMask(ref GTWindow needWindow, GTWindow w)
    {    
        if(w.IsVisable() == false)
        {
            return;
        }
        if (needWindow == null)
        {
            needWindow = w;
        }
        else
        {
            if (needWindow.Panel.depth < w.Panel.depth)
            {
                needWindow = w;
            }
        }
    }

    public GTWindow OpenWindow(EWindowID windowID)
    {
        if (!m_AllWindows.ContainsKey(windowID))
        {
            return null;
        }
        GTWindow window = m_AllWindows[windowID];
        DealWindowStack(window, true);
        window.Show();
        Transform trans = window.transform;
        if (trans == null)
        {
            return null;
        }
        if (window.MaskType != EWindowMaskType.None)
        {
            GTEventCenter.FireEvent(GTEventID.TYPE_FORCE_STOPJOYSTICK);
        }
        if (window.ShowMode == EWindowShowMode.SaveTarget && m_MutexStacks.Count > 0)
        {
            GTWindow w = m_MutexStacks[m_MutexStacks.Count - 1];
            window.TargetID = w.ID;
        }
        GTCameraManager.Instance.AddUI(trans.gameObject);
        if(m_OpenWindows.Contains(window) == false)
        {
            m_OpenWindows.Add(window);
        }
        RefreshDepth(window);
        DealMask();
        return window;
    }

    public void     HideWindow(EWindowID windowID)
    {
        GTWindow window = m_AllWindows[windowID];
        if (window == null)
        {
            return;
        }
        window.HideAsync();
        m_OpenWindows.Remove(window);
        DealWindowStack(window, false);
        DealMask();
    }

    public GTWindow GetWindow(EWindowID windowID)
    {
        GTWindow window = null;
        m_AllWindows.TryGetValue(windowID, out window);
        return window;
    }

    public T        GetWindow<T>(EWindowID windowID) where T : GTWindow
    {
        GTWindow baseWindow = null;
        m_AllWindows.TryGetValue(windowID, out baseWindow);
        T window = (T)baseWindow;
        return window;
    }

    public int      GetToggleGroupId()
    {
        m_ToggleGroupId++;
        return m_ToggleGroupId;
    }

    public void     LockNGUI(bool lockNGUI)
    {
        if(lockNGUI)
        {
            GTCameraManager.Instance.NGUICamera.GetComponent<UICamera>().eventReceiverMask = (1 << GTLayer.LAYER_DEFAULT);
        }
        else
        {
            GTCameraManager.Instance.NGUICamera.GetComponent<UICamera>().eventReceiverMask = (1 << GTLayer.LAYER_UI);
        }
    }

    public void     Release()
    {
        m_ToggleGroupId = 1;
        foreach (KeyValuePair<EWindowID, GTWindow> pair in m_AllWindows)
        {
            if (pair.Key != EWindowID.UILoading)
            {
                pair.Value.HideAsync();
            }
        }
        m_OpenWindows.Clear();
        m_MutexStacks.Clear();
    }
}