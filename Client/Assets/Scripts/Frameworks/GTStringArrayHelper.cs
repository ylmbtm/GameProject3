using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;


public static class GTStringArrayHelper
{
    public static string  GetString(this string[] array, int index)
    {
        return (index > array.Length - 1) ? string.Empty : array[index];
    }

    public static Int16   GetInt16(this string[] array, int index)
    {
        string s = GetString(array, index);
        return s.ToInt16();
    }

    public static Int32   GetInt32(this string[] array, int index)
    {
        string s = GetString(array, index);
        return s.ToInt32();
    }

    public static Int64   GetInt64(this string[] array, int index)
    {
        string s = GetString(array, index);
        return s.ToInt64();
    }

    public static UInt16  GetUInt16(this string[] array, int index)
    {
        string s = GetString(array, index);
        return s.ToUInt16();
    }

    public static UInt32  GetUInt32(this string[] array, int index)
    {
        string s = GetString(array, index);
        return s.ToUInt32();
    }

    public static UInt64  GetUInt64(this string[] array, int index)
    {
        string s = GetString(array, index);
        return s.ToUInt64();
    }

    public static bool    GetBool(this string[] array, int index)
    {
        string s = GetString(array, index);
        return s.ToBool();
    }

    public static T       GetEnum<T>(this string[] array, int index)
    {
        string s = GetString(array, index);
        if (string.IsNullOrEmpty(s))
        {
            return default(T);
        }
        else
        {
            return (T)Enum.Parse(typeof(T), s);
        }
    }

    public static Vector3 GetVector3(this string[] array, int index)
    {
        string s = GetString(array, index);
        string[] aa = s.Split('~');
        if (aa.Length == 3)
        {
            return new Vector3(aa[0].ToFloat(), aa[1].ToFloat(), aa[2].ToFloat());
        }
        else
        {
            return Vector3.zero;
        }
    }

    public static Vector2 GetVector2(this string[] array, int index)
    {
        string s = GetString(array, index);
        string[] aa = s.Split('~');
        if (aa.Length == 2)
        {
            return new Vector2(aa[0].ToFloat(), aa[1].ToFloat());
        }
        else
        {
            return Vector2.zero;
        }
    }

    public static string  GetString(this List<string> list, int index)
    {
        return (index > list.Count - 1) ? string.Empty : list[index];
    }

    public static Int16   GetInt16(this List<string> list, int index)
    {
        string s = GetString(list, index);
        return s.ToInt16();
    }

    public static Int32   GetInt32(this List<string> list, int index)
    {
        string s = GetString(list, index);
        return s.ToInt32();
    }

    public static Int64   GetInt64(this List<string> list, int index)
    {
        string s = GetString(list, index);
        return s.ToInt64();
    }

    public static UInt16  GetUInt16(this List<string> list, int index)
    {
        string s = GetString(list, index);
        return s.ToUInt16();
    }

    public static UInt32  GetUInt32(this List<string> list, int index)
    {
        string s = GetString(list, index);
        return s.ToUInt32();
    }

    public static UInt64  GetUInt64(this List<string> list, int index)
    {
        string s = GetString(list, index);
        return s.ToUInt64();
    }

    public static bool    GetBool(this List<string> list, int index)
    {
        string s = GetString(list, index);
        return s.ToBool();
    }

    public static T       GetEnum<T>(this List<string> list, int index)
    {
        string s = GetString(list, index);
        if (string.IsNullOrEmpty(s))
        {
            return default(T);
        }
        else
        {
            return (T)Enum.Parse(typeof(T), s);
        }
    }

    public static Vector3 GetVector3(this List<string> list, int index)
    {
        string s = GetString(list, index);
        string[] aa = s.Split('|');
        if (aa.Length == 3)
        {
            return new Vector3(aa[0].ToFloat(), aa[1].ToFloat(), aa[2].ToFloat());
        }
        else
        {
            return Vector3.zero;
        }
    }

    public static Vector2 GetVector2(this List<string> list, int index)
    {
        string s = GetString(list, index);
        string[] aa = s.Split('~');
        if (aa.Length == 2)
        {
            return new Vector2(aa[0].ToFloat(), aa[1].ToFloat());
        }
        else
        {
            return Vector2.zero;
        }
    }
}
