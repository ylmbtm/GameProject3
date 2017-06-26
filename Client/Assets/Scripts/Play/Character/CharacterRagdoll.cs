using UnityEngine;
using System.Collections;
using System;

public class CharacterRagdoll : ICharacterComponent
{
    private Transform   m_root;
    private Rigidbody[] m_rigidbodys;
    private Transform   m_bindTrans;
    private Vector3     m_force;
    private float       m_fStartTime;
    private string      m_sForceBind;

    public CharacterRagdoll(Transform  root)
    {
        this.m_root = root;
        this.m_rigidbodys = root.GetComponentsInChildren<Rigidbody>();
        this.EnableRagdoll(false);
    }

    public bool EnableRagdoll(bool enable)
    {
        if (m_rigidbodys.Length == 0)
        {
            return false;
        }
        for (int i = 0; i < m_rigidbodys.Length; i++)
        {
            Rigidbody rigid = m_rigidbodys[i];
            if(rigid.transform == m_root)
            {
                continue;
            }
            string name = rigid.transform.name;
            if (name.Equals("TriggerObject") || name.Equals("TriggerRange"))
            {
                continue;
            }
            rigid.isKinematic = !enable;
            rigid.detectCollisions = enable;
        }
        CharacterController cc = m_root.GetComponent<CharacterController>();
        if (cc == null)
        {
            cc.enabled = !enable;
        }
        Animator animator = m_root.GetComponentInChildren<Animator>();
        if(animator!=null)
        {
            animator.enabled = !enable;
        }
        return true;
    }

    public bool AddForce(string bindName, Vector3 force)
    {
        Transform bindTrans = GTTools.GetBone(m_root, bindName);
        if (bindTrans == null)
        {
            return false;
        }
        bool success = this.EnableRagdoll(true);
        if(success)
        {
            m_fStartTime = Time.time;
            m_force = force;
            m_bindTrans = bindTrans;
        }
        return success;
    }

    public void Execute()
    {
        if (m_fStartTime == 0)
        {
            return;
        }
        if (m_bindTrans == null)
        {
            return;
        }
        if (Time.time - m_fStartTime < 0.1f)
        {
            for (int i = 0; i < m_rigidbodys.Length; i++)
            {
                Rigidbody rigid = m_rigidbodys[i];
                if(rigid.name == m_root.name)
                {
                    continue;
                }
                rigid.AddForce(m_force, ForceMode.VelocityChange);
            }
        }
    }

    public void Release()
    {
        m_fStartTime = 0;
    }
}