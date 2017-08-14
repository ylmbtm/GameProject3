using UnityEngine;
using System.Collections;

public class UCEquipItemStrengthen
{
    public GameObject itemBtn;
    public UITexture  itemTexture;
    public UISprite   itemQuality;
    public GameObject itemPlus;
    public UILabel    itemNum;

    public void Show(bool isFilled)
    {
        itemTexture.gameObject.SetActive(isFilled);
        itemQuality.gameObject.SetActive(isFilled);
        itemNum.    gameObject.SetActive(isFilled);
        itemPlus.   gameObject.SetActive(isFilled == false);
    }
}