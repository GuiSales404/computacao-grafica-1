#ifndef CANVA_HPP
#define CANVA_HPP

#include "color.hpp"
#include "object.hpp"
#include <vector>

typedef unsigned char int8;

class Canva {
    private:
        int w, h;
        std::vector<std::vector<Color>> pixels; 
        std::vector<std::vector<Object*>> z_buffer;
        Color background;
    
    public:
        Canva();
        Canva(int w, int h, Color bg);
        void write_image(const char* image_name);
        void to_color(int i, int j, Color color);

        int get_w();
        void set_w(int width);
        int get_h();    
        void set_h(int height);
        Color get_background_color();
        void set_background_color(Color background_color);
        Color get_pixel(int i, int j);
        Object* get_object(int i, int j);
        void to_buffer(int i, int j, Object* o);
        std::vector<std::vector<Color>>& get_pixels();
};

#endif