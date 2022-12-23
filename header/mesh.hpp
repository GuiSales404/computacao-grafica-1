#ifndef MESH_HPP
#define MESH_HPP

#include "object.hpp"
#include "plan.hpp"
#include "matrix.hpp"

class Mesh : public Object {
    protected:
        std::vector<Vector*> vertices;

        class Face {
            private:
                Vector *p1, *p2, *p3;
                Vector normal;

                friend Mesh;

            public:
                Face();
                Face(Vector *&p1, Vector *&p2, Vector *&p3);

                bool in_face(Vector P);
                std::tuple<double, Vector> intersect(Vector Obs, Vector Ray);
                void update_normal();

                void set_p1(Vector v);
                void set_p2(Vector v);
                void set_p3(Vector v);

                Vector get_p1();
                Vector get_p2();
                Vector get_p3();

                void set_normal(Vector n);
                Vector get_normal();
        };

        std::vector<Face*> faces;

    public:
        Mesh();
        Mesh(Vector center, Color kd, Color ka, Color ke, double s, int obj_type = 2);
        Mesh(Vector center, const char * name, double s, int obj_type = 2);
        std::tuple<double, Vector> intersect(Vector O, Vector D, double t_min, double t_max);

        void transform();
        void update_normals();
        void update_normals(Matrix M);


        Vector get_center();
        void set_center(Vector c);

        std::vector<Vector*> get_vertices();
        void set_vertices(std::vector<Vector*> vertices);
};

#endif