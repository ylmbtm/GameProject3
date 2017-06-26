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
	public class MoverBullet : WeaponBase
	{
		public int Lifetime;
		public float Speed = 80;
		public float SpeedMax = 80;
		public float SpeedMult = 1;
		private float speedTemp;
	
		private void Awake ()
		{
			speedTemp = Speed;
			objectPool = this.GetComponent<ObjectPool> ();
		}
	
		private void Start ()
		{
			if (objectPool && WeaponSystem.Pool != null) {
				objectPool.SetDestroy (Lifetime);
			} else {
				Destroy (gameObject, Lifetime);
			}
		}
	
		public void OnEnable ()
		{
			Speed = speedTemp;
			if (objectPool && WeaponSystem.Pool != null) {
				objectPool.SetDestroy (Lifetime);
			}
		}

		private void FixedUpdate ()
		{
			if (WeaponSystem.Pool != null && objectPool != null && !objectPool.Active) 
				return;
		
			if (!this.GetComponent<Rigidbody>())
				return;
		
			if (!RigidbodyProjectile) {
				GetComponent<Rigidbody>().velocity = transform.forward * Speed;
			} else {
				if (this.GetComponent<Rigidbody>().velocity.normalized != Vector3.zero)
					this.transform.forward = this.GetComponent<Rigidbody>().velocity.normalized;	
			}
			if (Speed < SpeedMax) {
				Speed += SpeedMult * Time.fixedDeltaTime;
			}
		}
	}
}
