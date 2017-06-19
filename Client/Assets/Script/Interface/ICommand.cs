using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using LitJson;

public abstract class ICommand
{
    public Delegate         Del   { get; set; }
    public abstract Resp    Do();
}