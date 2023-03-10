#pragma once
#include <iostream>

using namespace std;

class Vector
{
public:
	Vector();
	Vector(double x, double y, double z);

	Vector operator+(const Vector& v) const;
	Vector operator-(const Vector& v) const;
	double operator*(const Vector& v) const;
	Vector operator^(const Vector& v) const;

	double length() const;
	void normalize();

	friend ostream& operator<<(ostream& os, const Vector& v);

private:
	double x, y, z;
};
