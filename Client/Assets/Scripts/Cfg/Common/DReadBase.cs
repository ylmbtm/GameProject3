using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Xml;
using System;

public class DReadBase<TKey, TValue> where TValue : DObj<TKey>
{
    static Dictionary<TKey, TValue> mDataMap = new Dictionary<TKey, TValue>();
    public static string p;

    public static void                    Read(string path)
    {
        p = path;
        if (string.IsNullOrEmpty(path))
            return;
        XmlDocument doc = new XmlDocument();
        TextAsset asset = Resources.Load<TextAsset>(path);
        if (asset == null)
            return;
        doc.LoadXml(asset.text);
        XmlNode root = doc.SelectSingleNode("RECORDS");
        if (root == null)
            return;
        XmlNodeList list = root.ChildNodes;
        for (int i = 0; i < list.Count; i++)
        {
            XmlNode pNode = list[i];
            XmlElement element = pNode as XmlElement;
            if(element==null)
            {
                continue;
            }
            TValue value = Activator.CreateInstance<TValue>();
            value.Read(element);
            mDataMap[value.GetKey()]=value;
        }
    }

    public static TValue                  GetDataById(TKey key)
    {
        TValue v = null;
        mDataMap.TryGetValue(key, out v);
        return v;
    }

    public static Dictionary<TKey,TValue> Dict
    {
        get { return mDataMap; }
    }

    public static int                     Count
    {
        get { return mDataMap.Count; }
    }

    public static bool                    ContainsKey(TKey key)
    {
        return mDataMap.ContainsKey(key);
    }
}