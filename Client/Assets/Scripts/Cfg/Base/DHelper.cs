using UnityEngine;
using System.Collections;
using System.IO;
using System.Xml;
using System;
using System.Collections.Generic;
using System.Xml.Linq;
using System.Linq;
using System.Reflection;

public abstract class DCFG
{
    static char[] SEPARATOR = new char[3] { '(', ',', ')' };

    public abstract void Read(XmlElement os);
    public abstract void Write(XmlDocument doc, XmlElement os);

    public static Int32    ReadInt32(string value)
    {
        int v = 0;
        int.TryParse(value, out v);
        return v;
    }

    public static float    ReadFloat(string value)
    {
        float  v = 0;
        float.TryParse(value, out v);
        return v;
    }

    public static Vector3  ReadVector3(string value)
    {
        string[] array = value.Split(SEPARATOR);
        return new Vector3(array[1].ToFloat(), array[2].ToFloat(), array[3].ToFloat());
    }

    public static Boolean  ReadBool(string value)
    {
        return value == "1" ? true : false;
    }

    public static T        ReadObj<T>(XmlElement os) where T : DCFG
    {
        Type type = typeof(T);
        try
        {
            object v = Activator.CreateInstance(type);
            T result = (T)v;
            result.Read(os);
            return result;
        }
        catch
        {
            Debug.LogError("XmlElement is null:" + type.ToString());
            return null;
        }
    }

    public static List<T>  ReadList<T>(XmlElement os) where T : DCFG
    {
        List<T> list = new List<T>();
        foreach(var current in GetChilds(os))
        {
            T obj = ReadObj<T>(current);
            if(obj!=null)
            {
                list.Add(obj);
            }
        }
        return list;
    }

    public static void Write(XmlDocument doc, XmlElement os, string name, int v)
    {
        os.SetAttribute(name, v.ToString());
    }

    public static void Write(XmlDocument doc, XmlElement os, string name, float v)
    {
        os.SetAttribute(name, v.ToString("0.00"));
    }

    public static void Write(XmlDocument doc, XmlElement os, string name, bool v)
    {
        os.SetAttribute(name, (v ? 1 : 0).ToString());
    }

    public static void Write(XmlDocument doc, XmlElement os, string name, Vector3 v)
    {
        string s = string.Format("({0},{1},{2})", v.x.ToString("0.00"), v.y.ToString("0.00"), v.z.ToString("0.00"));
        os.SetAttribute(name, s);
    }

    public static void Write(XmlDocument doc, XmlElement os, string name, string v)
    {
        os.SetAttribute(name, v);
    }

    public static void Write<T>(XmlDocument doc, XmlElement os, string name, T obj) where T : DCFG
    {
        XmlElement child = doc.CreateElement(name);
        obj.Write(doc, child);
        os.AppendChild(child);
    }

    public static void Write<T>(XmlDocument doc, XmlElement os, string name, List<T> list) where T : DCFG
    {
        XmlElement child = doc.CreateElement(name);
        os.AppendChild(child);
        for (int i = 0; i < list.Count; i++)
        {
            T obj = list[i];
            Write(doc, child, obj.GetType().Name, obj);
        }
    }

    public static List<XmlElement> GetChilds(XmlNode parent)
    {
        List<XmlElement> list = new List<XmlElement>();
        if (parent != null)
        {
            list.AddRange(
                from XmlElement sub in parent.ChildNodes
                where sub.NodeType == XmlNodeType.Element
                select sub);
        }
        return list;
    }

    public bool Load(string file)
    {
        XmlDocument xmlDocument = new XmlDocument();
        TextAsset pAsset = GTResourceManager.Instance.Load<TextAsset>(file);
        if (pAsset == null)
        {
            return false;
        }
        xmlDocument.LoadXml(pAsset.text);
        this.Read(xmlDocument.DocumentElement);
        return true;
    }

    public void Save(string file)
    {
        XmlDocument xmlDoc = new XmlDocument();
        FileStream fs = File.Create(file);
        XmlNode root = xmlDoc.CreateElement("Root");
        xmlDoc.AppendChild(root);
        this.Write(xmlDoc, root as XmlElement);
        fs.Close();
        fs.Dispose();
        xmlDoc.Save(file);
    }
}
