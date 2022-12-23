#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

class Viewport {
private:
    double w, h, d;

public:
    Viewport(double vp_w, double vp_h, double vp_d); //width, heigh, distance
    Viewport();

    double get_h();
    void set_h(double h);
    double get_w();
    void set_w(double w);
    double get_d();
    void set_d(double d);
};

#endif