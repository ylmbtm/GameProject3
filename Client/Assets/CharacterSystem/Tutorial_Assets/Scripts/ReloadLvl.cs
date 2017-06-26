using UnityEngine;

public class ReloadLvl : MonoBehaviour 
{
	// Update is called once per frame
	void Update () 
	{
		if (Input.GetKeyDown("0"))
		{
			Application.LoadLevel(0);
		}
		else if (Input.GetKeyDown("9"))
		{
			Application.LoadLevel(1);
		}
		else if (Input.GetKeyDown("8"))
		{
			Application.LoadLevel(2);
		}
		else if (Input.GetKeyDown("7"))
		{
			Application.LoadLevel(3);
		}
		else if (Input.GetKeyDown("6"))
		{
			Application.LoadLevel(4);
		}
		else if (Input.GetKeyDown("5"))
		{
			Application.LoadLevel(5);
		}
	}
}
