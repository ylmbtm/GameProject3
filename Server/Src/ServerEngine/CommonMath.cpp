#include "stdafx.h"
#include "CommonConvert.h"
#include "CommonMath.h"

Vector2D::Vector2D(float _x /*= 0.0f*/, float _y /*= 0.0f */)
{
    m_x = _x;
    m_y = _y;
}

float Vector2D::Length() const
{
    return sqrtf(m_x * m_x + m_y * m_y);
}

float Vector2D::SquaredLength() const
{
    return m_x * m_x + m_y * m_y;
}

float Vector2D::Distance(Vector2D pos)
{
    return (*this - pos).Length();
}

float Vector2D::Normalized()
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

FLOAT Vector2D::DistanceToSegment(Vector2D pt1, Vector2D pt2)
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

float Vector2D::AngleBetween(const Vector2D dest)
{
    return acos((m_x * dest.m_x + m_y * dest.m_y) / Length() / dest.Length());
}

float Vector2D::ToRadiansAngle()
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

float Vector2D::ToDegreesAngle()
{
    return ToRadiansAngle() * RAD_TO_DEG;
}

Vector2D Vector2D::Rotate(Vector2D A, FLOAT radianAngle)
{
    return Vector2D(A.m_x * cos(radianAngle) - A.m_y * sin(radianAngle), A.m_x * sin(radianAngle) + A.m_y * cos(radianAngle));
}

void Vector2D::Rotate(FLOAT radianAngle)
{
    float tmx = 0, tmy = 0;
    tmx = m_x * cos(radianAngle) - m_y * sin(radianAngle);
    tmy = m_x * sin(radianAngle) + m_y * cos(radianAngle);
    m_x = tmx;
    m_y = tmy;
}

BOOL Vector2D::FromString(std::string str)
{
    m_x = CommonConvert::StringToFloat(str.substr(0, str.find_first_of(',')).c_str());
    m_y = CommonConvert::StringToFloat(str.substr(str.find_last_of(',')).c_str());
    return TRUE;
}

Vector2D Vector2D::operator-(const Vector2D& rkVector) const
{
    return Vector2D(
               m_x - rkVector.m_x,
               m_y - rkVector.m_y);
}

bool Vector2D::operator==(Vector2D& pt)
{
    if (pt.m_x == m_x && pt.m_y == m_y)
    {
        return true;
    }
    return false;
}

Vector2D& Vector2D::operator-=(const Vector2D& v)
{
    m_x -= v.m_x;
    m_y -= v.m_y;
    return *this;
}

Vector2D& Vector2D::operator+=(const Vector2D& v)
{
    m_x += v.m_x;
    m_y += v.m_y;
    return *this;
}

Vector2D& Vector2D::operator+=(float v)
{
    m_x += v;
    m_y += v;
    return *this;
}

Vector2D& Vector2D::operator-=(float v)
{
    m_x -= v;
    m_y -= v;
    return *this;
}

Vector2D& Vector2D::operator/=(float v)
{
    m_x /= v;
    m_y /= v;
    return *this;
}

Vector2D& Vector2D::operator*=(float v)
{
    m_x *= v;
    m_y *= v;
    return *this;
}

Rect2D::Rect2D(float fLeft, float fTop, float fRight, float fBottom)
{
    m_fLeft = fLeft;
    m_fTop = fTop;
    m_fBottom = fBottom;
    m_fRight = fRight;
}

Rect2D::Rect2D()
{
    m_fLeft = 0;
    m_fTop = 0;
    m_fBottom = 0;
    m_fRight = 0;
}

VOID Rect2D::Init(FLOAT fLeft, FLOAT fTop, FLOAT fRight, FLOAT fBottom)
{
    m_fLeft = fLeft;
    m_fTop = fTop;
    m_fBottom = fBottom;
    m_fRight = fRight;
    return ;
}

BOOL Rect2D::Contains(CPoint2D pt)
{
    return  (pt.m_x >= m_fLeft && pt.m_y >= m_fTop && pt.m_x <= m_fRight && pt.m_y <= m_fBottom);
}

BOOL Rect2D::Contains(Rect2D rcArea)
{
    return (m_fLeft <= rcArea.m_fLeft && m_fBottom <= rcArea.m_fBottom && rcArea.m_fRight <= m_fRight && rcArea.m_fTop <= m_fTop);
}

BOOL Rect2D::Intersects(Rect2D rcArea)
{
    return !(m_fRight < rcArea.m_fLeft || rcArea.m_fRight < m_fLeft || m_fTop < rcArea.m_fBottom || rcArea.m_fTop < m_fBottom);
}

VOID Rect2D::Reset()
{
    m_fLeft = 0;
    m_fTop = 0;
    m_fBottom = 0;
    m_fRight = 0;
}

Vector3D::Vector3D() : m_x(0), m_y(0), m_z(0)
{

}

Vector3D::Vector3D(float x1, float y1, float z1) : m_x(x1), m_y(y1), m_z(z1)
{

}

Vector3D::Vector3D(const Vector3D& v) : m_x(v.m_x), m_y(v.m_y), m_z(v.m_z)
{

}

Vector3D::~Vector3D()
{

}

Vector3D Vector3D::operator+(const Vector3D& v)
{
    return Vector3D(m_x + v.m_x,
                    m_y + v.m_y,
                    m_z + v.m_z);
}

Vector3D Vector3D::operator+(float f)
{
    return Vector3D(m_x + f,
                    m_y + f,
                    m_z + f);
}

Vector3D Vector3D::operator-(const Vector3D& v)
{
    return Vector3D(m_x - v.m_x,
                    m_y - v.m_y,
                    m_z - v.m_z);
}

Vector3D Vector3D::operator-(float f)
{
    return Vector3D(m_x - f,
                    m_y - f,
                    m_z - f);
}

Vector3D Vector3D::operator*(const Vector3D& rhs) const
{
    return Vector3D(
               m_x * rhs.m_x,
               m_y * rhs.m_y,
               m_z * rhs.m_z);
}

float Vector3D::DotProduct(const Vector3D& v)
{
    return m_x * v.m_x +
           m_y * v.m_y +
           m_z * v.m_z;
}

float Vector3D::Length()
{
    return sqrtf(m_x * m_x + m_y * m_y + m_z * m_z);
}

void Vector3D::Reset()
{
    m_x = 0.0f;
    m_y = 0.0f;
    m_z = 0.0f;
}

float Vector3D::SquaredLength() const
{
    return m_x * m_x + m_y * m_y + m_z * m_z;
}

float Vector3D::Normalize()
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

Vector3D Vector3D::CrossProduct(const Vector3D& v)
{
    return Vector3D(m_y * v.m_z - m_z * v.m_y,
                    m_z * v.m_x - m_x * v.m_z,
                    m_x * v.m_y - m_y * v.m_x);
}

float Vector3D::AngleBetween(Vector3D dest)
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

float Vector3D::Distance2D(Vector3D pos)
{
    float dx = m_x - pos.m_x;
    float dz = m_z - pos.m_z;
    return sqrtf(dx * dx + dz * dz);
}

float Vector3D::AngleBetween2D(Vector3D& dest)
{
    return acosf((m_x * dest.m_x + m_z * dest.m_z) / sqrtf(m_x * m_x + m_z * m_z) / sqrtf(dest.m_x * dest.m_x + dest.m_z * dest.m_z));
}

Vector2D Vector3D::Rotate(Vector2D A, FLOAT radianAngle)
{
    return Vector2D(A.m_x * cos(radianAngle) - A.m_y * sin(radianAngle), A.m_x * sin(radianAngle) + A.m_y * cos(radianAngle));
}

Vector3D Vector3D::ParallelComponent(Vector3D& unitBasis)
{
    FLOAT projection = DotProduct(unitBasis);
    return unitBasis * projection;
}

Vector3D Vector3D::PerpendicularComponent(Vector3D& unitBasis)
{
    return (*this) - ParallelComponent(unitBasis);
}

float Vector3D::ToRadiansAngle()
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
            fAngle += PI / 2;
        }
    }
    return fAngle;
}

float Vector3D::ToDegreesAngle()
{
    return CommonMath::RadiansToDegrees(ToRadiansAngle());
}

BOOL Vector3D::FromString(const char* pStr)
{
    return CommonConvert::StringToPos((CHAR*)pStr, m_x, m_y, m_z);
}

Vector3D Vector3D::operator/(const Vector3D& rhs) const
{
    return Vector3D(
               m_x / rhs.m_x,
               m_y / rhs.m_y,
               m_z / rhs.m_z);
}

Vector3D& Vector3D::operator*=(float f)
{
    m_x *= f;
    m_y *= f;
    m_z *= f;
    return *this;
}

Vector3D& Vector3D::operator*=(const Vector3D& v)
{
    m_x *= v.m_x;
    m_y *= v.m_y;
    m_z *= v.m_z;
    return *this;
}

Vector3D Vector3D::operator*(float f)
{
    return Vector3D(m_x * f,
                    m_y * f,
                    m_z * f);
}

Vector3D Vector3D::operator*(const Vector3D& v)
{
    return Vector3D(m_x * v.m_x,
                    m_y * v.m_y,
                    m_z * v.m_z);
}

Vector3D& Vector3D::operator+=(const Vector3D& v)
{
    m_x += v.m_x;
    m_y += v.m_y;
    m_z += v.m_z;
    return *this;
}

Vector3D& Vector3D::operator-=(const Vector3D& v)
{
    m_x -= v.m_x;
    m_y -= v.m_y;
    m_z -= v.m_z;
    return *this;
}

Vector3D& Vector3D::operator/=(const Vector3D& rhs)
{
    m_x /= rhs.m_x;
    m_y /= rhs.m_y;
    m_z /= rhs.m_z;
    return *this;
}

Vector3D& Vector3D::operator+=(float f)
{
    m_x += f;
    m_y += f;
    m_z += f;
    return *this;
}

Vector3D& Vector3D::operator-=(float f)
{
    m_x -= f;
    m_y -= f;
    m_z -= f;
    return *this;
}

bool Vector3D::operator==(const Vector3D& rkVector) const
{
    return (m_x == rkVector.m_x && m_y == rkVector.m_y && m_z == rkVector.m_z);
}

bool Vector3D::operator!=(const Vector3D& rkVector) const
{
    return (m_x != rkVector.m_x || m_y != rkVector.m_y || m_z != rkVector.m_z);
}

Vector3D& Vector3D::operator=(const Vector3D& v)
{
    m_x = v.m_x;
    m_y = v.m_y;
    m_z = v.m_z;
    return *this;
}


FLOAT CommonMath::DegreesToRadians(FLOAT fDegrees)
{
    return fDegrees * DEG_TO_RAD;
}

FLOAT CommonMath::RadiansToDegrees(FLOAT fRadians)
{
    return fRadians * RAD_TO_DEG;
}

FLOAT CommonMath::Clamp(const FLOAT fValue, const FLOAT fMin, const FLOAT fMax)
{
    if (fValue < fMin)
    {
        return fMin;
    }

    if (fValue > fMax)
    {
        return fMax;
    }

    return fValue;
}

BOOL CommonMath::IsInCircle(Vector3D tTarPos, FLOAT fTarRadius, Vector3D tCirclePos, FLOAT fCircleRadius)
{
    return tTarPos.Distance2D(tCirclePos) < fCircleRadius + fTarRadius;
}

Circle2D::Circle2D(FLOAT fX, FLOAT fY, FLOAT fRadius)
{
    m_CenterPos.m_x = fX;
    m_CenterPos.m_y = fY;
    m_fRadius = fRadius;
}

Circle2D::Circle2D(CPoint2D pt, FLOAT fRadius)
{
    m_CenterPos = pt;
    m_fRadius = fRadius;
}

VOID Circle2D::Init(FLOAT fX, FLOAT fY, FLOAT fRadius)
{
    m_CenterPos.m_x = fX;
    m_CenterPos.m_y = fY;
    m_fRadius = fRadius;
}

BOOL Circle2D::Contains(CPoint2D pt)
{
    return pt.Distance(pt) < m_fRadius;
}

BOOL Circle2D::Intersects(Rect2D rcArea)
{
    return TRUE;
}

VOID Circle2D::Reset()
{
    m_CenterPos.m_x = 0;
    m_CenterPos.m_y = 0;
    m_fRadius = 0;
}
