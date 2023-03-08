#ifndef SUPERPOSITION_HPP
#define SUPERPOSITION_HPP

#include <math.h>
#include <iostream>

// REQUISITOS
// sigma = distancia del punto externo de la elipse a cuerda
// punto medio
// vector unitario e
// vector unitario w

const int MAX_DIMENSION_ = 100;

class Superposition
{
private:
    float Xd;
    float Yd;
    float a;
    float b;
    float ml;
    float bl;
    float theta_talud = 10 * 3.1416 / 180;
    float matriz_angulos[MAX_DIMENSION_][MAX_DIMENSION_];

public:
    Superposition(float Xd, float Yd, float a, float b, float ml, float bl)
    {
        this->Xd = Xd;
        this->Yd = Yd;
        this->a = a;
        this->b = b;
        this->ml = ml;
        this->bl = bl;
        matriz_angulos[0][0] = cos(theta_talud);
        matriz_angulos[0][1] = -sin(theta_talud);
        matriz_angulos[1][0] = sin(theta_talud);
        matriz_angulos[1][1] = cos(theta_talud);
    }

    void superposition(float ml, float bl, float vertices_talud_[])
    {
        float pA1 = (1 / (sqrt(1 + (ml * ml))));
        float pA2 = -(1 / (sqrt(1 + (ml * ml))));

        float sA1 = -ml * pA1;
        float sA2 = -ml * pA2;

        float xA1 = sA1 * a;
        float yA1 = pA1 * b;
        float xA2 = sA2 * a;
        float yA2 = pA2 * b;

        double xA1_ = ((matriz_angulos[0][0] * xA1) + (matriz_angulos[0][1] * yA1)) + Xd;
        double yA1_ = ((matriz_angulos[1][0] * xA1) + (matriz_angulos[1][1] * yA1)) + Yd;
        double xA2_ = ((matriz_angulos[0][0] * xA2) + (matriz_angulos[0][1] * yA2)) + Xd;
        double yA2_ = ((matriz_angulos[1][0] * xA2) + (matriz_angulos[1][1] * yA2)) + Yd;

        double A = -(vertices_talud_[4] - vertices_talud_[1]) / (vertices_talud_[3] - vertices_talud_[0]);
        double B = 1;
        double C = (-A * vertices_talud_[0]) - vertices_talud_[1];
        // double C = (-A*vertices_talud_[3]) - vertices_talud_[4];

        std::cout << A << " " << B << " " << C << std::endl;
        float per_1 = ((A * xA1_) + (B * yA1_) + C) / (sqrt((A * A) + (B * B)));
        float per_2 = ((A * xA2_) + (B * yA2_) + C) / (sqrt((A * A) + (B * B)));

        std::cout << per_1 << " " << per_2 << std::endl;
    }
};

#endif