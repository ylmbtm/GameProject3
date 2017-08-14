using UnityEngine;
using System.Collections;
using System.Xml;
using System;
using System.Reflection;
using System.IO;
using System.Collections.Generic;

public static class GTXmlHelper
{
    public const string ROOTNAME    = "RECORDS";
    public const string ELEMNAME    = "RECORD";
    public const string NODENAME    = "Item";
    public const string TYPENAME    = "Type";
    public const string KEYNAME     = "Key";
    public static bool  CanWrite    = true;

    public static Int16       GetInt16(this XmlElement element, string name)
    {
        string s = element.GetAttribute(name);
        if (string.IsNullOrEmpty(s)) return 0;
        Int16 v = 0;
        Int16.TryParse(s, out v);
        return v;
    }

    public static Int32       GetInt32(this XmlElement element, string name)
    {
        string s = element.GetAttribute(name);
        if (string.IsNullOrEmpty(s)) return 0;
        int v = 0;
        int.TryParse(s, out v);
        return v;
    }

    public static Int64       GetInt64(this XmlElement element, string name)
    {
        string s = element.GetAttribute(name);
        if (string.IsNullOrEmpty(s)) return 0;
        long v = 0;
        long.TryParse(s, out v);
        return v;
    }

    public static UInt16      GetUInt16(this XmlElement element, string name)
    {
        string s = element.GetAttribute(name);
        if (string.IsNullOrEmpty(s)) return 0;
        UInt16 v = 0;
        UInt16.TryParse(s, out v);
        return v;
    }

    public static UInt32      GetUInt32(this XmlElement element, string name)
    {
        string s = element.GetAttribute(name);
        if (string.IsNullOrEmpty(s)) return 0;
        UInt32 v = 0;
        UInt32.TryParse(s, out v);
        return v;
    }

    public static UInt64      GetUInt64(this XmlElement element, string name)
    {
        string s = element.GetAttribute(name);
        if (string.IsNullOrEmpty(s)) return 0;
        UInt64 v = 0;
        UInt64.TryParse(s, out v);
        return v;
    }

    public static Vector2     GetVector2(this XmlElement element, string name)
    {
        string s = element.GetAttribute(name);
        if (string.IsNullOrEmpty(s)) return Vector3.zero;
        return GTTools.ToVector2(s);
    }

    public static Vector3     GetVector3(this XmlElement element, string name)
    {
        string s = element.GetAttribute(name);
        if (string.IsNullOrEmpty(s)) return Vector3.zero;
        return GTTools.ToVector3(s,true);
    }

    public static bool        GetBool(this XmlElement element, string name)
    {
        string s = element.GetAttribute(name);
        if (string.IsNullOrEmpty(s)) return false;
        return (s == "1" || s == "true") ? true : false;
    }

    public static List<int>   GetList(this XmlElement element, string name)
    {
        List<int> list = new List<int>();
        string s = element.GetAttribute(name);
        if (string.IsNullOrEmpty(s)) return list;
        string[] array = s.Split(',');
        for (int i = 0; i < array.Length; i++)
        {
            int v = 0;
            int.TryParse(array[i], out v);
            list.Add(v);
        }
        return list;
    }

    public static float       GetFloat(this XmlElement element, string name)
    {
        string s = element.GetAttribute(name);
        if (string.IsNullOrEmpty(s)) return 0;
        float v = 0;
        float.TryParse(s, out v);
        return v;
    }

    public static string      GetString(this XmlElement element, string name)
    {
        string s = element.GetAttribute(name);
        if (string.IsNullOrEmpty(s)) return string.Empty;
        return s;
    }

    public static T           GetEnum<T>(this XmlElement element, string name)
    {
        string s = element.GetAttribute(name);
        if (string.IsNullOrEmpty(s)) return default(T);
        return (T)Enum.Parse(typeof(T), s);
    }

    public static XmlDocument LoadDocument(string path)
    {
        return LoadDocument(path, ROOTNAME);
    }

    public static XmlDocument LoadDocument(string path, string rootElementName)
    {
        XmlDocument xmlDoc = new XmlDocument();
        if (File.Exists(path))
        {
            xmlDoc.Load(path);
        }
        else
        {
            FileStream fs = File.Create(path);
            XmlDeclaration xmlDeclaration = xmlDoc.CreateXmlDeclaration("1.0", "UTF-8", "yes");
            XmlNode root = xmlDoc.CreateElement(rootElementName);
            xmlDoc.AppendChild(xmlDeclaration);
            xmlDoc.AppendChild(root);
            fs.Close();
            fs.Dispose();
            xmlDoc.Save(path);
        }
        return xmlDoc;
    }

    public static XmlNodeList GetXmlNodeList(string path)
    {
        XmlDocument xmlDoc = LoadDocument(path);
        XmlNodeList nodeList = xmlDoc.SelectSingleNode(ROOTNAME).ChildNodes;
        return nodeList;
    }

    public static void Append(string path, string key, object obj, EDataKeyType keyType)
    {
        if (CanWrite == false)
        {
            return;
        }

        XmlDocument xmlDoc = LoadDocument(path);
        FileStream fs = File.OpenRead(path);
        if (fs == null)
        {
            return;
        }
        XmlNode xmlNode = xmlDoc.SelectSingleNode(ROOTNAME);
        XmlElement em = xmlDoc.CreateElement(ELEMNAME);
        xmlNode.AppendChild(em);
        em.SetAttribute(keyType.ToString(), key);
        Save(em, obj);
        fs.Close();
        fs.Dispose();
        xmlDoc.Save(path);
    }

    public static void Update(string path, string key, object obj, EDataKeyType keyType)
    {
        if (CanWrite == false)
        {
            return;
        }
        XmlDocument xmlDoc = LoadDocument(path);
        FileStream fs = File.OpenRead(path);
        if (fs == null)
        {
            return;
        }
        XmlNodeList xmlNodeList = xmlDoc.SelectSingleNode(ROOTNAME).ChildNodes;
        for (int i = 0; i < xmlNodeList.Count; i++)
        {
            XmlElement xe = xmlNodeList.Item(i) as XmlElement;
            if (xe == null)
                continue;
            string id = xe.GetAttribute(keyType.ToString());
            if (id == key)
            {
                Save(xe, obj);
                break;
            }
        }
        fs.Close();
        fs.Dispose();
        xmlDoc.Save(path);
    }

    public static void Delete(string path, string key, EDataKeyType keyType)
    {
        if (CanWrite == false)
        {
            return;
        }
        XmlDocument xmlDoc = LoadDocument(path);
        FileStream fs = File.OpenRead(path);
        if (fs == null)
        {
            return;
        }
        XmlNodeList xmlNodeList = xmlDoc.SelectSingleNode(ROOTNAME).ChildNodes;
        XmlElement xe = null;
        for (int i = 0; i < xmlNodeList.Count; i++)
        {
            xe = xmlNodeList.Item(i) as XmlElement;
            if (xe == null)
                continue;
            string id = xe.GetAttribute(keyType.ToString());
            if (id == key)
            {
                XmlNode node = xmlDoc.SelectSingleNode(ROOTNAME);
                node.RemoveChild(xe);
                break;
            }
        }
        fs.Close();
        fs.Dispose();
        xmlDoc.Save(path);
    }

    public static bool ClearAll(string path)
    {
        if (CanWrite == false)
        {
            return false;
        }
        XmlDocument xmlDoc = LoadDocument(path);
        FileStream fs = File.OpenRead(path);
        if (fs == null)
        {
            return false;
        }
        XmlNode node = xmlDoc.SelectSingleNode(ROOTNAME);
        node.RemoveAll();
        fs.Close();
        fs.Dispose();
        xmlDoc.Save(path);
        return true;
    }

    public static void InsertAll<T>(string path, Dictionary<int, T> dict, EDataKeyType keyType)
    {
        if (CanWrite == false)
        {
            return;
        }
        XmlDocument xmlDoc = LoadDocument(path);
        FileStream fs = File.OpenRead(path);
        if (fs == null) return;
        if (dict == null) return;
        XmlNode xmlNode = xmlDoc.SelectSingleNode(ROOTNAME);
        Dictionary<int, T>.Enumerator em = dict.GetEnumerator();
        while (em.MoveNext())
        {
            XmlElement xe = xmlDoc.CreateElement(ELEMNAME);
            xmlNode.AppendChild(xe);
            int key = em.Current.Key;
            T obj = em.Current.Value;
            xe.SetAttribute(keyType.ToString(), key.ToString());
            Save<T>(xe, obj);
        }
        em.Dispose();
        fs.Close();
        fs.Dispose();
        xmlDoc.Save(path);
    }

    public static void Save<T>(XmlElement xe, T obj)
    {
        if (CanWrite == false)
        {
            return;
        }
        if (xe == null || obj == null)
        {
            return;
        }
        PropertyInfo[] fields = obj.GetType().GetProperties();
        for (int i = 0; i < fields.Length; i++)
        {
            PropertyInfo field = fields[i];
            string fieldName = field.Name;
            object val = field.GetValue(obj, null);
            string fieldValue = (val == null) ? string.Empty : val.ToString();
            xe.SetAttribute(fieldName, fieldValue);
        }
    }
}