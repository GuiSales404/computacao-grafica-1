#include <cmath>
#include "../header/scene.hpp"
#include <utility>

Scene::Scene(Vector O, Viewport vw, Canva c) : Obs(O), viewport(vw), canva(c) {
    this->dx = 1.0*vw.get_w()/c.get_w();
    this->dy = 1.0*vw.get_h()/c.get_h(); 
}


std::tuple<Color, Object*> Scene::raytracing_objects(Vector O, Vector D, double t_min, double t_max, int i, int j) {
    Object *closest_object = nullptr; 
    Color color = canva.get_background_color(); 
    bool nulo   = true;

    double t, closest = INF;
    Vector aux_N, N;

    for(Object *s : objects) {
        std::tie(t, aux_N) = s->intersect(O, D, t_min, t_max); 
        if(t - EPS >= t_min && t <= t_max && t < closest) {
            closest = t;
            closest_object = s;
            nulo = false;
            N = aux_N;
        }
    }

    
    if(closest == INF) return {canva.get_background_color(), closest_object};
    if(closest_object->has_image()) closest_object->set_current_color(i, j);

    Vector P = O + D * closest;

    return {compute_lighting(P, N, -D, closest_object->get_specular(), closest_object, t_min, t_max), closest_object};   
}

bool Scene::has_shadow(Vector P, Light* l, double t_min, double t_max) {
    Object *closest_object;
    Vector L = l->get_l(P);
    Vector N_L = L / ~L;
    if(~L == 0.0) return true;
    double t, closest = INF;
    Vector n;

    for(Object* o : objects) {
        std::tie(t, n) = o->intersect(P, N_L, t_min, t_max);
        if(t-EPS > t_min && t < t_max && t < closest) closest = t;   
    }
    return !(closest >= (~L));
}

Color Scene::compute_lighting(Vector P, Vector N, Vector V, double s, Object* o, double t_min, double t_max) {
    Color i;
    for(Light* l : lights)
        i = i + l->calculate_intensity(P, N, V, s, o, has_shadow(P, l, t_min, t_max));
    return i;
}


Vector Scene::canva_to_viewport(int i, int j) {
    return Vector(-viewport.get_w()/2.0 + dx/2.0 + j*dx, viewport.get_h()/2.0 - dy/2.0 - i*dy, viewport.get_d());
}

void Scene::lookAt(Vector e, Vector at, Vector up) {
    static int x = 0;
    this->CameraToWord(this->e, this->at, this->up);
    // if(x == 0) {
    //     this->CameraToWord(e, at, up);
    // } else {
    //     this->CameraToWord(this->e, this->at, this->up);
    // }

    x = 1;

    this->e  = e;
    this->at = at;
    this->up = up;

    Matrix m = Matrix::world_to_camera_matrix(e, at, up);
    for(Object *s : objects) s->world_to_camera(m);
    for(Light *l : lights) l->world_to_camera(m);
}

void Scene::CameraToWord(Vector e, Vector at, Vector up) {
    Matrix m = Matrix::camera_to_world_matrix(e, at, up);
    for(Object *s : objects) s->camera_to_world(m);
    for(Light *l : lights) l->camera_to_world(m);
}

std::tuple<Vector, Vector, Vector> Scene::get_lookat() {
    return {this->e, this->at, this->up};
}

Object* Scene::picking(int i, int j) { return canva.get_object(i, j); }

void Scene::add_object(Object *o){ objects.push_back(o); }
void Scene::add_light(Light* l){ lights.push_back(l); }

std::vector<std::vector<Color>>& Scene::draw_scenario(int type = 1) {
    Color color; 
    Object* obj;

    for(auto& obj : this->objects) {
        obj->get_obj_pixels().clear();
    }

    auto pespective = [color, &obj, this](Vector O, Vector D, int i, int j) -> std::tuple<Color, Object*> {
        return this->raytracing_objects(O, D, 1.0, INF, i, j);
    };

    auto pararell = [color, &obj, this](Vector O, Vector D, int i, int j) -> std::tuple<Color, Object*> {
        return this->raytracing_objects(D, {0, 0, -1}, 1.0, INF, i, j);
    };

    for(int i = 0; i < canva.get_w(); i++) {
        for(int j = 0; j < canva.get_h(); j++) {
            Vector D = canva_to_viewport(i, j);
            if(type)
                std::tie(color, obj) = pespective(this->Obs, (D/~D), i, j);
            else
                std::tie(color, obj) = pararell(this->Obs, (D/~D), i, j);

            canva.to_color(i, j, color);
            canva.to_buffer(i, j, obj);

            // guarda os pixels que forma o objeto no cenário..
            if(obj != nullptr) {
                obj->push_point_color({i, j, color});
            }
        }
    }

    return canva.get_pixels();
}

Color Scene::get_pixel(int i, int j) { return canva.get_pixel(i, j); }
Object* Scene::get_object(int i, int j) {
    if(i > this->canva.get_w() || j > this->canva.get_h() || i < 0 || j < 0) {
        return nullptr;
    }

    return canva.get_object(i, j);
}

std::vector<Object*>& Scene::get_object_list() {
    return this->objects;
}

std::vector<Light*>& Scene::get_light_list() {
    return this->lights;
}

void Scene::save_scenario(const char* image_name) {

    canva.write_image(image_name);
}
