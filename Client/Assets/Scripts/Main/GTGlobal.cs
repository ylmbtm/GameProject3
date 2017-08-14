using UnityEngine;
using System.Collections;
using UnityEngine.SceneManagement;
using System.Collections.Generic;
using Protocol;

public class GTGlobal
{
    public static int              CurPlayerCarrerID;
    public static ulong            CurPlayerGUID;
    public static int              CurCopyID;
    public static int              CurCopyGUID;
    public static ELanguage        Language;
    public static XCharacter       Main;
    public static XCharacter       MainPartner1;
    public static XCharacter       MainPartner2;
    public static int              LastCityID;
    public static bool             IsAlong;
    public static bool             IsLaunched;
    public static GTLocalData      LocalData;

    public static void             Read()
    {
        string key = System.IO.Directory.GetCurrentDirectory();
        string json = PlayerPrefs.GetString(key, string.Empty);
        if(string.IsNullOrEmpty(json))
        {
            LocalData = new GTLocalData();
        }
        else
        {
            LocalData = LitJson.JsonMapper.ToObject<GTLocalData>(json);
        }
    }
}