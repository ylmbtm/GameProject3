#ifndef __X_MATH_H_
#define __X_MATH_H_

#define PI			3.1415926f
#define TWO_PI		6.2831852f
#define DEG_TO_RAD	0.0174533f
#define RAD_TO_DEG	57.2957805f


// float Q_rsqrt( float number )
// {
// 	long i;
// 	float x2, y;
// 	const float threehalfs = 1.5F;
//
// 	x2 = number * 0.5F;
// 	y  = number;
// 	i  = * ( long* ) &y;                        // evil floating point bit level hacking
// 	i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
// 	y  = * ( float* ) &i;
// 	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
// //	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
// 	return y;
// }

class Vector2D
{
public:
	Vector2D(float _x = 0.0f, float _y = 0.0f );

	Vector2D&  operator *=(float v);

	Vector2D& operator /=(float v);

	Vector2D& operator -=(float v);

	Vector2D& operator +=(float v);

	Vector2D& operator +=(const  Vector2D& v);

	Vector2D& operator -=(const  Vector2D& v);

	bool operator ==(Vector2D& pt);

	Vector2D operator - (const Vector2D& v) const;

	float Length() const;

	float SquaredLength() const;

	float Distance(Vector2D pos);

	float Normalized();

	//求到线段的距离
	FLOAT DistanceToSegment(Vector2D pt1, Vector2D pt2);

	//求弧度范围: 0-π
	float AngleBetween(const Vector2D dest);

	//向量转弧度范围: 0-2π
	float ToRadiansAngle();

	//向量转角度范围: 0-360
	float ToDegreesAngle();

	Vector2D Rotate(Vector2D A, FLOAT radianAngle);

	void Rotate(FLOAT radianAngle);

	BOOL FromString(std::string str);

	FLOAT m_x;
	FLOAT m_y;
};

typedef Vector2D CPoint2D;

class Rect2D
{
public:


	Rect2D(float _left, float _top, float _right, float _bottom);

	Rect2D();

	bool PtInRect(CPoint2D pt);

	FLOAT m_Left, m_Top, m_Bottom, m_Right;
};


class  Vector3D
{
public:
	Vector3D();

	Vector3D(float x1, float y1, float z1);

	Vector3D(const Vector3D& v);

	~Vector3D();;

	Vector3D& operator=(const Vector3D& v);

	Vector3D operator+(const Vector3D& v);

	Vector3D operator-(const Vector3D& v);

	Vector3D operator * (const Vector3D& rhs) const;

	Vector3D operator / (const Vector3D& rhs) const;

	Vector3D operator*(const Vector3D& v);

	Vector3D operator+(float f);

	Vector3D operator-(float f);

	Vector3D operator*(float f);

	Vector3D& operator += (const Vector3D& v);

	Vector3D& operator -= (const Vector3D& v);

	Vector3D& operator *= (const Vector3D& v);

	Vector3D& operator /= (const Vector3D& rhs);

	Vector3D& operator += (float f);

	Vector3D& operator -= (float f);

	Vector3D& operator *= (float f);

	bool operator == (const Vector3D& rkVector) const;

	bool operator != (const Vector3D& rkVector) const;

	//点集
	float DotProduct(const Vector3D& v);

	float Length();

	void Reset();

	float SquaredLength() const;

	float Normalize();

	Vector3D CrossProduct(const Vector3D& v);

	float AngleBetween(Vector3D dest);

	float Distance2D(Vector3D pos);

	//求弧度范围: 0-π
	float AngleBetween2D(Vector3D& dest);

	Vector2D Rotate(Vector2D A, FLOAT radianAngle);

	//求向量在对应向量的平行分量
	Vector3D ParallelComponent(Vector3D& unitBasis);

	//求向量在对应向量的垂直分量
	Vector3D PerpendicularComponent(Vector3D& unitBasis);

	//求弧度范围: 0-2π
	FLOAT ToRadiansAngle();

	//求角度范围: 0-360
	FLOAT ToDegreesAngle();

	BOOL FromString(const char* pStr);

	FLOAT m_x, m_y, m_z;
};

typedef Vector3D CPoint3D;

namespace CommonMath
{

//角度转弧度
FLOAT DegreesToRadians(FLOAT fDegrees);

//弧度转角度
FLOAT RadiansToDegrees(FLOAT fRadians);

//限制取值
FLOAT Clamp(const FLOAT fValue, const FLOAT fMin, const FLOAT fMax);

}

#endif //_MAP_H_