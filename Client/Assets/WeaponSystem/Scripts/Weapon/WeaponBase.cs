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
	public class WeaponBase : MonoBehaviour
	{
		[HideInInspector]
		public GameObject Owner;
		[HideInInspector]
		public GameObject Target;
		[HideInInspector]
		public ObjectPool objectPool;
		public string[] TargetTag = new string[1]{"Enemy"};
		public string[] IgnoreTag;
		public bool RigidbodyProjectile;
		public Vector3 TorqueSpeedAxis;
		public GameObject TorqueObject;
	
		public bool DoDamageCheck (GameObject gob)
		{
			for (int i=0; i<IgnoreTag.Length; i++) {
				if (IgnoreTag [i] == gob.tag)
					return false;
			}
			return true;
		}

		public virtual void OnSpawn ()
		{
		
		}
	}
}
