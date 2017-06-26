using UnityEngine;
using System;
using System.Collections.Generic;

namespace BVT
{
    public class Node : MonoBehaviour, INode
    {
        [SerializeField]
        public int            ID;
        [SerializeField]
        public Node           Parent      = null;
        [SerializeField]
        public List<Node>     Children    = new List<Node>();
        [SerializeField]
        public Rect           NodeRect    = new Rect(300, 100, 140, 50);
        [SerializeField]
        public string         NodeComment = string.Empty;
        [NodeVariable]
        public bool           NeedExpand  = true;

        public ENST           State                 { get; set; }
        public bool           Running               { get; private set; }
        public float          StartupTime           { get; private set; }
        public float          ElapsedTime           { get { return Time.time - StartupTime; } }
        public virtual int    MaxChildCount         { get { return -1; } }
        public virtual bool   CanAsFirst            { get { return true; } }
        public bool           CanAsParent           { get { return MaxChildCount != 0; } }

        public virtual bool   AutoDrawNodeInspector { get { return true; } }
        public NodeTree       Tree                  { get; set; }
        public string         NodeIconName          { get; protected set; }
        public Texture2D      NodeIcon              { get; protected set; }
        public Node           FirstChild            { get { return Children.Count > 0 ? Children[0] : null; } }
        public Transform      Agent                 { get; set; }
        public virtual string Name                  { get { return GetType().Name; } }
    

        void OnEnable()
        {
            if (Tree == null)
            {
                Tree = GetComponentInParent<NodeTree>();
            }
        }

        public virtual void AddChild(Node child)
        {
            if (child == null || child.Parent != null)
            {
                return;
            }
            if (Children.Contains(child) == false)
            {
                Children.Add(child);
                child.Parent = this;
            }
        }

        public virtual void DelChild(Node child)
        {
            if (child == null)
            {
                return;
            }
            child.Parent = null;
            Children.Remove(child);
        }

        public virtual void OnCreate()
        {

        }

        public virtual bool OnEnter()
        {
            this.StartupTime = Time.time;
            return true;
        }

        public virtual ENST OnExecute()
        {
            return ENST.RUNNING;
        }

        public         void OnTick()
        {
            if (State == ENST.INITIAL)
            {
                bool checkCondition = OnEnter();
                this.State = checkCondition ? ENST.RUNNING : ENST.FAILURE;
            }
            if (State == ENST.RUNNING)
            {
                ENST nodeStatus = OnExecute();
                this.Running = nodeStatus == ENST.RUNNING;
                this.State = nodeStatus;
            }
            else
            {
                this.Running = false;
            }
        }

        public virtual void OnReset()
        {
            this.State = ENST.INITIAL;
            for (int i = 0; i < Children.Count; i++)
            {
                Children[i].OnReset();
            }
        }

        public virtual void OnExit(ENST state)
        {
            this.Running = false;
            this.State = state;
            for (int i = 0; i < Children.Count; i++)
            {
                Children[i].OnExit(state);
            }
        }


#if UNITY_EDITOR
        public static Color   SuccessColor = new Color(0.4f, 0.7f, 0.2f);
        public static Color   FailureColor = new Color(1.0f, 0.4f, 0.4f);
        public static Color   RunningColor = Color.yellow;
        public static Color   RestingColor = new Color(0.5f, 0.5f, 0.8f, 0.8f);
        public static Color   MistakeColor = Color.red;
        public static Color   ConnectColor = new Color(0.5f, 0.5f, 0.8f, 0.8f);
        public static Vector2 MinSize      = new Vector2(140, 50);
        public static float   LineSize     = 5;
        public static Port    ClickedPort  = null;

        public void DrawGUI()
        {
            if (Tree == null)
            {
                Tree = GetComponentInParent<NodeTree>();
            }
            if (Tree.First == this)
            {
                GUI.Box(new Rect(NodeRect.x, NodeRect.y - 20, NodeRect.width, 20), "<b>START</b>");
            }
            DrawNodeWindow();
            DrawNodeComments();
            DrawNodeConnections();
            BTUndo.CheckDirty(this);
        }

        void DrawNodeComments()
        {
            if (BTSettings.ShowNodeComment)
            {
                var rect = new Rect();
                var height = new GUIStyle("textArea").CalcHeight(new GUIContent(NodeComment), NodeRect.width);
                rect = new Rect(NodeRect.x, NodeRect.yMax + 5, NodeRect.width, height);
                if (!string.IsNullOrEmpty(NodeComment))
                {
                    GUI.color = new Color(1, 1, 1, 0.6f);
                    GUI.backgroundColor = new Color(1f, 1f, 1f, 0.2f);
                    GUI.Box(rect, NodeComment, "label");
                    GUI.backgroundColor = Color.white;
                    GUI.color = Color.white;
                }
            }
        }

        void DrawNodeConnections()
        {
            Event e = Event.current;

            if (ClickedPort != null && (e.type == EventType.MouseUp || e.type == EventType.Used))
            {
                if (NodeRect.Contains(e.mousePosition))
                {
                    if (ClickedPort.Parent != this)
                    {
                        ClickedPort.Parent.AddChild(this);
                        ClickedPort = null;
                    }
                }
            }

            if (CanAsParent == false)
            {
                return;
            }

            var nodeOutputBox = new Rect(NodeRect.x, NodeRect.yMax - 4, NodeRect.width, 12);
            GUI.Box(nodeOutputBox, "", BTStyle.BVT_SimpleBox);

            int count = NeedExpand ? Children.Count + 1 : Children.Count;
            if (MaxChildCount == -1 || Children.Count < MaxChildCount)
            {
                for (int i = 0; i < count; i++)
                {
                    float x = (NodeRect.width / count) * (i + 0.5f) + NodeRect.xMin;
                    float y = NodeRect.yMax + 6;

                    Rect portRect = new Rect(0, 0, 10, 10);
                    portRect.center = new Vector2(x, y);
                    GUI.Box(portRect, "", BTStyle.BVT_NodePort);

                    if (e.button == 0 && e.type == EventType.MouseDown && portRect.Contains(e.mousePosition))
                    {
                        ClickedPort = new Port(i, this, portRect.center);
                    }
                }
            }

            if (ClickedPort != null && ClickedPort.Parent == this)
            {
                UnityEditor.Handles.DrawBezier(ClickedPort.Pos, e.mousePosition, ClickedPort.Pos, e.mousePosition, new Color(0.5f, 0.5f, 0.8f, 0.8f), null, 2);
            }

            for (int i = 0; i < Children.Count; i++)
            {
                Node child = Children[i];
                float x = (NodeRect.width / count) * (i + 0.5f) + NodeRect.xMin;
                float y = NodeRect.yMax + 6;
                var srcPos = new Vector2(x, y);
                var tarPos = child.NodeRect.center + new Vector2(0, -child.NodeRect.height / 2 - 5);
                DrawNodeConnect(srcPos, tarPos, this, child, i);
            }
        }

        void DrawNodeConnect(Vector3 lineFrom, Vector3 lineTo, Node sourceNode, Node target, int index)
        {
            float mlt = 0;
            switch(BTSettings.NodeConnMode)
            {
                case ENodeConnMode.Bezier:
                    mlt = 0.8f;
                    break;
                case ENodeConnMode.Manhattan:
                    mlt = 1;
                    break;
                case ENodeConnMode.Straight:
                    mlt = 0;
                    break;
            }
            float tangentX = Mathf.Abs(lineFrom.x - lineTo.x) * mlt;
            float tangentY = Mathf.Abs(lineFrom.y - lineTo.y) * mlt;
            int hor = 0;

            GUI.color = ConnectColor;
            Rect arrowRect = new Rect(0, 0, 20, 20);
            arrowRect.center = lineTo;

            Vector3 lineFromTangent = Vector3.zero;
            Vector3 lineToTangent = Vector3.zero;

            if (lineFrom.x <= sourceNode.NodeRect.x)
            {
                lineFromTangent = new Vector3(-tangentX, 0, 0);
                hor--;
            }
            if (lineFrom.x >= sourceNode.NodeRect.xMax)
            {
                lineFromTangent = new Vector3(tangentX, 0, 0);
                hor++;
            }
            if (lineFrom.y <= sourceNode.NodeRect.y)
                lineFromTangent = new Vector3(0, -tangentY, 0);

            if (lineFrom.y >= sourceNode.NodeRect.yMax)
                lineFromTangent = new Vector3(0, tangentY, 0);


            if (lineTo.x <= target.NodeRect.x)
            {
                lineToTangent = new Vector3(-tangentX, 0, 0);
                hor--;
                GUI.Box(arrowRect, "", BTStyle.BVT_Arrow_Left);
            }

            if (lineTo.x >= target.NodeRect.xMax)
            {
                lineToTangent = new Vector3(tangentX, 0, 0);
                hor++;
                GUI.Box(arrowRect, "", BTStyle.BVT_Arrow_Right);
            }

            if (lineTo.y <= target.NodeRect.y)
            {
                lineToTangent = new Vector3(0, -tangentY, 0);
                GUI.Box(arrowRect, "", BTStyle.BVT_Arrow_Top);
            }

            if (lineTo.y >= target.NodeRect.yMax)
            {
                lineToTangent = new Vector3(0, tangentY, 0);
                GUI.Box(arrowRect, "", BTStyle.BVT_Arrow_Bottom);
            }
            GUI.color = Color.white;

            switch (target.State)
            {
                case ENST.FAILURE:
                    ConnectColor = FailureColor;
                    break;
                case ENST.SUCCESS:
                    ConnectColor = SuccessColor;
                    break;
                case ENST.INITIAL:
                    ConnectColor = RestingColor;
                    break;
                case ENST.RUNNING:
                    ConnectColor = RunningColor;
                    break;
            }

            float lineSize = 4;
            UnityEditor.Handles.color = ConnectColor;
            switch (BTSettings.NodeConnMode)
            {
                case ENodeConnMode.Bezier:
                    {
                        var shadow = new Vector3(3.5f, 3.5f, 0);
                        UnityEditor.Handles.DrawBezier(lineFrom, lineTo + shadow, lineFrom + shadow + lineFromTangent + shadow, lineTo + shadow + lineToTangent, new Color(0, 0, 0, 0.1f), null, lineSize + 10f);
                        UnityEditor.Handles.DrawBezier(lineFrom, lineTo, lineFrom + lineFromTangent, lineTo + lineToTangent, ConnectColor, null, lineSize);
                    }                 
                    break;
                case ENodeConnMode.Manhattan:
                    {
                        var shadow = new Vector3(1, 1, 0);
                        UnityEditor.Handles.DrawPolyLine(lineFrom, lineFrom + lineFromTangent * (hor == 0 ? 0.5f : 1), lineTo + lineToTangent * (hor == 0 ? 0.5f : 1), lineTo);
                        UnityEditor.Handles.DrawPolyLine(lineFrom + shadow, (lineFrom + lineFromTangent * (hor == 0 ? 0.5f : 1)) + shadow, (lineTo + lineToTangent * (hor == 0 ? 0.5f : 1)) + shadow, lineTo + shadow);
                    }                
                    break;
                case ENodeConnMode.Straight:
                    {
                        UnityEditor.Handles.DrawBezier(lineFrom, lineTo, lineFrom, lineTo, ConnectColor, null, lineSize);
                    }
                    break;
            }
            UnityEditor.Handles.color = Color.white;
            string connectStr = DrawConnectionForChild(target, index);
            if (!string.IsNullOrEmpty(connectStr))
            {
                float t = 0.5f;
                float u = 1.0f - t;
                float tt = t * t;
                float uu = u * u;
                float uuu = uu * u;
                float ttt = tt * t;
                Vector3 result = uuu * lineFrom;
                result += 3 * uu * t * (lineFrom + lineFromTangent);
                result += 3 * u * tt * (lineTo + lineToTangent);
                result += ttt * lineTo;
                Vector2 c = (Vector2)result;

                Vector2 finalSize = new GUIStyle("Box").CalcSize(new GUIContent(connectStr));
                Rect area = new Rect();
                area.center = result;
                area.size = finalSize;
                GUI.Box(area, string.Format("<size=12>{0}</size>", connectStr));
            }
        }

        void DrawNodeWindow()
        {
            GUI.color = (Tree.FocusedNode == this) ? new Color(0.9f, 0.9f, 1) : Color.white;

            GUI.Box(NodeRect, "", BTStyle.BVT_WindowShadow);
            NodeRect = GUILayout.Window(ID, NodeRect, DrawNodeWindowGUI, string.Empty, BTStyle.BVT_Window);
            GUI.Box(NodeRect, "", BTStyle.BVT_WindowShadow);
            GUI.color = new Color(1, 1, 1, 0.5f);
            GUI.Box(new Rect(NodeRect.x + 6, NodeRect.y + 6, NodeRect.width, NodeRect.height), "", BTStyle.BVT_WindowShadow);

            if (Application.isPlaying)
            {
                switch (State)
                {
                    case ENST.SUCCESS:
                        GUI.color = SuccessColor;
                        break;
                    case ENST.RUNNING:
                        GUI.color = RunningColor;
                        break;
                    case ENST.FAILURE:
                        GUI.color = FailureColor;
                        break;
                    case ENST.INITIAL:
                        GUI.color = RestingColor;
                        break;
                }
                GUI.Box(NodeRect, "", BTStyle.BVT_WindowHighlight);
            }
            else
            {
                if (Tree.FocusedNode == this)
                {
                    GUI.color = new Color(0.5f, 0.5f, 0.8f, 0.8f);
                    GUI.Box(NodeRect, "", BTStyle.BVT_WindowHighlight);
                }
            }
            GUI.color = Color.white;
        }

        void DrawNodeWindowGUI(int id)
        {
            Event e = Event.current;
            HandNodeWindowEvents(e);
            DrawNodeWindowName();
            DrawNodeWindowStatus();
            DrawNodeWindowContentStyle();
            DrawNodeWindowContents();
            HandNodeWindowContextMenu(e);
            GUI.DragWindow();
        }

        void DrawNodeWindowName()
        {
            NodeNameAttribute attr = BTHelper.GetAttribute<NodeNameAttribute>(this.GetType());
            string nodeName = attr == null ? string.Format("<b>{0}</b>", Name) : attr.Name;
            GUILayout.Label(nodeName, BTStyle.STYLE_CENTERLABEL);
        }

        void DrawNodeWindowStatus()
        {
            Rect markRect = new Rect(5, 5, 15, 15);
            switch (State)
            {
                case ENST.SUCCESS:
                    GUI.color = SuccessColor;
                    GUI.Box(markRect, "", BTStyle.BVT_Checkmark);
                    break;
                case ENST.FAILURE:
                    GUI.color = FailureColor;
                    GUI.Box(markRect, "", BTStyle.BVT_XMark);
                    break;
                case ENST.RUNNING:
                    GUI.color = RunningColor;
                    GUI.Box(markRect, "", BTStyle.BVT_ClockMark);
                    break;
            }
        }

        void DrawNodeWindowContentStyle()
        {
            GUI.color = Color.white;
            GUI.skin = null;
            GUI.skin.label.richText = true;
            GUI.skin.label.alignment = TextAnchor.MiddleCenter;
        }

        void HandNodeWindowEvents(Event e)
        {
            if (e.button == 0 && e.type == EventType.MouseDown)
            {
                Tree.FocusedNode = this;
            }
            if (e.button == 0 && e.type == EventType.MouseDrag && e.control)
            {
                PanNode(e.delta);
            }
            if (Tree.FocusedNode == this && e.keyCode == KeyCode.Delete && e.type == EventType.KeyDown)
            {
                Tree.PostGUI += () => { Tree.DelGraph(this); };
                e.Use();
            }
            if (e.isKey && e.control && e.keyCode == KeyCode.D && Tree.FocusedGraph != null)
            {
                Tree.PostGUI += () => { Tree.FocusedNode = Tree.FocusedGraph.Duplicate(); };
                e.Use();
            }
        }

        void HandNodeWindowContextMenu(Event e)
        {
            if (e.button == 1 && e.type == EventType.MouseDown)
            {
                UnityEditor.GenericMenu menu = new UnityEditor.GenericMenu();
                if (Tree.First != this && CanAsFirst)
                {
                    menu.AddItem(new GUIContent("Make First"), false, ContextMakeFirst);
                }
                if (this.Parent != null)
                {
                    menu.AddItem(new GUIContent("Make UnParent"), false, ContextMakeUnParent);
                }

                menu.AddItem(new GUIContent("Duplicate"), false, ContextDuplicate);
                menu.AddItem(new GUIContent("Delete"), false, ContextDelete);
                menu.ShowAsContext();
                e.Use();
            }
        }

        void ContextMakeFirst()
        {
            Tree.First = this;
        }

        void ContextDuplicate()
        {
            Tree.PostGUI += () => { Duplicate(); };
        }

        void ContextDelete()
        {
            Tree.PostGUI += () => { Tree.DelGraph(this); };
        }

        void ContextMakeUnParent()
        {
            Tree.PostGUI += () => { this.Parent.DelChild(this); };
        }

        void OnValidate()
        {
            this.hideFlags = HideFlags.HideInHierarchy;
        }

        public virtual void DrawNodeWindowContents()
        {
            GUILayout.Label(string.Empty, GUILayout.Height(1));
        }

        public virtual void DrawNodeInspectorGUI()
        {
            NodeComment = UnityEditor.EditorGUILayout.TextField("NodeComment", NodeComment);
            BTHelper.DrawSeparator();
            if (!AutoDrawNodeInspector)
            {
                return;
            }
            System.Reflection.FieldInfo[] fields = GetType().GetFields();
            for (int i = 0; i < fields.Length; i++)
            {
                System.Reflection.FieldInfo field = fields[i];
                if (field.IsDefined(typeof(NodeVariable), false))
                {
                    object v = field.GetValue(this);
                    Type t = field.FieldType;

                    if (t.BaseType == typeof(Enum))
                    {
                        v = UnityEditor.EditorGUILayout.EnumPopup(field.Name, (Enum)v);
                    }
                    else if (t == typeof(int))
                    {
                        v = UnityEditor.EditorGUILayout.IntField(field.Name, (int)v);
                    }
                    else if (t == typeof(bool))
                    {
                        v = UnityEditor.EditorGUILayout.Toggle(field.Name, (bool)v);
                    }
                    else if (t == typeof(float))
                    {
                        v = UnityEditor.EditorGUILayout.FloatField(field.Name, (float)v);
                    }
                    else if (t == typeof(string))
                    {
                        v = UnityEditor.EditorGUILayout.TextField(field.Name, (string)v);
                    }
                    else if (t == typeof(UnityEngine.Object))
                    {
                        v = UnityEditor.EditorGUILayout.ObjectField(field.Name, (UnityEngine.Object)v, t, false);
                    }
                    else if (t == typeof(double))
                    {
                        v = UnityEditor.EditorGUILayout.DoubleField(field.Name, (double)v);
                    }
                    else if (t == typeof(Vector3))
                    {
                        v = UnityEditor.EditorGUILayout.Vector3Field(field.Name, (Vector3)v);
                    }
                    else if (t == typeof(Vector2))
                    {
                        v = UnityEditor.EditorGUILayout.Vector2Field(field.Name, (Vector2)v);
                    }
                    else if (t == typeof(AnimationCurve))
                    {
                        v = UnityEditor.EditorGUILayout.CurveField(field.Name, (AnimationCurve)v);
                    }
                    else if (t == typeof(Color))
                    {
                        v = UnityEditor.EditorGUILayout.ColorField(field.Name, (Color)v);
                    }
                    try
                    {
                        field.SetValue(this, v);
                    }
                    catch (Exception ex)
                    {
                        Debug.LogError(ex.ToString());
                    }
                }
            }
        }

        public virtual string DrawConnectionForChild(Node node, int index)
        {
            return string.Empty;
        }

        public void PanNode(Vector2 delta)
        {
            float newX = NodeRect.center.x;
            float newY = NodeRect.center.y;
            newX += delta.x;
            newY += delta.y;
            NodeRect.center = new Vector2(newX, newY);
        }

        public Node Duplicate()
        {
            var child = GameObject.Instantiate(gameObject).GetComponent<Node>();
            child.name = this.GetType().ToString();
            child.Children.Clear();
            child.Parent = null;
            Tree.AddGraphToTree(child);
            child.NodeRect.center += new Vector2(50, 50);
            return child;
        }
#endif
    }
}