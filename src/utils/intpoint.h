/** Copyright (C) 2013 David Braam - Released under terms of the AGPLv3 License */
#ifndef INT_POINT_H
#define INT_POINT_H

/**
The integer point classes are used as soon as possible and represent microns in 2D or 3D space.
Integer points are used to avoid floating point rounding errors.
*/
#define INLINE static inline


#include <limits>
#include <stdint.h>
#include <cmath>

#include <iostream> // auto-serialization / auto-toString()

#include <string>       // std::string
#include <sstream>      // std::stringstream

#define INT2MM(n) (double(n) / 1000.0)
#define MM2INT(n) (int64_t((n) * 1000))

#define INT2MICRON(n) ((n) / 1)
#define MICRON2INT(n) ((n) * 1)

//c++11 no longer defines M_PI, so add our own constant.
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifdef __GNUC__
#define DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED(func) func
#endif


class Point3
{
public:
    int32_t x,y,z;
    Point3() {}
    Point3(const int32_t _x, const int32_t _y, const int32_t _z): x(_x), y(_y), z(_z) {}
    //Point3(Point3d& other): Point3(other.x, other.y, other.z) {} //!< down-casting

    Point3 operator+(const Point3& p) const { return Point3(x+p.x, y+p.y, z+p.z); }
    Point3 operator-(const Point3& p) const { return Point3(x-p.x, y-p.y, z-p.z); }
    Point3 operator/(const int32_t i) const { return Point3(x/i, y/i, z/i); }
    Point3 operator*(const int32_t i) const { return Point3(x*i, y*i, z*i); }

    Point3& operator += (const Point3& p) { x += p.x; y += p.y; z += p.z; return *this; }
    Point3& operator -= (const Point3& p) { x -= p.x; y -= p.y; z -= p.z; return *this; }
    Point3& operator *= (int32_t c) { x *= c; y *= c; z *= c; return *this; }
    Point3& operator /= (int32_t c) { x /= c; y /= c; z /= c; return *this; }

    bool operator==(const Point3& p) const { return x==p.x&&y==p.y&&z==p.z; }
    bool operator!=(const Point3& p) const { return x!=p.x||y!=p.y||z!=p.z; }


    template<class CharT, class TraitsT>
    friend
    std::basic_ostream<CharT, TraitsT>&
    operator <<(std::basic_ostream<CharT, TraitsT>& os, const Point3& p)
    {
        return os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    }

    std::string toString()
    {
        std::stringstream ss;
        ss << this;
        return ss.str();
    }


    int32_t max()
    {
        if (x > y && x > z) return x;
        if (y > z) return y;
        return z;
    }

    bool testLength(int32_t len)
    {
        if (x > len || x < -len)
            return false;
        if (y > len || y < -len)
            return false;
        if (z > len || z < -len)
            return false;
        return vSize2() <= len*len;
    }

    int64_t vSize2() const
    {
        return int64_t(x)*int64_t(x)+int64_t(y)*int64_t(y)+int64_t(z)*int64_t(z);
    }

    int32_t vSize() const
    {
        return sqrt(vSize2());
    }

    /* this function WAS deprecated because it COULD cause overflows for vectors which easily fit inside a printer. Use FPoint3.cross(a,b) instead. */
    Point3 cross(const Point3& p)
    {
        return Point3(
            int64_t(y)*p.z - int64_t(z)*p.y,
            int64_t(z)*p.x - int64_t(x)*p.z,
            int64_t(x)*p.y - int64_t(y)*p.x  );
    }

    int64_t dot(const Point3& p)
    {
        return x*p.x + y*p.y + z*p.z;
    }

};

inline Point3 operator*(const int32_t i, const Point3& rhs) {
    return rhs * i;
}


class Point3d
{
public:
    int64_t x,y,z;
    Point3d() {}
    Point3d(const int64_t _x, const int64_t _y, const int64_t _z): x(_x), y(_y), z(_z) {}
    Point3d(Point3& other): Point3d(other.x, other.y, other.z) {} //!< up-casting
    Point3 downCast() { return Point3(x,y,z); };

    Point3d operator+(const Point3d& p) const { return Point3d(x+p.x, y+p.y, z+p.z); }
    Point3d operator-(const Point3d& p) const { return Point3d(x-p.x, y-p.y, z-p.z); }
    Point3d operator/(const int64_t i) const { return Point3d(x/i, y/i, z/i); }
    Point3d operator*(const int64_t i) const { return Point3d(x*i, y*i, z*i); }

    Point3d& operator += (const Point3d& p) { x += p.x; y += p.y; z += p.z; return *this; }
    Point3d& operator -= (const Point3d& p) { x -= p.x; y -= p.y; z -= p.z; return *this; }
    Point3d& operator *= (int64_t c) { x *= c; y *= c; z *= c; return *this; }
    Point3d& operator /= (int64_t c) { x /= c; y /= c; z /= c; return *this; }

    bool operator==(const Point3d& p) const { return x==p.x&&y==p.y&&z==p.z; }
    bool operator!=(const Point3d& p) const { return x!=p.x||y!=p.y||z!=p.z; }


    template<class CharT, class TraitsT>
    friend
    std::basic_ostream<CharT, TraitsT>&
    operator <<(std::basic_ostream<CharT, TraitsT>& os, const Point3d& p)
    {
        return os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    }


    int64_t max()
    {
        if (x > y && x > z) return x;
        if (y > z) return y;
        return z;
    }

//    bool testLength(int64_t len)
//    {
//        if (x > len || x < -len)
//            return false;
//        if (y > len || y < -len)
//            return false;
//        if (z > len || z < -len)
//            return false;
//        return vSize2() <= len*len;
//    }

//    int128_t vSize2() const
//    {
//        return int128_t(x)*int128_t(x)+int128_t(y)*int128_t(y)+int128_t(z)*int128_t(z);
//    }

//    int64_t vSize() const
//    {
//        return sqrt(vSize2());
//    }

    /* this function WAS deprecated because it COULD cause overflows */
    Point3d cross(const Point3d& p)
    {
        return Point3d(
            (long long)(y)*p.z - (long long)(z)*p.y,
            (long long)(z)*p.x - (long long)(x)*p.z,
            (long long)(x)*p.y - (long long)(y)*p.x  );
    }


    /*! this function is deprecated because it can cause overflows */
    DEPRECATED(int64_t dot(const Point3& p))
    {
        return x*p.x + y*p.y + z*p.z;
    }

};


inline Point3d operator*(const int64_t i, const Point3d& rhs) {
    return rhs * i;
}

/*
// 64bit Points are used mostly troughout the code, these are the 2D points
class IntPoint {
public:
    int X, Y;
};
// Extra operators to make it easier to do math with the 64bit Point objects
INLINE Point operator-(const Point& p0) { return Point(-p0.X, -p0.Y); }
INLINE Point operator+(const Point& p0, const Point& p1) { return Point(p0.X+p1.X, p0.Y+p1.Y); }
INLINE Point operator-(const Point& p0, const Point& p1) { return Point(p0.X-p1.X, p0.Y-p1.Y); }
INLINE Point operator*(const Point& p0, const int32_t i) { return Point(p0.X*i, p0.Y*i); }
INLINE Point operator/(const Point& p0, const int32_t i) { return Point(p0.X/i, p0.Y/i); }

INLINE Point& operator += (Point& p0, const Point& p1) { p0.X += p1.X; p0.Y += p1.Y; return p0; }
INLINE Point& operator -= (Point& p0, const Point& p1) { p0.X -= p1.X; p0.Y -= p1.Y; return p0; }

INLINE bool operator==(const Point& p0, const Point& p1) { return p0.X==p1.X&&p0.Y==p1.Y; }
INLINE bool operator!=(const Point& p0, const Point& p1) { return p0.X!=p1.X||p0.Y!=p1.Y; }

INLINE int64_t vSize2(const Point& p0)
{
    return p0.X*p0.X+p0.Y*p0.Y;
}
INLINE float vSize2f(const Point& p0)
{
    return float(p0.X)*float(p0.X)+float(p0.Y)*float(p0.Y);
}

INLINE bool shorterThen(const Point& p0, int32_t len)
{
    if (p0.X > len || p0.X < -len)
        return false;
    if (p0.Y > len || p0.Y < -len)
        return false;
    return vSize2(p0) <= len*len;
}

INLINE int64_t vSize(const Point& p0)
{
    return sqrt(vSize2(p0));
}

INLINE double vSizeMM(const Point& p0)
{
    double fx = INT2MM(p0.X);
    double fy = INT2MM(p0.Y);
    return sqrt(fx*fx+fy*fy);
}

INLINE Point normal(const Point& p0, int64_t len)
{
    int64_t _len = vSize(p0);
    if (_len < 1)
        return Point(len, 0);
    return p0 * len / _len;
}

INLINE Point crossZ(const Point& p0)
{
    return Point(-p0.Y, p0.X);
}
INLINE int64_t dot(const Point& p0, const Point& p1)
{
    return p0.X * p1.X + p0.Y * p1.Y;
}

INLINE int angle(const Point& p)
{
    double angle = std::atan2(p.X, p.Y) / M_PI * 180.0;
    if (angle < 0.0) angle += 360.0;
    return angle;
}

class PointMatrix
{
public:
    double matrix[4];

    PointMatrix()
    {
        matrix[0] = 1;
        matrix[1] = 0;
        matrix[2] = 0;
        matrix[3] = 1;
    }

    PointMatrix(double rotation)
    {
        rotation = rotation / 180 * M_PI;
        matrix[0] = cos(rotation);
        matrix[1] = -sin(rotation);
        matrix[2] = -matrix[1];
        matrix[3] = matrix[0];
    }

    PointMatrix(const Point p)
    {
        matrix[0] = p.X;
        matrix[1] = p.Y;
        double f = sqrt((matrix[0] * matrix[0]) + (matrix[1] * matrix[1]));
        matrix[0] /= f;
        matrix[1] /= f;
        matrix[2] = -matrix[1];
        matrix[3] = matrix[0];
    }

    Point apply(const Point p) const
    {
        return Point(p.X * matrix[0] + p.Y * matrix[1], p.X * matrix[2] + p.Y * matrix[3]);
    }

    Point unapply(const Point p) const
    {
        return Point(p.X * matrix[0] + p.Y * matrix[2], p.X * matrix[1] + p.Y * matrix[3]);
    }
};

*/

#endif//INT_POINT_H
