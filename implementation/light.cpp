#include <cmath>
#include "../header/light.hpp"

Light::Light(Color intensity) : intensity(intensity){}

void Light::transform() {

}

double Light::specular(Vector N, Vector L, Vector V, double s) {
    double fs = 0.0;
    if(s > 0.0) {
        Vector R = (N*(N*L))*2 - L;
        fs = R*V;
        fs =  pow(fs, s);
    }
    return fs;
}

void Light::world_to_camera(Matrix wc) {
    this->transformations.push_back(wc);
    this->transform();
}

void Light::camera_to_world(Matrix cw) {
    this->transformations.push_back(cw);
    this->transform();
}

std::vector<Matrix> Light::get_transformation() { return this->transformations; }
void Light::clear_transform() { this->transformations.clear(); }

Color Light::get_intensity(){ return intensity; }
void Light::set_intensity(Color intensity){ this->intensity = intensity; }
