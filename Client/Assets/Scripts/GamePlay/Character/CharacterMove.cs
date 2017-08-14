using UnityEngine;
using System.Collections;
using System;
using UnityEngine.AI;
using System.Collections.Generic;

public class CharacterMove : ICharacterComponent
{
    private NavMeshPath     mNavMeshPath   = new NavMeshPath();
    private NavMeshAgent    mNavMeshAgent  = null;
    private Transform       mRoot          = null;
    private Vector3         mDestPosition  = Vector3.zero;
    private List<Callback>  mCallbacks     = new List<Callback>();
    private NavMeshObstacle mNavObs        = null;

    public CharacterMove(Transform root)
    {
        mRoot = root;
        mNavMeshAgent = mRoot.gameObject.GET<NavMeshAgent>();
        mNavMeshPath = new NavMeshPath();
        mNavMeshAgent.enabled = false;
        mNavMeshAgent.acceleration = 360;
        mNavMeshAgent.angularSpeed = 360;
        mNavMeshAgent.obstacleAvoidanceType = ObstacleAvoidanceType.HighQualityObstacleAvoidance;

        mNavObs = mRoot.gameObject.GET<NavMeshObstacle>();
        mNavObs.shape = NavMeshObstacleShape.Capsule;
        mNavObs.height = mNavMeshAgent.height;
        mNavObs.radius = mNavMeshAgent.radius * 0.5f;
        mNavObs.center = new Vector3(0, mNavObs.height / 2, 0);
        mNavObs.carving = false;
        mNavObs.carvingMoveThreshold = 0.5f;
        mNavObs.enabled = true;
    }

    public void Execute()
    {
        if (!mNavMeshAgent.enabled)
        {
            return;
        }
        if (!IsReached())
        {
            return;
        }
        for (int i = 0; i < mCallbacks.Count; i++)
        {
            mCallbacks[i].Invoke();
        }
        mCallbacks.Clear();
    }

    public void Release()
    {
        mCallbacks.Clear();
        mNavMeshAgent.enabled = false;
        mNavObs.enabled = false;
    }

    public void AddListener(Callback callback)
    {
        if (callback == null)
        {
            return;
        }
        bool isExist = false;
        for (int i = 0; i < mCallbacks.Count; i++)
        {
            if (mCallbacks[i] == callback)
            {
                isExist = true;
                break;
            }
        }
        if (!isExist)
        {
            mCallbacks.Add(callback);
        }
    }

    public void DelListener(Callback callback)
    {
        mCallbacks.Remove(callback);
    }

    public void SetDestination(Vector3 destPosition)
    {
        mNavObs.enabled = false;
        mNavMeshAgent.enabled = true;
        mDestPosition = destPosition;
        mNavMeshAgent.SetDestination(destPosition);
    }

    public void SetAgentActive(bool active)
    {
        mNavMeshAgent.enabled = active;
        mNavObs.enabled = false;
    }

    public void Stop()
    {
        if (mNavMeshAgent.enabled == true)
        {
            mNavMeshAgent.isStopped = true;
            mNavMeshAgent.enabled = false;
            mNavObs.enabled = true;
        }
    }

    public float Radius
    {
        get { return mNavMeshAgent.radius; }
        set { mNavMeshAgent.radius = value; }
    }

    public float Height
    {
        get { return mNavMeshAgent.height; }
        set { mNavMeshAgent.height= value; }
    }

    public float Speed
    {
        get { return mNavMeshAgent.speed; }
        set { mNavMeshAgent.speed = value; }
    }

    public bool IsReached()
    {
        if (!mNavMeshAgent.enabled)
        {
            return false;
        }
        return mNavMeshAgent.remainingDistance <= 0.2f && mNavMeshAgent.remainingDistance > 0.01f;
    }

    public bool IsCanReachPosition(Vector3 dest)
    {
        Vector3 position = GTTools.NavSamplePosition(dest);
        mNavMeshAgent.enabled = true;
        mNavMeshAgent.CalculatePath(position, mNavMeshPath);
        if (mNavMeshPath.status != NavMeshPathStatus.PathPartial)
        {
            return true;
        }
        return false;
    }
}
