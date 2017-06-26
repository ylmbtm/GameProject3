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
	public class GunHanddle : MonoBehaviour
	{

		public WeaponController weapon;
	
		void Start ()
		{
			weapon = this.transform.GetComponentInChildren (typeof(WeaponController)).GetComponent<WeaponController> ();
		}
	
		void Update ()
		{
			if (Input.GetButton ("Fire1")) {
				if (weapon)
					weapon.LaunchWeapon ();
			}
			if (Input.GetButton ("Fire2")) {
				if (weapon)
					weapon.SwitchWeapon ();
			}
			// You can access all weapon parameters by call GetCurrentWeapon
			// e.g. weapon.GetCurrentWeapon().Ammo.toString()
		}
	}
}
