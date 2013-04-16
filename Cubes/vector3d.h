#pragma once

class Vector3D{
	double x,y,z;//разложение по единичным вектрорам
public:
	Vector3D(double x,double y, double z);
	Vector3D operator+(Vector3D);
	Vector3D operator-(Vector3D);
	Vector3D operator*(Vector3D);
	double scaleMult(Vector3D);
	double getX();
	double getY();
	double getZ();
	double getLength();
	void setX(double x);
	void setY(double y);
	void setZ(double z);
	void setNewLength(double length);
};