#include "../header/ambient_light.hpp"

AmbientLight::AmbientLight(Color intensity) : Light(intensity){} 

Color AmbientLight::calculate_intensity(Vector P, Vector N, Vector V, double s,  Object* o, bool has_shadow){ 
    Color ka = o->has_image() ? o->get_current_color() : o->get_ka();
    return this->get_intensity() * ka; 
}

void AmbientLight::transform() {
    
}

Vector AmbientLight::get_l(Vector P) { return Vector(); }
