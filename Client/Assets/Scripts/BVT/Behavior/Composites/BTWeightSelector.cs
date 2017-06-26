using BVT;
using BVT;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;
using UnityEngine.Scripting;

namespace BVT
{
    [NodeAttribute(Type = "组合节点", Label = "加权随机-选择节点")]
    [NodeDesc("提供每次执行不同的First True Child Node的可能")]
    [NodeName("b3ff7f", "WeightSelector")]
    public class BTWeightSelector : BTComposite
    {
        [NodeVariable]
        public List<float> ChildWeights     = new List<float>();

        private float mProbability;
        private float mCurProbability;
        private float mTotal;

        public override void AddChild(Node child)
        {
            ChildWeights.Add(1);
            this.CalcTotal();
            base.AddChild(child);
        }

        public override void DelChild(Node child)
        {
            for (int i = 0; i < Children.Count; i++)
            {
                if (Children[i] == child)
                {
                    ChildWeights.RemoveAt(i);
                    break;
                }
            }
            this.CalcTotal();
            base.DelChild(child);
        }

        public override bool OnEnter()
        {
            CalcTotal();
            mProbability = UnityEngine.Random.Range(0, mTotal);
            return base.OnEnter();
        }

        public override ENST OnExecute()
        {
            this.mCurProbability = mProbability;
            for (int i = 0; i < Children.Count; i++)
            {
                if (mCurProbability >= ChildWeights[i])
                {
                    mCurProbability -= ChildWeights[i];
                    continue;
                }
                Node child = Children[i];
                if (child.State == ENST.RUNNING)
                {
                    return ENST.RUNNING;
                }
                if (child.State == ENST.SUCCESS)
                {
                    return ENST.SUCCESS;
                }
            }
            return ENST.FAILURE;
        }

        public          void CalcTotal()
        {
            mTotal = 0.001f;
            for (int i = 0; i < ChildWeights.Count; i++)
            {
                mTotal += ChildWeights[i];
            }
        }

#if UNITY_EDITOR
        public override bool AutoDrawNodeInspector
        {
            get { return false; }
        }

        public override void DrawNodeWindowContents()
        {
            if (Children.Count == 0)
            {
                GUILayout.Label("No Children");
                return;
            }
            CalcTotal();
            if (mTotal == 0)
            {
                GUILayout.Label("100% Failure");
                return;
            }
            string weigthString = string.Empty;
            for (int i = 0; i < ChildWeights.Count; i++)
            {
                float per = Mathf.Round((ChildWeights[i] / mTotal) * 100);
                weigthString += per + "%" + ((i == Children.Count - 1) ? " " : ", ");
            }
            GUILayout.Label(weigthString);
        }

        public override void DrawNodeInspectorGUI()
        {
            base.DrawNodeInspectorGUI();
            if (Children.Count == 0)
            {
                GUILayout.Label("Make children first");
                return;
            }
            CalcTotal();
            if (mTotal == 0)
            {
                GUILayout.Label("100% Failure");
                return;
            }
            for (int i = 0; i < ChildWeights.Count; i++)
            {
                GUILayout.BeginHorizontal();
                ChildWeights[i] = UnityEditor.EditorGUILayout.Slider("Weight", ChildWeights[i], 0, 1);
                float v = Mathf.Round((ChildWeights[i] / mTotal) * 100);
                GUILayout.Label(v + "%", GUILayout.Width(40));
                ChildWeights[i]= Mathf.Floor(ChildWeights[i] * 10 + 0.1f) / 10;
                GUILayout.EndHorizontal();
            }

            GUILayout.Space(5);
        }
#endif
    }
}