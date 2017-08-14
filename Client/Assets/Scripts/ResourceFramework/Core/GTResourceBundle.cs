using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class GTResourceBundle
{
    public const int    TYPE_NONE       = 0;
    public const int    TYPE_LOADING    = 1;
    public const int    TYPE_LOADED     = 2;

    public string       AssetBundleName = string.Empty;
    public List<string> Deps            = new List<string>();
    public AssetBundle  AB;
    public int          State           = TYPE_NONE;
}