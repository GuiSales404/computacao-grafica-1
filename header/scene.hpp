#ifndef SCENE_HPP
#define SCENE_HPP

#include "vector.hpp"
#include "canvas.hpp"
#include "sphere.hpp"
#include "light.hpp"
#include "viewport.hpp"
#include "plan.hpp"
#include "object.hpp"

class Scene {
    private:
        Vector Obs;
        Viewport viewport;
        Canva canva;
        std::vector<Object*> objects;
        std::vector<Light*> lights;
        double dx, dy;

        Color compute_lighting(Vector P, Vector N, Vector V, double s, Object* o, double t_min, double t_max);
        std::tuple<Color, Object*> raytracing_objects(Vector O, Vector D, double t_min, double t_max, int i, int j);
        Vector canva_to_viewport(int x, int y);
        bool has_shadow(Vector P, Light* l, double t_min, double t_max);
        Vector e  = {0, 0, 0};
        Vector at = {0, 0, -40};
        Vector up = {0, 1, 0};

    public:
        Scene(Vector O, Viewport vp, Canva c);
        void add_object(Object *s);
        void add_light(Light *l);
        std::vector<std::vector<Color>>& draw_scenario(int type);
        void save_scenario(const char* image_name);
        void lookAt(Vector e, Vector at, Vector up);
        Object* picking(int i, int j);
        Color get_pixel(int i, int j);
        Object* get_object(int i, int j);
        std::vector<Object*>& get_object_list();
        std::vector<Light*>& get_light_list();
        std::tuple<Vector, Vector, Vector> get_lookat();
        void CameraToWord(Vector e, Vector at, Vector up);
};

#endif