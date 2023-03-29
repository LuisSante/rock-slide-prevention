#ifndef PUNTO_CONTACTO_HPP
#define PUNTO_CONTACTO_HPP

#include <iostream>
#include <vector>
#include <iomanip>
#include <utility>
#include <fstream>

#include <cmath>

#include "draw.hpp"
// #include "algoritmos.hpp"

using namespace std;

constexpr int MAX_DIMENSION = 100;

std::ofstream impact("C:/Users/Usuario/Desktop/hilarios/src/reportes/impact.txt");
std::ofstream locals("C:/Users/Usuario/Desktop/hilarios/src/reportes/local.txt");

class PointContact
{
private:
    Draw draw;
    int dimension = 2;
    float theta_slope = 75.98 * 3.1416f / 180.0f;
    float matrix_angles[2][2] = {{cos(theta_slope), -sin(theta_slope)}, {sin(theta_slope), cos(theta_slope)}};
    float vertices_difference[6];
    float vertices_locales_slope[6];
    float inverse[2][2];

public:
    bool collision;
    pair<float, float> perpendicular; // Sigma n

    PointContact();
    PointContact(Draw &draw);
    void print_matrix(float matrix[2][2]);
    bool decomposition_LU(float matrix[2][2], int dimension, float L[2][2], float U[2][2]);
    bool resolver_sistema(float L[2][2], float U[2][2], float b[2], int dimension, float x[2]);
    bool calculate_inverse(float inverse[2][2]);

    vector<float> locales(float current_center_mass_X, float current_center_mass_Y, float vertices_slope_[], float inverse[2][2]);
    vector<float> machine(float current_center_mass_X, float current_center_mass_Y, float vertices_slope[]);
    void superposition(float current_center_mass_X, float current_center_mass_Y, float vertices_slope[]);
};

PointContact::PointContact()
{
    for (int i = 0; i < 6; i++)
    {
        vertices_difference[i] = 0;
        vertices_locales_slope[i] = 0;
    }

    collision = false;
    perpendicular.first = 0.0f;
    perpendicular.second = 0.0f;
    /*ml = 0;
    bl = 0;*/
}

PointContact::PointContact(Draw &draw) : draw(draw)
{
    for (int i = 0; i < 6; i++)
    {
        vertices_difference[i] = 0;
        vertices_locales_slope[i] = 0;
    }
    collision = false;
    perpendicular.first = 0.0f;
    perpendicular.second = 0.0f;
}

void PointContact::print_matrix(float matrix[2][2])
{
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
}

bool PointContact::decomposition_LU(float matrix[2][2], int dimension,
                                    float L[2][2], float U[2][2])
{
    // Verificar que la matrix sea cuadrada
    if (dimension <= 0 || dimension > MAX_DIMENSION)
    {
        return false;
    }

    // Copiar la matrix original a U
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            U[i][j] = matrix[i][j];
        }
    }

    // Inicializar L con la matrix identidad
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            if (i == j)
            {
                L[i][j] = 1;
            }
            else
            {
                L[i][j] = 0;
            }
        }
    }

    // Realizar la descomposición LU
    for (int k = 0; k < dimension; k++)
    {
        if (U[k][k] == 0)
        {
            return false; // La matrix no tiene inverse
        }

        for (int i = k + 1; i < dimension; i++)
        {
            float factor = U[i][k] / U[k][k];

            for (int j = k + 1; j < dimension; j++)
            {
                U[i][j] -= factor * U[k][j];
            }

            L[i][k] = factor;
            U[i][k] = 0;
        }
    }

    return true;
}

bool PointContact::resolver_sistema(float L[2][2], float U[2][2],
                                    float b[2], int dimension, float x[2])
{
    // Resolver Ly = b usando sustitución hacia adelante
    float y[2] = {};

    for (int i = 0; i < dimension; i++)
    {
        float sum = 0;

        for (int j = 0; j < i; j++)
        {
            sum += L[i][j] * y[j];
        }

        y[i] = b[i] - sum;
    }

    // Resolver Ux = y usando sustitución hacia atrás
    for (int i = dimension - 1; i >= 0; i--)
    {
        float sum = 0;

        for (int j = i + 1; j < dimension; j++)
        {
            sum += U[i][j] * x[j];
        }

        x[i] = (y[i] - sum) / U[i][i];

        if (isnan(x[i]))
        {
            return false; // La matrix no tiene inverse
        }
    }

    return true;
}

bool PointContact::calculate_inverse(float inverse[2][2])
{
    // Verificar que la matrix sea cuadrada
    if (dimension <= 0 || dimension > MAX_DIMENSION)
    {
        return false;
    } // Calcular la descomposición LU
    float L[2][2] = {};
    float U[2][2] = {};

    if (!decomposition_LU(matrix_angles, dimension, L, U))
    {
        return false; // La matrix no tiene inverse
    }

    // Inicializar la matrix inverse con ceros
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            inverse[i][j] = 0;
        }
    }

    // Resolver dimension sistemas de equationes para obtener la matrix inverse
    for (int i = 0; i < dimension; i++)
    {
        float b[2] = {};
        float x[2] = {};

        b[i] = 1;

        if (!resolver_sistema(L, U, b, dimension, x))
        {
            return false;
        }

        for (int j = 0; j < dimension; j++)
        {
            inverse[j][i] = x[j];
        }
    }

    return true;
}

vector<float> PointContact::locales(float current_center_mass_X, float current_center_mass_Y, float vertices_slope_[], float inverse[2][2])
{
    vector<float> local_middles;

    for (int i = 0; i < 6; i += 3)
    {
        vertices_difference[i] = current_center_mass_X;
        vertices_difference[i + 1] = current_center_mass_Y;
        vertices_difference[i + 2] = 0;
    }

    for (int i = 0; i < 6; i++)
    {
        vertices_locales_slope[i] = vertices_slope_[i] - vertices_difference[i];
    }

    float x1_ellipse_local = vertices_locales_slope[0];
    float y1_ellipse_local = vertices_locales_slope[1];
    float x2_ellipse_local = vertices_locales_slope[3];
    float y2_ellipse_local = vertices_locales_slope[4];

    float x1_ = (inverse[0][0] * x1_ellipse_local) + (inverse[0][1] * y1_ellipse_local);
    float y1_ = (inverse[1][0] * x1_ellipse_local) + (inverse[1][1] * y1_ellipse_local);
    float x2_ = (inverse[0][0] * x2_ellipse_local) + (inverse[0][1] * y2_ellipse_local);
    float y2_ = (inverse[1][0] * x2_ellipse_local) + (inverse[1][1] * y2_ellipse_local);

    local_middles.push_back(x1_);
    local_middles.push_back(y1_);
    local_middles.push_back(x2_);
    local_middles.push_back(y2_);

    /*cout << "FUNCTION LOCALES" << endl;
    for (int i = 0; i < local_middles.size(); i++)
    {
        cout << local_middles[i] << " ";
    }
    cout << endl
         << endl;*/

    return local_middles;
}

vector<float> PointContact::machine(float current_center_mass_X, float current_center_mass_Y, float vertices_slope[])
{
    vector<float> middle;

    cout << " draw.a : " << draw.a << endl;
    cout << " draw.b : " << draw.b << endl;
    cout << " current_center_mass_X : " << current_center_mass_X << endl;
    cout << " current_center_mass_Y : " << current_center_mass_Y << endl;

    if (!calculate_inverse(inverse))
    {
        cout << "No inverse" << endl;
    }

    vector<float> local_middles = locales(current_center_mass_X, current_center_mass_Y, vertices_slope, inverse);

    float ml = (draw.a / draw.b) * ((local_middles[3] - local_middles[1]) / (local_middles[2] - local_middles[0]));
    float bl = (local_middles[1] / draw.b) - (ml * (local_middles[0] / draw.a));

    middle.push_back(ml);
    middle.push_back(bl);

    cout << " ml: " << ml << " bl: " << bl << endl;
    locals << " ml: " << ml << " bl: " << bl << endl;

    // Quadratic equation
    double e_a = ((ml * ml) + 1);
    double e_b = 2 * ml * bl;
    double e_c = ((bl * bl) - 1);

    double s1 = 0, s2 = 0;
    double discriminant = (e_b * e_b) - 4 * e_a * e_c;
    double discriminant_root = sqrt(abs(discriminant));
    float real_part = 0.0f, part_imagine = 0.0f;

    impact << "discriminant : " << discriminant << endl;

    if (discriminant > 0 || discriminant == 0)
    {
        s1 = (-e_b + discriminant_root) / (2 * e_a);
        s2 = (-e_b - discriminant_root) / (2 * e_a);
        collision = true;
        // impact << "Intersection" << endl;
    }
    else
    {
        real_part = -e_b / (2 * e_a);
        part_imagine = discriminant_root / (2 * e_a);
        collision = false;
        // impact << "No hay intersection" << endl;
    }

    // Root

    double p1 = 0, p2 = 0;
    double _x_1 = 0, _y_1 = 0, _x_2 = 0, _y_2 = 0;
    p1 = (ml * s1) + bl;
    p2 = (ml * s2) + bl;

    _x_1 = s1 * draw.a;
    _y_1 = p1 * draw.b;
    _x_2 = s2 * draw.a;
    _y_2 = p2 * draw.b;

    cout << " _x_1: " << _x_1 << " _y_1: " << _y_1 << " _x_2: " << _x_2 << " _y_2: " << _y_2 << endl;

    double _x1_ = ((matrix_angles[0][0] * _x_1) + (matrix_angles[0][1] * _y_1)) + current_center_mass_X;
    double _y1_ = ((matrix_angles[1][0] * _x_1) + (matrix_angles[1][1] * _y_1)) + current_center_mass_Y;
    double _x2_ = ((matrix_angles[0][0] * _x_2) + (matrix_angles[0][1] * _y_2)) + current_center_mass_X;
    double _y2_ = ((matrix_angles[1][0] * _x_2) + (matrix_angles[1][1] * _y_2)) + current_center_mass_Y;

    cout << _x1_ << " " << _y1_ << " " << _x2_ << " " << _y2_ << endl;

    float point_middle_x = 30 *((_x1_ + _x2_) / 2.0f);
    float point_middle_y = 30 *((_y1_ + _y2_) / 2.0f);

    cout << "(XC , YC) = (" << point_middle_x << " ," << point_middle_y << ")" << endl;

    middle.push_back(point_middle_x);
    middle.push_back(point_middle_y);

    cout << "FUNCTION MACHINE FOR THE POINTS MIDDLES " << endl;
    cout << current_center_mass_X << " " << current_center_mass_Y << endl;
    for (int i = 0; i < middle.size(); i++)
    {
        cout << middle[i] << " ";
    }
    cout << endl
         << endl;

    return middle;
}

void PointContact::superposition(float current_center_mass_X, float current_center_mass_Y, float vertices_slope[])
{
    // vector<float> distance_perpendicular;
    vector<float> point_middle = machine(current_center_mass_X, current_center_mass_Y, vertices_slope);
    // cout << "Punto medio : " << point_middle[2] << " " << point_middle[3] << endl;
    float pA1 = (1 / (sqrt(1 + (point_middle[0] * point_middle[0]))));
    float pA2 = -(1 / (sqrt(1 + (point_middle[0] * point_middle[0]))));
    cout << " pA1 : " << pA1 << " pA2 : " << pA2 << endl;

    float sA1 = -point_middle[0] * pA1;
    float sA2 = -point_middle[0] * pA2;
    cout << " sA1: " << sA1 << " sA2 : " << sA2 << endl;

    float xA1 = sA1 * draw.a;
    float yA1 = pA1 * draw.b;
    float xA2 = sA2 * draw.a;
    float yA2 = pA2 * draw.b;
    cout << " xA1 : " << xA1 << " yA1: " << yA1 << " xA2: " << xA2 << " yA2: " << yA2 << endl;

    double xA1_ = ((matrix_angles[0][0] * xA1) + (matrix_angles[0][1] * yA1)) + current_center_mass_X;
    double yA1_ = ((matrix_angles[1][0] * xA1) + (matrix_angles[1][1] * yA1)) + current_center_mass_Y;
    double xA2_ = ((matrix_angles[0][0] * xA2) + (matrix_angles[0][1] * yA2)) + current_center_mass_X;
    double yA2_ = ((matrix_angles[1][0] * xA2) + (matrix_angles[1][1] * yA2)) + current_center_mass_Y;
    cout << xA1_ << " " << yA1_ << " " << xA2_ << " " << yA2_ << endl;

    double A = -(vertices_slope[4] - vertices_slope[1]) / (vertices_slope[3] - vertices_slope[0]);
    double B = 1;
    // double C = (-A * vertices_slope[0]) - vertices_slope[1];
    double C = (-A * vertices_slope[3]) - vertices_slope[4];
    // cout<<A<<" "<<B<<" "<<C<<endl;

    // std::cout << A << " " << B << " " << C << std::endl;
    float per_1 = 30 * (((A * xA1_) + (B * yA1_) + C) / (sqrt((A * A) + (B * B))));
    float per_2 = 30 * ((A * xA2_) + (B * yA2_) + C) / (sqrt((A * A) + (B * B)));

    cout << "per_1 : " << per_1 << " per_2 : " << per_2 << endl;

    perpendicular.first = per_1;
    perpendicular.second = per_2;
}

#endif