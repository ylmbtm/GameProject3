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
	public class ExplosionObject : MonoBehaviour
	{
		public Vector3 Force;
		public GameObject Prefab;
		public int Num;
		public int Scale = 20;
		public AudioClip[] Sounds;
		public float LifeTimeObject = 2;
		public bool RandomScale;
	
		private void OnEnable ()
		{

			if (Sounds.Length > 0) {
				AudioSource.PlayClipAtPoint (Sounds [Random.Range (0, Sounds.Length)], transform.position);
			}
			if (Prefab) {
				for (int i = 0; i < Num; i++) {
					Vector3 pos = new Vector3 (Random.Range (-10, 10), Random.Range (-10, 20), Random.Range (-10, 10)) / 10f;
				
					GameObject obj;
					if (WeaponSystem.Pool != null) {
						obj = WeaponSystem.Pool.Instantiate (Prefab, transform.position + pos, Random.rotation, LifeTimeObject);
					} else {
						obj = (GameObject)Instantiate (Prefab, transform.position + pos, Random.rotation);
						Destroy (obj, LifeTimeObject);
					}
                	
					float scale = Scale;
					if (RandomScale) {
						scale = Random.Range (1, Scale);
					}

					if (scale > 0)
						obj.transform.localScale = new Vector3 (scale, scale, scale);
					if (obj.GetComponent<Rigidbody>()) {
						Vector3 force = new Vector3 (Random.Range (-Force.x, Force.x), Random.Range (-Force.y, Force.y), Random.Range (-Force.z, Force.z));
						obj.GetComponent<Rigidbody>().AddForce (force);
					}
				}
			}
		}
	
		private void Start ()
		{
		
		}

	}
}
