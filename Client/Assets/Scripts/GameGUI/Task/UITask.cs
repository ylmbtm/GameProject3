using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using CFG;

public class UITask : GTWindow
{
    private GameObject                             btnClose;
    private GameObject                             btnReceive;
    private GameObject                             btnGoto;
    private Dictionary<ETaskType, UIToggle>        mMenus       = new Dictionary<ETaskType, UIToggle>();
    private ETaskType                              mCurTaskType = ETaskType.THREAD;
    private UIScrollView                           mTaskListView;
    private Dictionary<ETaskType, UIGrid>          mGrids       = new Dictionary<ETaskType, UIGrid>();
    private Dictionary<ETaskType, List<ItemTask>>  mTasks       = new Dictionary<ETaskType, List<ItemTask>>();
    private List<ItemReward>                       mTaskRewards = new List<ItemReward>();
    private UILabel                                mTaskDesc;
    private UILabel                                mTaskTargetTitle;
    private UILabel                                mTaskPlan;
    private int                                    mCurrTaskIndex = 0;
    private GameObject                             mTemp;

    class ItemReward
    {
        public GameObject itemBtn;
        public UITexture  itemTexture;
        public UISprite   itemQuality;
        public UILabel    itemNum;
        public GameObject itemChip;
        public int        itemID = 0;
    }

    class ItemTask
    {
        public GameObject go;
        public UISprite   taskIcon;
        public UILabel    taskName;
        public int        taskID;

        public void Show()
        {
            DTask db = ReadCfgTask.GetDataById(taskID);
            if (db == null)
            {
                return;
            }
            taskName.text = db.Name;
            taskIcon.spriteName = db.Icon;
        }
    }

    public UITask()
    {
        Path = "Task/UITask";
        Type = EWindowType.Window;
        MaskType = EWindowMaskType.Black;
        Resident = false;
        ShowMode = EWindowShowMode.HideOther;
    }

    protected override void OnAwake()
    {
        Transform pivot = transform.Find("Pivot");
        btnClose = pivot.Find("BtnClose").gameObject;
        btnReceive = pivot.Find("Content/Pivot/Btn_Receive").gameObject;
        btnGoto = pivot.Find("Content/Pivot/Btn_Goto").gameObject;
        mTaskListView = pivot.Find("List/View").GetComponent<UIScrollView>();
        for (int i = 1; i <= 3; i++)
        {
            mGrids[(ETaskType)i] = pivot.Find("List/View/Grid" + i).GetComponent<UIGrid>();
        }
        mTemp = pivot.Find("List/Temp").gameObject;
        mTemp.GetComponent<UIToggle>().group = GTWindowManager.Instance.GetToggleGroupId();
        mTemp.SetActive(false);
        mTaskTargetTitle = pivot.Find("Content/Pivot/TaskTargetTitle").GetComponent<UILabel>();
        mTaskDesc = pivot.Find("Content/Pivot/TaskDesc").GetComponent<UILabel>();
        mTaskPlan = pivot.Find("Content/Pivot/TaskPlan").GetComponent<UILabel>();
        for (int i = 1; i <= 6; i++)
        {
            Transform trans = pivot.Find("Content/Pivot/Rewards/" + i);
            ItemReward tab = new ItemReward();
            tab.itemBtn = trans.gameObject;
            tab.itemTexture = trans.Find("Texture").GetComponent<UITexture>();
            tab.itemQuality = trans.Find("Quality").GetComponent<UISprite>();
            tab.itemNum = trans.Find("Num").GetComponent<UILabel>();
            tab.itemChip = trans.Find("Chip").gameObject;
            mTaskRewards.Add(tab);
        }

        int group = GTWindowManager.Instance.GetToggleGroupId();
        for (int i = 1; i <= 3; i++)
        {
            UIToggle toggle = pivot.Find("Left/Menus/Menu_" + i).GetComponent<UIToggle>();
            toggle.group = group;
            mMenus.Add((ETaskType)i, toggle);
        }
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick = OnCloseClick;
        UIEventListener.Get(btnGoto).onClick = OnGotoClick;
        UIEventListener.Get(btnReceive).onClick = OnReceiveClick;
        foreach (KeyValuePair<ETaskType,UIToggle> pair in mMenus)
        {
            ETaskType type = pair.Key;
            GameObject btn = pair.Value.gameObject;
            UIEventListener.Get(btn).onClick = delegate (GameObject go)
            {
                GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
                if (type == mCurTaskType)
                {
                    return;
                }
                Switch(type);
            };
        }
    }

    protected override void OnEnable()
    {
        CreateTaskItemList(ETaskType.BRANCH);
        CreateTaskItemList(ETaskType.THREAD);
        CreateTaskItemList(ETaskType.DAILY);
        Switch(ETaskType.THREAD);
    }

    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler<int>(GTEventID.TYPE_TASK_SUBMIT_TASK, OnRecvSubmitTask);
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler<int>(GTEventID.TYPE_TASK_SUBMIT_TASK, OnRecvSubmitTask);
    }

    protected override void OnClose()
    {
        mMenus.Clear();
        mGrids.Clear();
        mTaskRewards.Clear();
        mTasks.Clear();
    }

    private void OnReceiveClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnGotoClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        Hide();
    }

    private void OnCloseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
        Hide();
    }

    private void ShowTaskReward(List<GTItem> list)
    {
        for (int i = 0; i < mTaskRewards.Count; i++)
        {
            ItemReward tab = mTaskRewards[i];
            if (list != null && i < list.Count)
            {
                tab.itemBtn.gameObject.SetActive(true);
                GTItem itemData = list[i];
                GTItemHelper.ShowItemTexture(tab.itemTexture, itemData.Id);
                GTItemHelper.ShowItemQuality(tab.itemQuality, itemData.Id);
                GTItemHelper.ShowItemNum(tab.itemNum, itemData.Num);
                GTItemHelper.ShowItemChip(tab.itemChip, itemData.Id);
            }
            else
            {
                tab.itemBtn.gameObject.SetActive(false);
            }
        }
    }

    private void ShowTaskState(ETaskState state)
    {
        switch (state)
        {
            case ETaskState.QUEST_DOING:
                btnGoto.SetActive(true);
                btnReceive.SetActive(false);
                break;
            case ETaskState.QUEST_CANSUBMIT:
                btnGoto.SetActive(false);
                btnReceive.SetActive(true);
                GTItemHelper.ShowImageBlack(btnReceive.GetComponent<UISprite>(), false);
                break;
            case ETaskState.QUEST_HASSUBMIT:
                btnGoto.SetActive(false);
                btnReceive.SetActive(true);
                GTItemHelper.ShowImageBlack(btnReceive.GetComponent<UISprite>(), true);
                break;
            default:
                btnGoto.SetActive(false);
                btnReceive.SetActive(false);
                break;
        }
    }

    private void ShowTaskViewById(ETaskType type,int taskID)
    {

       
    }

    private void OnRecvSubmitTask(int pTaskID)
    {
        DTask db = ReadCfgTask.GetDataById(pTaskID);
        List<ItemTask> pList = mTasks[db.Type];
        UIGrid grid = mGrids[db.Type];
        for (int i = 0; i < pList.Count; i++)
        {
            ItemTask tab = pList[i];
            if (tab.taskID == pTaskID)
            {
                grid.RemoveChild(tab.go.transform);
                pList.Remove(tab);
                GameObject.Destroy(tab.go);
                break;
            }
        }
        ResetView();
    }

    private ItemTask CreateItemTask(UIGrid grid,int taskID)
    {
        ItemTask tab = new ItemTask();
        GameObject item = NGUITools.AddChild(grid.gameObject, mTemp);
        item.SetActive(true);
        grid.AddChild(item.transform);
        tab.go = item;
        tab.taskID = taskID;
        tab.taskName = item.transform.Find("Name").GetComponent<UILabel>();
        tab.taskIcon = item.transform.Find("Icon").GetComponent<UISprite>();
        return tab;
    }

    private void CreateTaskItemList(ETaskType type)
    {

    }

    private void Switch(ETaskType type)
    {
        mMenus[type].value = true;
        mTaskListView.ResetPosition();
        mCurTaskType = type;
        mCurrTaskIndex = 0;
        ResetView();
    }

    private void ResetView()
    {
        UIGrid pGrid = mGrids[mCurTaskType];
        if (!mTasks.ContainsKey(mCurTaskType))
        {
            transform.Find("Pivot/Content/Pivot").gameObject.SetActive(false);
            return;
        }
        List<ItemTask> pList = mTasks[mCurTaskType];
        if (pList.Count > 0)
        {
            transform.Find("Pivot/Content/Pivot").gameObject.SetActive(true);
            pList[0].go.GetComponent<UIToggle>().value = true;
            ShowTaskViewById(mCurTaskType, pList[0].taskID);
        }
        else
        {
            transform.Find("Pivot/Content/Pivot").gameObject.SetActive(false);
        }
    }
}
