#include "../header/matrix.hpp"

Matrix::Matrix() {}
Matrix::Matrix(int row, int col): row(row), col(col) { this->values.resize(row, std::vector<double>(col)); }

void Matrix::print(){
    for(int i = 0; i < this->row; i++){
        for(int j = 0; j < this->col; j++){
            std::cout << this->values[i][j] << " ";
        }
        std::cout << "\n";
    }
}

Matrix Matrix::operator * (Matrix M) {  
    if(this->col != M.row) return Matrix(1, 1);

    Matrix B(this->row, M.col);    

    for(int i = 0; i < this->row; i++)
        for(int j = 0; j < M.col; j++)
            for(int k = 0; k < M.row; k++)
                B.values[i][j] += this->values[i][k]*M.values[k][j];
    return B;
}

Matrix Matrix::operator * (double e) {
    Matrix A(this->row, this->col);
    for(int i = 0; i < this->col; i++)
        for(int j = 0; j < this->row; j++)
            A.values[i][j] = this->values[i][j] * e;
    return A;
}

Matrix Matrix::operator - (Matrix m) {
    if(this->row != m.row || this->col != m.col) return Matrix(1, 1);
    Matrix B(m.row, m.col);
    for(int i = 0; i < this->col; i++)
        for(int j = 0; j < this->row; j++)
            B.values[i][j] = this->values[i][j] - m.values[i][j];
    return B;
}

Matrix Matrix::operator - () {
    Matrix A(this->row, this->col);
    for(int i = 0; i < this->col; i++)
        for(int j = 0; j < this->row; j++)
            A.values[i][j] = -this->values[i][j];
    return A;
}

Matrix Matrix::operator ~ () {  
    Matrix A(this->col, this->row);  
    for(int i = 0; i < this->col; i++)
        for(int j = 0; j < this->row; j++)
            A.values[i][j] = this->values[j][i];
    return A; 
}

Vector Matrix::matrix_to_vector() {
    Vector v;
    v.set_x(this->values[0][0]);
    v.set_y(this->values[1][0]);
    v.set_z(this->values[2][0]);
    v.set_a(this->values[3][0]);
    return v;
}

Matrix Matrix::vector_to_matrix(Vector v, bool ponto) {
    Matrix M(4, 1);
    M.values[0][0] = v.get_x();
    M.values[1][0] = v.get_y();
    M.values[2][0] = v.get_z();
    M.values[3][0] = ponto;
    return M;
}

Matrix Matrix::identity(int size) {
    Matrix M(size, size);
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            if(i == j) M.values[i][j] = 1.;
            else M.values[i][j] = 0.;
        }
    }
    return M;
}

Matrix Matrix::rotate_around_x(double angle) {
    Matrix M(4, 4);
    M.values[0] = {1., 0., 0. , 0.};
    M.values[1] = {0., std::cos(angle), -std::sin(angle), 0.};
    M.values[2] = {0., std::sin(angle),  std::cos(angle), 0.};
    M.values[3] = {0., 0., 0. , 1.};
    return M;
}

Matrix Matrix::rotate_around_y(double angle) {
    Matrix M(4, 4);
    M.values[0] = { std::cos(angle), 0., std::sin(angle), 0.};
    M.values[1] = {0., 1., 0., 0.};
    M.values[2] = {-std::sin(angle), 0., std::cos(angle), 0.};
    M.values[3] = {0., 0., 0. , 1.};
    return M;
}

Matrix Matrix::rotate_around_z(double angle) {
    Matrix M(4, 4);
    M.values[0] = {std::cos(angle), -std::sin(angle), 0.};
    M.values[1] = {std::sin(angle),  std::cos(angle), 0.};
    M.values[2] = {0., 0., 1., 0.};
    M.values[3] = {0., 0., 0., 1.};
    return M;
}

Matrix Matrix::translation(Vector v) {
    Matrix M(4, 4);
    M.values[0] = {1., 0., 0., v.get_x()};
    M.values[1] = {0., 1., 0., v.get_y()};
    M.values[2] = {0., 0., 1., v.get_z()};
    M.values[3] = {0., 0., 0., 1.};
    return M;
}

Matrix Matrix::scaling(double x, double y, double z) {
    Matrix M(4, 4);
    M.values[0] = {x , 0., 0., 0.};
    M.values[1] = {0., y , 0., 0.};
    M.values[2] = {0., 0., z , 0.};
    M.values[3] = {0., 0., 0., 1.};
    return M;
}

Matrix Matrix::shearing_matrix_xy(double angle) {
    Matrix M(4, 4);
    M.values[0] = {1., tan(angle), 0., 0.};
    M.values[1] = {0., 1.,         0., 0.};
    M.values[2] = {0., 0.,         1., 0.};
    M.values[3] = {0., 0.,         0., 1.};
    return M;
}

Matrix Matrix::shearing_matrix_xz(double angle) {
    Matrix M(4, 4);
    M.values[0] = {1., 0., tan(angle), 0.};
    M.values[1] = {0., 1., 0.,         0.};
    M.values[2] = {0., 0., 1.,         0.};
    M.values[3] = {0., 0., 0.,         1.};
    return M;
}

Matrix Matrix::shearing_matrix_yx(double angle) {
    Matrix M(4, 4);
    M.values[0] = {1.,         0., 0., 0.};
    M.values[1] = {tan(angle), 1., 0., 0.};
    M.values[2] = {0.,         0., 1., 0.};
    M.values[3] = {0.,         0., 0., 1.};
    return M;
}

Matrix Matrix::shearing_matrix_yz(double angle) {
    Matrix M(4, 4);
    M.values[0] = {1., 0., 0.,         0.};
    M.values[1] = {0., 1., tan(angle), 0.};
    M.values[2] = {0., 0., 1.,         0.};
    M.values[3] = {0., 0., 0.,         1.};
    return M;
}

Matrix Matrix::shearing_matrix_zx(double angle) {
    Matrix M(4, 4);
    M.values[0] = {1.,         0., 0., 0.};
    M.values[1] = {0.,         1., 0., 0.};
    M.values[2] = {tan(angle), 0., 1., 0.};
    M.values[3] = {0.,         0., 0., 1.};
    return M;
}

Matrix Matrix::shearing_matrix_zy(double angle) {
    Matrix M(4, 4);
    M.values[0] = {1., 0.,         0., 0.};
    M.values[1] = {0., 1.,         0., 0.};
    M.values[2] = {0., tan(angle), 1., 0.};
    M.values[3] = {0., 0.,         0., 1.};
    return M;
}

Matrix Matrix::reflection_matrix_xy() {
    Matrix M(4, 4);
    M.values[0] = {1., 0.,  0., 0.};
    M.values[1] = {0., 1.,  0., 0.};
    M.values[2] = {0., 0., -1., 0.};
    M.values[3] = {0., 0.,  0., 1.};
    return M;
}
Matrix Matrix::reflection_matrix_yz() {
    Matrix M(4, 4);
    M.values[0] = {-1., 0., 0., 0.};
    M.values[1] = { 0., 1., 0., 0.};
    M.values[2] = { 0., 0., 1., 0.};
    M.values[3] = { 0., 0., 0., 1.};
    return M;
}
Matrix Matrix::reflection_matrix_xz() {
    Matrix M(4, 4);
    M.values[0] = {1.,  0., 0., 0.};
    M.values[1] = {0., -1., 0., 0.};
    M.values[2] = {0.,  0., 1., 0.};
    M.values[3] = {0.,  0., 0., 1.};
    return M;
}

Matrix Matrix::householder_matrix(Vector n) {
    Matrix normal = Matrix::vector_to_matrix(n);
    Matrix M = Matrix::identity(4) - normal * (~normal) * 2;
    M.values[0][3] = M.values[1][3] = M.values[2][3] = M.values[3][0] = M.values[3][1] = M.values[3][2] = 0.;
    M.values[3][3] = 1.;
    return M;
}

Matrix Matrix::reflection_matrix_at(Vector p, Vector n) {
    return Matrix::translation(p) * Matrix::householder_matrix(n/~n) * Matrix::translation(-p);
}

Matrix Matrix::world_to_camera_matrix(Vector e, Vector at, Vector up) {
    Vector k = (e - at) / ~(e - at);
    Vector vUpk = (up - e) % k;
    Vector i = vUpk / ~vUpk;
    Vector j = (k % i);

    Matrix M(4, 4);
    M.values[0] = {i.get_x(),  i.get_y(), i.get_z(), -(i * e)};
    M.values[1] = {j.get_x(),  j.get_y(), j.get_z(), -(j * e)};
    M.values[2] = {k.get_x(),  k.get_y(), k.get_z(), -(k * e)};
    M.values[3] = {0.,  0., 0., 1.};

    return M;
}

Matrix Matrix::camera_to_world_matrix(Vector e, Vector at, Vector up) {
    Vector k = (e - at) / ~(e - at);
    Vector vUpk = (up - e) % k;
    Vector i = vUpk / ~vUpk;
    Vector j = (k % i);

    Matrix M(4, 4);
    M.values[0] = {i.get_x(),  j.get_x(), k.get_x(), e.get_x()};
    M.values[1] = {i.get_y(),  j.get_y(), k.get_y(), e.get_y()};
    M.values[2] = {i.get_z(),  j.get_z(), k.get_z(), e.get_z()};
    M.values[3] = {0.,  0., 0., 1.};

    return M;
}

std::vector<std::vector<double>> Matrix::get_values() { return this->values; }
void Matrix::set_values(std::vector<std::vector<double>> v) { values =  v; }

int Matrix::get_row() { return this->row; }
void Matrix::set_row(int row) { this->row = row; }

int Matrix::get_col() { return this->col; }
void Matrix::set_col(int col) { this->col = col; }