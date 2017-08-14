using UnityEngine;
using System.Collections;

/// <summary>
/// 移动命令
/// </summary>
public class CommandMove : ICommand
{
    public Vector3     DestPosition
    {
        get
        {
            switch (MoveType)
            {
                case EMoveType.SeekActor:
                    return TargetActor.Pos;
                case EMoveType.SeekPosition:
                    return TargetPos;
                case EMoveType.SeekTransform:
                    return Target.position;
                default:
                    return Vector3.zero;
            }
        }
    }

    public EMoveType   MoveType;
    public Vector3     TargetPos;
    public Transform   Target = null;
    public Character   TargetActor = null;
    public Callback    OnFinish;
    public Vector3     Motion;

    public override Resp Do()
    {
        CmdHandler<CommandMove> call = Del as CmdHandler<CommandMove>;
        return call == null ? Resp.TYPE_NO : call(this);
    }

    public CommandMove Update(Vector3 destPosition, Callback onFinish)
    {
        this.TargetPos = destPosition;
        this.MoveType  = EMoveType.SeekPosition;
        this.OnFinish  = onFinish;
        return this;
    }

    public CommandMove Update(Transform target, Callback onFinish)
    {
        this.Target    = target;
        this.MoveType  = EMoveType.SeekTransform;
        this.OnFinish  = onFinish;
        return this;
    }

    public CommandMove Update(Character actor, Callback onFinish)
    {
        this.TargetActor = actor;
        this.MoveType    = EMoveType.SeekActor;
        this.OnFinish    = onFinish;
        return this;
    }

    public CommandMove Update(Vector3 motion)
    {
        this.Motion = motion;
        this.MoveType = EMoveType.MoveForce;
        return this;
    }
}
