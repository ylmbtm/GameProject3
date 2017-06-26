using UnityEngine;
using System.Collections;
using System;
using System.Reflection;

namespace LVL
{
    public class LevelElement : MonoBehaviour, ILevelElement
    {
        [LevelVariable]
        public int             ID        { get; set; }

        [LevelVariable]
        public int             GUID      { get; set; }

        [LevelVariable]
        public Vector3         Pos
        {
            get { return transform.position; }
            set { transform.position = value; }
        }

        [LevelVariable]
        public Vector3         Scale
        {
            get { return transform.localScale; }
            set { transform.localScale = value; }
        }

        [LevelVariable]
        public Vector3         Euler
        {
            get { return transform.eulerAngles; }
            set { transform.eulerAngles = value; }
        }

        public virtual void Build()
        {
            
        }

        public void Import(DCFG cfg)
        {
            FieldInfo[] fields = this.GetType().GetFields();
            for (int i = 0; i < fields.Length; i++)
            {
                FieldInfo newField = fields[i];
                if (newField.IsDefined(typeof(LevelVariable), true))
                {
                    FieldInfo cfgField = cfg.GetType().GetField(newField.Name);
                    if (cfgField != null)
                    {
                        newField.SetValue(this, cfgField.GetValue(cfg));
                    }
                }
            }

            PropertyInfo[] propertys = this.GetType().GetProperties();
            for (int i = 0; i < propertys.Length; i++)
            {
                PropertyInfo newProperty = propertys[i];
                if (newProperty.IsDefined(typeof(LevelVariable), true))
                {
                    FieldInfo cfgField = cfg.GetType().GetField(newProperty.Name);
                    if (cfgField != null)
                    {
                        newProperty.SetValue(this, cfgField.GetValue(cfg), null);
                    }
                }
            }
        }

        public DCFG Export()
        {
            return null;
        }
    }
}
