/*
http://www.cgsoso.com/forum-211-1.html

CG搜搜 Unity3d 每日Unity3d插件免费更新 更有VIP资源！

CGSOSO 主打游戏开发，影视设计等CG资源素材。

插件如若商用，请务必官网购买！

daily assets update for try.

U should buy the asset from home store if u use it in your project!
*/

using System;
using UnityEngine;
using System.Collections;
using Random = UnityEngine.Random;

namespace HWRWeaponSystem
{
	[RequireComponent(typeof(AudioSource))]
	public class WeaponLauncher : WeaponBase
	{
		public bool OnActive;
		public bool OnScreenAiming;
		public Transform[] MissileOuter;
		public GameObject Missile;
		public float FireRate = 0.1f;
		public float Spread = 1;
		public float ForceShoot = 8000;
		public int NumBullet = 1;
		public int Ammo = 10;
		public int AmmoMax = 10;
		public bool InfinityAmmo = false;
		public float ReloadTime = 1;
		public bool ShowHUD = true;
		public int MaxAimRange = 10000;
		public bool ShowCrosshair;
		public Texture2D CrosshairTexture;
		public Texture2D TargetLockOnTexture;
		public Texture2D TargetLockedTexture;
		public float DistanceLock = 200;
		public float TimeToLock = 2;
		public float AimDirection = 0.8f;
		public bool Seeker;
		public GameObject Shell;
		public float ShellLifeTime = 4;
		public Transform[] ShellOuter;
		public int ShellOutForce = 300;
		public GameObject Muzzle;
		public float MuzzleLifeTime = 2;
		public AudioClip[] SoundGun;
		public AudioClip SoundReloading;
		public AudioClip SoundReloaded;
		private float timetolockcount = 0;
		private float nextFireTime = 0;
		private GameObject target;
		private Vector3 torqueTemp;
		private float reloadTimeTemp;
		private AudioSource audioSource;
		[HideInInspector]
		public bool Reloading;
		[HideInInspector]
		public float ReloadingProcess;
		public GameObject CrosshairObject;
		private GameObject crosshair;

		private void Start ()
		{
			if (!Owner) 
				Owner = this.transform.root.gameObject;
		
			if (!audioSource) {
				audioSource = this.GetComponent<AudioSource> ();
				if (!audioSource) {
					this.gameObject.AddComponent<AudioSource> ();	
				}
			}
			if (CrosshairObject) {
				crosshair = (GameObject)GameObject.Instantiate (CrosshairObject.gameObject, this.transform.position, CrosshairObject.transform.rotation);	
		
			}
		}

		[HideInInspector]
		public Vector3 AimPoint;
		[HideInInspector]
		public GameObject AimObject;

		private void rayAiming ()
		{
			RaycastHit hit;
			if (OnScreenAiming) {
				if (CurrentCamera) {
					var ray = CurrentCamera.ScreenPointToRay (Input.mousePosition);
					if (Physics.Raycast (ray, out hit, MaxAimRange)) {
						if (Missile != null && hit.collider.tag != Missile.tag) {
							AimPoint = hit.point;
							AimObject = hit.collider.gameObject;
						}
					} else {
						AimPoint = ray.origin + (ray.direction * MaxAimRange);
						AimObject = null;
					}
				}
			} else {

				if (Physics.Raycast (transform.position, this.transform.forward, out hit, MaxAimRange)) {
					if (Missile != null && hit.collider.tag != Missile.tag) {
						AimPoint = hit.point;
						AimObject = hit.collider.gameObject;
					}
				} else {
					AimPoint = this.transform.position + (this.transform.forward * MaxAimRange);
					AimObject = null;
				}
			}
		
		}
	
		void FixedUpdate ()
		{
			if (OnActive) {
				rayAiming ();
			}
		}
	
		private void Update ()
		{
			if (CurrentCamera == null) {
			
				CurrentCamera = Camera.main;
			
				if (CurrentCamera == null)
					CurrentCamera = Camera.current;
			}
			if (OnActive) {
		
			
				if (TorqueObject) {
					TorqueObject.transform.Rotate (torqueTemp * Time.deltaTime);
					torqueTemp = Vector3.Lerp (torqueTemp, Vector3.zero, Time.deltaTime);
				}
				if (Seeker) {
			
					for (int t=0; t<TargetTag.Length; t++) {
						if (GameObject.FindGameObjectsWithTag (TargetTag [t]).Length > 0) {
							GameObject[] objs = GameObject.FindGameObjectsWithTag (TargetTag [t]);
							float distance = int.MaxValue;
						
							if (AimObject != null && AimObject.tag == TargetTag [t]) {
								float dis = Vector3.Distance (AimObject.transform.position, transform.position);
								if (DistanceLock > dis) {
									if (distance > dis) {
										if (timetolockcount + TimeToLock < Time.time) {	
											distance = dis;
											target = AimObject;
										}
									}
								}	
							} else {
								for (int i = 0; i < objs.Length; i++) {
									if (objs [i]) {
										Vector3 dir = (objs [i].transform.position - transform.position).normalized;
										float direction = Vector3.Dot (dir, transform.forward);
										float dis = Vector3.Distance (objs [i].transform.position, transform.position);
										if (direction >= AimDirection) {
											if (DistanceLock > dis) {
												if (distance > dis) {
													if (timetolockcount + TimeToLock < Time.time) {	
														distance = dis;
														target = objs [i];
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
				if (target) {
					float targetdistance = Vector3.Distance (transform.position, target.transform.position);
					Vector3 dir = (target.transform.position - transform.position).normalized;
					float direction = Vector3.Dot (dir, transform.forward);

					if (targetdistance > DistanceLock || direction <= AimDirection) {
						Unlock ();
					}
				}
		
				if (Reloading) {
					ReloadingProcess = ((1 / ReloadTime) * (reloadTimeTemp + ReloadTime - Time.time));
					if (Time.time >= reloadTimeTemp + ReloadTime) {
						Reloading = false;
						if (SoundReloaded) {
							if (audioSource) {
								audioSource.PlayOneShot (SoundReloaded);
							}
						}
						Ammo = AmmoMax;
					}
				} else {
					if (Ammo <= 0) {
						Unlock ();
						Reloading = true;
						reloadTimeTemp = Time.time;
				
						if (SoundReloading) {
							if (audioSource) {
								audioSource.PlayOneShot (SoundReloading);
							}
						}
					}
				}
			}
		}

		public Camera CurrentCamera;

		private void DrawTargetLockon (Transform aimtarget, bool locked)
		{
			if (!ShowHUD)
				return;
		
			if (CurrentCamera) {
			
				
				if (crosshair) {
					crosshair.transform.position = AimPoint;
					crosshair.transform.forward = this.transform.forward;
					//Quaternion lookat = Quaternion.LookRotation((crosshair.transform.position - CurrentCamera.transform.position).normalized);
				}
			
				Vector3 dir = (aimtarget.position - CurrentCamera.transform.position).normalized;
				float direction = Vector3.Dot (dir, CurrentCamera.transform.forward);
				if (direction > 0.5f) {
					Vector3 screenPos = CurrentCamera.WorldToScreenPoint (aimtarget.transform.position);
					float distance = Vector3.Distance (transform.position, aimtarget.transform.position);
					if (locked) {
						if (TargetLockedTexture)
							GUI.DrawTexture (new Rect (screenPos.x - TargetLockedTexture.width / 2, Screen.height - screenPos.y - TargetLockedTexture.height / 2, TargetLockedTexture.width, TargetLockedTexture.height), TargetLockedTexture);
						GUI.Label (new Rect (screenPos.x + 40, Screen.height - screenPos.y, 200, 30), aimtarget.name + " " + Mathf.Floor (distance) + "m.");
					} else {
						if (TargetLockOnTexture)
							GUI.DrawTexture (new Rect (screenPos.x - TargetLockOnTexture.width / 2, Screen.height - screenPos.y - TargetLockOnTexture.height / 2, TargetLockOnTexture.width, TargetLockOnTexture.height), TargetLockOnTexture);
					}
				
            	
				}
			} else {
				//Debug.Log("Can't Find camera");
			}
		}

		private Vector3 crosshairPos;

		private void DrawCrosshair ()
		{
			if (!ShowCrosshair)
				return;
		
		
			if (CurrentCamera) {
			
				Vector3 screenPosAim = CurrentCamera.WorldToScreenPoint (AimPoint);
			
				crosshairPos += ((screenPosAim - crosshairPos) / 5);
				if (CrosshairTexture) {
					GUI.DrawTexture (new Rect (crosshairPos.x - CrosshairTexture.width / 2, Screen.height - crosshairPos.y - CrosshairTexture.height / 2, CrosshairTexture.width, CrosshairTexture.height), CrosshairTexture);
				
				}
			}
		}

		private void OnGUI ()
		{
			if (OnActive) {
				if (Seeker) {
           
					if (target) {
						DrawTargetLockon (target.transform, true);
					}
            
					for (int t=0; t<TargetTag.Length; t++) {
						if (GameObject.FindGameObjectsWithTag (TargetTag [t]).Length > 0) {
							GameObject[] objs = GameObject.FindGameObjectsWithTag (TargetTag [t]);
							for (int i = 0; i < objs.Length; i++) {
								if (objs [i]) {
									Vector3 dir = (objs [i].transform.position - transform.position).normalized;
									float direction = Vector3.Dot (dir, transform.forward);
									if (direction >= AimDirection) {
										float dis = Vector3.Distance (objs [i].transform.position, transform.position);
										if (DistanceLock > dis) {
											DrawTargetLockon (objs [i].transform, false);
										}
									}
								}
							}
						}
					}
				}
				DrawCrosshair ();
			
			}
		
		}

		private void Unlock ()
		{
			timetolockcount = Time.time;
			target = null;
		}
	
		private int currentOuter = 0;

		public void Shoot ()
		{
			if (InfinityAmmo) {
				Ammo = 1;	
			}
			if (Ammo > 0) {
				if (Time.time > nextFireTime + FireRate) {
					nextFireTime = Time.time;
					torqueTemp = TorqueSpeedAxis;
					Ammo -= 1;
					Vector3 missileposition = this.transform.position;
					Quaternion missilerotate = this.transform.rotation;
					if (MissileOuter.Length > 0) {
						missilerotate = MissileOuter [currentOuter].rotation;	
						missileposition = MissileOuter [currentOuter].position;	
					}

					if (MissileOuter.Length > 0) {
						currentOuter += 1;
						if (currentOuter >= MissileOuter.Length)
							currentOuter = 0;
					}
			
					if (Muzzle) {
						GameObject muzzle;
						if (WeaponSystem.Pool != null) {	
							muzzle = WeaponSystem.Pool.Instantiate (Muzzle, missileposition, missilerotate, MuzzleLifeTime);
						} else {
							muzzle = (GameObject)GameObject.Instantiate (Muzzle, missileposition, missilerotate);
							GameObject.Destroy (muzzle, MuzzleLifeTime);
						}

						muzzle.transform.parent = this.transform;
						if (MissileOuter.Length > 0) {
							muzzle.transform.parent = MissileOuter [currentOuter].transform;
						}
					}
			
					for (int i = 0; i < NumBullet; i++) {
						if (Missile) {
							Vector3 spread = new Vector3 (Random.Range (-Spread, Spread), Random.Range (-Spread, Spread), Random.Range (-Spread, Spread)) / 100;
							Vector3 direction = this.transform.forward + spread;
							missilerotate = Quaternion.LookRotation (direction);

							GameObject bullet;
							if (WeaponSystem.Pool != null) {
								bullet = WeaponSystem.Pool.Instantiate (Missile, missileposition, missilerotate);
							} else {
								bullet = (GameObject)GameObject.Instantiate (Missile, missileposition, missilerotate);
							}
							if (bullet) {
								DamageBase damangeBase = bullet.GetComponent<DamageBase> ();
								if (damangeBase) {
									damangeBase.Owner = Owner;
									damangeBase.TargetTag = TargetTag;
									damangeBase.IgnoreTag = IgnoreTag;
									damangeBase.IgnoreSelf(this.gameObject);
								}
								WeaponBase weaponBase = bullet.GetComponent<WeaponBase> ();
								if (weaponBase) {
									weaponBase.Owner = Owner;
									weaponBase.Target = target;
									weaponBase.TargetTag = TargetTag;
									weaponBase.IgnoreTag = IgnoreTag;
								}
								if (RigidbodyProjectile) {
									if (bullet.GetComponent<Rigidbody>()) {
										if (Owner != null && Owner.GetComponent<Rigidbody>()) {
											bullet.GetComponent<Rigidbody>().velocity = Owner.GetComponent<Rigidbody>().velocity;
										}
										bullet.GetComponent<Rigidbody>().AddForce (direction * ForceShoot);	
									}
								}
							}
					
						}
					}
					if (Shell) {
						Transform shelloutpos = this.transform;
						if (ShellOuter.Length > 0) {
							shelloutpos = ShellOuter [currentOuter];
						}
						GameObject shell;
						if (WeaponSystem.Pool != null) {
							shell = WeaponSystem.Pool.Instantiate (Shell, shelloutpos.position, Random.rotation, ShellLifeTime);
						} else {
							shell = (GameObject)Instantiate (Shell, shelloutpos.position, Random.rotation);
							GameObject.Destroy (shell.gameObject, ShellLifeTime);
						}
					
						if (shell.GetComponent<Rigidbody>()) {
							shell.GetComponent<Rigidbody>().AddForce (shelloutpos.forward * ShellOutForce);
						}
					}
				
					if (SoundGun.Length > 0) {
						if (audioSource) {
							audioSource.PlayOneShot (SoundGun [Random.Range (0, SoundGun.Length)]);
						}
					}
			
					nextFireTime += FireRate;
				}
			} 
		
		}
	}
}
