using UnityEngine;
using System.Collections;
using CFG;
using BVT.AI;

namespace LVL
{
    [LevelElementVariable(CfgType = typeof(MapRegion))]
    public class LevelRegion : LevelElement
    {
        void OnTriggerEnter(Collider other)
        {
            if (AIHelper.GetAIInterface(other.transform) == CharacterManager.Main)
            {
                GTWindowManager.Instance.OpenWindow(EWindowID.UI_MAINBOSSHP);
            }
        }

        bool isFinish = false;

        void Update()
        {
            if (isFinish)
            {
                return;
            }
            if (LevelManager.Instance.IsStart() == false)
            {
                return;
            }
            if (CharacterManager.Boss != null && CharacterManager.Boss.IsDead())
            {
                GTWindowManager.Instance.CloseWindow(EWindowID.UI_MAINBOSSHP);
                LevelData.SceneID = GTLauncher.CurSceneID;
                LevelData.CopyID = 10001;
                LevelData.Chapter = 1;
                LevelData.CopyType = ECopyType.TYPE_EASY;
                LevelData.Star = 3;
                LevelData.Win = true;
                GTWindowManager.Instance.OpenWindow(EWindowID.UI_MAINRESULT);
                isFinish = true;
            }

            if (CharacterManager.Main != null && CharacterManager.Main.IsDead())
            {
                GTWindowManager.Instance.CloseWindow(EWindowID.UI_MAINBOSSHP);
                LevelData.SceneID = GTLauncher.CurSceneID;
                LevelData.CopyID = 10001;
                LevelData.Chapter = 1;
                LevelData.CopyType = ECopyType.TYPE_EASY;
                LevelData.Star = 0;
                LevelData.Win = false;
                GTWindowManager.Instance.OpenWindow(EWindowID.UI_MAINRESULT);
                isFinish = true;
            }
        }
    }
}
