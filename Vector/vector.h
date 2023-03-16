#pragma once
#include <iostream>

using namespace std;

class Vector
{
	friend ostream& operator<<(ostream& os, const Vector& v);
public:
	Vector();
	Vector(double x, double y, double z);

	Vector operator+(const Vector& v) const;
	Vector operator-(const Vector& v) const;
	double operator*(const Vector& v) const;
	Vector operator^(const Vector& v) const;

	double length() const;
	void normalize();

private:
	double x, y, z;
};
