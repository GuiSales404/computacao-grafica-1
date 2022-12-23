#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "object.hpp"

class Sphere : public Object  {
    private:
        double radius;
        
    public:
        Sphere();
        Sphere(Vector i, double j, Color kd, Color ka, Color ke, double s, int obj_type = 3);
        Sphere(Vector i, double j, const char * name, double s, int obj_type = 3);

        std::tuple<double, Vector> intersect(Vector Obs, Vector Ray, double t_min, double t_max);
        Vector get_normal(Vector Obs, Vector Ray, double &t);
        void transform();
        void update_normals();
        void update_normals(Matrix M);
        void scaling(double size);


        Vector get_center();
        void set_center(Vector center);
        double get_radius();
        void set_radius(double radius);
};

#endif