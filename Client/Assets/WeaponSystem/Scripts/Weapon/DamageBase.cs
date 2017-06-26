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
	public class DamageBase : MonoBehaviour
	{
		public GameObject Effect;
		[HideInInspector]
		public GameObject Owner;
		public int Damage = 20;
		[HideInInspector]
		public ObjectPool objectPool;
		public string[] TargetTag = new string[1]{"Enemy"};
		public string[] IgnoreTag;
	
		public bool DoDamageCheck (GameObject gob)
		{
			for (int i=0; i<IgnoreTag.Length; i++) {
				if (IgnoreTag [i] == gob.tag)
					return false;
			}
			return true;
		}
		
		
		public void IgnoreSelf (GameObject owner)
		{
			if (GetComponent<Collider>() && owner && owner.GetComponent<Collider>()) {
				Physics.IgnoreCollision (GetComponent<Collider>(), owner.GetComponent<Collider>());
				if (Owner.transform.root) {
					foreach (Collider col in Owner.transform.root.GetComponentsInChildren<Collider>()) {
						Physics.IgnoreCollision (GetComponent<Collider>(), col);
					}
				}
			}
		}
	}
}
