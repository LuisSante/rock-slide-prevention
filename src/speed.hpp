#ifndef SPEED_HPP
#define SPEED_HPP

#include <math.h>
#include <iostream>
#include <vector>

#include "draw.hpp"

using namespace std;

class Speed
{
private:
    Draw draw;
    float sigma_n_i;
    float sigma_n_j;

public:
    Speed() : draw()
    {
    }

    float distancia_euclidiana(float x1, float y1, float x2, float y2);
    vector<float> vectores_rc(float vertices_talud_[], float Xc, float Yc);
    vector<float> velocidad_vec_contacto(float vertices_talud_[], float Xc, float Yc);
    vector<float> velocidad(float vertices_talud_[], float Xc, float Yc);
};

float Speed::distancia_euclidiana(float x1, float y1, float x2, float y2)
{
    float dif1 = (x2 - x1) * (x2 - x1);
    float dif2 = (y2 - y1) * (y2 - y1);
    float distancia = sqrt(dif1 + dif2);

    return distancia;
}

vector<float> Speed::vectores_rc(float vertices_talud_[], float Xc, float Yc)
{
    std::vector<float> result = {};

    float r1 = (draw.Xd - Xc) * (draw.Xd - Xc);
    float r2 = (draw.Yd - Yc) * (draw.Yd - Yc);
    float rc = sqrt(r1 + r2);

    // vectores
    float rc_vi = Xc - draw.Xd;
    float rc_vj = Yc - draw.Yd;

    float angle = atan(rc_vj / rc_vi) + 3.1416;

    // std::cout << " " << rc_vi << " " << rc_vj << " " << std::endl;

    // vectores
    rc_vi = rc * cos(angle);
    rc_vj = rc * sin(angle);
    result.push_back(rc_vi);
    result.push_back(rc_vj);
    // std::cout << " " << rc_vi << " " << rc_vj << " " << std::endl;

    return result;
}

vector<float> Speed::velocidad_vec_contacto(float vertices_talud_[], float Xc, float Yc)
{
    // arreglos
    vector<float> vectores_rc_ = vectores_rc(vertices_talud_, Xc, Yc);
    vector<float> vectores_contacto = {/*vc_i , vc_j , w_i , w_j , e_i , e_j*/};
    float vc_i = draw.vx - (draw.w * vectores_rc_[1]);
    vectores_contacto.push_back(vc_i);
    float vc_j = draw.vy + (draw.w * vectores_rc_[0]);
    vectores_contacto.push_back(vc_j);

    // vectores unitarios
    float distance = distancia_euclidiana(vertices_talud_[0], vertices_talud_[1], vertices_talud_[3], vertices_talud_[4]);

    float w_i = (vertices_talud_[3] - vertices_talud_[0]) / distance;
    vectores_contacto.push_back(w_i);
    float w_j = (vertices_talud_[4] - vertices_talud_[1]) / distance;
    vectores_contacto.push_back(w_j);
    float e_i = w_j;
    vectores_contacto.push_back(e_i);
    float e_j = -w_i;
    vectores_contacto.push_back(e_j);

    return vectores_contacto;
}

vector<float> Speed::velocidad(float vertices_talud_[], float Xc, float Yc)
{
    vector<float> vectores_v_contacto = velocidad_vec_contacto(vertices_talud_, Xc, Yc);
    vector<float> velocidad = {};
    // vector velocidad
    float sigma_t_i = (vectores_v_contacto[2] * vectores_v_contacto[0] + vectores_v_contacto[3] * vectores_v_contacto[1]) * vectores_v_contacto[2];
    velocidad.push_back(sigma_t_i);
    float sigma_t_j = (vectores_v_contacto[2] * vectores_v_contacto[0] + vectores_v_contacto[3] * vectores_v_contacto[1]) * vectores_v_contacto[3];
    velocidad.push_back(sigma_t_j);
    float sigma_n_i = (vectores_v_contacto[4] * vectores_v_contacto[0] + vectores_v_contacto[5] * vectores_v_contacto[1]) * vectores_v_contacto[4];
    velocidad.push_back(sigma_n_i);
    float sigma_n_j = (vectores_v_contacto[4] * vectores_v_contacto[0] + vectores_v_contacto[5] * vectores_v_contacto[1]) * vectores_v_contacto[5];
    // std::cout << " " << vc_i << " " << vc_j << " " << std::endl;
    velocidad.push_back(sigma_n_j);
    std::cout << sigma_t_i << " " << sigma_t_j << " " << sigma_n_i << " " << sigma_n_j << std::endl;

    return velocidad;
}

#endif