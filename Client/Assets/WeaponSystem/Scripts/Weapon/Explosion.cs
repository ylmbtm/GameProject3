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
	public class Explosion : MonoBehaviour
	{
		public int Force;
		public int Radius;
		public AudioClip[] Sounds;

		private void Start ()
		{
      		
		}
	
		private void OnEnable ()
		{
			Vector3 explosionPos = transform.position;
			Collider[] colliders = Physics.OverlapSphere (explosionPos, Radius);
			if (Sounds.Length > 0) {
				AudioSource.PlayClipAtPoint (Sounds [Random.Range (0, Sounds.Length)], transform.position);
			}
			foreach (Collider hit in colliders) {
				if (hit.GetComponent<Rigidbody>()) {
					hit.GetComponent<Rigidbody>().AddExplosionForce (Force, explosionPos, Radius, 3.0f);
				}
			}
		}
	}
}
