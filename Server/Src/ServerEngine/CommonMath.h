#ifndef __X_MATH_H_
#define __X_MATH_H_

#define PI          3.1415926f
#define TWO_PI      6.2831852f
#define DEG_TO_RAD  0.0174533f
#define RAD_TO_DEG  57.2957805f

// FLOAT Q_rsqrt( FLOAT number )
// {
//  long i;
//  FLOAT x2, y;
//  const FLOAT threehalfs = 1.5F;
//
//  x2 = number * 0.5F;
//  y  = number;
//  i  = * ( long* ) &y;                        // evil FLOATing point bit level hacking
//  i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
//  y  = * ( FLOAT* ) &i;
//  y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
// //   y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
//  return y;
// }

class Vector2D
{
public:
    Vector2D(FLOAT _x = 0.0f, FLOAT _y = 0.0f );

    Vector2D&  operator *=(FLOAT v);

    Vector2D& operator /=(FLOAT v);

    Vector2D& operator -=(FLOAT v);

    Vector2D& operator +=(FLOAT v);

    Vector2D& operator +=(const  Vector2D& v);

    Vector2D& operator -=(const  Vector2D& v);

    bool operator ==(Vector2D& pt);

    Vector2D operator - (const Vector2D& v) const;

    FLOAT Length() const;

    FLOAT SquaredLength() const;

    FLOAT Distance(Vector2D pos);

    FLOAT Normalized();

    //求到线段的距离
    FLOAT DistanceToSegment(Vector2D pt1, Vector2D pt2);

    //求弧度范围: 0-π
    FLOAT AngleBetween(const Vector2D dest);

    //向量转弧度范围: 0-2π
    FLOAT ToRadiansAngle();

    //向量转角度范围: 0-360
    FLOAT ToDegreesAngle();

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
    Rect2D(FLOAT fLeft, FLOAT fTop, FLOAT fRight, FLOAT fBottom);

    Rect2D();

    VOID Init(FLOAT fLeft, FLOAT fTop, FLOAT fRight, FLOAT fBottom);

    BOOL Contains(CPoint2D pt);

    BOOL Contains(Rect2D rcArea);

    BOOL Intersects(Rect2D rcArea);

    VOID Reset();

    FLOAT m_fLeft = 0, m_fTop = 0, m_fBottom = 0, m_fRight = 0;
};

class Circle2D
{
public:
    Circle2D(FLOAT fX, FLOAT fY, FLOAT fRadius);

    Circle2D(CPoint2D pt, FLOAT fRadius);

    VOID Init(FLOAT fX, FLOAT fY, FLOAT fRadius);

    BOOL Contains(CPoint2D pt);

    BOOL Intersects(Rect2D rcArea);

    VOID Reset();

    CPoint2D m_CenterPos;
    FLOAT    m_fRadius;
};


class  Vector3D
{
public:
    Vector3D();

    Vector3D(FLOAT x1, FLOAT y1, FLOAT z1);

    Vector3D(const Vector3D& v);

    ~Vector3D();;

    Vector3D& operator=(const Vector3D& v);

    Vector3D operator+(const Vector3D& v);

    Vector3D operator-(const Vector3D& v);

    Vector3D operator * (const Vector3D& rhs) const;

    Vector3D operator / (const Vector3D& rhs) const;

    Vector3D operator*(const Vector3D& v);

    Vector3D operator+(FLOAT f);

    Vector3D operator-(FLOAT f);

    Vector3D operator*(FLOAT f);

    Vector3D& operator += (const Vector3D& v);

    Vector3D& operator -= (const Vector3D& v);

    Vector3D& operator *= (const Vector3D& v);

    Vector3D& operator /= (const Vector3D& rhs);

    Vector3D& operator += (FLOAT f);

    Vector3D& operator -= (FLOAT f);

    Vector3D& operator *= (FLOAT f);

    bool operator == (const Vector3D& rkVector) const;

    bool operator != (const Vector3D& rkVector) const;

    //点集
    FLOAT DotProduct(const Vector3D& v);

    FLOAT Length();

    void Reset();

    FLOAT SquaredLength() const;

    FLOAT Normalize();

    Vector3D CrossProduct(const Vector3D& v);

    FLOAT AngleBetween(Vector3D dest);

    FLOAT Distance2D(Vector3D pos);

    //求弧度范围: 0-π
    FLOAT AngleBetween2D(Vector3D& dest);

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

BOOL  IsInCircle(Vector3D tTarPos, FLOAT fTarRadius, Vector3D tCirclePos, FLOAT fCircleRadius);

}

#endif //_MAP_H_