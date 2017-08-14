using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class UCEquipStrengthen
{
    public GameObject                  panel;
    public UITexture                   equipTexture;
    public UISprite                    equipQuality;
    public GameObject                  btnAutoToAddMat;
    public GameObject                  btnStrengthen;
    public GameObject                  btnStrengthenTenTimes;
    public UILabel                     equipStrengthLevel;
    public UISlider                    equipStrengthExpBar;
    public UILabel                     equipStrengthExp;
    public UILabel                     equipName;
    public UILabel                     propertyText;
    public UILabel                     strengthText;
    public UITexture                   costMoneyTexture;
    public UILabel                     costMoneyNum;
    public List<UCEquipItemStrengthen> strengthenItems = new List<UCEquipItemStrengthen>();

    public void OnClose()
    {
        strengthenItems.Clear();
    }

    public void OnStart(bool isShow)
    {
        panel.SetActive(isShow);
    }
}
