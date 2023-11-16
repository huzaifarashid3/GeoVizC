#pragma once
#include "olcPixelGameEngine.h"
using namespace std;
class Point
{
public:
    int x, y;
    olc::vi2d xy;
    Point() : x(0), y(0), xy({ 0,0 }) {}
    Point(int a, int b) : x(a), y(b), xy({ a,b }) {}
    Point(olc::vi2d xy): xy(xy), x(xy.x), y(xy.y) {} 
    Point(const Point& p): x(p.x), y(p.y), xy(p.xy)  {}
    Point& operator=(const Point& p) = default;
    
    Point operator+(const Point& b) const
    {
        return Point(x + b.x, y + b.y);
    }
    void operator+=(const Point& b)
    {
        x += b.x;
        y += b.y;
    }
    Point operator-(const Point& b)
    {
        return  Point(x - b.x, y - b.y);
    }
    void operator-=(const Point& b)
    {
        x -= b.x;
        y -= b.y;
    }
    int operator*(const Point& b) const
    {
        return x * b.y - b.x * y;
    }
    int cross(Point& a, Point& b) const
    {
        return (a - *this) * (b - *this);
    }
    bool operator<(const Point& b) const
    {
        return make_pair(this->x, this->y) < make_pair(b.x, b.y);
    }
};

