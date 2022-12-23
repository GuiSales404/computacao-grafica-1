
#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "object.hpp"

class Light {
    private:
        Color intensity;
        std::vector<Matrix> transformations;

    public:
        Light(Color i);
        virtual Color calculate_intensity(Vector P, Vector N, Vector V, double s, Object* o, bool has_shadow) = 0;
        virtual Vector get_l(Vector P) = 0;
        double specular(Vector N, Vector L, Vector V, double s);

        virtual void transform() = 0;
        void clear_transform();

        Color get_intensity();
        void set_intensity(Color intensity);
        void world_to_camera(Matrix wc);
        void camera_to_world(Matrix cw);
        std::vector<Matrix> get_transformation();
};

#endif