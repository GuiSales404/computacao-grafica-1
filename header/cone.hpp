#ifndef CONE_HPP
#define CONE_HPP

#include "object.hpp"

class Cone : public Object {
    private:
        Vector V, dc; //vertice do cone e direcao de crescimento
        double radius, height; 
        bool has_base;

        //diferenciacao p/ interesccao na casca e na base

        bool in_shell(Vector P); 
        bool in_base(Vector P, Vector PI, Vector DC);
        std::tuple<double, double> intersection_cone_shell_vector(Vector Obs, Vector Ray);
        double ray_intersection_base(Vector Obs, Vector Ray, Vector PI, Vector DC);

    public:
        Cone();
        Cone(Vector c, Vector dc, double r, double h, Color kd, Color ka, Color ke, double s, bool has_base, int obj_type = 1);
        Cone(Vector c, Vector dc, double r, double h, const char * name, double s, bool has_base, int obj_type = 1);
        Cone(Vector c, Vector V, double r, Color kd, Color ka, Color ke, double s, bool has_base, int obj_type = 1);
        Cone(Vector c, Vector V, double r, const char * name, double s, bool has_base, int obj_type = 1);

        std::tuple<double, Vector> intersect(Vector Obs, Vector Ray, double t_min, double t_max);
        Vector get_normal(Vector Obs, Vector Ray, double &t);
        void transform();
        void translate(Vector v);
        void update_normals();
        void update_normals(Matrix M);
        void scaling(double size);

        Vector get_center();
        Vector get_v();
        double get_radius();
        double get_height();

        void set_center(Vector c);
        void set_v(Vector v);
        void set_radius(double r);
        void set_height(double h);
};

#endif