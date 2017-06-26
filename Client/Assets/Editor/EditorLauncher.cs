using UnityEngine;
using UnityEditor;

namespace EDT
{
    [CustomEditor(typeof(GTLauncher), false)]
    public class EditorLauncher : Editor
    {
        public override void OnInspectorGUI()
        {
            base.OnInspectorGUI();
            GTLauncher manager = target as GTLauncher;

            EditorGUILayout.LabelField("当前场景名",   manager.CurrSceneName);
            EditorGUILayout.LabelField("当前游戏状态", manager.CurrSceneType.ToString());
            EditorGUILayout.LabelField("下一游戏状态", manager.NextSceneType.ToString());

            manager.UseGuide     = EditorGUILayout.Toggle("使用新手引导", manager.UseGuide);
            bool musicActive  = EditorGUILayout.Toggle("关闭音乐", manager.MusicDisable);
            if(manager.MusicDisable != musicActive)
            {
                manager.MusicDisable = musicActive;
                PlayerPrefs.SetInt(GTAudioManager.MUSIC_ACTIVE_KEY, manager.MusicDisable ? 1 : 0);
            }
            manager.TestScene    = EditorGUILayout.Toggle("测试", manager.TestScene);
            if(manager.TestScene)
            {
                manager.TestActorID = EditorGUILayout.IntField("测试角色ID", manager.TestActorID);
            }
        }
    }
}