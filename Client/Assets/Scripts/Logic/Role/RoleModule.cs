using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Protocol;

public class RoleModule : GTSingleton<RoleModule>
{
    private List<string> mRandomNames;

    public string     GetRandomName()
    {
        if (mRandomNames == null)
        {
            mRandomNames = new List<string>();
            TextAsset randomNameText = GTResourceManager.Instance.Load<TextAsset>("Text/Name/RandomNames");
            if (randomNameText == null)
            {
                return string.Empty;
            }
            string[] nameArray = randomNameText.text.Split('\n');
            for (int i = 0; i < nameArray.Length; i++)
            {
                if (!mRandomNames.Contains(nameArray[i]))
                {
                    mRandomNames.Add(nameArray[i]);
                }
            }
        }
        int r = UnityEngine.Random.Range(0, mRandomNames.Count);
        return mRandomNames[r];
    }

    public XCharacter GetCurPlayer()
    {
        XCharacter player = null;
        DataDBSRole.Dict.TryGetValue(GTLauncher.CurPlayerID, out player);
        if (player == null)
        {
            player = new XCharacter();
            player.Level = 1;
            player.Id = GTLauncher.Instance.TestActorID;
        }
        return player;
    }

    public XCharacter GetMainPlayer()
    {
        XCharacter c = GetCurPlayer();
        c.Equips.Clear();
        Dictionary<int, XItem>.Enumerator em1 = DataDBSDressEquip.Dict.GetEnumerator();
        while (em1.MoveNext())
        {
            XEquip it = DataManager.Instance.GetEquipDataByPos(EPosType.RoleEquip, em1.Current.Value.Pos);
            if (it != null)
            {
                c.Equips.Add(it);
            }
        }
        em1.Dispose();

        c.Gems.Clear();
        Dictionary<int, XItem>.Enumerator em2 = DataDBSDressGem.Dict.GetEnumerator();
        while (em2.MoveNext())
        {
            XGem it = DataManager.Instance.GetGemDataByPos(EPosType.RoleGem, em1.Current.Value.Pos);
            if (it != null)
            {
                c.Gems.Add(it);
            }
        }
        em2.Dispose();

        c.BaseAttr.Clear();
        c.CurrAttr.Clear();
        c.BaseAttr.AddRange(AttrHelper.GetPropertys(c).Values);
        for (int i = 0; i < c.BaseAttr.Count; i++)
        {
            c.CurrAttr.Add(c.BaseAttr[i]);
        }
        return c;
    }

    public XCharacter GetMainPartner(int id)
    {
        if (id == 0)
        {
            return null;
        }
        XPartner data = DataDBSPartner.GetDataById(id);
        if (data == null)
        {
            return null;
        }
        XCharacter c = new XCharacter();
        DActor db = ReadCfgActor.GetDataById(id);
        c.Id     = id;
        c.Level  = data.Level;
        c.Sex    = 0;
        c.Title  = 0;
        c.Name   = db.Name;
        c.BaseAttr.Clear();
        c.CurrAttr.Clear();
        c.BaseAttr.AddRange(AttrHelper.GetPropertys(data).Values);
        for (int i = 0; i < c.BaseAttr.Count; i++)
        {
            c.CurrAttr.Add(c.BaseAttr[i]);
        }
        return c;
    }

    public XCharacter GetMainPartnerByPos(int pos)
    {
        XCharacter c = GetCurPlayer();
        switch (pos)
        {
            case 1:
                return GetMainPartner(c.Partner1);
            case 2:
                return GetMainPartner(c.Partner2);
            default:
                return null;
        }
    }
}
