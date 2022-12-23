#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "vector.hpp"
#include <vector>

#include <cmath>
#include <iostream>

class Matrix {
    private:
        int row, col;
        std::vector<std::vector<double>> values;
    public:
        Matrix();
        Matrix(int row, int col);

        Matrix operator * (Matrix m);
        Matrix operator * (double e);
        Matrix operator - (Matrix m);
        Matrix operator - ();
        Matrix operator ~ ();

        void print();

        Vector matrix_to_vector();
        static Matrix vector_to_matrix(Vector v, bool ponto = true);

        static Matrix identity(int size);

        static Matrix rotate_around_x(double angle);
        static Matrix rotate_around_y(double angle);
        static Matrix rotate_around_z(double angle);
        static Matrix translation(Vector v);
        static Matrix scaling(double x, double y, double z);
        static Matrix shearing_matrix_xy(double angle);
        static Matrix shearing_matrix_xz(double angle);
        static Matrix shearing_matrix_yx(double angle);
        static Matrix shearing_matrix_yz(double angle);
        static Matrix shearing_matrix_zx(double angle);
        static Matrix shearing_matrix_zy(double angle);

        static Matrix reflection_matrix_xy();
        static Matrix reflection_matrix_yz();
        static Matrix reflection_matrix_xz();

        static Matrix householder_matrix(Vector n);
        static Matrix reflection_matrix_at(Vector p, Vector n);

        static Matrix world_to_camera_matrix(Vector e, Vector at, Vector up);
        static Matrix camera_to_world_matrix(Vector e, Vector at, Vector up);

        std::vector<std::vector<double>> get_values();
        void set_values(std::vector<std::vector<double>> v);


        int get_row();
        void set_row(int row);

        int get_col();
        void set_col(int col);

        static Matrix rotation_x_matrix();
};

#endif