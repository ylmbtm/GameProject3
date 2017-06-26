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
	public static class MouseLock
	{
		private static bool mouseLocked;
	
		public static bool MouseLocked {
			get {
				return mouseLocked;
			}
			set {
				mouseLocked = value;
			
			#if UNITY_4_6
				Screen.lockCursor = mouseLocked;
			#else
				Cursor.visible = !value;
				if (Cursor.visible) {	
					Cursor.lockState = CursorLockMode.None;
				} else {
					Cursor.lockState = CursorLockMode.Locked;
			
				}
			#endif
			}
		}
	}
}

