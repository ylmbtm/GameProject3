using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using Protocol;

public class CharacterManager : GTMonoSingleton<CharacterManager>
{
    public static List<Character>  Characters = new List<Character>();
    public static Character Main { get; set; }
    public static Character Enem { get; set; }
    public static Character Boss { get; set; }

    public Character AddActor(int id, EBattleCamp camp, EActorType type, KTransform bornData, XCharacter data, bool isMain = false)
    {
        Character cc = new Character(id, GTGUID.NewGUID(), type, camp, isMain);
        bornData.Pos = GTTools.NavSamplePosition(bornData.Pos);
        cc.Load(bornData);
        cc.SyncData(data, ESyncDataType.TYPE_ALL);
        cc.CacheTransform.parent = transform;
        Characters.Add(cc);
        return cc;
    }

    public Character AddActorNoneSync(int id, EBattleCamp camp, EActorType type, KTransform bornData)
    {
        return AddActor(id, camp, type, bornData, null, false);
    }

    public Character AddActorSync(XCharacter data)
    {
        if (data == null)
        {
            return null;
        }
        if (data.AOI == null)
        {
            return null;
        }
        KTransform dTrans = KTransform.Create(data.AOI);
        Character cc = AddActor(data.Id, (EBattleCamp)data.Camp, (EActorType)data.Type, dTrans, data, false);
        return cc;
    }

    public Character AddRole(int id, KTransform bornData)
    {
        return AddActorNoneSync(id, EBattleCamp.D, EActorType.PLAYER, bornData);
    }

    public Character AddMonster(int id, KTransform bornData)
    {
        Character cc = AddActorNoneSync(id, EBattleCamp.B, EActorType.MONSTER, bornData);
        cc.ChangeAutoFight(true);
        cc.OnDeadFinish = () =>
        {
            if (cc != null && cc.CacheTransform != null)
            {
                cc.CacheTransform.gameObject.SetActive(false);
            }
        };
        if(cc.IsBoss())
        {
            Boss = cc;
        }
        return cc;
    }

    public Character AddMainPlayer(KTransform bornData)
    {
        XCharacter data = RoleModule.Instance.GetMainPlayer();
        Main = AddActor(data.Id, EBattleCamp.A, EActorType.PLAYER, bornData, data, true);
        return Main;
    }

    public Character AddMainPartner(int pos)
    {
        XCharacter data = RoleModule.Instance.GetMainPartnerByPos(pos);
        if (data == null)
        {
            return null;
        }
        Vector3 offset = Vector3.zero;
        switch(pos)
        {
            case 1:
                offset = new Vector3(-2, 0, 0);
                break;
            case 2:
                offset = new Vector3( 2, 0, 0);
                break;                           
        }
        KTransform bornData = new KTransform();
        bornData.Euler = Main.Euler;
        bornData.Pos = Main.Pos + offset;
        Character actor = AddActor(data.Id, EBattleCamp.A, EActorType.PARTNER, bornData, data);
        actor.Host = Main;
        actor.FollowOffset = offset;
        actor.ChangeAutoFight(true);
        switch(pos)
        {
            case 1:
                Main.Partner1 = actor;
                break;
            case 2:
                Main.Partner2 = actor;
                break;
        }
        return actor;
    }

    public Character DelActor(Character cc)
    {
        if (cc == null)
        {
            return null;
        }
        Characters.Remove(cc);
        cc.Release();
        if (cc == Main)
        {
            Main = null;
        }
        if (cc == Boss)
        {
            Boss = null;
        }
        return cc;
    }

    public Character GetActor(int guid)
    {
        for (int i = 0; i < Characters.Count; i++)
        {
            if(Characters[i].GUID==guid)
            {
                return Characters[i];
            }
        }
        return null;
    }

    public CharacterAvatar AddAvatar(int modelID)
    {
        DActorModel cfg = ReadCfgActorModel.GetDataById(modelID);
        if (cfg == null)
        {
            return null;
        }
        GameObject obj = GTResourceManager.Instance.Load<GameObject>(cfg.Model, true);
        if (obj == null)
        {
            return null;
        }
        CharacterAvatar avatar = new CharacterAvatar(obj.transform);
        CharacterController cc = obj.GetComponent<CharacterController>();
        if (cc != null)
        {
            cc.enabled = false;
        }
        return avatar;
    }

    public CharacterAvatar DelAvatar(CharacterAvatar avatar)
    {
        if (avatar == null)
        {
            return null;
        }
        avatar.Release();
        GTResourceManager.Instance.DestroyObj(avatar.GetRootObj());
        return null;
    }

    public void SyncMainPlayerData(ESyncDataType type)
    {
        if (Main != null)
        {
            XCharacter data = RoleModule.Instance.GetMainPlayer();
            Main.SyncData(data, type);
        }
    }

    public void SetCharacterParent(Character cc)
    {
        cc.CacheTransform.parent = transform;
    }

    public void DelCharacters()
    {
        for (int i = 0; i < Characters.Count; i++)
        {
            Character cc = Characters[i];
            cc.Release();
        }
        Characters.Clear();
        Main = null;
        Enem = null;
    }
}
