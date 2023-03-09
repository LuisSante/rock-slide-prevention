#ifndef SPEED_HPP
#define SPEED_HPP

#include <math.h>
#include <iostream>
#include "rungek.hpp"

class Speed
{
private:
    RungeKutta rk;

public:

    Speed() : rk(){}

    float distancia_euclidiana(float x1, float y1, float x2, float y2)
    {
        float dif1 = (x2 - x1) * (x2 - x1);
        float dif2 = (y2 - y1) * (y2 - y1);
        float distancia = sqrt(dif1 + dif2);

        return distancia;
    }

    void velocidad(float vertices_talud_[] ,float Xc, float Yc)
    {
        float r1 = (rk.Xd - Xc) * (rk.Xd - Xc);
        float r2 = (rk.Yd - Yc) * (rk.Yd - Yc);
        float rc = sqrt(r1 + r2);

        // vectores
        float rc_vi = Xc - rk.Xd;
        float rc_vj = Yc - rk.Yd;

        float angle = atan(rc_vj / rc_vi) + 3.1416;

        // std::cout << " " << rc_vi << " " << rc_vj << " " << std::endl;

        // vectores
        rc_vi = rc * cos(angle);
        rc_vj = rc * sin(angle);

        // std::cout << " " << rc_vi << " " << rc_vj << " " << std::endl;

        float vc_i = rk.vx - (rk.w * rc * sin(angle));
        float vc_j = rk.vy + (rk.w * rc * cos(angle));

        // vectores unitarios
        float distance = distancia_euclidiana(vertices_talud_[0], vertices_talud_[1], vertices_talud_[3], vertices_talud_[4]);

        float w_i = (vertices_talud_[3] - vertices_talud_[0]) / distance;
        float w_j = (vertices_talud_[4] - vertices_talud_[1]) / distance;
        float e_i = w_j;
        float e_j = -w_i;

        // vector velocidad
        float sigma_t_i = (w_i * vc_i + w_j * vc_j) * w_i;
        float sigma_t_j = (w_i * vc_i + w_j * vc_j) * w_j;
        float sigma_n_i = (e_i * vc_i + e_j * vc_j) * e_i;
        float sigma_n_j = (e_i * vc_i + e_j * vc_j) * e_j;
        // std::cout << " " << vc_i << " " << vc_j << " " << std::endl;

        std::cout << sigma_t_i << " " << sigma_t_j << " " << sigma_n_i << " " << sigma_n_j << std::endl;
    }
};

#endif