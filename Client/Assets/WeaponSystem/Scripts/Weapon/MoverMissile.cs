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
	public class MoverMissile : WeaponBase
	{
		public float Damping = 3;
		public float Speed = 80;
		public float SpeedMax = 80;
		public float SpeedMult = 1;
		public Vector3 Noise = new Vector3 (20, 20, 20);
		public float TargetLockDirection = 0.5f;
		public int DistanceLock = 70;
		public int DurationLock = 40;
		public bool Seeker;
		public float LifeTime = 5.0f;
		private bool locked;
		private int timetorock;
		private float timeCount = 0;
		private float speedTemp;
	
		private void Awake ()
		{
			speedTemp = Speed;
			objectPool = this.GetComponent<ObjectPool> ();
			if (objectPool && WeaponSystem.Pool != null)
				objectPool.LifeTime = LifeTime;	
		}
	
		private void Start ()
		{
			if (objectPool && WeaponSystem.Pool != null) {
				objectPool.LifeTime = LifeTime;
				objectPool.SetDestroy (LifeTime);
			} else {
				Destroy (gameObject, LifeTime);
			}
			timeCount = Time.time;
		}
	
		public void OnEnable ()
		{
			Speed = speedTemp;
			Target = null;
			timeCount = Time.time;
			if (objectPool) {
				objectPool.LifeTime = LifeTime;
				objectPool.SetDestroy (LifeTime);
			}
		}
	
		private void FixedUpdate ()
		{
			GetComponent<Rigidbody>().velocity = new Vector3 (transform.forward.x * Speed * Time.fixedDeltaTime, transform.forward.y * Speed * Time.fixedDeltaTime, transform.forward.z * Speed * Time.fixedDeltaTime);
			GetComponent<Rigidbody>().velocity += new Vector3 (Random.Range (-Noise.x, Noise.x), Random.Range (-Noise.y, Noise.y), Random.Range (-Noise.z, Noise.z));
		
			if (Speed < SpeedMax) {
				Speed += SpeedMult * Time.fixedDeltaTime;
			}
		}

		private void Update ()
		{
		
			if (WeaponSystem.Pool != null && objectPool != null && !objectPool.Active) 
				return;
		
			if (Time.time >= (timeCount + LifeTime) - 0.5f) {
				if (GetComponent<Damage> ()) {
					GetComponent<Damage> ().Active ();
					timeCount = Time.time;
				}
			}
		
			if (Target) {
				Quaternion rotation = Quaternion.LookRotation (Target.transform.position - transform.transform.position);
				transform.rotation = Quaternion.Slerp (transform.rotation, rotation, Time.deltaTime * Damping);
				Vector3 dir = (Target.transform.position - transform.position).normalized;
				float direction = Vector3.Dot (dir, transform.forward);
				if (direction < TargetLockDirection) {
					Target = null;
				}
			}
		
			if (Seeker) {
				if (timetorock > DurationLock) {
					if (!locked && !Target) {
						float distance = int.MaxValue;
						for (int t=0; t<TargetTag.Length; t++) {
							if (GameObject.FindGameObjectsWithTag (TargetTag [t]).Length > 0) {
								GameObject[] objs = GameObject.FindGameObjectsWithTag (TargetTag [t]);

								for (int i = 0; i < objs.Length; i++) {
									if (objs [i]) {
										Vector3 dir = (objs [i].transform.position - transform.position).normalized;
										float direction = Vector3.Dot (dir, transform.forward);
										float dis = Vector3.Distance (objs [i].transform.position, transform.position);
										if (direction >= TargetLockDirection) {
											if (DistanceLock > dis) {
												if (distance > dis) {
													distance = dis;
													Target = objs [i];
												}
												locked = true;
											}
										}
									}
								}
							}
						}
					}
					timetorock = 0;
				} else {
					timetorock += 1;
				}

				if (Target) {
				
				} else {
					locked = false;

				}
			}
		}
	}
}
