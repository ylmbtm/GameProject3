using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;

public class UIAwardTip : GTWindow
{
    private GameObject btnMask;
    private UIGrid     itemGrid;
    private GameObject itemTemplate;

    public UIAwardTip()
    {
        Resident = false;
        Path = "Common/UIAwardTip";
        Type = EWindowType.Window;
        MaskType = EWindowMaskType.BlackTransparent;
        ShowMode = EWindowShowMode.DoNothing;
    }

    protected override void OnAwake()
    {
        itemGrid = transform.Find("Pivot/View/Grid").GetComponent<UIGrid>();
        itemTemplate = transform.Find("Pivot/View/Item").gameObject;
        itemTemplate.SetActive(false);
    }

    protected override void OnAddButtonListener()
    {
        this.OnMaskClick = () =>
        {
            Hide();
            GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
        };
    }

    protected override void OnAddHandler()
    {
    
    }

    protected override void OnEnable()
    {

    }

    protected override void OnDelHandler()
    {
        
    }

    protected override void OnClose()
    {
      
    }

    public void ShowView(List<GTItem> list)
    {
        for (int i = 0; i < list.Count; i++)
        {
            int itemId = list[i].Id;
            int num = list[i].Num;
            GameObject it = NGUITools.AddChild(itemGrid.gameObject, itemTemplate);
            it.gameObject.SetActive(true);
            itemGrid.AddChild(it.transform);
            it.name = itemId.ToString();
            UITexture itemTexture = it.transform.Find("Texture").GetComponent<UITexture>();
            UISprite itemQuality = it.transform.Find("Quality").GetComponent<UISprite>();
            UILabel itemName = it.transform.Find("Name").GetComponent<UILabel>();
            UILabel itemNum = it.transform.Find("Num").GetComponent<UILabel>();
            GTItemHelper.ShowItemTexture(itemTexture, itemId);
            GTItemHelper.ShowItemQuality(itemQuality, itemId);
            GTItemHelper.ShowItemName(itemName, itemId);
            itemNum.text = GTTools.Format("x{0}", num);
        }
    }
}
