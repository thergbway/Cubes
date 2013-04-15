#pragma once
#include <cmath>
#include "vector3d.h"

Vector3D::Vector3D(double _x,double _y, double _z){
	x=_x;
	y=_y;
	z=_z;
}

Vector3D Vector3D::operator+(Vector3D vec){
	Vector3D result(x+vec.x,y+vec.y,z+vec.z);
	return result;
}

Vector3D Vector3D::operator-(Vector3D vec){
	Vector3D result(x-vec.x,y-vec.y,z-vec.z);
	return result;
}

Vector3D Vector3D::operator*(Vector3D vec){
	Vector3D result(0,0,0);//это не итог
	result.setX(y*vec.z-z*vec.y);
	result.setY(z*vec.x-x*vec.z);
	result.setZ(x*vec.y-y*vec.x);
	return result;
}

double Vector3D::getX(){
	return x;
}

double Vector3D::getY(){
	return y;
}

double Vector3D::getZ(){
	return z;
}

double Vector3D::getLength(){
	double length=sqrt(x*x+y*y+z*z);
	return length;
}

void Vector3D::setX(double _x){
	x=_x;
}

void Vector3D::setY(double _y){
	y=_y;
}

void Vector3D::setZ(double _z){
	z=_z;
}

void Vector3D::setNewLength(double newlength){
	//нормируем
	double oldLength=getLength();
	x/=oldLength;
	y/=oldLength;
	z/=oldLength;

	//делаем новую длину
	x*=newlength;
	y*=newlength;
	z*=newlength;
}
