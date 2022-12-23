#ifndef SPOTLIGHT_HPP
#define SPOTLIGHT_HPP

#include "light.hpp"

class SpotLight : public Light {
    private:
        Vector direction; 
        Vector position;
        double grau;
        
    public:
        SpotLight(Color intensity, Vector position, Vector direction, double grau);

        Color calculate_intensity(Vector P, Vector N, Vector V, double s,  Object* o, bool has_shadow);
        Vector get_l(Vector P); //posicao da luz

        void transform();
        Vector get_direction();
        void set_direction(Vector direction);

        Vector get_position();
        void set_position(Vector position);
};

#endif