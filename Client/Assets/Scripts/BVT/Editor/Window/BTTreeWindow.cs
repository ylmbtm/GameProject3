using BVT;
using System;
using UnityEditor;
using UnityEngine;

namespace BVT
{
    public class BTTreeWindow : EditorWindow
    {
        private GUIStyle     mToolbarStyle;
        private GUIStyle     mToolbarTextFieldStyle;
        private GUIStyle     mToolbarButtonStyle;
        private GUIStyle     mTextCenterStyle;
        private GUISkin      mBVTSkin;
        private Rect         mBlackboardRect      = new Rect(15, 55, 0, 0);
        private Rect         mInspectorRect       = new Rect(15, 55, 0, 0);
        private NodeTree     mTree                = null;
        private int          mTreeInstanceID      = 0;

        private Vector2      mTranslation;
        private float        mZoomFactor = 1;

        private Rect         canvasRect = new Rect(0, 0, 2000, 2000);
        private Rect         viewRect;
        private Rect         nodeBounds; 
        private Rect         totalCanvas;
        private Rect         zoomRecoveryRect;

        private Matrix4x4    oldMatrix;
        private Vector2      scrollPos = Vector2.zero;
        private bool         mouseButton2Down = false;
        private Action       OnDoPopup;

        private Vector2?     smoothPan = null;
        private float?       smoothZoomFactor = null;
        private Vector2      panVelocity = Vector2.one;
        private float        zoomVelocity = 1;

        private readonly static float   topMargin      = 20;
        private readonly static float   unityTabHeight = 22;
        private readonly static int     gridSize       = 15;
        private readonly static Vector2 virtualCenterOffset = new Vector2(-5000, 5000);
        private readonly static Vector2 zoomPoint = new Vector2(0, 20);


        private Vector2      Pan
        {
            get { return BVTTree != null ? Vector2.Min(mTranslation, Vector2.zero) : VirtualCenter; }
            set
            {
                if (BVTTree != null)
                {
                    var t = mTranslation;
                    t = Vector2.Min(value, Vector2.zero);
                    if (smoothPan == null)
                    {
                        t.x = Mathf.Round(t.x); 
                        t.y = Mathf.Round(t.y); 
                    }
                    mTranslation = t;
                }
            }
        }

        private float        ZoomFactor
        {
            get
            {
                mZoomFactor = Mathf.Clamp(1, 0.25f, 1f);
                return mZoomFactor;
            }
            set
            {
                mZoomFactor = Mathf.Clamp(value, 0.25f, 1f);
            }
        }

        private Vector2      VirtualPanPosition
        {
            get { return (Pan - virtualCenterOffset) * -1; }
        }

        private Vector2      VirtualCenter
        {
            get { return -virtualCenterOffset + viewRect.size / 2; }
        }

        private Vector2      MousePosInCanvas
        {
            get { return ViewToCanvas(Event.current.mousePosition); }
        }

        private bool         NodesOutOfView
        {
            get { return viewRect.center != nodeBounds.center; }
        }

        private GUIStyle     ToolbarStyle
        {
            get
            {
                if(mToolbarStyle==null)
                {
                    this.mToolbarStyle = new GUIStyle("toolbar");
                    this.mToolbarStyle.fixedHeight = 30;
                    this.mToolbarStyle.alignment = TextAnchor.MiddleCenter;
                    this.mToolbarStyle.fontSize = 20;
                }
                return mToolbarStyle;
            }
        }

        private GUIStyle     ToolbarTextFieldStyle
        {
            get
            {
                if (mToolbarTextFieldStyle == null)
                {
                    this.mToolbarTextFieldStyle = new GUIStyle("toolbarTextField");
                    this.mToolbarTextFieldStyle.fixedHeight = 30;
                    this.mToolbarTextFieldStyle.alignment = TextAnchor.MiddleCenter;
                    this.mToolbarTextFieldStyle.fontSize = 20;
                }
                return mToolbarTextFieldStyle;
            }
        }

        private GUIStyle     ToolbarButtonStyle
        {
            get
            {
                if (mToolbarButtonStyle == null)
                {
                    this.mToolbarButtonStyle = new GUIStyle("toolbarButton");
                    this.mToolbarButtonStyle.fixedHeight = 30;
                    this.mToolbarButtonStyle.alignment = TextAnchor.MiddleCenter;
                    this.mToolbarButtonStyle.fontSize = 20;
                }
                return mToolbarButtonStyle;
            }
        }

        private GUIStyle     TextCenterStyle
        {
            get
            {
                if (mTextCenterStyle == null)
                {
                    this.mTextCenterStyle = new GUIStyle("label");
                    this.mTextCenterStyle.alignment = TextAnchor.UpperCenter;
                }
                return mTextCenterStyle;
            }
        }

        private GUISkin      BVTSkin
        {
            get
            {
                if (mBVTSkin == null)
                {
                    mBVTSkin = AssetDatabase.LoadAssetAtPath<GUISkin>("Assets/Scripts/BVT/Editor/Resource/Skin/BVTSkin.guiskin");
                }
                return mBVTSkin;
            }
        }

        private NodeTree     BVTTree
        {
            get { return mTree; }
            set { mTree = value; }
        }

        private System.Type  FocuseType
        {
            get { return this.mTree.FocusedGraph.GetType(); }
        }      

        void OnEnable()
        {
            this.titleContent = new GUIContent("BVT Editor");
        }

        void OnGUI()
        {
            if(EditorApplication.isCompiling)
            {
                return;
            }
            if (mTree == null)
            {
                return;
            }
            if(PrefabUtility.GetPrefabType(mTree)==PrefabType.Prefab)
            {
                return;
            }
            DrawToorBar();
            GUI.skin = BVTSkin;

            canvasRect = new Rect(5, topMargin, position.width - 10, position.height - topMargin - 5);
            if (ZoomFactor != 1)
            {
                canvasRect = StartZoomArea(canvasRect);
            }

            GUI.BeginGroup(canvasRect);

            totalCanvas = canvasRect;
            totalCanvas.x = 0;
            totalCanvas.y = 0;
            totalCanvas.x += Pan.x / ZoomFactor;
            totalCanvas.y += Pan.y / ZoomFactor;
            totalCanvas.width -= Pan.x / ZoomFactor;
            totalCanvas.height -= Pan.y / ZoomFactor;

                GUI.BeginGroup(totalCanvas);
                viewRect = totalCanvas;
                viewRect.x = 0;
                viewRect.y = 0;
                viewRect.x -= Pan.x / ZoomFactor;
                viewRect.y -= Pan.y / ZoomFactor;
                viewRect.width += Pan.x / ZoomFactor;
                viewRect.height += Pan.y / ZoomFactor;
                nodeBounds = GetNodeBounds(BVTTree, viewRect, true); 
                DrawGrid(viewRect, Pan, ZoomFactor);
                BeginWindows();
                DrawAllNodes();
                EndWindows();
                GUI.EndGroup();

            GUI.EndGroup();

            if (ZoomFactor != 1)
            {
                EndZoomArea();
            }
            ShowScrollBars();
            DrawBlackboardInfo();
            DrawSelectNodeInfo();
            DrawNodeGraphControls();
            AcceptDrops(Event.current, MousePosInCanvas);
            if (BVTTree.PostGUI != null)
            {
                BVTTree.PostGUI.Invoke();
                BVTTree.PostGUI = null;
            }

            Repaint();
            GUI.skin = null;
        }

        void Update()
        {
            DoSmoothPan();
            DoSmoothZoom();
        }

        void OnInspectorUpdate()
        {
            if (EditorUtility.InstanceIDToObject(mTreeInstanceID) is NodeTree)
            {
                mTree = EditorUtility.InstanceIDToObject(mTreeInstanceID) as NodeTree;
                Repaint();
            }
        }

        void OnSelectionChange()
        {
            if (Selection.activeGameObject == null)
            {
                return;
            }
            var lastWindow = EditorWindow.focusedWindow;
            var tree = Selection.activeGameObject.GetComponent<NodeTree>();
            if (tree != null)
            {
                OpenWindow(tree);
            }
            if (lastWindow != null)
            {
                EditorWindow.GetWindow(lastWindow.GetType());
            }
        }

        private void DoSmoothPan()
        {
            if (smoothPan == null)
                return;
            var smooth = (Vector2)smoothPan;
            if ((smooth - Pan).magnitude < 0.1f)
            {
                smoothPan = null;
                return;
            }
            Pan = Vector2.SmoothDamp(Pan, smooth, ref panVelocity, Application.isPlaying ? 0.05f : 0.0001f, 5, Time.deltaTime);
            Repaint();
        }

        private void DoSmoothZoom()
        {
            if (smoothZoomFactor == null)
                return;
            var zmooth = (float)smoothZoomFactor;
            if (Mathf.Abs(zmooth - ZoomFactor) < 0.00001f)
            {
                smoothZoomFactor = null;
                return;
            }
            ZoomFactor = Mathf.SmoothDamp(ZoomFactor, zmooth, ref zoomVelocity, Application.isPlaying ? 0.05f : 0.0001f);
            if (ZoomFactor > 0.99999f)
            {
                ZoomFactor = 1;
            }

            Repaint();
        }

        private void DrawToorBar()
        {
            GUILayout.BeginHorizontal(ToolbarStyle, GUILayout.ExpandWidth(true));
            bool showNodeIcon     = BTSettings.ShowNodeIcon;
            bool showBlackboard   = BTSettings.ShowBlackboard;
            bool showNodeDesc     = BTSettings.ShowNodeComment;

            GUI.backgroundColor = new Color(0.8f, 0.8f, 1);
            if (GUILayout.Button("类型", ToolbarButtonStyle, GUILayout.Width(120)))
            {

            }

            GUI.backgroundColor = Color.green;
            BTSettings.ShowNodeComment  = GUILayout.Toggle(showNodeDesc,     showNodeDesc ?      "隐藏备注":"显示备注", ToolbarButtonStyle, GUILayout.Width(120));
            BTSettings.ShowNodeIcon     = GUILayout.Toggle(showNodeIcon,     showNodeIcon ?      "文字模式":"图片模式", ToolbarButtonStyle, GUILayout.Width(120));
            BTSettings.ShowBlackboard   = GUILayout.Toggle(showBlackboard,   showBlackboard ?    "隐藏黑板":"显示黑板", ToolbarButtonStyle, GUILayout.Width(120));
            BTSettings.NodeConnMode     = (ENodeConnMode)EditorGUILayout.EnumPopup(BTSettings.NodeConnMode, ToolbarButtonStyle, GUILayout.Width(120));

            GUILayout.FlexibleSpace();
            GUI.backgroundColor = Color.cyan;
            if (GUILayout.Button("加载Tree", ToolbarButtonStyle, GUILayout.Width(120)))
            {

            }
            GUI.backgroundColor = Color.yellow;
            if (GUILayout.Button("保存Tree", ToolbarButtonStyle, GUILayout.Width(120)))
            {

            }

            GUILayout.Space(100);
            GUI.backgroundColor = Color.red;
            if (GUILayout.Button("清除所有节点", ToolbarButtonStyle, GUILayout.Width(200)))
            {

            }
            GUI.backgroundColor = Color.white;
            GUILayout.EndHorizontal();
        }

        private void DrawTreeInfo()
        {
            GUILayout.BeginVertical();
            GUI.color = Color.yellow;
            TextCenterStyle.fontSize = 20;
            GUILayout.Label(mTree.Name, TextCenterStyle);
            TextCenterStyle.fontSize = 12;
            GUILayout.Label(string.Format("{0}  {1}", mTree, mTree.ID), TextCenterStyle);
            GUI.color = Color.white;
            GUILayout.EndVertical();
        }

        private void DrawBlackboardInfo()
        {
            if (!BTSettings.ShowBlackboard)
            {
                return;
            }
            mBlackboardRect.width = 300;
            mBlackboardRect.x = Screen.width - 310;
            mBlackboardRect.y = 50;
            mBlackboardRect.height = mTree.Blackboard.Data.Count * 20 + 110;
            GUISkin lastSkin = GUI.skin;
            GUI.Box(mBlackboardRect, "", BTStyle.BVT_WindowShadow);
            GUILayout.BeginArea(mBlackboardRect, "Variables", new GUIStyle(BTStyle.BVT_Panel));
            GUILayout.Space(10);
            GUI.skin = null;
            mTree.Blackboard.SetRoot(mTree.transform);
            mTree.Blackboard.DrawGUI();
            GUILayout.Box("", GUILayout.Height(5), GUILayout.Width(mBlackboardRect.width - 10));
            GUI.skin = lastSkin;
            GUILayout.EndArea();
        }

        private void DrawSelectNodeInfo()
        {
            if (mTree.FocusedGraph == null)
            {
                mInspectorRect.height = 0;
                GUILayout.BeginArea(Rect.zero);
                GUILayout.EndArea();
                return;
            }
            mInspectorRect.width = 320;
            mInspectorRect.x = 10;
            mInspectorRect.y = 50;
            GUISkin lastSkin = GUI.skin;
            GUI.Box(mInspectorRect, "", BTStyle.BVT_WindowShadow);

            NodeNameAttribute nameAttr = BTHelper.GetAttribute<NodeNameAttribute>(FocuseType);
            GUILayout.BeginArea(mInspectorRect, nameAttr != null ? nameAttr.Name : FocuseType.Name, new GUIStyle(BTStyle.BVT_Panel));
            GUILayout.Space(5);
            GUI.skin = null;

            if (BTSettings.ShowNodeInfo)
            {
                GUI.backgroundColor = new Color(0.8f, 0.8f, 1);
                NodeDescAttribute descAttr = BTHelper.GetAttribute<NodeDescAttribute>(FocuseType);
                EditorGUILayout.HelpBox(descAttr != null ? descAttr.Desc : string.Empty, MessageType.None, true);
                GUI.backgroundColor = Color.white;
                mTree.FocusedGraph.DrawNodeInspectorGUI();
            }

            GUILayout.Box("", GUILayout.Height(5), GUILayout.Width(mInspectorRect.width - 1));
            GUI.skin = lastSkin;
            if (Event.current.type == EventType.Repaint)
            {
                mInspectorRect.height = GUILayoutUtility.GetLastRect().yMax + 5;
            }
            GUILayout.EndArea();
        }

        private void AcceptDrops(Event e, Vector2 canvasMousePos)
        {
            if (e.type == EventType.DragUpdated && DragAndDrop.objectReferences.Length == 1)
            {
                DragAndDrop.visualMode = DragAndDropVisualMode.Link;
            }

            if (e.type == EventType.DragPerform)
            {
                if (DragAndDrop.objectReferences.Length != 1)
                {
                    return;
                }
                DragAndDrop.AcceptDrag();
            }
        }

        private Vector2 ViewToCanvas(Vector2 viewPos)
        {
            return (viewPos - Pan) / ZoomFactor;
        }

        private Vector2 CanvasToView(Vector2 canvasPos)
        {
            return (canvasPos * ZoomFactor) + Pan;
        }

        private Rect StartZoomArea(Rect container)
        {
            GUI.EndGroup();
            container.height += unityTabHeight;
            container.width *= 1 / ZoomFactor;
            container.height *= 1 / ZoomFactor;
            oldMatrix = GUI.matrix;
            var matrix1 = Matrix4x4.TRS(zoomPoint, Quaternion.identity, Vector3.one);
            var matrix2 = Matrix4x4.Scale(new Vector3(ZoomFactor, ZoomFactor, 1f));
            GUI.matrix = matrix1 * matrix2 * matrix1.inverse * GUI.matrix;
            return container;
        }

        private void EndZoomArea()
        {
            GUI.matrix = oldMatrix;
            zoomRecoveryRect.y = unityTabHeight;
            zoomRecoveryRect.width = Screen.width;
            zoomRecoveryRect.height = Screen.height;
            GUI.BeginGroup(zoomRecoveryRect); //Recover rect
        }

        private void DrawNodeGraphControls()
        {
            Event e = Event.current;
            if (e.button == 0 && e.type == EventType.MouseDown && mInspectorRect.Contains(e.mousePosition))
            {
                e.Use();
            }
            if (!mInspectorRect.Contains(e.mousePosition) &&
                !mBlackboardRect.Contains(e.mousePosition))
            {
                if (e.button == 0 && e.isMouse && e.type == EventType.MouseDown)
                {
                    mTree.FocusedNode = null;
                    return;
                }
                if (e.button == 1 && e.type == EventType.MouseDown)
                {
                    var pos = e.mousePosition + scrollPos;
                    BTHelper.ShowComponentsSelectionMenu(typeof(Node), (selectType) =>
                    {
                        Node graph = mTree.AddGraph(selectType);
                        graph.NodeRect.center = pos;
                        mTree.FocusedNode = graph;
                    });
                    e.Use();
                }
            }
            if (e.isKey && e.alt && e.keyCode == KeyCode.Q)
            {
                foreach(Node graph in mTree.AllNodes)
                {
                    graph.NodeRect.width  = Node.MinSize.x;
                    graph.NodeRect.height = Node.MinSize.y;
                }
                e.Use();
            }
            if (e.type == EventType.MouseUp || e.type == EventType.KeyUp)
            {
                
            }
            if (e.type == EventType.KeyDown && e.keyCode == KeyCode.F && GUIUtility.keyboardControl == 0)
            {
                if (BVTTree.AllNodes.Count > 0)
                {
                    FocusPosition(GetNodeBounds(BVTTree, viewRect, false).center);
                }
                else
                {
                    FocusPosition(VirtualCenter);
                }
            }
            if (e.type == EventType.MouseDown && e.button == 2 && e.clickCount == 2)
            {
                FocusPosition(ViewToCanvas(e.mousePosition));
            }
            if (e.type == EventType.ScrollWheel)
            {
                if(canvasRect.Contains(e.mousePosition))
                {
                    var zoomDelta = e.shift ? 0.1f : 0.25f;
                    ZoomAt(e.mousePosition, -e.delta.y > 0 ? zoomDelta : -zoomDelta);
                }
            }
            if ((e.button == 2 && e.type == EventType.MouseDrag && canvasRect.Contains(e.mousePosition))
            || ((e.type == EventType.MouseDown || e.type == EventType.MouseDrag) && e.alt && e.isMouse))
            {
                Pan += e.delta;
                smoothPan = null;
                smoothZoomFactor = null;
                e.Use();
            }
            if (e.type == EventType.MouseDown && e.button == 2 && canvasRect.Contains(e.mousePosition))
            {
                mouseButton2Down = true;
            }
            if (e.type == EventType.MouseUp && e.button == 2)
            {
                mouseButton2Down = false;
            }
            if (mouseButton2Down == true)
            {
                EditorGUIUtility.AddCursorRect(new Rect(0, 0, Screen.width, Screen.height), MouseCursor.Pan);
            }
        }

        private void FocusPosition(Vector2 targetPos)
        {
            smoothPan = -targetPos;
            smoothPan += new Vector2(viewRect.width / 2, viewRect.height / 2);
            smoothPan *= ZoomFactor;
        }

        private void ZoomAt(Vector2 center, float delta)
        {
            var pinPoint = (center - Pan) / ZoomFactor;
            var newZ = ZoomFactor;
            newZ += delta;
            newZ = Mathf.Clamp(newZ, 0.25f, 1f);
            smoothZoomFactor = newZ;
            var a = (pinPoint * newZ) + Pan;
            var b = center;
            var diff = b - a;
            smoothPan = Pan + diff;
        }

        private void ShowScrollBars()
        {
            if (!NodesOutOfView)
                return;

            GUI.enabled = false;
            scrollPos = -Pan;

            var xLeftDif = Mathf.Max(viewRect.x - nodeBounds.x, 0);
            var xRightDiff = Mathf.Max(nodeBounds.xMax - viewRect.xMax, 0);
            var xHowMuchSee = nodeBounds.width - xLeftDif - xRightDiff;
            if (xLeftDif > 0 || xRightDiff > 0)
            {
                var xScrollBarRect = new Rect(5, position.height - 18, position.width - 10, 10);
                scrollPos.x = GUI.HorizontalScrollbar(xScrollBarRect, scrollPos.x, xHowMuchSee, nodeBounds.x, nodeBounds.xMax);
            }

            var yTopDiff = Mathf.Max(viewRect.y - nodeBounds.y, 0);
            var yBottomDiff = Mathf.Max(nodeBounds.yMax - viewRect.yMax, 0);
            var yHowMuchSee = nodeBounds.height - yTopDiff - yBottomDiff;
            if (yTopDiff > 0 || yBottomDiff > 0)
            {
                var yScrollBarRect = new Rect(position.width - 20, topMargin, 10, position.height - topMargin - 20);
                scrollPos.y = GUI.VerticalScrollbar(yScrollBarRect, scrollPos.y, yHowMuchSee, nodeBounds.y, nodeBounds.yMax);
            }
            GUI.enabled = true;
        }

        private Rect GetNodeBounds(NodeTree graph, Rect container, bool bound = false)
        {
            var minX = float.PositiveInfinity;
            var minY = float.PositiveInfinity;
            var maxX = float.NegativeInfinity;
            var maxY = float.NegativeInfinity;

            for (var i = 0; i < graph.AllNodes.Count; i++)
            {
                minX = Mathf.Min(minX, graph.AllNodes[i].NodeRect.xMin);
                minY = Mathf.Min(minY, graph.AllNodes[i].NodeRect.yMin);
                maxX = Mathf.Max(maxX, graph.AllNodes[i].NodeRect.xMax);
                maxY = Mathf.Max(maxY, graph.AllNodes[i].NodeRect.yMax);
            }

            minX -= 50;
            minY -= 50;
            maxX += 50;
            maxY += 50;

            if (bound)
            {
                minX = Mathf.Min(minX, container.xMin + 50);
                minY = Mathf.Min(minY, container.yMin + 50);
                maxX = Mathf.Max(maxX, container.xMax - 50);
                maxY = Mathf.Max(maxY, container.yMax - 50);
            }
            return Rect.MinMaxRect(minX, minY, maxX, maxY);
        }

        private Rect GetSelectionRect(Vector2 startPos, Vector2 endPos)
        {
            var num1 = (startPos.x < endPos.x) ? startPos.x : endPos.x;
            var num2 = (startPos.x > endPos.x) ? startPos.x : endPos.x;
            var num3 = (startPos.y < endPos.y) ? startPos.y : endPos.y;
            var num4 = (startPos.y > endPos.y) ? startPos.y : endPos.y;
            return new Rect(num1, num3, num2 - num1, num4 - num3);
        }

        private void DrawGrid(Rect container, Vector2 offset, float zoomFactor)
        {
            var scaledX = (container.width - offset.x) / zoomFactor;
            var scaledY = (container.height - offset.y) / zoomFactor;
            for (var i = 0 - (int)offset.x; i < scaledX; i++)
            {
                if (i % gridSize == 0)
                {
                    Handles.color = new Color(0, 0, 0, i % (gridSize * 5) == 0 ? 0.2f : 0.1f);
                    Handles.DrawLine(new Vector3(i, 0, 0), new Vector3(i, scaledY, 0));
                }
            }
            for (var i = 0 - (int)offset.y; i < scaledY; i++)
            {
                if (i % gridSize == 0)
                {
                    Handles.color = new Color(0, 0, 0, i % (gridSize * 5) == 0 ? 0.2f : 0.1f);
                    Handles.DrawLine(new Vector3(0, i, 0), new Vector3(scaledX, i, 0));
                }
            }
            Handles.color = Color.white;
        }

        private bool RectEncapsulates(Rect a, Rect b)
        {
            return a.x < b.x && a.xMax > b.xMax && a.y < b.y && a.yMax > b.yMax;
        }

        private void SnapNodes()
        {
            foreach (var node in BVTTree.AllNodes)
            {
                var snapedPos = new Vector2(node.NodeRect.xMin, node.NodeRect.yMin);
                snapedPos.y = Mathf.Round(snapedPos.y / 15) * 15;
                snapedPos.x = Mathf.Round(snapedPos.x / 15) * 15;
                node.NodeRect = new Rect(snapedPos.x, snapedPos.y, node.NodeRect.width, node.NodeRect.height);
            }
        }

        private void DrawAllNodes()
        {
            GUI.color = Color.white;
            GUI.backgroundColor = Color.white;
            foreach (var current in mTree.AllNodes)
            {
                if (current != null)
                {
                    current.DrawGUI();
                }
            }
            EditorUtility.SetDirty(this);
        }

        public static BTTreeWindow OpenWindow(NodeTree tree)
        {
            var window = EditorWindow.GetWindow<BTTreeWindow>();
            window.mTreeInstanceID = tree.GetInstanceID();
            window.mTree = tree;
            window.mTree.FocusedNode = null;
            window.mTree.UpdateGraphIDs();
            return window;
        }

        public static BTTreeWindow ShutWindow()
        {
            var window = EditorWindow.GetWindow<BTTreeWindow>();
            window.Close();
            return null;
        }
    }
}