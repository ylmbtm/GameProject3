using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class UCEquipStar
{
    public GameObject         panel;
    public UILabel            starText;
    public GameObject         btnUpStar;
    public UILabel            costMoneyNum;
    public UITexture          costMoneyTexture;

    public UITexture          nowEquipTexture;
    public UITexture          nexEquipTexture;
    public UISprite           nowEquipQuality;
    public UISprite           nexEquipQuality;
    public UILabel            nowEquipName;
    public UILabel            nexEquipName;
    public UILabel            nowEquipPropertys;
    public UILabel            nexEquipPropertys;

    public List<UISprite>     nowEquipStars = new List<UISprite>();
    public List<UISprite>     nexEquipStars = new List<UISprite>();

    public GameObject         costItemBtn;
    public UITexture          costItemTexture;
    public UISprite           costItemQuality;
    public UILabel            costItemNum;

    public void OnClose()
    {
        nowEquipStars.Clear();
        nexEquipStars.Clear();
    }

    public void OnStart(bool isShow)
    {
        panel.SetActive(isShow);
    }
}
