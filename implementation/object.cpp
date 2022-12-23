#include <cmath>
#include <iostream>
#include "../header/object.hpp"
#include "../header/stb_image.hpp"
#include "../header/stb_image_write.hpp"
#include "../header/cube.hpp"

Object::Object(){}
Object::Object(Vector center, Color kd, Color ka, Color ke, double s, int obj_type) : center(center), kd(kd), ka(ka), ke(ke),
                                                                                      specular(s), obj_type(obj_type) {}

Object::Object(Vector center, const char* name, double s, int obj_type) : obj_type(obj_type) {
    this->center   = center;
    this->has_img  = true;
    this->specular = s;
    int8* image = stbi_load(name, &this->width, &this->height, &this->channel, 0);
    if(image == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }

    this->image_pixels.resize(this->height, std::vector<Color>(this->width));

    double r, g, b;
    for(int i = 0, c = 0; i < this->height; i++)  {
        for(int j = 0; j < this->width; j++) {
            r = 1.0 * image[c++]/255, g = 1.0 * image[c++]/255, b = 1.0 * image[c++]/255;
            this->image_pixels[i][j] = Color(r, g, b);
        }
    }
    stbi_image_free(image);
}

void Object::push_point_color(point_color pc) {
    this->obj_pixels.push_back(pc);
}

std::vector<point_color>& Object::get_obj_pixels() {
    return this->obj_pixels;
}

void Object::set_kd(Color color){ this->kd = kd; }
Color Object::get_kd(){ return kd; }
void Object::set_ka(Color color){ this->ka = ka; }
Color Object::get_ka(){ return ka; }
void Object::set_ke(Color color){ this->ke = ke; }
Color Object::get_ke(){ return ke; }

double Object::get_specular(){ return specular; }
void Object::set_specular(double specular){ this->specular = specular; }

std::vector<Matrix> Object::get_transformation() { return this->transformations; }
void Object::clear_transform() { this->transformations.clear(); }


void Object::rotation_x(double angle) {
    Matrix r_x = Matrix::rotate_around_x(angle);
    this->transformations.push_back(r_x);
    this->transform();
    this->update_normals();
}
void Object::rotation_y(double angle) {
    Matrix r_y = Matrix::rotate_around_y(angle);
    this->transformations.push_back(r_y);
    this->transform();
    this->update_normals();
}
void Object::rotation_z(double angle) {
    Matrix r_z = Matrix::rotate_around_z(angle);
    this->transformations.push_back(r_z);
    this->transform();
    this->update_normals();
}

void Object::scaling(double x, double y, double z) {
    if(this->obj_type == 2) {
        ((Cube*)this)->set_dim(x, y, z);
    }

    Matrix scaling = Matrix::scaling(x, y, z);
    this->transformations.push_back(scaling);
    this->transform();
    if(x != y && x != z && z != y) this->update_normals(scaling);
    else this->update_normals();
}
void Object::scaling(double size) {
    if(this->obj_type == 2) {
        ((Cube*)this)->set_dim(size, size, size);
    }
    Matrix scaling = Matrix::scaling(size, size, size);
    this->transformations.push_back(scaling);
    this->transform();
    this->update_normals();
}
void Object::shearing_xy(double angle) {
    Matrix shear = Matrix::shearing_matrix_xy(angle);
    this->transformations.push_back(shear);
    this->transform();
    this->update_normals();
}
void Object::shearing_xz(double angle) {
    Matrix shear = Matrix::shearing_matrix_xz(angle);
    this->transformations.push_back(shear);
    this->transform();
    this->update_normals();
}

int Object::get_type() const {
    return this->obj_type;
}

void Object::shearing_yx(double angle) {
    Matrix shear = Matrix::shearing_matrix_yx(angle);
    this->transformations.push_back(shear);
    this->transform();
    this->update_normals();
}

void Object::shearing_yz(double angle) {
    Matrix shear = Matrix::shearing_matrix_yz(angle);
    this->transformations.push_back(shear);
    this->transform();
    this->update_normals();
}

void Object::shearing_zx(double angle) {
    Matrix shear = Matrix::shearing_matrix_zx(angle);
    this->transformations.push_back(shear);
    this->transform();
    this->update_normals();
}

void Object::shearing_zy(double angle) {
    Matrix shear = Matrix::shearing_matrix_zy(angle);
    this->transformations.push_back(shear);
    this->transform();
    this->update_normals();
}

void Object::reflection_xy() {
    this->invert = this->invert * -1;
    Matrix reflection = Matrix::reflection_matrix_xy();
    this->transformations.push_back(reflection);  
    this->transform();
    this->update_normals();
}

void Object::reflection_yz() {
    this->invert = this->invert * -1;
    Matrix reflection = Matrix::reflection_matrix_yz();
    this->transformations.push_back(reflection);
    this->transform();
    this->update_normals();
}

void Object::reflection_xz() {
    this->invert = this->invert * -1;
    Matrix reflection = Matrix::reflection_matrix_xz();
    this->transformations.push_back(reflection);
    this->transform();
    this->update_normals();
}

void Object::reflection_at(Vector p, Vector n) {
    this->invert = this->invert * -1;
    Matrix reflection = Matrix::reflection_matrix_at(p, n);
    this->transformations.push_back(reflection);
    this->transform();
    this->update_normals();
}

void Object::translate(Vector v) {
    Matrix translate = Matrix::translation(v - center);
    this->transformations.push_back(translate);
    this->transform();
}

void Object::world_to_camera(Matrix wc) {
    this->transformations.push_back(wc);
    this->transform();
    this->update_normals();
}
void Object::camera_to_world(Matrix cw) {
    this->transformations.push_back(cw);
    this->transform();
    this->update_normals();
}

void Object::set_transformation(Matrix m) { this->transformations.push_back(m); }

int Object::get_invert() { return this->invert; }

void Object::change_color(Color color) { this->ka = this->ke = this->kd = color; }

// imagens

Vector Object::get_center() { return this->center; }
void Object::set_center(Vector c) { this->center = c; }
bool Object::has_image() { return this->has_img; }
int Object::get_width() { return this->width; }
void Object::set_width(int w) { this->width = w; }
int Object::get_height() { return this->height; }
void Object::set_height(int h) { this->height = h; }
void Object::set_current_color(int i, int j) { this->current_color = this->image_pixels[i%this->height][j%this->width]; }
Color Object::get_current_color() { return this->current_color; }