using UnityEngine;
using System.Collections;
using BVT;
using System.Collections.Generic;

namespace BVT
{
    public class NodeTree : MonoBehaviour
    {
        [SerializeField]
        public int               ID;
        [SerializeField]
        public string            Name       = string.Empty;
        [SerializeField]
        public string            Desc       = string.Empty;
        [SerializeField]
        public bool              Loop       = false;
        [SerializeField]
        public Node              First;
        [SerializeField]
        public List<Node>        AllNodes   = new List<Node>();

        public bool              IsRunning { get; private set; }
        public bool              IsPause   { get; private set; }
        public System.Action     PostGUI   { get; set; }

        private Object           mCurrentSelection = null;
        private NodeBlackboard   mBlackboard       = null;
        private Transform        mNodesRoot        = null;

        public Node              AddGraph(System.Type type)
        {
            Node child = NodeFactory.CreateNode(this, type);
            child.hideFlags = HideFlags.HideInHierarchy;
            AddGraphToTree(child);
            return child;
        }

        public Node              AddGraphToTree(Node child)
        {
            child.transform.parent = NodesRoot;
            child.transform.localPosition = Vector3.zero;
            child.transform.localScale = Vector3.one;
            child.transform.localEulerAngles = Vector3.zero;
            child.Tree = this;
            AllNodes.Add(child);
            if (child.CanAsFirst)
            {
                this.First = First == null ? child : this.First;
            }
            this.UpdateGraphIDs();
            child.OnCreate();
            return child;
        }

        public Node              DelGraph(Node graph)
        {
            if (graph.Parent != null)
            {
                graph.Parent.DelChild(graph);
            }
            if (graph.Children.Count > 0)
            {
                for (int i = 0; i < graph.Children.Count; i++)
                {
                    graph.Children[i].Parent = null;
                }
            }
            this.AllNodes.Remove(graph);
            this.First = First == graph ? null : First;
            this.UpdateFirst();
            this.UpdateGraphIDs();
            DestroyImmediate(graph.gameObject);
            return null;
        }

        public Object            FocusedNode
        {
            get { return mCurrentSelection; }
            set
            {
                GUIUtility.keyboardControl = 0;
                mCurrentSelection = value;
            }
        }

        public Node              FocusedGraph
        {
            get
            {
                if (FocusedNode is Node)
                    return FocusedNode as Node;
                else
                    return null;
            }
        }

        public NodeBlackboard    Blackboard
        {
            get
            {
                if (mBlackboard == null)
                {
                    mBlackboard = gameObject.GetComponent<NodeBlackboard>();
                }
                if (mBlackboard == null)
                {
                    mBlackboard = gameObject.AddComponent<NodeBlackboard>();
                    mBlackboard.hideFlags = HideFlags.HideInInspector;
                }
                return mBlackboard;
            }
        }

        public Transform         NodesRoot
        {
            get
            {
                if (mNodesRoot == null)
                {
                    mNodesRoot = transform.Find("Nodes");
                }
                if (mNodesRoot == null)
                {
                    mNodesRoot = new GameObject("Nodes").transform;
                    mNodesRoot.transform.parent = transform;
                    mNodesRoot.transform.localPosition = Vector3.zero;
                }
                mNodesRoot.hideFlags = HideFlags.HideInHierarchy;
                return mNodesRoot;
            }
        }

        public void              UpdateGraphIDs()
        {
            int minID = 1;
            if (First != null)
            {
                First.ID = minID;
                First.name = minID.ToString();
            }
            foreach (var current in AllNodes)
            {
                if (current != First)
                {
                    minID++;
                    current.ID = minID;
                    current.name = minID.ToString();
                }
            }    
        }

        public void              UpdateFirst()
        {
            if (this.First != null)
            {
                return;
            }
            for (int i = 0; i < AllNodes.Count; i++)
            {
                if(AllNodes[i].CanAsFirst)
                {
                    this.First = AllNodes[i];
                    break;
                }
            }
        }

        public void              SetAgent(Transform agent)
        {
            for (int i = 0; i < AllNodes.Count; i++)
            {
                AllNodes[i].Agent = agent;
            }
        }

        public void              Release()
        {
            if (First != null)
            {
                First.OnReset();
            }
        }

        void Awake()
        {
            UpdateFirst();
        }

        void Update()
        {
            if (First == null)
            {
                return;
            }
            First.OnTick();
            if (Loop && (First.State == ENST.SUCCESS || First.State == ENST.FAILURE))
            {
                First.OnReset();
            }
        }
    }
}
