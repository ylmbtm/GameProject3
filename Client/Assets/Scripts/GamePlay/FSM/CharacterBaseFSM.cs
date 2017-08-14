using UnityEngine;
using System.Collections;
using System;

public class CharacterBaseFSM : IState<Character, FSMState>
{

    private float m_StatTime;

    public override void Enter()
    {
        m_StatTime = Time.time;
    }

    public override void Execute()
    {
        if (LastTime > 0 && Time.time - m_StatTime > LastTime)
        {
            Stop();
        }
    }

    public override void Exit()
    {
        LastTime = 0;
    }

    public override void Stop()
    {
        LastTime = 0;
        Owner.ChangeEmptyFSM();  
    }
}
