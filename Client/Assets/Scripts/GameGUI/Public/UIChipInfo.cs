using UnityEngine;
using System.Collections;
using System;
using Protocol;

public class UIChipInfo : GTWindow
{
    private GameObject btnCheckSource;
    private GameObject btnCompose;
    private GameObject btnClose;

    private UILabel    sourceItemName;
    private UILabel    sourceItemNum;
    private UITexture  sourceItemTexture;
    private UISprite   sourceItemQuality;

    private UILabel    targetItemName;
    private UILabel    targetItemNum;
    private UITexture  targetItemTexture;
    private UISprite   targetItemQuality;

    private UILabel    itemDesc;
    private int        pos;
    private Transform  pivot;


    public UIChipInfo()
    {
        Resident = false;
        Path = "Public/UIChipInfo";
        Type = EWindowType.Window;
        MaskType = EWindowMaskType.BlackTransparent;
    }

    protected override void OnAwake()
    {
        pivot = transform.Find("Pivot");
        btnClose = pivot.Find("BtnClose").gameObject;
        btnCheckSource = pivot.Find("BtnCheckSource").gameObject;
        btnCompose = pivot.Find("BtnCompose").gameObject;
        itemDesc = pivot.Find("ItemDesc").GetComponent<UILabel>();

        sourceItemName = pivot.Find("SourceItem/Name").GetComponent<UILabel>();
        sourceItemNum = pivot.Find("SourceItem/Num").GetComponent<UILabel>();
        sourceItemTexture = pivot.Find("SourceItem/Texture").GetComponent<UITexture>();
        sourceItemQuality = pivot.Find("SourceItem/Quality").GetComponent<UISprite>();

        targetItemName = pivot.Find("TargetItem/Name").GetComponent<UILabel>();
        targetItemNum = pivot.Find("TargetItem/Num").GetComponent<UILabel>();
        targetItemTexture = pivot.Find("TargetItem/Texture").GetComponent<UITexture>();
        targetItemQuality = pivot.Find("TargetItem/Quality").GetComponent<UISprite>();
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick = OnCloseClick;
        UIEventListener.Get(btnCheckSource).onClick = OnCheckSourceClick;
        UIEventListener.Get(btnCompose).onClick = OnComposeClick;
    }

    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler<int>(GTEventID.TYPE_BAG_COMPOSE_CHIP, OnRecvComposeChip);
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler<int>(GTEventID.TYPE_BAG_COMPOSE_CHIP, OnRecvComposeChip);
    }

    protected override void OnEnable()
    {

    }

    protected override void OnClose()
    {

    }

    private void OnComposeClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        XItem item = DataDBSBagChip.Dict[pos];
        int itemID = item.Id;
        DItem sourceDB = ReadCfgItem.GetDataById(itemID);
        if(item.Num < sourceDB.Data1)
        {
            GTItemHelper.ShowTip("数量不足，无法合成");
            return;
        }
        GTNetworkSend.Instance.TryComposeChip(pos);
    }

    private void OnCheckSourceClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnCloseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
        Hide();
    }

    private void OnRecvComposeChip(int arg1)
    {
        Hide();
    }


    public void ShowViewByPos(int pos)
    {
        this.pos = pos;
        XItem item = DataDBSBagChip.Dict[pos];
        int itemID = item.Id;
        DItem sourceDB = ReadCfgItem.GetDataById(itemID);
        DItem targetDB = ReadCfgItem.GetDataById(sourceDB.Data2);

        itemDesc.text = sourceDB.Desc;

        sourceItemNum.text = GTTools.Format("拥有数量：{0}", item.Num);
        GTItemHelper.ShowItemTexture(sourceItemTexture, itemID);
        GTItemHelper.ShowItemName(sourceItemName, itemID);
        GTItemHelper.ShowItemQuality(sourceItemQuality, itemID);

        targetItemNum.text = GTTools.Format("合成数量：{0}", item.Num / sourceDB.Data1);
        GTItemHelper.ShowItemTexture(targetItemTexture, sourceDB.Data2);
        GTItemHelper.ShowItemName   (targetItemName, sourceDB.Data2);
        GTItemHelper.ShowItemQuality(targetItemQuality, sourceDB.Data2);
    }
}
