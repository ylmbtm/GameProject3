using UnityEngine;
using System.Collections;
using System.Reflection;
using System;
using System.Collections.Generic;

namespace ACT
{
    public class ActBuffItem : ActNode
    {
        [SerializeField]
        public float             StTime;
        [SerializeField]
        public float             EdTime;
        [SerializeField][ActFieldAttribute]
        public List<ActBuffItem> Children = new List<ActBuffItem>();

        public Character Carryer  { get; set; }
        public Character Caster   { get; set; }
        public ActBuff   Buff     { get; set; }
        public float     Duration { get { return EdTime - StTime; } }


        public virtual bool Check()
        {
            return true;
        }

        public virtual void Enter()
        {
     
        }

        public virtual void Execute()
        {

        }

        public virtual void Stop()
        {

        }

        public virtual void Exit()
        {

        }

        public virtual void Refresh()
        {

        }

        public virtual void Overlay()
        {

        }

        public static ActBuffItem Clone(ActBuffItem src)
        {
            ActBuffItem item = (ActBuffItem)Activator.CreateInstance(src.GetType());
            FieldInfo[] fields = item.GetType().GetFields();
            for (int i = 0; i < fields.Length; i++)
            {
                FieldInfo newField = fields[i];
                if (newField.IsDefined(typeof(SerializeField), true) &&
                    newField.IsDefined(typeof(ActFieldAttribute), true) == false)
                {
                    FieldInfo oldField = src.GetType().GetField(newField.Name);
                    if (oldField != null)
                    {
                        newField.SetValue(item, oldField.GetValue(src));
                    }
                }
            }
            for (int i = 0; i < src.Children.Count; i++)
            {
                ActBuffItem child = Clone(src.Children[i]);
                item.AddChild(child);
            }
            return item;
        }

        public static T Clone<T>(ActBuffItem src) where T : ActBuffItem
        {
            T item = Activator.CreateInstance<T>();
            FieldInfo[] fields = item.GetType().GetFields();
            for (int i = 0; i < fields.Length; i++)
            {
                FieldInfo newField = fields[i];
                if (newField.IsDefined(typeof(SerializeField), true) &&
                    newField.IsDefined(typeof(ActFieldAttribute), true) == false)
                {
                    FieldInfo oldField = src.GetType().GetField(newField.Name);
                    if (oldField != null)
                    {
                        newField.SetValue(item, oldField.GetValue(src));
                    }
                }
            }
            for (int i = 0; i < src.Children.Count; i++)
            {
                ActBuffItem child = Clone(src.Children[i]);
                item.AddChild(child);
            }
            return item;
        }

        public virtual void AddChild(ActBuffItem item)
        {
            Children.Add(item);
        }

        public virtual void DelChild(ActBuffItem item)
        {
            Children.Remove(item);
        }
    }
}