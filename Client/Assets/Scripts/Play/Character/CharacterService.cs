using UnityEngine;
using System.Collections;
using System;

public class CharacterService : GTSingleton<CharacterService>
{
    public void TryCastSkill(ESkillPos pos)
    {
        GTEventCenter.FireEvent(GTEventID.TYPE_CAST_SKILL, pos);
    }

    public void TryStartMount()
    {
        GTEventCenter.FireEvent(GTEventID.TYPE_START_MOUNT);
    }

    public void TryLeaveMount()
    {
        GTEventCenter.FireEvent(GTEventID.TYPE_LEAVE_MOUNT);
    }

    public void TryMoveJoystick(float x,float y)
    {
        GTEventCenter.FireEvent(GTEventID.TYPE_MOVE_JOYSTICK, x, y);
    }

    public void TryStopJoystick()
    {
        GTEventCenter.FireEvent(GTEventID.TYPE_STOP_JOYSTICK);
    }

    public void TryPursue(Vector3 destPosition)
    {
        GTEventCenter.FireEvent(GTEventID.TYPE_MOVE_PURSUE, destPosition);
    }

    public void TryJump()
    {
        GTEventCenter.FireEvent(GTEventID.TYPE_JUMP);
    }
}
