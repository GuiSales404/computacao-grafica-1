#ifndef PLAN_HPP
#define PLAN_HPP

#include "object.hpp"

class Plan : public Object {
    private:
        Vector P, N;

    public:
        Plan(Vector P, Vector N, Color kd, Color ka, Color ke, double s, int obj_type = 0);
        Plan(Vector P, Vector N, const char * name, double s, int obj_type = 0);
        Plan(Vector P, Vector N);
        Plan();

        std::tuple<double, Vector> intersect(Vector Obs, Vector Ray, double t_min, double t_max);
        Vector get_normal(Vector Obs, Vector Ray, double &t);
        void transform();
        void update_normals();
        void update_normals(Matrix M);


        void set_p(Vector point);
        Vector get_p();
        void set_n(Vector normal);
        Vector get_n();
};

#endif