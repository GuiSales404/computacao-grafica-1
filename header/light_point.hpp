#ifndef LIGHT_POINT_HPP
#define LIGHT_POINT_HPP

#include "light.hpp"

class LightPoint : public Light {
    private:
        Vector position; 
        
    public:
        LightPoint(Color intensity, Vector position);
        Color calculate_intensity(Vector P, Vector N, Vector V, double s,  Object* o, bool has_shadow); //point, normal, vector
        Vector get_l(Vector P);
        void transform();
        Vector get_position();
        void set_position(Vector position);
};

#endif