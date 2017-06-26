using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActHelper
    {
        public static void RunChildrenInNewTree(ActItem src, Callback<ActTree> initTreeCallback)
        {
            ActTree tree = new ActTree();
            for (int i = 0; i < src.Children.Count; i++)
            {
                ActItem child = ActItem.Clone(src.Children[i]);
                tree.AddChild(child);
            }
            tree.Skill = src.Skill;
            if (initTreeCallback != null)
            {
                initTreeCallback.Invoke(tree);
            }
            GTWorld.Instance.Act.Run(tree);
        }
    }
}

