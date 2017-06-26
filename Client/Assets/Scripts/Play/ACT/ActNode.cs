using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Reflection;
using System.Xml;
using System;

namespace ACT
{
    public class ActNode
    {
        public virtual void LoadDoc(XmlElement element)
        {
            FieldInfo[] fields = this.GetType().GetFields();
            for (int i = 0; i < fields.Length; i++)
            {
                FieldInfo field = fields[i];
                Type t = field.FieldType;
                string value = element.GetAttribute(field.Name);
                if (string.IsNullOrEmpty(value))
                {
                    continue;
                }
                object v = default(object);
                if (t.BaseType == typeof(Enum))
                {
                    v = Enum.ToObject(t, value.ToInt32());
                }
                else if (t == typeof(int))
                {
                    v = value.ToInt32();
                }
                else if (t == typeof(bool))
                {
                    v = value == "true";
                }
                else if (t == typeof(float))
                {
                    v = value.ToFloat();
                }
                else if (t == typeof(string))
                {
                    v = value;
                }
                else if (t == typeof(Vector3))
                {
                    v = value.ToVector3(true);
                }
                else if (t == typeof(List<string>))
                {
                    string[] array = value.Split('~');
                    List<string> list = (List<string>)field.GetValue(this);
                    list.AddRange(array);
                    v = list;
                }
                else if (t == typeof(List<float>))
                {
                    string[] array = value.Split('~');
                    List<float> list = (List<float>)field.GetValue(this);
                    for (int k = 0; k < array.Length; k++)
                    {
                        float kk = array[k].ToFloat();
                        list.Add(kk);
                    }
                    v = list;
                }
                else if (t == typeof(List<Int32>))
                {
                    string[] array = value.Split('~');
                    List<Int32> list = (List<Int32>)field.GetValue(this);
                    for (int k = 0; k < array.Length; k++)
                    {
                        Int32 kk = array[k].ToInt32();
                        list.Add(kk);
                    }
                    v = list;
                }
                field.SetValue(this, v);
            }
        }

        public virtual void SaveDoc(XmlDocument doc, XmlElement element)
        {
            FieldInfo[] fields = this.GetType().GetFields();
            for (int i = 0; i < fields.Length; i++)
            {
                FieldInfo field = fields[i];
                Type t = field.FieldType;
                if (t.BaseType == typeof(Enum))
                {
                    element.SetAttribute(field.Name, ((int)field.GetValue(this)).ToString());
                }
                else if (t == typeof(int))
                {
                    element.SetAttribute(field.Name, field.GetValue(this).ToString());
                }
                else if (t == typeof(bool))
                {
                    element.SetAttribute(field.Name, field.GetValue(this).ToString());
                }
                else if (t == typeof(float))
                {
                    element.SetAttribute(field.Name, field.GetValue(this).ToString());
                }
                else if (t == typeof(string))
                {
                    element.SetAttribute(field.Name, field.GetValue(this).ToString());
                }
                else if (t == typeof(Vector3))
                {
                    Vector3 vector3 = (Vector3)field.GetValue(this);
                    element.SetAttribute(field.Name, GTTools.SaveVector3(vector3));
                }
            }
        }
    }
}

