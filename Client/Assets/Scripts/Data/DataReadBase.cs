using Protocol;
using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Text;
using System.Xml;
using UnityEngine;


public class DataReadBase<TType, TVal> where TVal : ProtoBuf.IExtensible
{
    public static string                    XmlPath = string.Empty;
    public static EDataKeyType              KeyType = EDataKeyType.Id;
    public static Dictionary<int, TVal>     Dict    = new Dictionary<int, TVal>();

    public static void Read(string path, EDataKeyType keyType)
    {
        XmlPath = path;
        KeyType = keyType;
        XmlNodeList nodeList = GTXmlHelper.GetXmlNodeList(XmlPath);
        for (int i = 0; i < nodeList.Count; i++)
        {
            XmlElement xe = nodeList.Item(i) as XmlElement;
            if (xe == null)
            {
                continue;
            }
            int key = xe.GetAttribute(KeyType.ToString()).ToInt32();
            for (int j = 0; j < xe.Attributes.Count; j++)
            {
                string name = xe.Attributes[j].Name;
                string value = xe.Attributes[j].Value;
                AppendAttribute(key, name, value);
            }
        }
    }

    public static void AppendAttribute(int key, string name, string value)
    {
        if (Dict == null)
        {
            return;
        }
        TVal obj = default(TVal);
        Dict.TryGetValue(key, out obj);
        if (obj == null)
        {
            obj = Activator.CreateInstance<TVal>();
            Dict.Add(key, obj);
        }
        PropertyInfo[] fields = obj.GetType().GetProperties();
        for (int i = 0; i < fields.Length; i++)
        {
            PropertyInfo field = fields[i];
            if (field.Name == name)
            {
                Parse(ref field, obj, value);
                break;
            }
        }
    }

    public static void Insert(int key, TVal obj)
    {
        Dict[key] = obj;
        GTXmlHelper.Append(XmlPath, key.ToString(), obj, KeyType);
    }

    public static void Delete(int key)
    {
        Dict.Remove(key);
        GTXmlHelper.Delete(XmlPath, key.ToString(), KeyType);
    }

    public static void Update(int key, TVal obj)
    {
        if (Dict.ContainsKey(key))
        {
            GTXmlHelper.Update(XmlPath, key.ToString(), obj, KeyType);
        }
        else
        {
            Insert(key, obj);
        }
    }

    public static TVal GetDataById(int key)
    {
        TVal obj = default(TVal);
        Dict.TryGetValue(key, out obj);
        return obj;
    }

    public static void Clear(bool onlyClearMemory = true)
    {
        if (!onlyClearMemory)
        {
            GTXmlHelper.ClearAll(XmlPath);
        }
        if (Dict != null)
        {
            Dict.Clear();
        }
    }

    public static void Parse(ref PropertyInfo field, object obj, string value)
    {
        Type fieldType = field.PropertyType;
        if (fieldType      == typeof(int))
        {
            field.SetValue(obj, value.ToInt32(), null);
        }
        else if (fieldType == typeof(string))
        {
            field.SetValue(obj, value, null);
        }
        else if (fieldType == typeof(Vector3))
        {
            field.SetValue(obj, value.ToVector3(true), null);
        }
        else if (fieldType == typeof(float))
        {
            field.SetValue(obj, value.ToFloat(), null);
        }
    }

    public static bool ContainsKey(int key)
    {
        return Dict.ContainsKey(key);
    }
}

public sealed class DataDBSBagFashion :   DataReadBase<DataDBSBagFashion, XItem> { }
public sealed class DataDBSBagGem :       DataReadBase<DataDBSBagGem, XItem> { }
public sealed class DataDBSBagItem :      DataReadBase<DataDBSBagItem, XItem> { }
public sealed class DataDBSBagRune :      DataReadBase<DataDBSBagRune, XItem> { }
public sealed class DataDBSDressFashion : DataReadBase<DataDBSDressFashion, XItem> { }
public sealed class DataDBSDressGem :     DataReadBase<DataDBSDressGem, XItem> { }
public sealed class DataDBSDressRune :    DataReadBase<DataDBSDressRune, XItem> { }
public sealed class DataDBSBagChip :      DataReadBase<DataDBSBagChip, XItem> { }
public sealed class DataDBSDressEquip :   DataReadBase<DataDBSDressEquip, XItem> { }
public sealed class DataDBSEquip :        DataReadBase<DataDBSEquip, XEquip> { }
public sealed class DataDBSGem :          DataReadBase<DataDBSGem, XGem> { }
public sealed class DataDBSMoney :        DataReadBase<DataDBSMoney, XMoney> { }
public sealed class DataDBSSoul :         DataReadBase<DataDBSSoul, XSoul> { }
public sealed class DataDBSRaid :         DataReadBase<DataDBSRaid, XRaid> { }
public sealed class DataDBSMainChapter :  DataReadBase<DataDBSMainChapter, XMainChapter> { }
public sealed class DataDBSCopy :         DataReadBase<DataDBSCopy, XCopy> { }
public sealed class DataDBSRole :         DataReadBase<DataDBSRole, XCharacter> { }
public sealed class DataDBSRune :         DataReadBase<DataDBSRune, XRune> { }
public sealed class DataDBSAction :       DataReadBase<DataDBSAction, XAction> { }
public sealed class DataDBSMount :        DataReadBase<DataDBSMount, XMount> { }
public sealed class DataDBSRelics :       DataReadBase<DataDBSRelics, XRelics> { }
public sealed class DataDBSPartner :      DataReadBase<DataDBSPartner, XPartner> { }
public sealed class DataDBSPet :          DataReadBase<DataDBSPet, XPet> { }
public sealed class DataDBSGuide :        DataReadBase<DataDBSGuide, XGuide> { }
public sealed class DataDBSThreadTask :   DataReadBase<DataDBSThreadTask, XTaskThread> { }
public sealed class DataDBSBranchTask :   DataReadBase<DataDBSBranchTask, XTaskBranch> { }
public sealed class DataDBSDailyTask :    DataReadBase<DataDBSDailyTask, XTaskDaily> { }
