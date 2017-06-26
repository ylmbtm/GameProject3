using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.Xml;

namespace ACT
{
    public class DctSkillContainer : MonoBehaviour
    {
        [SerializeField]
        public ActSkillSystem Container = new ActSkillSystem();
    }
}

