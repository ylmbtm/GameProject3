#ifndef __X_MATH_H_
#define __X_MATH_H_
#include "CommonConvert.h"

#define PI			3.1415926f

class CPoint2D
{
public:
	float m_x;
	float m_y;


	CPoint2D(float _x = 0.0f, float _y = 0.0f )
	{
		m_x = _x;
		m_y = _y;
	}


	CPoint2D&  operator *=(float v)
	{
		m_x *= v;
		m_y *= v;
		return *this;
	}

	CPoint2D& operator /=(float v)
	{
		m_x /= v;
		m_y /= v;
		return *this;
	}


	inline  CPoint2D& operator -=(float v)
	{
		m_x -= v;
		m_y -= v;
		return *this;
	}


	inline  CPoint2D& operator +=(float v)
	{
		m_x += v;
		m_y += v;
		return *this;
	}

	inline  CPoint2D& operator +=(const  CPoint2D& v)
	{
		m_x += v.m_x;
		m_y += v.m_y;
		return *this;
	}

	inline  bool operator ==(CPoint2D& pt)
	{
		if(pt.m_x == m_x && pt.m_y == m_y)
		{
			return true;
		}
		return false;
	}


	float GetDistance(CPoint2D pos)
	{
		return sqrt(float(pow(fabs(pos.m_x - m_x), 2) + pow(fabs(pos.m_y - m_y), 2)));
	}

	CPoint2D& Normalized()
	{
		float mo = sqrt(m_x * m_x + m_y * m_y);
		if(mo >= 0.0001f)
		{
			m_x = m_x / mo;
			m_y = m_y / mo;
		}
		return *this;
	}

	BOOL From(std::string str)
	{
		m_x = CommonConvert::StringToFloat(str.substr(0, str.find_first_of(',')).c_str());
		m_y = CommonConvert::StringToFloat(str.substr(str.find_last_of(',')).c_str());
		return TRUE;
	}
};


inline CPoint2D operator +(const CPoint2D& pos, float v)
{
	return CPoint2D(pos.m_x + v, pos.m_y + v);
}


inline CPoint2D operator +(const CPoint2D& pos, const CPoint2D& v)
{
	return CPoint2D(pos.m_x + v.m_x, pos.m_y + v.m_y);
}
inline CPoint2D operator -(const CPoint2D& pos, const CPoint2D& v)
{
	return CPoint2D(pos.m_x - v.m_x, pos.m_y - v.m_y);
}

inline CPoint2D operator -(const CPoint2D& pos, float v)
{
	return CPoint2D(pos.m_x - v, pos.m_y - v);
}

inline CPoint2D operator *(const CPoint2D& pos, float v)
{
	return CPoint2D(pos.m_x * v, pos.m_y * v);
}

inline CPoint2D operator / (const CPoint2D& pos, float v)
{
	return CPoint2D(pos.m_x / v, pos.m_y / v);
}



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


	Vector3D& operator+(const Vector3D& v)
	{
		m_x += v.m_x;
		m_y += v.m_y;
		m_z += v.m_z;
		return *this;
	}

	Vector3D& operator-(const Vector3D& v)
	{
		m_x -= v.m_x;
		m_y -= v.m_y;
		m_z -= v.m_z;
		return *this;
	}

	Vector3D& operator*(const Vector3D& v)
	{
		m_x *= v.m_x;
		m_y *= v.m_y;
		m_z *= v.m_z;
		return *this;
	}

	Vector3D& operator+(float f)
	{
		m_x += f;
		m_y += f;
		m_z += f;
		return *this;
	}

	Vector3D& operator-(float f)
	{
		m_x -= f;
		m_y -= f;
		m_z -= f;
		return *this;
	}

	Vector3D& operator*(float f)
	{
		m_x *= f;
		m_y *= f;
		m_z *= f;
		return *this;
	}

	float Dot(const Vector3D& v)
	{
		return m_x * v.m_x +
		       m_y * v.m_y +
		       m_z * v.m_z;
	}

	float Length()
	{
		return sqrtf(Dot(*this));
	}

	void Normalize()
	{
		float len = Length();

		if (len < 0.0)
		{
			len = 1;
		}

		m_x /= len;
		m_y /= len;
		m_z /= len;
	}

	Vector3D CrossProduct(const Vector3D& v)
	{
		return Vector3D(m_y * v.m_z - m_z * v.m_y,
		                m_z * v.m_x - m_x * v.m_z,
		                m_x * v.m_y - m_y * v.m_x);
	}

	BOOL From(const char* pStr)
	{
		return CommonConvert::StringToPos((CHAR*)pStr, m_x, m_y, m_z);
	}


	float m_x, m_y, m_z;
};

typedef Vector3D CPoint3D;

#endif //_MAP_H_