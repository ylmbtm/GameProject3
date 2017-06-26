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
	public class BuyMenu : MonoBehaviour
	{

		public GameObject[] Towers;
		public int[] TowersPrice;
		public int Money = 0;
		public int Score = 0;
		private int indexSelected = -1;

		void Start ()
		{
	
		}
	
		// Update is called once per frame
		void Update ()
		{
			if (Input.GetKey (KeyCode.Escape)) {
				Application.LoadLevel ("Menu");	
			}
		
			if (indexSelected != -1) {
				if (Input.GetMouseButtonDown (0) && GUIUtility.hotControl == 0) {
					Ray ray = Camera.main.ScreenPointToRay (Input.mousePosition);
					RaycastHit hit;
					if (Physics.Raycast (ray, out hit, 100))
						PlaceTower (hit.point);
				}
			
				if (Input.GetMouseButtonDown (1)) {
					indexSelected = -1;	
				}
			}
		}
	
		void OnGUI ()
		{
			if (indexSelected != -1) {
				GUI.Label (new Rect (Input.mousePosition.x, Screen.height - Input.mousePosition.y, 200, 50), Towers [indexSelected].name);
			}
			for (int i=0; i<Towers.Length; i++) {
				if (GUI.Button (new Rect (20, 35 * i + 20, 150, 30), Towers [i].name + " -" + TowersPrice [i] + " $")) {
					if (Money >= TowersPrice [i]) {
						indexSelected = i;
					}
				}
			}
			GUI.skin.label.alignment = TextAnchor.UpperRight;
			GUI.skin.label.fontSize = 25;
			GUI.Label (new Rect (Screen.width - 300, 20, 250, 50), Money + " $");
			GUI.Label (new Rect (Screen.width - 300, 50, 250, 50), Score + " Kills");
		}
	
		public void PlaceTower (Vector3 position)
		{
			if (indexSelected != -1 && Money >= TowersPrice [indexSelected]) {
				Money -= TowersPrice [indexSelected];
			
				GameObject.Instantiate (Towers [indexSelected].gameObject, position, Towers [indexSelected].gameObject.transform.rotation);
				indexSelected = -1;
			}
		}
	}
}
