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
	public class RayShoot : DamageBase
	{
		public int Range = 10000;
		public Vector3 AimPoint;
		public GameObject Explosion;
		public float LifeTime = 1;
		public LineRenderer Trail;
		private ObjectPool objPool;
		private bool actived = false;
	
		private void Awake ()
		{
			objPool = this.GetComponent<ObjectPool> ();	
		}
	
		void Start ()
		{
			ShootRay ();
			actived = true;
		}
	
		private void OnEnable ()
		{
			if (objPool)
				objPool.SetDestroy (LifeTime);
		
			if (actived)
				ShootRay ();
		}
	
		void ShootRay ()
		{
			if (GetComponent<Collider>()) {
				Physics.IgnoreCollision (GetComponent<Collider>(), Owner.GetComponent<Collider>());
				if (Owner.transform.root) {
					foreach (Collider col in Owner.transform.root.GetComponentsInChildren<Collider>()) {
						Physics.IgnoreCollision (GetComponent<Collider>(), col);
					}
				}
			}
		
			RaycastHit hit;
			GameObject explosion = null;
			if (Physics.Raycast (this.transform.position, this.transform.forward, out hit, Range)) {
				AimPoint = hit.point;
				if (Explosion != null) {
					if (WeaponSystem.Pool != null) {
						explosion = WeaponSystem.Pool.Instantiate (Explosion, AimPoint, this.transform.rotation);
					} else {
						explosion = (GameObject)GameObject.Instantiate (Explosion, AimPoint, this.transform.rotation);
					}
				}
				hit.collider.gameObject.SendMessage ("ApplyDamage", Damage, SendMessageOptions.DontRequireReceiver);
			} else {
				AimPoint = this.transform.position + this.transform.forward * Range;
				if (Explosion != null) {
					if (WeaponSystem.Pool != null) {
						explosion = WeaponSystem.Pool.Instantiate (Explosion, AimPoint, this.transform.rotation);
					} else {
						explosion = (GameObject)GameObject.Instantiate (Explosion, AimPoint, this.transform.rotation);
					}
				}
			}
			if (explosion) {
				explosion.transform.forward = this.transform.forward;
				DamageBase dmg = explosion.GetComponent<DamageBase> ();
				if (dmg) {
					dmg.TargetTag = TargetTag;	
				}
			}
			if (Trail) {
				Trail.SetPosition (0, this.transform.position);
				Trail.SetPosition (1, AimPoint);
			
			}
		
			if (WeaponSystem.Pool == null) {
				Destroy (this.gameObject, LifeTime);
			}
		}
	
		void Update ()
		{
		
		}
	}
	
}
