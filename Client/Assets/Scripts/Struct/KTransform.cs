using UnityEngine;
using System.Collections;
using Protocol;

public class KTransform
{
    public Vector3 Pos    = Vector3.zero;
    public Vector3 Euler  = Vector3.zero;
    public Vector3 Scale  = Vector3.one;

    static KTransform()
    {
        Default = new KTransform();
    }

    public static KTransform Create(Vector3 pos, Vector3 angle)
    {
        KTransform data = new KTransform();
        data.Pos = pos;
        data.Euler = angle;
        data.Scale = Vector3.one;
        return data;
    }

    public static KTransform Create(Vector3 pos, Vector3 angle, Vector3 scale)
    {
        KTransform data = new KTransform();
        data.Pos = pos;
        data.Euler = angle;
        data.Scale = scale;
        return data;
    }

    public static KTransform Default { get; private set; }

    public static KTransform Create(XTransform data)
    {
        KTransform d = new KTransform();
        d.Pos        = new Vector3(data.Pos.x, data.Pos.y, data.Pos.z);
        d.Euler     = new Vector3(data.Forward.x, data.Forward.y, data.Forward.z);
        d.Scale      = Vector3.one;
        return d;
    }
}
