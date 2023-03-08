#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include <iostream>
#include <cmath>
using namespace std;

const int MAX_DIMENSION = 100;

class Intersection
{
private:
    float Xd;
    float Yd;
    float a;
    float b;
    int dimension = 2;
    float theta_talud = 10 * 3.1416 / 180;
    float matriz_angulos[MAX_DIMENSION][MAX_DIMENSION];
    float vertices_resta[6];
    float vertices_locales_talud[6];
    float x1_elipse_local; 
    float y1_elipse_local; 
    float x2_elipse_local; 
    float y2_elipse_local;
    float x1_;
    float y1_;
    float x2_;
    float y2_;

public:
    Intersection(float Xd, float Yd, float a, float b)
    {
        this -> Xd = Xd;
        this -> Yd = Yd;
        this -> a = a;
        this -> b = b;
        this -> x1_elipse_local = x1_elipse_local;
        this -> y1_elipse_local = y1_elipse_local;
        this -> x2_elipse_local = x2_elipse_local;
        this -> y2_elipse_local = y2_elipse_local;
        matriz_angulos[0][0] = cos(theta_talud);
        matriz_angulos[0][1] = -sin(theta_talud);
        matriz_angulos[1][0] = sin(theta_talud);
        matriz_angulos[1][1] = cos(theta_talud);
    }

    void imprimir_matriz(float matriz[MAX_DIMENSION][MAX_DIMENSION])
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

    bool descomposicion_LU(float matriz[MAX_DIMENSION][MAX_DIMENSION], int dimension,
                           float L[MAX_DIMENSION][MAX_DIMENSION], float U[MAX_DIMENSION][MAX_DIMENSION])
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

    bool resolver_sistema(float L[MAX_DIMENSION][MAX_DIMENSION], float U[MAX_DIMENSION][MAX_DIMENSION],
                          float b[MAX_DIMENSION], int dimension, float x[MAX_DIMENSION])
    {
        // Resolver Ly = b usando sustitución hacia adelante
        float y[MAX_DIMENSION] = {};

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

    bool calcular_inversa(float inversa[MAX_DIMENSION][MAX_DIMENSION])
    {
        // Verificar que la matriz sea cuadrada
        if (dimension <= 0 || dimension > MAX_DIMENSION)
        {
            return false;
        } // Calcular la descomposición LU
        float L[MAX_DIMENSION][MAX_DIMENSION] = {};
        float U[MAX_DIMENSION][MAX_DIMENSION] = {};

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
            float b[MAX_DIMENSION] = {};
            float x[MAX_DIMENSION] = {};

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

    void vertices_locales(float vertices_talud_[])
    {
        for (int i = 0; i < 6; i += 3)
        {
            vertices_resta[i] = Xd;
            vertices_resta[i + 1] = Yd;
            vertices_resta[i + 2] = 0;
        }

        for (int i = 0; i < 6; i++)
        {
            vertices_locales_talud[i] = vertices_talud_[i] - vertices_resta[i];
        }

        x1_elipse_local = vertices_locales_talud[0];
        y1_elipse_local = vertices_locales_talud[1];
        x2_elipse_local = vertices_locales_talud[3];
        y2_elipse_local = vertices_locales_talud[4];
    }

    void mult_matriz_array(float inversa[MAX_DIMENSION][MAX_DIMENSION])
    {
        x1_ = (inversa[0][0] * x1_elipse_local) + (inversa[0][1] * y1_elipse_local);
        y1_ = (inversa[1][0] * x1_elipse_local) + (inversa[1][1] * y1_elipse_local);
        x2_ = (inversa[0][0] * x2_elipse_local) + (inversa[0][1] * y2_elipse_local);
        y2_ = (inversa[1][0] * x2_elipse_local) + (inversa[1][1] * y2_elipse_local);
    }

    void machine(float &ml, float &bl, float &punto_medio_x, float &punto_medio_y)
    {
        ml = (a / b) * ((y2_ - y1_) / (x2_ - x1_));
        bl = (y1_ / b) - (ml * (x1_ / a));

        // ecuacion cuadrativa
        double e_a = ((ml * ml) + 1);
        double e_b = 2 * ml * bl;
        double e_c = ((bl * bl) - 1);

        // raices
        double discriminante = e_b * e_b - 4 * e_a * e_c;
        double raizDiscriminante = sqrt(abs(discriminante));
        double s1, s2;
        float parteReal, parteImaginaria;
        if (discriminante > 0)
        {
            s1 = (-e_b + raizDiscriminante) / (2 * e_a);
            s2 = (-e_b - raizDiscriminante) / (2 * e_a);
        }
        else if (discriminante == 0)
        {
            s1 = -e_b / (2 * e_a);
            s2 = -e_b / (2 * e_a);
        }
        else
        {
            parteReal = -e_b / (2 * e_a);
            parteImaginaria = raizDiscriminante / (2 * e_a);
            cout << "No hay interseccion" << endl;
        }

        double p1, p2;
        double _x_1, _y_1, _x_2, _y_2;
        p1 = (ml * s1) + bl;
        p2 = (ml * s2) + bl;

        _x_1 = s1 * a;
        _y_1 = p1 * b;
        _x_2 = s2 * a;
        _y_2 = p2 * b;

        // cout<<_x_1<<" "<<_y_1<<" "<<_x_2<<" "<<_y_2<<endl;

        double _x1_ = ((matriz_angulos[0][0] * _x_1) + (matriz_angulos[0][1] * _y_1)) + Xd;
        double _y1_ = ((matriz_angulos[1][0] * _x_1) + (matriz_angulos[1][1] * _y_1)) + Yd;
        double _x2_ = ((matriz_angulos[0][0] * _x_2) + (matriz_angulos[0][1] * _y_2)) + Xd;
        double _y2_ = ((matriz_angulos[1][0] * _x_2) + (matriz_angulos[1][1] * _y_2)) + Yd;

        cout << _x1_ << " " << _y1_ << " " << _x2_ << " " << _y2_ << endl;

        punto_medio_x = (_x1_ + _x2_) / 2;
        punto_medio_y = (_y1_ + _y2_) / 2;

        cout << "(XC , YC) = (" << punto_medio_x << " ," << punto_medio_y << ")" << endl;
    }
};

#endif