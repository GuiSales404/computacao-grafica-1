#ifndef LIGHT_DIRECTION
#define LIGHT_DIRECTION

#include "light.hpp"

class LightDirection : public Light {
    private:
        Vector direction; 
        
    public:
        LightDirection(Color intensity, Vector direction);

        Color calculate_intensity(Vector P, Vector N, Vector V, double s,  Object* o, bool has_shadow);
        
        Vector get_l(Vector P);

        void transform();

        Vector get_direction();
        void set_direction(Vector direction);
};

#endif