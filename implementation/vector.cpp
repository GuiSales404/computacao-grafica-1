#include <cmath>
#include <stdio.h>
#include "../header/vector.hpp"

Vector::Vector() : x(0), y(0), z(0), a(0){}
Vector::Vector(double x, double y, double z) : x(x), y(y), z(z), a(1){}
Vector::Vector(double x, double y, double z, double a) : x(x), y(y), z(z), a(a){}

Vector Vector::operator + (Vector v) { return Vector(this->x+v.x, this->y+v.y, this->z+v.z); }
Vector Vector::operator - (Vector v) { return Vector(this->x-v.x, this->y-v.y, this->z-v.z); }
Vector Vector::operator - () { return Vector(-this->x, -this->y, -this->z); }
double Vector::operator * (Vector v) { return this->x*v.x + this->y*v.y + this->z*v.z; }
Vector Vector::operator * (double alp){ return Vector(this->x*alp, this->y*alp, this->z*alp); }
Vector Vector::operator % (Vector n) { return Vector((this->get_y() * n.get_z()) - (this->get_z() * n.get_y()), (this->get_z() * n.get_x()) - (this->get_x() * n.get_z()), (this->get_x() * n.get_y()) - (this->get_y() * n.get_x()));  }
Vector Vector::operator / (double alp) { return Vector(this->x/alp, this->y/alp, this->z/alp, this->a/alp); }
double Vector::operator ~ (){ return sqrt(this->x*this->x + this->y*this->y + this->z*this->z); }

double Vector::get_x(){ return this->x; }
void Vector::set_x(double x){ this->x = x; }
double Vector::get_y(){ return this->y; }
void Vector::set_y(double y){ this->y = y; }
double Vector::get_z(){ return this->z; }
void Vector::set_z(double z){ this->z = z; }
double Vector::get_a(){ return this->a; }
void Vector::set_a(double a){ this->a = a; }

void Vector::print() {
    printf("Vector: %.2f, %.2f, %.2f\n", x, y, z);
}