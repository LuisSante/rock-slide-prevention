#ifndef PUNTO_CONTACTO_HPP
#define PUNTO_CONTACTO_HPP

#include <iostream>
#include <vector>
#include <iomanip>
#include <utility>

#include <cmath>

#include "draw.hpp"
// #include "algoritmos.hpp"

using namespace std;

constexpr int MAX_DIMENSION = 100;

class PuntoContacto
{
private:
    Draw draw;
    int dimension = 2;
    float theta_talud = 0.0f * 3.1416f / 180.0f;
    float matriz_angulos[2][2] = {{cos(theta_talud), -sin(theta_talud)}, {sin(theta_talud), cos(theta_talud)}};
    float vertices_resta[6];
    float vertices_locales_talud[6];
    float inversa[2][2];

public:
    bool collision;
    pair<float , float> perpendicular; // Sigma n

    PuntoContacto();
    PuntoContacto(Draw draw);
    void imprimir_matriz(float matriz[2][2]);
    bool descomposicion_LU(float matriz[2][2], int dimension, float L[2][2], float U[2][2]);
    bool resolver_sistema(float L[2][2], float U[2][2], float b[2], int dimension, float x[2]);
    bool calcular_inversa(float inversa[2][2]);

    vector<float> locales(float current_center_mass_X, float current_center_mass_Y, float vertices_talud_[], float inversa[2][2]);
    vector<float> machine(float current_center_mass_X, float current_center_mass_Y, float vertices_talud[]);
    void superposition(float current_center_mass_X, float current_center_mass_Y, float vertices_talud[]);
};

PuntoContacto::PuntoContacto()
{
    for (int i = 0; i < 6; i++)
    {
        vertices_resta[i] = 0;
        vertices_locales_talud[i] = 0;
    }

    collision = false;
    perpendicular.first = 0.0f;
    perpendicular.second = 0.0f;
    /*ml = 0;
    bl = 0;*/
}

PuntoContacto::PuntoContacto(Draw draw) : draw(draw)
{
    for (int i = 0; i < 6; i++)
    {
        vertices_resta[i] = 0;
        vertices_locales_talud[i] = 0;
    }
    collision = false;
    perpendicular.first = 0.0f;
    perpendicular.second = 0.0f;
}

void PuntoContacto::imprimir_matriz(float matriz[2][2])
{
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            cout << matriz[i][j] << "\t";
        }
        cout << endl;
    }
}

bool PuntoContacto::descomposicion_LU(float matriz[2][2], int dimension,
                                      float L[2][2], float U[2][2])
{
    // Verificar que la matriz sea cuadrada
    if (dimension <= 0 || dimension > MAX_DIMENSION)
    {
        return false;
    }

    // Copiar la matriz original a U
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            U[i][j] = matriz[i][j];
        }
    }

    // Inicializar L con la matriz identidad
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
            return false; // La matriz no tiene inversa
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

bool PuntoContacto::resolver_sistema(float L[2][2], float U[2][2],
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
            return false; // La matriz no tiene inversa
        }
    }

    return true;
}

bool PuntoContacto::calcular_inversa(float inversa[2][2])
{
    // Verificar que la matriz sea cuadrada
    if (dimension <= 0 || dimension > MAX_DIMENSION)
    {
        return false;
    } // Calcular la descomposición LU
    float L[2][2] = {};
    float U[2][2] = {};

    if (!descomposicion_LU(matriz_angulos, dimension, L, U))
    {
        return false; // La matriz no tiene inversa
    }

    // Inicializar la matriz inversa con ceros
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            inversa[i][j] = 0;
        }
    }

    // Resolver dimension sistemas de ecuaciones para obtener la matriz inversa
    for (int i = 0; i < dimension; i++)
    {
        float b[2] = {};
        float x[2] = {};

        b[i] = 1;

        if (!resolver_sistema(L, U, b, dimension, x))
        {
            return false; // La matriz no tiene inversa
        }

        for (int j = 0; j < dimension; j++)
        {
            inversa[j][i] = x[j];
        }
    }

    return true;
}

vector<float> PuntoContacto::locales(float current_center_mass_X, float current_center_mass_Y, float vertices_talud_[], float inversa[2][2])
{
    vector<float> medio_locales;

    for (int i = 0; i < 6; i += 3)
    {
        vertices_resta[i] = current_center_mass_X;
        vertices_resta[i + 1] = current_center_mass_Y;
        vertices_resta[i + 2] = 0;
    }

    for (int i = 0; i < 6; i++)
    {
        vertices_locales_talud[i] = vertices_talud_[i] - vertices_resta[i];
    }

    float x1_elipse_local = vertices_locales_talud[0];
    float y1_elipse_local = vertices_locales_talud[1];
    float x2_elipse_local = vertices_locales_talud[3];
    float y2_elipse_local = vertices_locales_talud[4];

    float x1_ = (inversa[0][0] * x1_elipse_local) + (inversa[0][1] * y1_elipse_local);
    float y1_ = (inversa[1][0] * x1_elipse_local) + (inversa[1][1] * y1_elipse_local);
    float x2_ = (inversa[0][0] * x2_elipse_local) + (inversa[0][1] * y2_elipse_local);
    float y2_ = (inversa[1][0] * x2_elipse_local) + (inversa[1][1] * y2_elipse_local);

    medio_locales.push_back(x1_);
    medio_locales.push_back(y1_);
    medio_locales.push_back(x2_);
    medio_locales.push_back(y2_);

    /*cout << "FUNCION LOCALES" << endl;
    for (int i = 0; i < medio_locales.size(); i++)
    {
        cout << medio_locales[i]<< " ";
    }
    cout << endl
         << endl;*/

    return medio_locales;
}

vector<float> PuntoContacto::machine(float current_center_mass_X, float current_center_mass_Y, float vertices_talud[])
{
    vector<float> medio;

    if (!calcular_inversa(inversa))
    {
        cout << "NO TIENE INVERSA" << endl;
    }

    vector<float> medio_locales = locales(current_center_mass_X, current_center_mass_Y, vertices_talud, inversa);

    float ml = (draw.a / draw.b) * ((medio_locales[3] - medio_locales[1]) / (medio_locales[2] - medio_locales[0]));
    float bl = (medio_locales[1] / draw.b) - (ml * (medio_locales[0] / draw.a));

    medio.push_back(ml);
    medio.push_back(bl);

    // ecuacion cuadrativa
    double e_a = ((ml * ml) + 1);
    double e_b = 2 * ml * bl;
    double e_c = ((bl * bl) - 1);

    double s1 = 0, s2 = 0;
    double discriminant = (e_b * e_b) - 4 * e_a * e_c;
    double raizdiscriminant = sqrt(abs(discriminant));
    float parteReal = 0.0f, parteImaginaria = 0.0f;

    if (discriminant > 0 || discriminant == 0)
    {
        s1 = (-e_b + raizdiscriminant) / (2 * e_a);
        s2 = (-e_b - raizdiscriminant) / (2 * e_a);
        collision = true;
        // cout << "CHOCO AL FIN" << endl;
    }
    else
    {
        parteReal = -e_b / (2 * e_a);
        parteImaginaria = raizdiscriminant / (2 * e_a);
        collision = false;
        // cout << "No hay interseccion" << endl;
    }

    // raices

    double p1 = 0, p2 = 0;
    double _x_1 = 0, _y_1 = 0, _x_2 = 0, _y_2 = 0;
    p1 = (ml * s1) + bl;
    p2 = (ml * s2) + bl;

    _x_1 = s1 * draw.a;
    _y_1 = p1 * draw.b;
    _x_2 = s2 * draw.a;
    _y_2 = p2 * draw.b;

    // cout<<_x_1<<" "<<_y_1<<" "<<_x_2<<" "<<_y_2<<endl;

    double _x1_ = ((matriz_angulos[0][0] * _x_1) + (matriz_angulos[0][1] * _y_1)) + current_center_mass_X;
    double _y1_ = ((matriz_angulos[1][0] * _x_1) + (matriz_angulos[1][1] * _y_1)) + current_center_mass_Y;
    double _x2_ = ((matriz_angulos[0][0] * _x_2) + (matriz_angulos[0][1] * _y_2)) + current_center_mass_X;
    double _y2_ = ((matriz_angulos[1][0] * _x_2) + (matriz_angulos[1][1] * _y_2)) + current_center_mass_Y;

    // cout << _x1_ << " " << _y1_ << " " << _x2_ << " " << _y2_ << endl;

    float punto_medio_x = (_x1_ + _x2_) / 2.0f;
    float punto_medio_y = (_y1_ + _y2_) / 2.0f;

    // cout << "(XC , YC) = (" << punto_medio_x << " ," << punto_medio_y << ")" << endl;

    medio.push_back(punto_medio_x);
    medio.push_back(punto_medio_y);

    /*cout << "FUNCION MACHINE PARA SACAR LOS PUNTO MEDIOS" << endl;
    for (int i = 0; i < medio.size(); i++)
    {
        cout << medio[i] << " ";
    }
    cout << endl
         << endl;*/

    return medio;
}

void PuntoContacto::superposition(float current_center_mass_X, float current_center_mass_Y, float vertices_talud[])
{
    //vector<float> distance_perpendicular;
    vector<float> point_middle = machine(current_center_mass_X, current_center_mass_Y, vertices_talud);
    float pA1 = (1 / (sqrt(1 + (point_middle[0] * point_middle[0]))));
    float pA2 = -(1 / (sqrt(1 + (point_middle[0] * point_middle[0]))));
    // cout<<pA1<<" "<<pA2<<endl;

    float sA1 = -point_middle[0] * pA1;
    float sA2 = -point_middle[0] * pA2;
    // cout<<sA1<<" "<<sA2<<endl;

    float xA1 = sA1 * draw.a;
    float yA1 = pA1 * draw.b;
    float xA2 = sA2 * draw.a;
    float yA2 = pA2 * draw.b;
    // cout<<xA1<<" "<<yA1<<" "<<xA2<<" "<<yA2<<endl;

    double xA1_ = ((matriz_angulos[0][0] * xA1) + (matriz_angulos[0][1] * yA1)) + current_center_mass_X;
    double yA1_ = ((matriz_angulos[1][0] * xA1) + (matriz_angulos[1][1] * yA1)) + current_center_mass_Y;
    double xA2_ = ((matriz_angulos[0][0] * xA2) + (matriz_angulos[0][1] * yA2)) + current_center_mass_X;
    double yA2_ = ((matriz_angulos[1][0] * xA2) + (matriz_angulos[1][1] * yA2)) + current_center_mass_Y;
    // cout<<xA1_<<" "<<yA1_<<" "<<xA2_<<" "<<yA2_<<endl;

    double A = -(vertices_talud[4] - vertices_talud[1]) / (vertices_talud[3] - vertices_talud[0]);
    double B = 1;
    // double C = (-A * vertices_talud[0]) - vertices_talud[1];
    double C = (-A * vertices_talud[3]) - vertices_talud[4];
    // cout<<A<<" "<<B<<" "<<C<<endl;

    // std::cout << A << " " << B << " " << C << std::endl;
    // MOSTRARLE ESTO AL INGENIERO
    float per_1 = ((A * xA1_) + (B * yA1_) + C) / (sqrt((A * A) + (B * B)));
    float per_2 = ((A * xA2_) + (B * yA2_) + C) / (sqrt((A * A) + (B * B)));

    perpendicular.first = per_1;
    perpendicular.second = per_2;

    /*cout << "FUNCION PARA LA SUPERPOSICION: DISTANCIAS PERPENDICULARES" << endl;
    for (int i = 0; i < distance_perpendicular.size(); i++)
    {
        cout << distance_perpendicular[i] << " ";
    }
    cout << endl
         << endl;*/
}

#endif