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
	public class TankHUD : MonoBehaviour
	{

		int currentWeapon;
		WeaponController weaponManager;

		void Start ()
		{
			weaponManager = this.GetComponent<WeaponController> ();
		}
	
		void Update ()
		{
			if (!weaponManager)
				return;
		
			if (Input.GetKey (KeyCode.Escape)) {
				Application.LoadLevel ("Menu");	
			}
			if (Input.GetAxis ("Mouse ScrollWheel") < 0) { // back
				weaponManager.CurrentWeapon -= 1;
				if (weaponManager.CurrentWeapon < 0)
					weaponManager.CurrentWeapon = weaponManager.WeaponLists.Length - 1;
			}
			if (Input.GetAxis ("Mouse ScrollWheel") > 0) { // forward
				weaponManager.CurrentWeapon += 1;
				if (weaponManager.CurrentWeapon >= weaponManager.WeaponLists.Length) {
					weaponManager.CurrentWeapon = 0;
				}
			}
			currentWeapon = weaponManager.CurrentWeapon;
		}
	
		void OnGUI ()
		{
			if (!weaponManager || currentWeapon > weaponManager.WeaponLists.Length)
				return;
		
			GUI.skin.label.fontSize = 15;
			GUI.Label (new Rect (20, 20, 300, 30), "Weapon Index " + currentWeapon);
			GUI.Label (new Rect (20, Screen.height - 95, 300, 30), "Esc back to mainmenu");
			GUI.Label (new Rect (20, Screen.height - 50, 300, 30), "Scroll Mouse to Change weapons");
			GUI.Label (new Rect (20, Screen.height - 70, 300, 30), "W A S D to Move");
			if (!weaponManager.WeaponLists [currentWeapon].InfinityAmmo) {
				GUI.Label (new Rect (20, 70, 300, 50), "Ammo " + weaponManager.WeaponLists [currentWeapon].Ammo + " / " + weaponManager.WeaponLists [currentWeapon].AmmoMax);
			} else {
				GUI.Label (new Rect (20, 70, 300, 50), "Infinity ammo");
			}
			GUI.skin.label.fontSize = 25;
			GUI.Label (new Rect (20, 40, 300, 50), "" + weaponManager.WeaponLists [currentWeapon].name);
		
	
		}
	}
}
