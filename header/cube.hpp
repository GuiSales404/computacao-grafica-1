#ifndef CUBE_HPP
#define CUBE_HPP

#include "vector.hpp"
#include "mesh.hpp"

class Cube: public Mesh {
    private:
        double size;
        double sizex, sizey, sizez;
        
    public:
        Cube();
        Cube(Color kd, Color ka, Color ke, double s, int obj_type = 2);
        Cube(const char * name, double s, int obj_type = 2);

        std::array<double, 3> get_sizexyz();

        void set_size(double size);
        int get_size();
        void set_dim(double x, double y, double z);
        std::array<double, 3> get_dim() const;

        void set_faces();
};

#endif