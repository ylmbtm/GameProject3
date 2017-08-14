using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;

public class CharacterNetwork : ICharacterComponent
{
    private Character       m_Character;
    private float           m_SyncTimer;
    private Vector3         m_SyncMove;
    private List<Vector3>   m_SyncMoveList = new List<Vector3>();
    private float           m_LerpRate;
    private float           m_SyncMinDistance = 0.11f;
    private float           m_NormalLerpRate = 16f;
    private float           m_FasterLerpRate = 27f;


    public CharacterNetwork(Character cc)
    {
        m_Character = cc;
        m_SyncTimer = 0;
    }

    public void Execute()
    {
        if (m_Character.IsMain)
        {
            if (Time.realtimeSinceStartup - m_SyncTimer > 0.2f)
            {
                GTNetworkSend.Instance.TrySyncMove();
                m_SyncTimer = Time.realtimeSinceStartup;
            }
            if (m_Character.FSMPrev != FSMState.FSM_IDLE && m_Character.FSM == FSMState.FSM_IDLE)
            {
                GTNetworkSend.Instance.TrySyncIdle();
            }
            return;
        }

        if (m_Character.IsRemoteActor)
        {
            if (m_SyncMoveList.Count > 0)
            {
                m_Character.Pos = Vector3.Lerp(m_Character.Pos, m_SyncMoveList[0], Time.deltaTime * m_LerpRate);
                if (Vector3.Distance(m_Character.Pos, m_SyncMoveList[0]) < m_SyncMinDistance)
                {
                    m_SyncMoveList.RemoveAt(0);
                }
                if (m_SyncMoveList.Count > 10)
                {
                    m_LerpRate = m_FasterLerpRate;
                }
                else
                {
                    m_LerpRate = m_NormalLerpRate;
                }
            }
            return;
        }
    }

    public void Release()
    {

    }

    public void SyncPosition(Vector3 target, Vector3 forward)
    {
        this.m_SyncMove      = target;
        this.m_SyncMoveList.Add(target);
        this.m_Character.Dir = forward;
    }

    public void SyncIdle(Vector3 target, Vector3 forward)
    {
        this.m_Character.Pos = target;
        this.m_Character.Dir = forward;
    }
}
