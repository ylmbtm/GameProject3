using UnityEngine;
using System.Collections;
using ProtoBuf;
using Protocol;

public class GTDataFactory
{
    public static XCharacter CreatePlayer(int id, string name)
    {
        XCharacter tab      = new XCharacter();
        tab.Name            = name;
        tab.Id              = id;
        tab.Level           = 1;
        tab.GUID            = GTGUID.NewGUID();
        return tab;
    }

    public static XEquip     CreateEquip(int id, int instance)
    {
        XEquip tab          = new XEquip();
        tab.Id              = id;
        tab.Instance        = instance;
        tab.StrengthenLevel = 0;
        tab.StarLevel       = 0;
        tab.StrengthenExp   = 0;
        tab.AdvanceLevel    = 0;
        return tab;
    }

    public static XGem       CreateGem(int id, int instance)
    {
        XGem tab            = new XGem();
        tab.Id              = id;
        tab.Instance        = instance;
        tab.StrengthenLevel = 0;
        tab.StrengthenExp   = 0;
        return tab;
    }

    public static XPartner   CreatePartner(int id)
    {
        XPartner tab        = new XPartner();
        tab.Id              = id;
        tab.Level           = 0;
        tab.Star            = 0;
        tab.Wake            = 0;
        tab.Exp             = 0;
        tab.Advance         = 0;
        return tab;
    }

    public static XPet       CreatePet(int id)
    {
        XPet tab            = new XPet();
        tab.Id              = id;
        tab.Level           = 1;
        tab.CurExp          = 0;
        return tab;
    }

    public static XRune      CreateRune(int id, int instance)
    {
        XRune tab           = new Protocol.XRune();
        tab.Id              = id;
        tab.Instance        = instance;
        tab.Level           = 0;
        return tab;
    }
}
