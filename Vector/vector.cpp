#include "vector.h"

#include <cmath>

Vector::Vector()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

Vector::Vector(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector Vector::operator+(const Vector& v) const
{
	return Vector(x + v.x, y + v.y, z + v.z);
}

Vector Vector::operator-(const Vector& v) const
{
	return Vector(x - v.x, y - v.y, z - v.z);
}

double Vector::operator*(const Vector& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

Vector Vector::operator^(const Vector& v) const
{
	return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

double Vector::length() const
{
	return sqrt(x * x + y * y + z * z);
}

void Vector::normalize()
{
	double len = length();
	if (len > 0)
	{
		x /= len;
		y /= len;
		z /= len;
	}
}

ostream& operator<<(ostream& os, const Vector& v)
{
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}
