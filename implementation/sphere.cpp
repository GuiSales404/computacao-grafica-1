#include <cmath>
#include "../header/sphere.hpp"

Sphere::Sphere() {}
Sphere::Sphere(Vector center, double r, Color kd, Color ka, Color ke, double s, int obj_type) : radius(r), Object(center, kd, ka, ke, s, obj_type){}
Sphere::Sphere(Vector center, double r, const char * name, double s, int obj_type) : radius(r), Object(center, name, s, obj_type) {}

std::tuple<double, Vector> Sphere::intersect(Vector O, Vector D, double t_min, double t_max){
    Vector CO = O - this->center;
    double t = INF;
    Vector n;

    double a = D*D;
    double b = (CO*D);
    double c = CO*CO - this->radius*this->radius;
    double delta = b*b - a*c;
    
    if(delta < 0) return {INF, n};        
    double t1 = (-b + sqrt(delta))/(a);
    double t2 = (-b - sqrt(delta))/(a);

    if(t1-EPS > t_min && t1 < t_max && t1 < t) t = t1, n = this->get_normal(O, D, t1);
    if(t2-EPS > t_min && t2 < t_max && t2 < t) t = t2, n = this->get_normal(O, D, t2);

    return {t, n};
}

Vector Sphere::get_normal(Vector O, Vector D, double &t) {
    Vector P = O + D*t;
    return (P - center)/ radius;
}

void Sphere::transform() {
    Matrix M = Matrix::identity(4);
    for(Matrix m:this->get_transformation()) M = M * m;

    this->center = (M * Matrix::vector_to_matrix(this->center)).matrix_to_vector();
    
    this->clear_transform();
}

void Sphere::update_normals() {}
void Sphere::update_normals(Matrix m) {}

void Sphere::scaling(double size) {
    radius *= size;
}

Vector Sphere::get_center() { return this->center; }
void Sphere::set_center(Vector center){ this->center = center; }
double Sphere::get_radius(){ return this->radius; }
void Sphere::set_radius(double radius){ this->radius = radius; }
