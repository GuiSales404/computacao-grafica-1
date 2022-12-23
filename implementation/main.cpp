#include "../header/vector.hpp"
#include "../header/color.hpp"
#include "../header/viewport.hpp"
#include "../header/canvas.hpp"
#include "../header/sphere.hpp"
#include "../header/cone.hpp"
#include "../header/scene.hpp"
#include "../header/light.hpp"
#include "../header/spotlight.hpp"
#include "../header/cylinder.hpp"
#include "../header/ambient_light.hpp"
#include "../header/light_point.hpp"
#include "../header/light_direction.hpp"
#include "../header/cube.hpp"
#include <SFML/Graphics.hpp>
#include <map>
#include <chrono>
#include <thread>
#include <windows.h>

using namespace std;
using namespace chrono;

int paint = 0;

void debug(string s) {
    cout << s << endl;
    system("pause");
}

int camera_style = 1;

class Menu {
public:
    map<string, pair<int, vector<Object*>>> tree_obj;
    map<string, pair<int, Light*>>  tree_light;

    Scene& scene;
private:

    sf::Image& img;
    int menu_flag = 1;
    int cursor_pos  = 0;
    int cursor_pos2 = 0;

    #define VALID_INDEX       0
    #define QUIT_INDEX        1
    #define CREATE_INDEX      2
    #define INVALID_INDEX     3
    #define SAVE_INDEX        4
    #define REMOVE_INDEX      5
    #define CHANGE_CAMERA     6
    #define MOVE_CAM          7

    int draw_animated_menu(vector<string> opt, int& cursor_pos, int menu1 = 0) {
        system("clear");

        if(menu1) {
            opt.push_back("Salvar objetos");
            opt.push_back("Criar objeto");
            opt.push_back("Remover objeto");
            opt.push_back("Mudar camera");
            opt.push_back("Andar com a camera");
        }

        opt.push_back("Sair");

        for(int i = 0; i < opt.size(); i++) {
            printf("[%d] %s\n", i, opt[i].c_str());
        }

        int op; cin >> op; getchar();

        cursor_pos = op;

        if(op >= opt.size() || op < 0)   return INVALID_INDEX;
        if(op == opt.size()-1)           return QUIT_INDEX;
        if(not menu1)                    return VALID_INDEX;

        if(op == opt.size()-6)           return SAVE_INDEX;
        if(op == opt.size()-5)           return CREATE_INDEX;
        if(op == opt.size()-4)           return REMOVE_INDEX;
        if(op == opt.size()-3)           return CHANGE_CAMERA;
        if(op == opt.size()-2)           return MOVE_CAM;

        return VALID_INDEX;
    }

public:
	Menu(sf::Image& img, Scene& scene) : img(img), scene(scene) {

    }

	void register_obj(Object* obj, string label, int type) {
        tree_obj[label].first = type;
        tree_obj[label].second.push_back(obj);
	}

    void register_light(Light* light, string label, int type) {
        tree_light[label] = {type, light};
	}

    void exec_shearing(string func, Object* obj, double angle) {
        string type = func.substr(func.size()-2);
        Vector e, at, up;
        std::tie(e, at, up) = this->scene.get_lookat();
        obj->camera_to_world(Matrix::camera_to_world_matrix(e, at, up));

        if(type == "xy") {
            obj->shearing_xy(angle);
        } else if(type == "xz") {
            obj->shearing_xz(angle);
        } else if(type == "yx") {
            obj->shearing_yx(angle);
        } else if(type == "yz") {
             obj->shearing_yz(angle);
        } else {
            obj->shearing_zx(angle);
        }

        obj->world_to_camera(Matrix::world_to_camera_matrix(e, at, up));
    }

    void exec_rotation(string func, Object* obj, double angle) {
        string type = func.substr(func.size()-1);
        Vector e, at, up;
        std::tie(e, at, up) = this->scene.get_lookat();
        obj->camera_to_world(Matrix::camera_to_world_matrix(e, at, up));

        auto v = obj->get_center();

        obj->translate(-v);

        if(type == "x") {
            obj->rotation_x(angle);
        } else if(type == "y") {
            obj->rotation_y(angle);
        } else {
            obj->rotation_z(angle);
        }

        obj->translate(v);
        obj->world_to_camera(Matrix::world_to_camera_matrix(e, at, up));
    }

    void exec_scaling(Object* obj, double args) {
        Vector e, at, up;
        std::tie(e, at, up) = this->scene.get_lookat();
        obj->camera_to_world(Matrix::camera_to_world_matrix(e, at, up));

        auto v = obj->get_center();
        obj->translate(-v);
        obj->scaling(args);
        obj->translate(v);
        obj->world_to_camera(Matrix::world_to_camera_matrix(e, at, up));
    }

    void exec_scaling(Object* obj, array<double, 3> args) {
        Vector e, at, up;
        std::tie(e, at, up) = this->scene.get_lookat();
        obj->camera_to_world(Matrix::camera_to_world_matrix(e, at, up));

        auto v = obj->get_center();
        obj->translate(-v);
        obj->scaling(args[0], args[1], args[2]);
        //((Cube*)obj)->set_dim(args[0], args[1], args[2]);
        obj->translate(v);
        obj->world_to_camera(Matrix::world_to_camera_matrix(e, at, up));
    }

    void exec_translating(Object* obj, array<double, 3> args, int origin = 0) {
        Vector e, at, up;
        std::tie(e, at, up) = this->scene.get_lookat();
        obj->camera_to_world(Matrix::camera_to_world_matrix(e, at, up));

        auto v = obj->get_center();
        obj->translate(-v);
        
        if(!origin) {
            obj->translate(Vector(args[0]+v.get_x(), args[1]+v.get_y(), args[2]+v.get_z()));
        }

        obj->world_to_camera(Matrix::world_to_camera_matrix(e, at, up));
        //obj->translate({args[0], args[1], args[2]});
    }

    void exec_reflection(string func, Object* obj) {
        string type = func.substr(func.size()-2);

        Vector e, at, up;
        std::tie(e, at, up) = this->scene.get_lookat();
        obj->camera_to_world(Matrix::camera_to_world_matrix(e, at, up));

        if(type == "xy") {
            obj->reflection_xy();
        } else if(type == "yz") {
            obj->reflection_yz();
        } else {
            obj->reflection_xz();
        }

        obj->world_to_camera(Matrix::world_to_camera_matrix(e, at, up));
    }

    string get_label(Object* obj) {
        for(auto& ls : this->tree_obj) {
            for(auto& obj_it : ls.second.second) {
                if(obj_it == obj) {
                    return ls.first;
                }
            }
        }

        return "";
    }

    void menu2() {
        static int draw = 1;
        auto it = this->tree_obj.begin();
        advance(it, this->cursor_pos);

        auto arr_obj = it->second.second; // vetor de objetos que serão aplicada as transformações

        cout << arr_obj.size() << endl;

        system("pause");

        vector<string> opt = {  "translate",    "rotation_x",    "rotation_y",      "rotation_z",   "scaling",
                                "shearing_xy",  "shearing_xz"  , "shearing_yx",     "shearing_yz",  "shearing_zx",
                                "shearing_zy",  "reflection_xy", "reflection_yz",   "reflection_xz"
                             };

        auto get_func_type = [&opt](int idx) -> int {
            if(opt[idx].find("translate") != string::npos) {
                return 1;
            } 

            if(opt[idx].find("rotation") != string::npos) {
                return 2;
            } 

            if(opt[idx].find("scaling") != string::npos) {
                return 3;
            } 

            if(opt[idx].find("shearing") != string::npos) {
                return 4;
            } 

            if(opt[idx].find("reflection") != string::npos) {
                return 5;
            } 
        };

        auto clear_buffer = []() -> void {
            string clear_out; getline(cin, clear_out);
        };

        auto angle_menu = [clear_buffer]() -> double {
            //clear_buffer();
            system("cls");
            printf("Digite o angulo: ");
            double angle; cin >> angle;
            return angle;
        };

        auto scaling_menu = [clear_buffer]() -> double {
            double factor;
            system("cls");
            printf("Digite o fator: ");
            cin >> factor; getchar();
            return factor;
        };

        auto scaling_menu_cube = [clear_buffer]() -> array<double, 3> {
            array<double, 3> factors;
            system("cls");
            printf("Digite o fator x: ");
            cin >> factors[0]; 
            printf("Digite o fator y: ");
            cin >> factors[1];
            printf("Digite o fator z: ");
            cin >> factors[2];
            return factors;
        };

        auto translate_menu = [clear_buffer]() -> array<double, 3> {
            system("cls");
            array<double, 3> factors;
            cout << "Digite o eixo x: ";
            cin >> factors[0];
            cout << "Digite o eixo y: ";
            cin >> factors[1];
            cout << "Digite o eixo z: ";
            cin >> factors[2]; getchar();

            return factors;
        };

        while(1) {
            switch (draw_animated_menu(opt, this->cursor_pos2)) {
                case INVALID_INDEX : {
                    cout << "opcao invalida" << endl;
                    getchar();
                    break;
                }

                case QUIT_INDEX: {
                    return;
                    break;
                }
                
                default: {
                    int func_type = get_func_type(this->cursor_pos2);
                    if(func_type == 1) {
                        auto args = translate_menu();
                        for(auto& obj : arr_obj) {
                            this->exec_translating(obj, args);
                        }
                    } else if(func_type == 2) {
                        auto angle = angle_menu();
                        for(auto& obj : arr_obj) {
                            this->exec_rotation(opt[this->cursor_pos2], obj, angle);
                        } 
                    } else if(func_type == 3) {
                        if(arr_obj.size() == 1 && arr_obj.front()->get_type() == 2) {
                            this->exec_scaling(arr_obj.front(), scaling_menu_cube());
                        } else {
                            double args = scaling_menu();
                            for(auto& obj : arr_obj) {
                                this->exec_scaling(obj, args);
                            } 
                        }
                    } else if(func_type == 4) {
                        auto angle = angle_menu();
                        for(auto& obj : arr_obj) {
                            this->exec_shearing(opt[this->cursor_pos2], obj, angle);
                        }  
                    } else {
                        for(auto& obj : arr_obj) {
                            this->exec_reflection(opt[this->cursor_pos2], obj);
                        }
                    }

                    paint = 1; 
                    break;
                }
            }
        }
    }

    int menu1() {
        vector<string> opt;

        for(auto tree : this->tree_obj) {
            opt.push_back(tree.first);
        }

        return this->draw_animated_menu(opt, this->cursor_pos, 1);
    }

    void menu3() {
        vector<string> opt = { "Cubo", "Esfera", "Cilindro", "Cone", "Luz Ambiente", "Luz Direcional", "Ponto de Luz", "Luz Focal" };

        int cursor_pos3;

        auto create_color = []() -> Color {
            double x, y, z;
            printf("Digite os 3 valores: ");
            cin >> x >> y >> z;
            return Color::to_rgb(x, y, z);
        };

        struct obj_data {
            string label;
            Color kd, ka, ks;
            double s;
        };

        auto origin = [this](string s = "Posicao") -> Vector {
            system("cls");
            cout << s << endl;
            printf("x: ");
            double x; cin >> x;
            printf("y: ");
            double y; cin >> y;
            printf("z: ");
            double z; cin >> z; getchar();
            return Vector(x, y, z);
        };

        auto coeficients = [this, create_color]() -> obj_data {
            system("cls");
            string label;
            printf("Label: ");
            cin >> label;
            printf("kd\n");
            Color kd = create_color();
            printf("ka\n");
            Color ka = create_color();
            printf("ks\n");
            Color ks = create_color();
            double specular;
            printf("specular: "); cin >> specular; getchar();
            return {label, kd, ka, ks, specular};
        };

        auto word_to_camera_obj = [this](Object* obj) {
            Vector e, at, up;
            std::tie(e, at, up) = this->scene.get_lookat();
            obj->world_to_camera(Matrix::world_to_camera_matrix(e, at, up));
        };

        auto word_to_camera_light = [this](Light* light) {
            Vector e, at, up;
            std::tie(e, at, up) = this->scene.get_lookat();
            light->world_to_camera(Matrix::world_to_camera_matrix(e, at, up));
        };

        auto create_cube = [this, coeficients, origin, word_to_camera_obj]() {
            obj_data od = coeficients();
            Object* cube = new Cube(od.kd, od.ka, od.ks, od.s);
            cube->translate(origin());
            word_to_camera_obj(cube);
            this->register_obj(cube, od.label, 1);
            this->scene.add_object(cube);
        };

        auto create_sphere = [this, coeficients, origin, word_to_camera_obj]() {
            obj_data od = coeficients();
            printf("digite o raio: ");
            double r; cin >> r; getchar();
            Object* sphere = new Sphere(origin(), r, od.kd, od.ka, od.ks, od.s);
            word_to_camera_obj(sphere);
            this->register_obj(sphere, od.label, 1);
            this->scene.add_object(sphere);
        };

        auto create_cone = [this, coeficients, origin, word_to_camera_obj]() {
            obj_data od = coeficients();
            printf("digite a altura: ");
            double h; cin >> h;
            printf("digite o raio: ");
            double r; cin >> r;
            printf("tem base?: ");
            bool b; cin >> b; getchar();
            // o dc sempre será perpendicular ao plano que passa por x e z.
            Object* cone = new Cone(origin(), {0,1,0}, r, h, od.kd, od.ka, od.ks, od.s, b);
            word_to_camera_obj(cone);
            this->register_obj(cone, od.label, 1);
            this->scene.add_object(cone);
        };

        auto create_cylinder = [this, coeficients, origin, word_to_camera_obj]() {
            obj_data od = coeficients();
            printf("digite a altura: ");
            double h; cin >> h;
            printf("digite o raio: ");
            double r; cin >> r;
            printf("tem base?: ");
            bool b; cin >> b;
            printf("tem topo?: ");
            bool t; cin >> t; getchar();
            // o dc sempre será perpendicular ao plano que passa por x e z.
            Object* cylinder = new Cylinder(origin(), {0,1,0}, r, h, od.kd, od.ka, od.ks, od.s, b, t);
            word_to_camera_obj(cylinder);
            this->register_obj(cylinder, od.label, 1);
            this->scene.add_object(cylinder);
        };

        auto create_ambient_light = [this, create_color, word_to_camera_light]() {
            system("cls");
            string label;
            printf("Label: ");
            cin >> label; getchar();
            printf("Intensidade\n");
            auto intensity = create_color();
            Light* al = new AmbientLight(intensity);
            word_to_camera_light(al);
            this->register_light(al, label, 2);
            this->scene.add_light(al);
        };

        auto create_diretion_light = [this, create_color, origin, word_to_camera_light]() {
            system("cls");
            string label;
            printf("Label: ");
            cin >> label; getchar();
            printf("Intensidade\n");
            auto intensity = create_color();
            Light* ld = new LightDirection(intensity, origin("Direcao"));
            word_to_camera_light(ld);
            this->register_light(ld, label, 2);
            this->scene.add_light(ld);
        };

        auto create_point_light = [this, create_color, origin, word_to_camera_light]() {
            system("cls");
            string label;
            printf("Label: ");
            cin >> label; getchar();
            printf("Intensidade\n");
            auto intensity = create_color();
            Light* lp      = new LightPoint(intensity, origin());
            word_to_camera_light(lp);
            this->register_light(lp, label, 2);
            this->scene.add_light(lp);
        };

        auto create_spot_light = [this, create_color, origin, word_to_camera_light]() {
            system("cls");
            string label;
            printf("Label: ");
            cin >> label; getchar();
            printf("Intensidade\n");
            auto intensity = create_color();
            auto pos = origin();
            auto dir = origin("Direcao");
            printf("digite o grau: ");
            double grau; cin >> grau;
            Light* sl = new SpotLight(intensity, pos, dir, grau);
            word_to_camera_light(sl);
            this->register_light(sl, label, 2);
            this->scene.add_light(sl);
        };

        while(1) {
            switch (draw_animated_menu(opt, cursor_pos3)) {
            case VALID_INDEX: {
                switch (cursor_pos3) {
                    case 0: {
                        create_cube();
                        break;
                    }

                    case 1: {
                        create_sphere();
                        break;
                    }

                    case 2: {
                        create_cylinder();
                        break;
                    }

                    case 3: {
                        create_cone();
                        break;
                    }

                    case 4: {
                        create_ambient_light();
                        break;
                    }

                    case 5: {
                        create_diretion_light();
                        break;
                    }

                    case 6: {
                        create_point_light();
                        break;
                    }

                    case 7: {
                        create_spot_light();
                        break;
                    }
                }

                paint = 1;
                break;
            }

            case INVALID_INDEX: {
                cout << "Digite uma opcao valida" << endl;
                getchar();
                break;
            }

            case QUIT_INDEX: {
                return;
            }
            }
        }

    }

    void remove_menu() {
        while(1) {
            vector<string> opt;

            for(auto tb : this->tree_obj) {
                opt.push_back(tb.first);
            }

            for(auto tl : this->tree_light) {
                opt.push_back(tl.first);
            }

            int count_obj   = this->tree_obj.size();
            int light_count = this->tree_light.size();

            int cursor_pos4;

            switch (draw_animated_menu(opt, cursor_pos4)) {
                case VALID_INDEX: {
                    if(cursor_pos4+1 <= count_obj) {
                        auto it = this->tree_obj.begin();
                        advance(it, cursor_pos4);

                        auto& vt_obj = this->scene.get_object_list();

                        for(auto& o : it->second.second) {
                            for(auto i = vt_obj.begin();; i++) {
                                if(*i == o) {
                                    vt_obj.erase(i);
                                    break;
                                }
                            }
                        }

                        this->tree_obj.erase(it->first);
                    } else {
                        auto it = this->tree_light.begin();
                        advance(it, cursor_pos4-count_obj);

                        auto& vt_obj = this->scene.get_light_list();

                        for(auto i = vt_obj.begin();; i++) {
                            if(*i == it->second.second) {
                                vt_obj.erase(i);
                                break;
                            }
                        }

                        this->tree_light.erase(it->first);
                    }

                    paint = 1;
                    printf("Objeto removido\n");
                    getchar();
                    break;
                }

                case INVALID_INDEX: {
                    printf("Digite uma opcao valida\n"); getchar();
                    break;
                }

                case QUIT_INDEX: {
                    return;
                }
            }
        }

    }

    void save() {
        string s = "CG.txt";

        FILE* fp = fopen(s.c_str(), "w");

        int idx = 1;

        fprintf(fp, "CAMERA\n");

        Vector e, at, up;

        std::tie(e, at, up) = scene.get_lookat();

        fprintf(fp, "Origem -> %.2f, %.2f, %.2f\n", e.get_x(), e.get_y(), e.get_z());
        fprintf(fp, "Direcao -> %.2f, %.2f, %.2f\n\n", at.get_x(), at.get_y(), at.get_z());

        for(auto& it : this->tree_obj) {
            for(auto& obj : it.second.second) {
                Vector e, at, up;
                std::tie(e, at, up) = scene.get_lookat();
                obj->camera_to_world(Matrix::camera_to_world_matrix(e, at, up));

                fprintf(fp, "OBJECTO [%d]\n", idx++);
                fprintf(fp, "Label: %s\n", it.first.c_str());
                auto v = obj->get_center();
                fprintf(fp, "Center -> %.2f, %.2f, %.2f\n", v.get_x(), v.get_y(), v.get_z());
                if(obj->get_type() == 3) {
                    fprintf(fp, "Radius -> %.2f\n", ((Sphere*)obj)->get_radius());
                } else if(obj->get_type() == 4) {
                    fprintf(fp, "Radius -> %.2f\n", ((Cylinder*)obj)->get_radius());
                    fprintf(fp, "Height -> %.2f\n", ((Cylinder*)obj)->get_height());
                } else if(obj->get_type() == 1) {
                    fprintf(fp, "Radius -> %.2f\n", ((Cone*)obj)->get_radius());
                    fprintf(fp, "Height -> %.2f\n", ((Cone*)obj)->get_height());
                } else {
                    auto dim = ((Cube*)obj)->get_dim();
                    fprintf(fp, "size_x -> %.2f\n", dim[0]);
                    fprintf(fp, "size_y -> %.2f\n", dim[1]);
                    fprintf(fp, "size_z -> %.2f\n", dim[2]);
                }

                fprintf(fp, "\n");

                obj->world_to_camera(Matrix::world_to_camera_matrix(e, at, up));
            }
        }

        fclose(fp);
        printf("Arquivo salvo com sucesso\n");
        getchar();
    }

    void change_camera() {
        auto get_vector = [this](string s = "Posicao") -> Vector {
            system("cls");
            cout << s << endl;
            printf("x: ");
            double x; cin >> x;
            printf("y: ");
            double y; cin >> y;
            printf("z: ");
            double z; cin >> z; getchar();
            return Vector(x, y, z);
        };

        auto pos = get_vector("Origem");
        auto at  = get_vector("Direcao");

        if(pos.get_x() <= 0 && pos.get_x() >= 0) {
            pos.set_x(1);
        }

        scene.lookAt(pos, at, {pos.get_x(), pos.get_y()+1, pos.get_z()});
        paint = 1;
    }

    void move_cam() {
        auto get_vector = [this](string s = "Posicao") -> Vector {
            system("cls");
            cout << s << endl;
            printf("x: ");
            double x; cin >> x;
            printf("y: ");
            double y; cin >> y;
            printf("z: ");
            double z; cin >> z; getchar();
            return Vector(x, y, z);
        };

        int skip = 0;

        auto get_vector_custom = [this, &skip](string s = "Posicao") -> Vector {
            system("cls");
            printf("Digite 0 caso queira manter a direcao: ");
            cin >> skip;

            if(skip == 0) {
                return {};
            }

            cout << endl << s << endl;
            printf("x: ");
            double x; cin >> x;
            printf("y: ");
            double y; cin >> y;
            printf("z: ");
            double z; cin >> z; getchar();
            return Vector(x, y, z);
        };

        auto pos = get_vector("Eixos");
        auto at  = get_vector_custom("Direcao");

        Vector e, at_aux, up;
        tie(e, at_aux, up) = scene.get_lookat();

        if(skip == 0) {
            at = at_aux;
        }

        pos = e + pos;

        if(pos.get_x() <= 0 && pos.get_x() >= 0) {
            pos.set_x(1);
        }

        scene.lookAt(pos, at, Vector(pos.get_x(), pos.get_y()+1, pos.get_z()));
        paint = 1;
    }

	void event_loop() {
        while(1) {
            switch (menu1()) {
                case VALID_INDEX: {
                    menu2();
                    break;
                }

                case CREATE_INDEX: {
                    menu3();
                    break;
                }

                case CHANGE_CAMERA: {
                    change_camera();
                    break;
                }

                case SAVE_INDEX: {
                    save();
                    break;
                }

                case MOVE_CAM: {
                    move_cam();
                    break;
                }

                case REMOVE_INDEX: {
                    remove_menu();
                    break;
                }

                case QUIT_INDEX: {
                    exit(0);
                }
                
                case INVALID_INDEX: {
                    cout << "Digite uma opcao valida\n" << endl;
                    getchar();
                    break;
                }
            }
        }
	}
};

int toggle;

class obj_basic_prop {
private:
    string label;
    Vector pos;
    Color kd, ka, ke;
    double s;

    void print_color(Color c) {   
        printf("Color: (%f, %f, %f)\n", floor(c.get_r()*255), floor(c.get_g()*255), floor(c.get_b()*255));
    }

    void print_pos(Vector c) {   
        printf("Position: (%.2f, %.2f, %.2f)\n", c.get_x(), c.get_y(), c.get_z());
    }
public:

    obj_basic_prop() = default;

    obj_basic_prop(Vector pos, Color kd, Color ka, Color ke, double s) {
        cout << s << endl;
        this->pos  = pos;
        this->kd   = kd;
        this->ka   = ka;
        this->ke   = ke;
        this->s    = s;

        toggle = 1;
    }

    void show_info(Menu menu, Object* obj) {
        Vector e, at, up;
        std::tie(e, at, up) = menu.scene.get_lookat();
        obj->camera_to_world(Matrix::camera_to_world_matrix(e, at, up));

        this->pos = obj->get_center();

        printf("Label: %s\n", menu.get_label(obj).c_str());
        print_pos(this->pos);
        printf("kd: ");
        print_color(this->kd);
        printf("ka: ");
        print_color(this->ka);
        printf("ke: ");
        print_color(this->ke);
        printf("Specular: %f\n\n", s);
        obj->world_to_camera(Matrix::world_to_camera_matrix(e, at, up));
    }
};

obj_basic_prop basic_info;

const int h = 500;
const int w = 500;

unsigned char* apiPixels_to_SFMLPixels(std::vector<std::vector<Color>>& px) {
    int w = px.front().size();
    int h = px.size();

    unsigned char* res = new unsigned char[w * h * 4];

    int idx = 0;

    for(auto& row : px) {
        for(auto& col : row) {
            res[idx]   = col.to_red();
            res[idx+1] = col.to_green();
            res[idx+2] = col.to_blue();
            res[idx+3] = 255; // ALPHA, tá aqui só pelo SFML msm
            idx       += 4;
        }
    }

    return res;
}

void repaint_obj(sf::Image& img, vector<point_color> pc, bool highlight = 1) {
    for(auto& e : pc) {
        auto aux = e.c;
        sf::Color c(aux.to_red(), aux.to_green(), aux.to_blue(), 255);
        if(highlight) {
            img.setPixel(e.j, e.i, sf::Color::Red);
        } else {
            img.setPixel(e.j, e.i, c);
        }
    }
}

// Color random_color() {
//     return Color((rand() % 255)/255.0, (rand() % 255)/255.0, (rand() % 255)/255.0);
// }
// int random_between(int min, int max) {
//     return min + (rand() % (max-min+1));
// }
// void generate_random_scene(Scene& scene) {
//     for(int i = 0; i < 10; i++) {
//         if(rand() % 2) {
//             Cube *cubo  = new Cube(random_color(), random_color(), random_color(), rand() % 20 + 1);
//             cubo->rotation_z(1 + (rand() % 12)/10);
//             int sc = 2 + rand() % 2;
//             cubo->scaling(sc, sc, sc);
//             cubo->translate(Vector(random_between(-5, 5), random_between(-5, 5), -5 + -(rand() % 22)));
//             scene.add_object(cubo);
//         } else {
//             Sphere *esfera   = new Sphere(Vector(0, 0, 0), 1 + rand() % 1, random_color(), random_color(), random_color(), 10);
//             esfera->translate(Vector(random_between(-5, 5), random_between(-5, 5), -5 + -(rand() % 22)));
//             scene.add_object(esfera);
//         }
//     }
// }

int main() {
    srand(time(NULL));
    Vector O(0, 0, 0);
    Viewport viewport(300, 300, -10);
    Canva canva(h, w, Color::to_rgb(100, 100, 100));

    Scene scene(O, viewport, canva);

    // kd
    // ks 
    // ke

    Object *skybox    = new Cube("ceu.png", 1000);

    Object *chao      = new Cube("piso.png", 1000);
    Object *parede1   = new Cube("lateral.png", 1000);
    Object *parede2   = new Cube("lateral.png", 1000);
    Object *parede3   = new Cube("lateral.png", 1000);
    //Object *cubo2   = new Cube(Color(Color::to_rgb(214, 214, 214)), Color(Color::to_rgb(214, 214, 214)), Color(Color::to_rgb(214, 214, 214)), 10);
    //Object *cubo2   = new Cube(Color(Color::to_rgb(20, 20, 20)), Color(Color::to_rgb(70, 214, 120)), Color(Color::to_rgb(214, 214, 214)), 10);
    //Object *esf1    = new Sphere(Vector({0, 0, 0}), 1, Color(1, 1, 0), Color(1, 1, 0), Color(1, 1, 0), 10);
    Object *abaju         = new Cone(Vector(0, -65, -250.0), Vector(0, 1, 0), 10, 10, Color(Color::to_rgb(214, 214, 214)), Color(Color::to_rgb(20, 20, 20)), Color(Color::to_rgb(100, 100, 100)), 1000, 1);
    Object *barra1        = new Cylinder(Vector(-40.00, -70.80, -250.00), Vector(0, 1, 0), 1.5, 20, Color(Color::to_rgb(214, 214, 214)), Color(Color::to_rgb(214, 214, 214)), Color(Color::to_rgb(214, 214, 214)), 1000, 1, 1);
    Object *topo_arvore   = new Cone(Vector(-40.00, -50.80, -250.00), Vector(0, 1, 0), 8, 25, Color(Color::to_rgb(214, 214, 214)), Color(Color::to_rgb(214, 214, 214)), Color(Color::to_rgb(214, 214, 214)), 1000, 1);

    skybox->scaling(10);
    skybox->translate({0, 0, -30});

    // CASA DO LULA MOLUSCO

    // CAMERA
    // Origem -> 1.00, 0.00, 20.00
    // Direcao -> 0.00, 0.00, -40.00

    // OBJECTO [1]
    // Label: casa_do_lula
    // Center -> 0.00, 0.00, -76.00
    // Radius -> 20.00
    // Height -> 60.00

    //Object *casa_do_lula  = new Cylinder(Vector(0.00, 0.00, -76), Vector(0, 1, 0), 20, 60, Color(Color::to_rgb(214, 214, 214)), Color(Color::to_rgb(214, 214, 214)), Color(Color::to_rgb(214, 214, 214)), 1000, 1, 1);
    Object *casa_do_lula  = new Cylinder(Vector(10.00, -70.00, -256.00), Vector(0, 1, 0), 20, 60, "casa_lula_text.png", 1000, 1, 1);

    casa_do_lula->get_center().print();
    system("pause");

    // OBJECTO [5]
    // Label: orelha
    // Center -> -20.00, 42.00, -45.00
    // size_x -> 5.00
    // size_y -> 13.00
    // size_z -> 1.00

    Object *orelha1 = new Cube("casa_lula_text.png", 1000);
    orelha1->scaling(5, 13, 1);
    orelha1->translate({27.75, -33.00, -239.66});

    // OBJECTO [6]
    // Label: orelha2
    // Center -> 20.00, 42.00, -45.00
    // size_x -> 1.00
    // size_y -> 1.00
    // size_z -> 1.00

    Object *orelha2 = new Cube("casa_lula_text.png", 1000);
    orelha2->scaling(5, 13, 1);
    orelha2->translate({-7.75, -33.00, -239.66});

    // OBJECTO [2]
    // Label: nariz
    // Center -> 0.00, 35.00, -56.00
    // size_x -> 5.10
    // size_y -> 19.00
    // size_z -> 1.00

    Object *nariz = new Cube(Color(Color::to_rgb(0, 20, 30)), Color(Color::to_rgb(30, 30, 30)), Color(Color::to_rgb(50, 30, 20)), 1000);
    nariz->scaling(5.10, 19, 1);
    nariz->translate({10.00, -35.00, -236.00});


    // OBJECTO [3]
    // Label: olho1
    // Center -> -9.00, 37.00, -55.00
    // Radius -> 5.60

    Object *olho1 = new Sphere(Vector({1.00, -33.00, -235.00}), 5.6, Color(1, 1, 0), Color(1, 1, 0), Color(1, 1, 0), 1000);

    // OBJECTO [4]
    // Label: olho2
    // Center -> 9.00, 37.00, -55.00
    // Radius -> 5.60

    Object *olho2 = new Sphere(Vector({19.00, -33.00, -235.00}), 5.6, Color(1, 1, 0), Color(1, 1, 0), Color(1, 1, 0), 1000);

    // OBJECTO [5]
    // Label: porta_do_lula
    // Center -> -0.00, 8.00, -56.00
    // size_x -> 11.05
    // size_y -> 16.57
    // size_z -> 1.00

    Object *porta_do_lula = new Cube("tronco_text.png", 1000);
    porta_do_lula->scaling(11.05, 16.57, 1.f);
    porta_do_lula->translate({10.00, -62.00, -236.00});

    // OBJECTO [6]
    // Label: sobrancelha
    // Center -> -0.00, 45.00, -56.00
    // size_x -> 28.00
    // size_y -> 1.95
    // size_z -> 1.00

    Object *sobrancelha = new Cube(Color(Color::to_rgb(0, 20, 30)), Color(Color::to_rgb(30, 30, 30)), Color(Color::to_rgb(50, 30, 20)), 1000);
    sobrancelha->scaling(28.f, 1.95, 1.f);
    sobrancelha->translate({10.00, -25.00, -236.00});

    //Object* plano = new Plan({0,0,-5}, {0,1,0}, Color(Color::to_rgb(0, 20, 30)), Color(Color::to_rgb(30, 30, 30)), Color(Color::to_rgb(50, 30, 20)), 1000);

    //scene.add_object(skybox);
    //scene.add_object(plano);


    chao->scaling(240, 6.4, 240);
    chao->translate({0, -73, -250});

    parede1->scaling(6, 102, 235.88);
    parede1->translate({-119, -20, -249});

    parede2->scaling(238.95, 100, 1);
    parede2->translate({1.5, -20, -367});

    parede3->scaling(6, 102, 235.88);
    parede3->translate({118, -20, -249});

    Light* lp  = new LightPoint(Color(0.1, 0.9, 0.2), Vector(0, -45, -250));
    Light* pl2 = new LightPoint(Color(0.7, 0.7, 0.7), Vector(0, 5, -70));
    Light* al  = new AmbientLight(Color(0.3, 0.3, 0.3));
    Light* sl  = new SpotLight(Color(Color::to_rgb(255, 255, 255)), Vector(10, 100, -256), Vector(10, -70, -256), 0.175);
    Light* ld  = new LightDirection(Color(0.2, 0.2, 1), {0, 0, -1});

    Object *poste1  = new Cylinder(Vector(50, -70.00, -250.00), Vector(0, 1, 0), 3.00, 55, "tronco_poste.png", 1000, 1, 1);

    Object *poste_bola = new Sphere(Vector({50, -10.00, -250.00}), 5.2, "ostra.png", 1000);

    Object *copa_arvore  = new Cone(Vector(-40.00, -50.00, -220.00), Vector(0, 1, 0), 15, 20, "copa.png", 1000, 1);

    Object *tronco_arvore  = new Cylinder(Vector(-40.00, -70.00, -220.00), Vector(0, 1, 0), 4.20, 20, "tronco_text.png", 1000, 1, 1);

    Object *Creto = new Cube("tronco_text.png", 1000);
    Creto->scaling(10.40, 5.98, 0.5);
    Creto->translate({-40.00, -55.00, -210.00});

    // Center -> -40.00, -55.00, -210.00
    // size_x -> 10.40
    // size_y -> 5.98
    // size_z -> 0.50

    scene.add_object(copa_arvore);
    scene.add_object(tronco_arvore);

    scene.add_object(poste1);
    scene.add_object(poste_bola);

    scene.add_light(al);
    //scene.add_light(lp);
    //scene.add_light(pl2);
    scene.add_light(sl);
    //scene.add_light(sl);

    scene.add_object(chao);
    scene.add_object(parede1);
    scene.add_object(parede2);
    scene.add_object(parede3);

    scene.add_object(sobrancelha);
    scene.add_object(porta_do_lula);
    scene.add_object(olho2);
    scene.add_object(olho1);
    scene.add_object(nariz);
    scene.add_object(casa_do_lula);
    scene.add_object(orelha1);
    scene.add_object(orelha2);
    scene.add_object(Creto);

    sf::Image   img;
    sf::Texture tx;
    sf::Sprite  spt;

    // CAMERA
    // Origem -> x: 100.00, y: -40.00, z: -250.00
    // Direcao -> x: -1.00, y: -70.00, z: -250.00

    // CAMERA
    // Origem -> x: 20.00, y: -67.00, z: -250.00
    // Direcao -> x: 0.00, y: -65.00, z: -250.00

    // Origem -> 41.00, 0.00, -200.00
    // Direcao -> 0.00, -153.00, -361.00

    // Origem -> 1.00, 0.00, -100.00
//     Direcao -> 10.00, -70.00, -256.00

    scene.lookAt(Vector(-40, -63, -300), Vector(10, -50, -256), Vector(-40, -53, -300));

    auto config_img = [&img, &tx, &spt, &scene](int first_call = 1) {
        if(first_call == 1)
            img.create(h, w, apiPixels_to_SFMLPixels(scene.draw_scenario(0)));
        tx.loadFromImage(img);
        spt.setTexture(tx, true);
    };

    config_img();

    sf::RenderWindow window(sf::VideoMode(h, w), "CG");

    sf::Mouse mouse;

    bool is_pressed  = 0;
    Object* cur_obj  = nullptr;
    Object* last_obj = nullptr;

    Menu menu(img, scene);

    menu.register_light(al, "al", 2);
    menu.register_light(sl, "ponto_focal", 2);
    //menu.register_light(lp, "lp", 2);
    
    menu.register_obj(sobrancelha, "casa_lula_molusco", 1);
    menu.register_obj(porta_do_lula, "casa_lula_molusco", 1);
    menu.register_obj(olho2, "casa_lula_molusco", 1);
    menu.register_obj(olho1, "casa_lula_molusco", 1);
    menu.register_obj(nariz, "casa_lula_molusco", 1);
    menu.register_obj(casa_do_lula, "casa_lula_molusco", 1);
    menu.register_obj(orelha1, "casa_lula_molusco", 1);
    menu.register_obj(orelha2, "casa_lula_molusco", 1);

    //menu.register_light(pl2, "pl2", 2);
    menu.register_obj(chao, "chao", 1);
    menu.register_obj(parede1, "parede1", 1);
    menu.register_obj(parede2, "parede2", 1);
    menu.register_obj(parede3, "parede3", 1);
    menu.register_obj(copa_arvore, "arvore", 1);
    menu.register_obj(tronco_arvore, "arvore", 1);
    menu.register_obj(poste1, "poste", 1);
    menu.register_obj(poste_bola, "poste", 1);
    menu.register_obj(Creto, "Creto_Imagem", 1);

    window.setFramerateLimit(60);

    std::thread tmenu(&Menu::event_loop, ref(menu));

    while (window.isOpen()) {
        sf::Event event;

        // event loop
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if(paint) {
            paint = 0;
            config_img();
            cur_obj = last_obj = nullptr;
        }

        if(mouse.sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            auto mouse_pos = mouse.getPosition(window);
            cur_obj        = scene.get_object(mouse_pos.y, mouse_pos.x);

            if(cur_obj != nullptr && last_obj != cur_obj) {
                if(last_obj != nullptr) {
                    repaint_obj(img, last_obj->get_obj_pixels(), 0);
                    config_img(0);
                    last_obj = cur_obj;
                }

                auto info = obj_basic_prop(cur_obj->get_center(), cur_obj->get_kd(), cur_obj->get_ka(), 
                                            cur_obj->get_ke(), cur_obj->get_specular());

                info.show_info(menu, cur_obj);

                last_obj = cur_obj;
                repaint_obj(img, cur_obj->get_obj_pixels(), 1);
                config_img(0);
            } else if(last_obj != nullptr && last_obj != cur_obj) {
                repaint_obj(img, last_obj->get_obj_pixels(), 0);
                toggle = 0;
                config_img(0);
                last_obj = cur_obj;
            }
        }

        window.draw(spt);
        window.display();
    }

    return 0;
}