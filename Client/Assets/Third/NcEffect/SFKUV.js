
var scrollSpeed = 5;
var countX : int = 4;
var countY : int = 4;

private var offsetX = 0.0;
private var offsetY = 0.0;
private var singleTexSize;
private var timef : float;

function Start() {
	singleTexSize = Vector2(1.0/countX, 1.0/countY);
	GetComponent.<Renderer>().material.mainTextureScale = singleTexSize;
}
function Update ()
{
	timef = Time.time - Mathf.Floor(Time.time);
var	frame = Mathf.Floor(timef*scrollSpeed);
	offsetX = frame/countX;
	offsetY = -(frame - frame%countX) /countY / countX;
	GetComponent.<Renderer>().material.SetTextureOffset ("_MainTex", Vector2(offsetX, offsetY));
}