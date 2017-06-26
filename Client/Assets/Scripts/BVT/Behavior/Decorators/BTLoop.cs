using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using BVT;
using UnityEngine;
using BVT;

namespace BVT
{
    [NodeAttribute(Type = "装饰节点", Label = "循环节点")]
    [NodeDesc("循环节点循环执行指定的次数")]
    [NodeName("05CBF2", "Repeater")]
    public class BTLoop : BTDecorator
    {
        public enum RepeatTypes
        {
            RepeatTimes,
            RepeatUntil,
        }

        public enum RepeatUntil
        {
            Failure,
            Success,
        }

        [NodeVariable]
        public RepeatTypes Type  = RepeatTypes.RepeatTimes;
        [NodeVariable]
        public RepeatUntil Until = RepeatUntil.Success;
        [NodeVariable]
        public int         Times = 1;

        private int        mUseTimes = 0;

        public override ENST OnExecute()
        {
            if (Type == RepeatTypes.RepeatTimes)
            {
                if (mUseTimes >= Times && Times > 0)
                {
                    return ENST.SUCCESS;
                }
                else
                {
                    FirstChild.OnTick();
                    switch (FirstChild.State)
                    {
                        case ENST.SUCCESS:
                        case ENST.FAILURE:
                            FirstChild.OnReset();
                            mUseTimes++;
                            break;
                        case ENST.RUNNING:
                            break;
                    }
                    return ENST.RUNNING;
                }
            }
            else
            {
                FirstChild.OnTick();
                if (Until == RepeatUntil.Success && FirstChild.State == ENST.SUCCESS)
                {
                    return ENST.SUCCESS;
                }
                if (Until == RepeatUntil.Failure && FirstChild.State == ENST.FAILURE)
                {
                    return ENST.SUCCESS;
                }
                return ENST.RUNNING;
            }
        }

        public override void OnReset()
        {
            mUseTimes = 0;
        }

        public override bool AutoDrawNodeInspector
        {
            get { return false; }
        }

#if UNITY_EDITOR
        public override void DrawNodeWindowContents()
        {
            Color tempColor = GUI.color;
            if (Type == RepeatTypes.RepeatTimes)
            {
                {
                    GUI.color = Color.green;
                    GUILayout.Label(Times + " Times");
                }
                if (Application.isPlaying)
                {
                    GUI.color = Color.yellow;
                    GUILayout.Label(mUseTimes + " Times");
                }
            }
            else
            {
                GUI.color = Color.green;
                GUILayout.Label("Until " + Until);
            }
            GUI.color = tempColor;
        }

        public override void DrawNodeInspectorGUI()
        {
            base.DrawNodeInspectorGUI();
            this.Type = (RepeatTypes)UnityEditor.EditorGUILayout.EnumPopup("Type", Type);
            switch(this.Type)
            {
                case RepeatTypes.RepeatTimes:
                    this.Times = UnityEditor.EditorGUILayout.IntField("Times", this.Times);
                    this.Times = this.Times < 1 ? 1 : this.Times;
                    break;
                case RepeatTypes.RepeatUntil:
                    this.Until= (RepeatUntil)UnityEditor.EditorGUILayout.EnumPopup("Until", this.Until);
                    break;
            }
        }
#endif
    }
}