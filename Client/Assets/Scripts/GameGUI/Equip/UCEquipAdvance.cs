using UnityEngine;
using System.Collections;

public class UCEquipAdvance
{
    public GameObject panel;
    public GameObject btnAdvance;
    public UILabel    costMoneyNum;
    public UITexture  costMoneyTexture;
    public UILabel    advanceText;

    public UITexture  nowEquipTexture;
    public UITexture  nexEquipTexture;
    public UISprite   nowEquipQuality;
    public UISprite   nexEquipQuality;
    public UILabel    nowEquipName;
    public UILabel    nexEquipName;
    public UILabel    nowEquipFightValue;
    public UILabel    nexEquipFightValue;
    public UILabel    nowEquipPropertys;
    public UILabel    nexEquipPropertys;

    public GameObject costItemBtn;
    public UITexture  costItemTexture;
    public UISprite   costItemQuality;
    public UILabel    costItemNum;

    public GameObject costEquipBtn;
    public UITexture  costEquipTexture;
    public UISprite   costEquipQuality;
    public UILabel    costEquipNum;

    public void OnStart(bool isShow)
    {
        panel.SetActive(isShow);
    }

    public void OnClose()
    {

    }
}
