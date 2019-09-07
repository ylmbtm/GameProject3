#ifndef __X_MATH_H_
#define __X_MATH_H_
#include "CommonConvert.h"

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
	float m_x;
	float m_y;


	Vector2D(float _x = 0.0f, float _y = 0.0f )
	{
		m_x = _x;
		m_y = _y;
	}


	Vector2D&  operator *=(float v)
	{
		m_x *= v;
		m_y *= v;
		return *this;
	}

	Vector2D& operator /=(float v)
	{
		m_x /= v;
		m_y /= v;
		return *this;
	}


	inline  Vector2D& operator -=(float v)
	{
		m_x -= v;
		m_y -= v;
		return *this;
	}


	inline  Vector2D& operator +=(float v)
	{
		m_x += v;
		m_y += v;
		return *this;
	}

	inline  Vector2D& operator +=(const  Vector2D& v)
	{
		m_x += v.m_x;
		m_y += v.m_y;
		return *this;
	}

	inline  Vector2D& operator -=(const  Vector2D& v)
	{
		m_x -= v.m_x;
		m_y -= v.m_y;
		return *this;
	}

	inline  bool operator ==(Vector2D& pt)
	{
		if(pt.m_x == m_x && pt.m_y == m_y)
		{
			return true;
		}
		return false;
	}


	inline Vector2D operator - (const Vector2D& rkVector) const
	{
		return Vector2D(
		           m_x - rkVector.m_x,
		           m_y - rkVector.m_y);
	}

	float Length() const
	{
		return sqrtf(m_x * m_x + m_y * m_y);
	}

	float SquaredLength() const
	{
		return m_x * m_x + m_y * m_y;
	}

	float Distance(Vector2D pos)
	{
		return (*this - pos).Length();
	}

	float Normalized()
	{
		float fLength = sqrtf(m_x * m_x + m_y * m_y);

		if (fLength > 1e-08)
		{
			float fInvLength = 1.0f / fLength;
			m_x *= fInvLength;
			m_y *= fInvLength;
		}

		return fLength;
	}


	FLOAT DistanceToSegment(Vector2D pt1, Vector2D pt2)
	{
		FLOAT cross = (pt2.m_x - pt1.m_x) * (m_x - pt1.m_x) + (pt2.m_y - pt1.m_y) * (m_y - pt1.m_y);
		if (cross <= 0)
		{
			return sqrtf((m_x - pt1.m_x) * (m_x - pt1.m_x) + (m_y - pt1.m_y) * (m_y - pt1.m_y));
		}

		FLOAT d2 = (pt2.m_x - pt1.m_x) * (pt2.m_x - pt1.m_x) + (pt2.m_y - pt1.m_y) * (pt2.m_y - pt1.m_y);
		if (cross >= d2)
		{
			return sqrtf((m_x - pt2.m_x) * (m_x - pt2.m_x) + (m_y - pt2.m_y) * (m_y - pt2.m_y));
		}

		FLOAT r = cross / d2;
		FLOAT px = pt1.m_x + (pt2.m_x - pt1.m_x) * r;
		FLOAT py = pt1.m_y + (pt2.m_y - pt1.m_y) * r;

		return sqrtf((m_x - px) * (m_x - px) + (py - pt1.m_y) * (py - pt1.m_y));
	}

	//范围: 0~~π (0~~180)
	float AngleBetween(const Vector2D dest)
	{
		return acos((m_x * dest.m_x + m_y * dest.m_y) / Length() / dest.Length());
	}

	//范围: 0~~2π (0~~360)
	float ToRadiansAngle()
	{
		float fAngle = acos(m_x / Length());

		if (m_y < 0.0f)
		{
			if (m_x < 0.0f)
			{
				fAngle += PI / 2;
			}
			else
			{
				fAngle += PI;
			}
		}
		return fAngle;
	}

	//范围: 0~~360
	float ToDegreesAngle()
	{
		return ToRadiansAngle() * RAD_TO_DEG;
	}

	Vector2D Rotate(Vector2D A, FLOAT radianAngle)
	{
		return Vector2D(A.m_x * cos(radianAngle) - A.m_y * sin(radianAngle), A.m_x * sin(radianAngle) + A.m_y * cos(radianAngle));
	}

	void Rotate(FLOAT radianAngle)
	{
		float tmx = 0, tmy = 0;
		tmx = m_x * cos(radianAngle) - m_y * sin(radianAngle);
		tmy = m_x * sin(radianAngle) + m_y * cos(radianAngle);
		m_x = tmx;
		m_y = tmy;
	}

	BOOL FromString(std::string str)
	{
		m_x = CommonConvert::StringToFloat(str.substr(0, str.find_first_of(',')).c_str());
		m_y = CommonConvert::StringToFloat(str.substr(str.find_last_of(',')).c_str());
		return TRUE;
	}
};

typedef Vector2D CPoint2D;

class Rect2D
{
public:
	FLOAT m_Left, m_Top, m_Bottom, m_Right;
	Rect2D(float _left, float _top, float _right, float _bottom)
	{
		m_Left = _left;
		m_Top = _top;
		m_Bottom = _bottom;
		m_Right = _right;
	};

	Rect2D()
	{
		m_Left = 0;
		m_Top = 0;
		m_Bottom = 0;
		m_Right = 0;
	};
	bool PtInRect(CPoint2D pt)
	{
		if(pt.m_x >= m_Left && pt.m_y >= m_Top && pt.m_x <= m_Right && pt.m_y <= m_Bottom)
		{
			return true;
		}
		return false;
	};
};


class  Vector3D
{

public:

	Vector3D(): m_x(0), m_y(0), m_z(0) {}

	Vector3D(float x1, float y1, float z1): m_x(x1), m_y(y1), m_z(z1) {}

	Vector3D(const Vector3D& v): m_x(v.m_x), m_y(v.m_y), m_z(v.m_z) {}

	~Vector3D() {};

	Vector3D& operator=(const Vector3D& v)
	{
		m_x = v.m_x;
		m_y = v.m_y;
		m_z = v.m_z;
		return *this;
	}

	Vector3D operator+(const Vector3D& v)
	{
		return Vector3D(m_x + v.m_x,
		                m_y + v.m_y,
		                m_z + v.m_z);
	}

	Vector3D operator-(const Vector3D& v)
	{
		return Vector3D(m_x - v.m_x,
		                m_y - v.m_y,
		                m_z - v.m_z);
	}

	Vector3D operator * (const Vector3D& rhs) const
	{
		return Vector3D(
		           m_x * rhs.m_x,
		           m_y * rhs.m_y,
		           m_z * rhs.m_z);
	}

	Vector3D operator / (const Vector3D& rhs) const
	{
		return Vector3D(
		           m_x / rhs.m_x,
		           m_y / rhs.m_y,
		           m_z / rhs.m_z);
	}

	Vector3D operator*(const Vector3D& v)
	{
		return Vector3D(m_x * v.m_x,
		                m_y * v.m_y,
		                m_z * v.m_z);
	}

	Vector3D operator+(float f)
	{
		return Vector3D(m_x + f,
		                m_y + f,
		                m_z + f);
	}

	Vector3D operator-(float f)
	{
		return Vector3D(m_x - f,
		                m_y - f,
		                m_z - f);
	}

	Vector3D operator*(float f)
	{
		return Vector3D(m_x * f,
		                m_y * f,
		                m_z * f);
	}

	Vector3D& operator += (const Vector3D& v)
	{
		m_x += v.m_x;
		m_y += v.m_y;
		m_z += v.m_z;
		return *this;
	}

	Vector3D& operator -= (const Vector3D& v)
	{
		m_x -= v.m_x;
		m_y -= v.m_y;
		m_z -= v.m_z;
		return *this;
	}

	Vector3D& operator *= (const Vector3D& v)
	{
		m_x *= v.m_x;
		m_y *= v.m_y;
		m_z *= v.m_z;
		return *this;
	}

	Vector3D& operator /= (const Vector3D& rhs)
	{
		m_x /= rhs.m_x;
		m_y /= rhs.m_y;
		m_z /= rhs.m_z;
		return *this;
	}

	Vector3D& operator += (float f)
	{
		m_x += f;
		m_y += f;
		m_z += f;
		return *this;
	}

	Vector3D& operator -= (float f)
	{
		m_x -= f;
		m_y -= f;
		m_z -= f;
		return *this;
	}

	Vector3D& operator *= (float f)
	{
		m_x *= f;
		m_y *= f;
		m_z *= f;
		return *this;
	}

	bool operator == (const Vector3D& rkVector) const
	{
		return (m_x == rkVector.m_x && m_y == rkVector.m_y && m_z == rkVector.m_z);
	}

	bool operator != (const Vector3D& rkVector) const
	{
		return (m_x != rkVector.m_x || m_y != rkVector.m_y || m_z != rkVector.m_z);
	}

	//点集
	float DotProduct(const Vector3D& v)
	{
		return m_x * v.m_x +
		       m_y * v.m_y +
		       m_z * v.m_z;
	}

	float Length()
	{
		return sqrtf(m_x * m_x + m_y * m_y + m_z * m_z);
	}

	void Reset()
	{
		m_x = 0.0f;
		m_y = 0.0f;
		m_z = 0.0f;
	}

	float SquaredLength() const
	{
		return m_x * m_x + m_y * m_y + m_z * m_z;
	}

	float Normalize()
	{
		float fLength = Length();
		if (fLength > 1e-08)
		{
			float fInvLength = 1.0f / fLength;
			m_x *= fInvLength;
			m_y *= fInvLength;
			m_z *= fInvLength;
		}

		return fLength;
	}

	Vector3D CrossProduct(const Vector3D& v)
	{
		return Vector3D(m_y * v.m_z - m_z * v.m_y,
		                m_z * v.m_x - m_x * v.m_z,
		                m_x * v.m_y - m_y * v.m_x);
	}

	float AngleBetween(Vector3D dest)
	{
		float lenProduct = Length();
		lenProduct *= dest.Length();

		if (lenProduct < 1e-6f)
		{
			lenProduct = 1e-6f;
		}

		float f = DotProduct(dest) / lenProduct;

		if (f > 1.0f)
		{
			f = 1.0f;
		}
		else if (f < -1.0f)
		{
			f = -1.0f;
		}
		return acosf(f);
	}

	float Distance2D(Vector3D pos)
	{
		float dx = m_x - pos.m_x;
		float dz = m_z - pos.m_z;
		return sqrtf(dx * dx + dz * dz);
	}

	//范围: 0~~π (0~~180)
	float AngleBetween2D(Vector3D& dest)
	{
		return acosf((m_x * dest.m_x + m_z * dest.m_z) / sqrtf(m_x * m_x + m_z * m_z) / sqrtf(dest.m_x * dest.m_x + dest.m_z * dest.m_z));
	}

	Vector2D Rotate(Vector2D A, FLOAT radianAngle)
	{
		return Vector2D(A.m_x * cos(radianAngle) - A.m_y * sin(radianAngle), A.m_x * sin(radianAngle) + A.m_y * cos(radianAngle));
	}

	//范围: 0~~2π
	float ToRadiansAngle()
	{
		float fAngle = acos(m_x / Length());

		if (m_z < 0.0f)
		{
			if (m_x < 0.0f)
			{
				fAngle += PI;
			}
			else
			{
				fAngle += PI / 2 ;
			}
		}
		return fAngle;
	}

	//范围: 0~~360
	float ToDegreesAngle()
	{
		return Vector3D::RadiansToDegrees(ToRadiansAngle());
	}

	BOOL FromString(const char* pStr)
	{
		return CommonConvert::StringToPos((CHAR*)pStr, m_x, m_y, m_z);
	}

	static inline float DegreesToRadians(float degrees)
	{
		return degrees * DEG_TO_RAD;
	}
	static inline float RadiansToDegrees(float radians)
	{
		return radians * RAD_TO_DEG;
	}

	float m_x, m_y, m_z;
};

typedef Vector3D CPoint3D;

#endif //_MAP_H_