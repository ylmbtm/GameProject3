// ----------------------------------------------------------------------------------
//
// FXMaker
// Created by ismoon - 2012 - ismoonto@gmail.com
//
// ----------------------------------------------------------------------------------

// Attribute ------------------------------------------------------------------------
// Property -------------------------------------------------------------------------
// Loop Function --------------------------------------------------------------------
// Control Function -----------------------------------------------------------------
// Event Function -------------------------------------------------------------------

using UnityEngine;
using System.Collections;

public class NcDontActive : NcEffectBehaviour
{
	// --------------------------------------------------------------------------
	// --------------------------------------------------------------------------
	void Awake()
	{
#if UNITY_EDITOR
		if (IsCreatingEditObject() == false)
#endif
		{
#if (UNITY_4_0 || UNITY_4_1 || UNITY_4_2 || UNITY_4_3 || UNITY_4_4 || UNITY_4_5 || UNITY_4_6 || UNITY_4_7 || UNITY_4_8 || UNITY_4_9)
			Object.Destroy(gameObject);
#else
            gameObject.SetActive(false);
#endif
        }
	}

	void OnEnable()
	{
#if (UNITY_4_0 || UNITY_4_1 || UNITY_4_2 || UNITY_4_3 || UNITY_4_4 || UNITY_4_5 || UNITY_4_6 || UNITY_4_7 || UNITY_4_8 || UNITY_4_9)
#else
        gameObject.SetActive(false);
#endif
	}

	// --------------------------------------------------------------------------
}
