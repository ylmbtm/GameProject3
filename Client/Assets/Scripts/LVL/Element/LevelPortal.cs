using UnityEngine;
using System.Collections;
using CFG;
using BVT.AI;

namespace LVL
{
    [LevelElementVariable(CfgType = typeof(MapPortal))]
    public class LevelPortal : LevelElement
    {
        [SerializeField]
        public int SceneID;

        void OnTriggerEnter(Collider other)
        {
            if (AIHelper.GetAIInterface(other.transform) == CharacterManager.Main)
            {
                GTLauncher.Instance.LoadScene(SceneID);
            }
        }
    }
}

