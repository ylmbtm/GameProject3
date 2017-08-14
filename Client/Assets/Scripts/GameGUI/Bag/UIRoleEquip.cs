using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using Protocol;

public class UIRoleEquip : GTWindow
{
    private GameObject                      btnFetter;
    private GameObject                      btnStrong;
    private UIGrid                          propertyGrid;
    private GameObject                      propertyTemplate;
    private UITexture                       modelTexture;
    private List<ItemEquip>                 mEquipCells    = new List<ItemEquip>();
    private Dictionary<EAttr, ItemProperty> mPropertyItems = new Dictionary<EAttr, ItemProperty>();
    private ERender                         mRender;
    private CharacterAvatar                 mAvatar;

    class ItemEquip
    {
        public UITexture  itemTexture;
        public UISprite   itemQuality;
        public GameObject itemBtn;

        public void Show(bool hasDress)
        {
            itemTexture.gameObject.SetActive(hasDress);
            itemQuality.gameObject.SetActive(hasDress);
        }
    }

    class ItemProperty
    {
        public UILabel propertyName;
        public UILabel propertyNum;
    }

    public UIRoleEquip()
    {
        MaskType = EWindowMaskType.None;
        Type = EWindowType.Window;
        ShowMode = EWindowShowMode.SaveTarget;
        Path = "Bag/UIRoleEquip";
        Resident = false;
    }

    protected override void OnAwake()
    {
        Transform pivot = transform.Find("Pivot");
        btnFetter = pivot.Find("Buttons/Btn_Fetter").gameObject;
        btnStrong = pivot.Find("Buttons/Btn_Strong").gameObject;
        propertyGrid = pivot.Find("Propertys/Grid").GetComponent<UIGrid>();
        propertyTemplate = pivot.Find("Propertys/Template").gameObject;
        modelTexture = pivot.Find("ModelTexture").GetComponent<UITexture>();
        for (int i = 1; i <= 8; i++)
        {
            ItemEquip item = new ItemEquip();
            item.itemBtn = pivot.Find("Equips/" + i).gameObject;
            item.itemBtn.name = i.ToString();
            item.itemTexture = item.itemBtn.transform.Find("Texture").GetComponent<UITexture>();
            item.itemQuality = item.itemBtn.transform.Find("Quality").GetComponent<UISprite>();
            mEquipCells.Add(item);
        }
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnFetter).onClick = OnFetterClick;
        UIEventListener.Get(btnStrong).onClick = OnStrongClick;
        for (int i = 0; i < mEquipCells.Count; i++)
        {
            int index = i;
            UIEventListener.Get(mEquipCells[index].itemBtn).onClick = OnEquipCellClick;
        }
        UIEventListener.Get(modelTexture.gameObject).onDrag = OnHeroTextureDrag;
    }

    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler<int, int>(GTEventID.TYPE_BAG_DRESS_EQUIP,  OnRecvDressEquip);
        GTEventCenter.AddHandler<int, int>(GTEventID.TYPE_BAG_UNLOAD_EQUIP, OnRecvUnloadEquip);
        GTEventCenter.AddHandler(GTEventID.TYPE_CHANGE_FIGHTVALUE,      OnRecvChangeFightValue);
    }

    protected override void OnEnable()
    {
        ShowModelView();
        ShowEquipsView();
        ShowPropertys();
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler<int, int>(GTEventID.TYPE_BAG_DRESS_EQUIP,  OnRecvDressEquip);
        GTEventCenter.DelHandler<int, int>(GTEventID.TYPE_BAG_UNLOAD_EQUIP, OnRecvUnloadEquip);
        GTEventCenter.DelHandler(GTEventID.TYPE_CHANGE_FIGHTVALUE,      OnRecvChangeFightValue);
    }

    protected override void OnClose()
    {
        mEquipCells.Clear();
        mPropertyItems.Clear();
        if (mAvatar != null)
        {
            mAvatar = null;
        }
        if (mRender != null)
        {
            mRender.Release();
            mRender = null;
        }
    }

    private void OnHeroTextureDrag(GameObject go, Vector2 delta)
    {
        if (mAvatar == null && mAvatar.GetRootObj() != null)
            return;
        ESpin.Get(mAvatar.GetRootObj()).OnSpin(delta, 2);
    }

    private void OnEquipCellClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        int pos = go.name.ToInt32();
        GTItemHelper.ShowItemDialogByPosType(EPosType.RoleEquip, pos);
    }

    private void OnStrongClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnFetterClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void ShowModelView()
    {
        XCharacter role = GTGlobal.Main;
        DActor db = ReadCfgActor.GetDataById(role.Id);
        mRender = ERender.AddRender(modelTexture);
        mAvatar = GTWorld.Instance.AddAvatar(db.Model);
        if (mAvatar == null)
        {
            return;
        }
        for (int i = 1; i <= 8; i++)
        {
            XEquip dressEquip = GTDataManager.Instance.GetEquipDataByPos(EPosType.RoleEquip, i);
            int id = dressEquip == null ? 0 : dressEquip.Id;
            mAvatar.ChangeAvatar(i, id);
        }
        mAvatar.PlayAnim("idle", null);
        GameObject model = mRender.AddModel(mAvatar.GetRootObj());
        model.transform.localPosition = new Vector3(0, -0.8f, 2.7f);
        model.transform.localEulerAngles = new Vector3(0, 180, 0);
    }

    private void ShowPropertys()
    {
        propertyTemplate.SetActive(false);
        XCharacter role = GTGlobal.Main;
        Dictionary<EAttr, int> propertys = GTAttrHelper.GetPropertys(role);
        Dictionary<EAttr, int>.Enumerator em = propertys.GetEnumerator();
        while(em.MoveNext())
        {
            EAttr e = em.Current.Key;
            if ((int)e > 10)
            {
                continue;
            }
            ItemProperty item;
            if (!mPropertyItems.ContainsKey(e))
            {
                item = new ItemProperty();
                GameObject go = NGUITools.AddChild(propertyGrid.gameObject, propertyTemplate);
                item.propertyNum = go.transform.Find("Num").GetComponent<UILabel>();
                item.propertyName = go.transform.Find("Name").GetComponent<UILabel>();
                go.SetActive(true);
                mPropertyItems.Add(e, item);
            }
            else
            {
                item = mPropertyItems[e];
            }           
            DProperty db = ReadCfgProperty.GetDataById(em.Current.Key);
            item.propertyName.text = db.Name;
            item.propertyNum.text = db.IsPercent==false ? em.Current.Value.ToString() : (em.Current.Value / 100f).ToPercent();
        }
    }

    private void ShowCellView(int pos)
    {
        Dictionary<int, XItem> dressEquips = DataDBSDressEquip.Dict;
        ItemEquip cell = mEquipCells[pos - 1];
        if(!dressEquips.ContainsKey(pos))
        {
            cell.Show(false);
            return;
        }
        cell.Show(true);
        XItem item = dressEquips[pos];
        GTItemHelper.ShowItemTexture(cell.itemTexture, item.Id);
        GTItemHelper.ShowItemQuality(cell.itemQuality, item.Id);
    }

    private void ShowEquipsView()
    {
        for (int i = 0; i < mEquipCells.Count; i++)
        {
            ShowCellView(i + 1);
        }
    }

    private void OnRecvUnloadEquip(int newPos, int tarPos)
    {
        ShowCellView(tarPos);
        if (mAvatar == null)
        {
            return;
        }
        XEquip dressEquip = GTDataManager.Instance.GetEquipDataByPos(EPosType.RoleEquip, tarPos);
        mAvatar.ChangeAvatar(tarPos, 0);
    }

    private void OnRecvDressEquip(int srcPos, int tarPos)
    {
        ShowCellView(tarPos);
        if (mAvatar == null)
        {
            return;
        }
        XEquip dressEquip = GTDataManager.Instance.GetEquipDataByPos(EPosType.RoleEquip, tarPos);
        mAvatar.ChangeAvatar(tarPos, dressEquip == null ? 0 : dressEquip.Id);
    }

    private void OnRecvChangeFightValue()
    {
        ShowPropertys();
    }
}
