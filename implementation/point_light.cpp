#include "../header/light_point.hpp"
#include <iostream>

LightPoint::LightPoint(Color intensity, Vector position) : Light(intensity), position(position){}

Color LightPoint::calculate_intensity(Vector P, Vector N, Vector V, double s,  Object* o, bool has_shadow){ 
    Color i;

    if(has_shadow) return i;

    Color kd = o->has_image() ? o->get_current_color() : o->get_kd(), 
          ke = o->has_image() ? o->get_current_color() : o->get_ke();
          
    Vector L = this->get_position()-P;  
    L = L/~L;
    double fd = N*L;
    if(fd > 0.0) i = i + ((this->get_intensity()) * kd) * fd;

    double fs = specular(N, L, V, s);
    if(fs > 0.0) i = i + ((this->get_intensity()) * ke) * fs;

    return i;
}

void LightPoint::transform() {
    Matrix M = Matrix::identity(4);
    for(Matrix m:this->get_transformation()) M = M * m;

    this->position = (M * Matrix::vector_to_matrix(this->position)).matrix_to_vector();  
    
    this->clear_transform();
}

Vector LightPoint::get_l(Vector P) { return this->position - P; }
Vector LightPoint::get_position(){ return this->position; }
void LightPoint::set_position(Vector position) { this->position = position; }
