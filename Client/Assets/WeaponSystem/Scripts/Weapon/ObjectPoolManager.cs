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
using System.Collections.Generic;

namespace HWRWeaponSystem
{
	public class ObjectPoolManager : MonoBehaviour
	{

		private List<ObjectPool> usedObject;

		void Start ()
		{
			usedObject = new List<ObjectPool> ();
		}

		public GameObject Instantiate (GameObject obj, Vector3 position, Quaternion rotation, float lifeTime)
		{
			if (usedObject == null)
				return null;
		
			foreach (ObjectPool bu in usedObject) {
				if (bu != null) {
					if (bu.Prefab.gameObject == obj.gameObject) {
						if (bu.Active == false) {
							bu.OnSpawn (position, obj.transform.localScale, rotation, obj, lifeTime);
							return bu.gameObject;
						}
					}
				}
			}
		
			GameObject newobj = (GameObject)GameObject.Instantiate (obj.gameObject, position, obj.transform.rotation);
			ObjectPool newpoolobj = newobj.GetComponent<ObjectPool> ();

			if (newpoolobj != null) {
				newpoolobj.OnSpawn (position, newobj.transform.localScale, rotation, obj, lifeTime);
				usedObject.Add (newpoolobj);
			}
		
			return newobj;
		}
	
		public GameObject Instantiate (GameObject obj, Vector3 position, Quaternion rotation)
		{
			if (usedObject == null)
				return null;
		
			foreach (ObjectPool bu in usedObject) {
				if (bu != null) {
					if (bu.Prefab.gameObject == obj.gameObject) {
						if (bu.Active == false) {
							bu.OnSpawn (position, obj.transform.localScale, rotation, obj, -1);
							return bu.gameObject;
						}
					}
				}
			}
		
			GameObject newobj = (GameObject)GameObject.Instantiate (obj.gameObject, position, obj.transform.rotation);
			ObjectPool newpoolobj = newobj.GetComponent<ObjectPool> ();

			if (newpoolobj != null) {
				newpoolobj.OnSpawn (position, newobj.transform.localScale, rotation, obj, -1);
				usedObject.Add (newpoolobj);
			}
		
			return newobj;
		}
	}
}
