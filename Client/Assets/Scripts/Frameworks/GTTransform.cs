using UnityEngine;
using System.Collections;
using Protocol;

public class GTTransform
{
    public Vector3 Pos    = Vector3.zero;
    public Vector3 Euler  = Vector3.zero;

    static GTTransform()
    {
        Default = new GTTransform();
    }

    public static GTTransform Create(Vector3 pos, Vector3 angle)
    {
        GTTransform data = new GTTransform();
        data.Pos = pos;
        data.Euler = angle;
        return data;
    }

    public static GTTransform Create(Vector3 pos, Vector3 angle, Vector3 scale)
    {
        GTTransform data = new GTTransform();
        data.Pos = pos;
        data.Euler = angle;
        return data;
    }

    public static GTTransform Create(Vector3 pos, float face)
    {
        GTTransform data = new GTTransform();
        data.Pos = pos;
        data.Euler = new Vector3(0, face, 0);
        return data;
    }

    public static GTTransform Create(Vector3 pos, float vx, float vy, float vz)
    {
        GTTransform data = new GTTransform();
        data.Pos = pos;
        data.Euler = Quaternion.FromToRotation(Vector3.forward, new Vector3(vx, vy, vz)).eulerAngles;
        return data;
    }

    public static GTTransform Create(float x, float y, float z,float face)
    {
        GTTransform data = new GTTransform();
        data.Pos   = new Vector3(x, y, z);
        data.Euler = new Vector3(0, face, 0);
        return data;
    }

    public static GTTransform Create(float x, float y, float z, float vx, float vy, float vz)
    {
        GTTransform data = new GTTransform();
        data.Pos   = new Vector3(x, y, z);
        data.Euler = Quaternion.FromToRotation(Vector3.forward, new Vector3(vx, vy, vz)).eulerAngles;
        return data;
    }

    public static GTTransform Default { get; private set; }
}
