#include "../header/cube.hpp"

Cube::Cube() {}
Cube::Cube(Color kd, Color ka, Color ke, double s, int obj_type): size(1), Mesh(Vector(0, 0, 0, 1), kd, ka, ke, s, obj_type) { 
    set_faces(); 
    this->sizex = 1;
    this->sizey = 1;
    this->sizez = 1;
}

void Cube::set_dim(double x, double y, double z) {
    this->sizex *= x;
    this->sizey *= y;
    this->sizez *= z;
}

std::array<double, 3> Cube::get_dim() const {
    return {sizex, sizey, sizez};
}

Cube::Cube(const char* name, double s, int obj_type): size(1), Mesh(Vector(0, 0, 0, 1), name, s, obj_type) { 
    set_faces(); 
    this->sizex = 1;
    this->sizey = 1;
    this->sizez = 1;
}

void Cube::set_faces() {
    std::vector<Vector*> vertices = {  
        new Vector(this->center.get_x() - this->size/2, this->center.get_y() - this->size/2, this->center.get_z() + this->size/2, 1.),
        new Vector(this->center.get_x() + this->size/2, this->center.get_y() - this->size/2, this->center.get_z() + this->size/2, 1.),
        new Vector(this->center.get_x() + this->size/2, this->center.get_y() + this->size/2, this->center.get_z() + this->size/2, 1.),
        new Vector(this->center.get_x() - this->size/2, this->center.get_y() + this->size/2, this->center.get_z() + this->size/2, 1.),
        new Vector(this->center.get_x() - this->size/2, this->center.get_y() + this->size/2, this->center.get_z() - this->size/2, 1.),
        new Vector(this->center.get_x() - this->size/2, this->center.get_y() - this->size/2, this->center.get_z() - this->size/2, 1.),
        new Vector(this->center.get_x() + this->size/2, this->center.get_y() - this->size/2, this->center.get_z() - this->size/2, 1.),
        new Vector(this->center.get_x() + this->size/2, this->center.get_y() + this->size/2, this->center.get_z() - this->size/2, 1.)
    };

    this->faces = {
        new Face(vertices[0], vertices[1], vertices[3]), new Face(vertices[1], vertices[2], vertices[3]),
        new Face(vertices[1], vertices[6], vertices[2]), new Face(vertices[2], vertices[6], vertices[7]),
        new Face(vertices[5], vertices[7], vertices[6]), new Face(vertices[5], vertices[4], vertices[7]),
        new Face(vertices[0], vertices[4], vertices[5]), new Face(vertices[3], vertices[4], vertices[0]),
        new Face(vertices[4], vertices[2], vertices[7]), new Face(vertices[4], vertices[3], vertices[2]),
        new Face(vertices[0], vertices[5], vertices[1]), new Face(vertices[1], vertices[5], vertices[6])
    };

    this->vertices = vertices;
}

void Cube::set_size(double size) { this->size = size; }
int Cube::get_size() { return this->size; }


std::array<double, 3> Cube::get_sizexyz() {
    auto p        = *this->vertices[0];
    double dist_x = 2.*fabs(this->center.get_x() - p.get_x());
    double dist_y = 2.*fabs(this->center.get_y() - p.get_y());
    double dist_z = 2.*fabs(this->center.get_z() - p.get_z());

    return {dist_x, dist_y, dist_z};
}