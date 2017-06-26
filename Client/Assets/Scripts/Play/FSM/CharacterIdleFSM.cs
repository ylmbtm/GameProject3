using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class CharacterIdleFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        if(Cmd==null)
        {
            Owner.DoIdle(null);
        }
        else
        {
            Owner.DoIdle(Cmd as CommandIdle);
        }
    }
}