#ifndef SUPERPOSITION_HPP
#define SUPERPOSITION_HPP

#include <math.h>
#include <iostream>


//REQUISITOS
//sigma = distancia del punto externo de la elipse a cuerda
//punto medio
//vector unitario e
//vector unitario w


void superposition(float ml, float bl, float matriz_angulos[][100], float vertices_talud_[] ,  float a , float b, float Xd, float Yd){
    float pA1 = (1/(sqrt(1 + (ml * ml))));
    float pA2 = -(1/(sqrt(1 + (ml * ml))));

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

    double A = -(vertices_talud_[4] - vertices_talud_[1])/(vertices_talud_[3] - vertices_talud_[0]);
    double B = 1;
    //float C = (A*xA1_) - yA1_;
    double C = (-A*vertices_talud_[3]) - vertices_talud_[4];

    std::cout<<A << " " << B << " " << C << std::endl;
    float per_1 = ((A*xA1_) + (B*yA1_) + C)/(sqrt((A*A)+(B*B)));
    float per_2 = ((A*xA2_) + (B*yA2_) + C)/(sqrt((A*A)+(B*B)));

    std::cout<< per_1 << " " << per_2 << std::endl;
}

#endif