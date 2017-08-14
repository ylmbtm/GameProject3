using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using Protocol;

public class GTInputManager : GTMonoSingleton<GTInputManager>
{
    public override void SetRoot(Transform parent)
    {
        base.SetRoot(parent);
    }

    float mX;
    float mY;

    void Update()
    {
        OnEscape();
        OnKeyDown();
        OnUpdate();
    }

    void OnEscape()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            Application.Quit();
        }
    }

    void OnKeyDown()
    {
        if (Input.GetKeyDown(KeyCode.Alpha0))
        {
            GTNetworkSend.Instance.TryCastSkill(ESkillPos.Skill_0);       
        }
        if (Input.GetKeyDown(KeyCode.Alpha1))
        {
            GTNetworkSend.Instance.TryCastSkill(ESkillPos.Skill_1);
        }
        if (Input.GetKeyDown(KeyCode.Alpha2))
        {
            GTNetworkSend.Instance.TryCastSkill(ESkillPos.Skill_2);
        }
        if (Input.GetKeyDown(KeyCode.Alpha3))
        {
            GTNetworkSend.Instance.TryCastSkill(ESkillPos.Skill_3);
        }
        if (Input.GetKeyDown(KeyCode.Alpha4))
        {
            GTNetworkSend.Instance.TryCastSkill(ESkillPos.Skill_4);
        }
        if (Input.GetKeyDown(KeyCode.Alpha5))
        {
            GTNetworkSend.Instance.TryCastSkill(ESkillPos.Skill_5);
        }
        if (Input.GetKeyDown(KeyCode.Alpha6))
        {
            GTNetworkSend.Instance.TryCastSkill(ESkillPos.Skill_6);
        }
        if (Input.GetKeyDown(KeyCode.Alpha7))
        {
            GTNetworkSend.Instance.TryCastSkill(ESkillPos.Skill_7);
        }
        if (Input.GetKeyDown(KeyCode.Alpha8))
        {
            GTNetworkSend.Instance.TryCastSkill(ESkillPos.Skill_8);
        }
        if (Input.GetKeyDown(KeyCode.Alpha9))
        {
            GTNetworkSend.Instance.TryCastSkill(ESkillPos.Skill_9);
        }
        if (Input.GetKeyDown(KeyCode.Space))
        {
            GTNetworkSend.Instance.TryJump();
        }
    }

    void OnUpdate()
    {
        if (Input.GetMouseButton(0))
        {
            GTEventCenter.FireEvent(GTEventID.TYPE_MOUSE_BUTTON);
        }
        if (Input.GetMouseButtonUp(0))
        {
            GTEventCenter.FireEvent(GTEventID.TYPE_MOUSE_BUTTON_UP);
        }
        if (Input.GetMouseButtonDown(0))
        {
            GTEventCenter.FireEvent(GTEventID.TYPE_MOUSE_BUTTON_DOWN);
        }

        if (Input.touchCount > 0)
        {
            GTEventCenter.FireEvent(GTEventID.TYPE_MOUSE_BUTTON_DOWN);
        }
        else
        {
            GTEventCenter.FireEvent(GTEventID.TYPE_MOUSE_BUTTON_UP);
        }
    }
}