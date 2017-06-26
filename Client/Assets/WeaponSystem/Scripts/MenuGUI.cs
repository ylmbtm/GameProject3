/*
http://www.cgsoso.com/forum-211-1.html

CG搜搜 Unity3d 每日Unity3d插件免费更新 更有VIP资源！

CGSOSO 主打游戏开发，影视设计等CG资源素材。

插件如若商用，请务必官网购买！

daily assets update for try.

U should buy the asset from home store if u use it in your project!
*/

using UnityEngine;
using System.Collections;

namespace HWRWeaponSystem
{
	public class MenuGUI : MonoBehaviour
	{

		public Texture2D logo;
		public GUISkin skin;

		void Start ()
		{
	
		}

		void Update ()
		{
			MouseLock.MouseLocked = false;
		}
	
		void OnGUI ()
		{
			if (skin)
				GUI.skin = skin;
		
			GUI.skin.button.fontSize = 18;
			GUI.DrawTexture (new Rect (Screen.width / 2 - logo.width / 2, 10, logo.width, logo.height), logo);
			if (GUI.Button (new Rect (Screen.width / 2 - 400, 300, 200, 30), "Demo 1"))
				Application.LoadLevel ("Demo1");
			if (GUI.Button (new Rect (Screen.width / 2 - 400, 340, 200, 30), "Demo 2"))	
				Application.LoadLevel ("Demo2");
			if (GUI.Button (new Rect (Screen.width / 2 - 400, 380, 200, 30), "3D First Person"))	
				Application.LoadLevel ("Demo3");
			if (GUI.Button (new Rect (Screen.width / 2 - 400, 420, 200, 30), "2D Side Scrolling"))
				Application.LoadLevel ("Demo2D");
			if (GUI.Button (new Rect (Screen.width / 2 - 400, 460, 200, 30), "Tower Defend"))
				Application.LoadLevel ("TD");
		
			if (GUI.Button (new Rect (Screen.width - 320, Screen.height - 50, 300, 30), "Get this project"))
				Application.OpenURL ("https://www.assetstore.unity3d.com/#/content/7676");

		
		
		}
	}
}
