#include "../header/light_direction.hpp"

LightDirection::LightDirection(Color intensity, Vector direction) : Light(intensity), direction(direction){direction.set_a(1);}

Color LightDirection::calculate_intensity(Vector P, Vector N, Vector V, double s,  Object* o, bool has_shadow){ 
    Color i;
    if(has_shadow) return i;

    Color kd = o->has_image() ? o->get_current_color() : o->get_kd(), 
          ke = o->has_image() ? o->get_current_color() : o->get_ke();

    Vector L = get_direction();  
    L = L/~L;
    double fd = N*L;
    if(fd > 0.0) i = i + ((this->get_intensity()) * kd) * fd;

    double fs = specular(N, L, V, s);
    if(fs > 0.0) i = i + ((this->get_intensity()) * ke) * fs;

    return i;
}

void LightDirection::transform() {
    Matrix M = Matrix::identity(4);
    for(Matrix m:this->get_transformation()) M = M * m;

    this->direction = (M * Matrix::vector_to_matrix(this->direction, 0)).matrix_to_vector();  
    this->direction = this->direction / ~this->direction;
    
    this->clear_transform();
}

Vector LightDirection::get_l(Vector P) { return this->direction; }
Vector LightDirection::get_direction(){ return this->direction; }
void LightDirection::set_direction(Vector direction) { this->direction = direction; }
