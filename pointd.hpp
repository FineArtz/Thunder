/*
 * Coded by Jingxiao Chen.
 *
 */

#ifndef POINTD_HPP
#define POINTD_HPP

#include <cmath>

struct PointD
{
	PointD( const double &X=0, const double &Y=0 )
		:x(X),y(Y){}
	double x,y;
	double length()
	{
		return sqrt(x*x+y*y);
	}
	void rotate( double rad )
	{
		double a,b;
		a = x;
		b = y;
		x = a*cos(rad) - b*sin(rad);
		y = a*sin(rad) + b*cos(rad);
	}
};
PointD operator + (const PointD &a, const PointD &b)
{
	return PointD(a.x+b.x, a.y+b.y);
}
PointD operator - (const PointD &a, const PointD &b)
{
	return PointD(a.x-b.x, a.y-b.y);
}
PointD operator * (const PointD &a, const double &b)
{
	return PointD(a.x*b, a.y*b);
}
PointD operator / (const PointD &a, const double &b)
{
	return PointD(a.x/b, a.y/b);
}

double cross( const PointD &a, const PointD &b )
{
	return a.x*b.y - a.y*b.x;
}
double dot( const PointD &a, const PointD &b )
{
	return a.x*b.x + a.y*b.y;
}

#endif
