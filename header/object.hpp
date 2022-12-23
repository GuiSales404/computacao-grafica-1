#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "color.hpp"
#include "vector.hpp"
#include "matrix.hpp"
#include <tuple>
#include <cmath>
#include <vector>

typedef unsigned char int8;

struct point_color {
    int i, j;
    Color c;
};

class Object {
    protected:
        int width, height, channel;
        std::vector<std::vector<Color>> image_pixels;

        std::vector<point_color> obj_pixels;

        double specular;
        Color kd, ka, ke;
        Color current_color;
        bool has_img = false;
        Vector center;  
        int obj_type;

        std::vector<Matrix> transformations;

        int invert = 1;
    public:

        Object();
        Object(Vector center, Color kd, Color ka, Color ke, double s, int obj_type);
        Object(Vector center, const char * name, double s, int obj_type);
        void push_point_color(point_color pc);
        std::vector<point_color>& get_obj_pixels();

        virtual std::tuple<double, Vector> intersect(Vector Obs, Vector Ray, double t_min, double t_max) = 0;

        virtual void transform() = 0;
        virtual void update_normals() = 0;
        virtual void update_normals(Matrix M) = 0;

        void change_color(Color color);

        void rotation_x(double angle);
        void rotation_y(double angle);
        void rotation_z(double angle);
        void scaling(double x, double y, double z);
        virtual void scaling(double size);
        void shearing_xy(double angle);
        void shearing_xz(double angle);
        void shearing_yx(double angle);
        void shearing_yz(double angle);
        void shearing_zx(double angle);
        void shearing_zy(double angle);
        void translate(Vector v);
        void reflection_xy();
        void reflection_yz();
        void reflection_xz();
        void reflection_at(Vector p, Vector n);
        void world_to_camera(Matrix wc);
        void camera_to_world(Matrix cw);

        void set_kd(Color color);
        Color get_kd();
        void set_ka(Color color);
        Color get_ka();
        void set_ke(Color color);
        Color get_ke();

        double get_specular();
        void set_specular(double specular);

        int get_width();
        void set_width(int w);

        int get_height();
        void set_height(int h);

        void set_current_color(int i, int j);
        Color get_current_color();

        Vector get_center();
        void set_center(Vector c);

        bool has_image();
        void clear_transform();

        int get_type() const;

        void set_transformation(Matrix m);
        std::vector<Matrix> get_transformation();

        int get_invert();
};

#endif