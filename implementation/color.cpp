#include "../header/color.hpp"
#include <algorithm>

Color::Color(){ r = g = b = 0; } 
Color::Color(double r, double g, double b): r(r), g(g), b(b){}

Color Color::operator*(double alp){ return Color(alp*this->r, alp*this->g, alp*this->b); }
Color Color::operator*(Color c){ return Color(c.get_r()*this->r, c.get_g()*this->g, c.get_b()*this->b); }
Color Color::operator/(double n){ return Color(this->r/n, this->g/n, this->b/n); }
Color Color::operator+(Color c){ return Color(c.get_r() + this->r, c.get_g() + this->g, c.get_b() + this->b); }

unsigned int Color::to_red() { return std::min(r, 1.0) * 255;}
unsigned int Color::to_green(){ return std::min(g, 1.0) * 255; }
unsigned int Color::to_blue(){ return std::min(b, 1.0) * 255; }

double Color::get_r(){ return this->r; }
void Color::set_r(double r) { this->r = r; }
double Color::get_g(){ return this->g; }
void Color::set_g(double g) { this->g = g; }
double Color::get_b(){ return this->b; }
void Color::set_b(double b) { this->b = b; }
