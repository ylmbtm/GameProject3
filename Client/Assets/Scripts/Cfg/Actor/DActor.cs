using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using System.Xml;

public class DActor : DObj<int>
{
    public int               Id;
    public int               Level;
    public int               Title;
    public string            Name;
    public string            Desc;
    public string            Icon;
    public EActorRace        Race;
    public EActorSex         Sex;
    public EActorType        Type;
    public EActorSort        Sort;
    public EActorUnit        Unit;
    public ECarrer           Carrer;
    public int               Group;
    public int               Quality;
    public int               Exp;
    public int               Gold;
    public float             WSpeed;
    public float             RSpeed;
    public int               BornEffectID;
    public int               DeadEffectID;
    public int               Model;
    public Vector3           StagePos;
    public float             StageScale;
    public string            Skill;
    public Dictionary<EAttr, int>         Propertys = new Dictionary<EAttr, int>();
    public Dictionary<EActorNature, bool> Natures   = new Dictionary<EActorNature, bool>();

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id           = element.GetInt("Id");
        this.Level        = element.GetInt("Level");
        this.Title        = element.GetInt("Title");
        this.Name         = element.GetString("Name");
        this.Desc         = element.GetString("Desc");
        this.Icon         = element.GetString("Icon");

        this.Race         = (EActorRace)element.GetInt("Race");
        this.Type         = (EActorType)element.GetInt("Type");
        this.Sex          = (EActorSex)element.GetInt("Sex");
        this.Sort         = (EActorSort)element.GetInt("Sort");
        this.Unit         = (EActorUnit)element.GetInt("Unit");
        this.Carrer       = (ECarrer)element.GetInt("Carrer");
        this.Group        = element.GetInt("Group");
        this.Quality      = element.GetInt("Quality");

        this.RSpeed       = element.GetFloat("RSpeed");
        this.WSpeed       = element.GetFloat("WSpeed");
        this.BornEffectID = element.GetInt("BornEffectID");
        this.DeadEffectID = element.GetInt("DeadEffectID");

        this.StagePos     = element.GetString("StagePos").ToVector3(true);
        this.StageScale   = element.GetFloat("StageScale");
        this.Skill        = element.GetString("Skill");
        this.Model        = element.GetInt("Model");
        this.Exp          = element.GetInt("Exp");
        this.Gold         = element.GetInt("Gold");

        for (int i = 1; i <= 16; i++)
        {
            int v = element.GetInt("P" + i.ToString());
            this.Propertys.Add((EAttr)i, v);
        }

        this.Natures[EActorNature.CAN_MOVE]         = element.GetInt("CanMove") == 1;
        this.Natures[EActorNature.CAN_KILL]         = element.GetInt("CanKill") == 1;
        this.Natures[EActorNature.CAN_MANUALATTACK] = element.GetInt("CanManualAttack") == 1;
        this.Natures[EActorNature.CAN_TURN]         = element.GetInt("CanTurn") == 1;
        this.Natures[EActorNature.CAN_STUN]         = element.GetInt("CanStun") == 1;
        this.Natures[EActorNature.CAN_BEATBACK]     = element.GetInt("CanBeatBack") == 1;
        this.Natures[EActorNature.CAN_BEATFLY]      = element.GetInt("CanBeatFly") == 1;
        this.Natures[EActorNature.CAN_BEATDOWN]     = element.GetInt("CanBeatDown") == 1;
        this.Natures[EActorNature.CAN_WOUND]        = element.GetInt("CanWound") == 1;
        this.Natures[EActorNature.CAN_REDUCESPEED]  = element.GetInt("CanReduceSpeed") == 1;
        this.Natures[EActorNature.CAN_FIXBODY]      = element.GetInt("CanFixBody") == 1;
        this.Natures[EActorNature.CAN_SLEEP]        = element.GetInt("CanSleep") == 1;
        this.Natures[EActorNature.CAN_VARISTION]    = element.GetInt("CanVaristion") == 1;
        this.Natures[EActorNature.CAN_PARALY]       = element.GetInt("CanParaly") == 1;
        this.Natures[EActorNature.CAN_FEAR]         = element.GetInt("CanFear") == 1;
    }
}


public class ReadCfgActor : DReadBase<int, DActor>
{
  
}