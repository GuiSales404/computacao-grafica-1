#include "../header/viewport.hpp"

Viewport::Viewport(){ h = w = d = 0; }
Viewport::Viewport(double h, double w, double d) : h(h), w(w), d(d){}

double Viewport::get_h(){ return this->h; }
void Viewport::set_h(double h){ this->h = h; }
double Viewport::get_w(){ return this->w; }
void Viewport::set_w(double w){ this->w = w; }
double Viewport::get_d(){ return this->d; }
void Viewport::set_d(double d){ this->d = d; }
