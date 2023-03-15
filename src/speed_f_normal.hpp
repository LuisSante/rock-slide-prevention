#ifndef SPEED_HPP
#define SPEED_HPP

#include <math.h>
#include <iostream>
#include <vector>

#include "draw.hpp"
#include "punto_contacto.hpp"

const float PI_ = 3.1415;

using namespace std;

class Speed_F_Normal
{
private:
    Draw draw;
    PuntoContacto contact;
    float sigma_n_i;
    float sigma_n_j;

    float E = 350000;
    float v = 0.275;
    float m_star = 120;
    float g_young = 9.7639;
    float en_coe_restitution = 0.75;
    float et_coe_restitution = 0.59;
    float mu = 0.25;

public:
    Speed_F_Normal() : draw()
    {
    }

    float distancia_euclidiana(float x1, float y1, float x2, float y2);
    vector<float> vectores_rc(float vertices_talud_[]);
    vector<float> velocidad_vec_contacto(float vertices_talud_[]);
    vector<float> velocidad(float vertices_talud_[]);

    void is_negative(float &a_i, float &a_j, float &b_i, float &b_j);
    float gamma_n();
    float gamma_t();

    float radio_equivalente();
    float young_module();
    float equivalent_shear_modulus();
    vector<float> fuerza_normal(float vertices_talud[]);
    float coeficiente_friccion(float vertices_talud[]);
    vector<float> fuerza_tangencial(float vertices_talud[]);
    float momentos(float vertices_talud[]);
};

float Speed_F_Normal::distancia_euclidiana(float x1, float y1, float x2, float y2)
{
    float dif1 = (x2 - x1) * (x2 - x1);
    float dif2 = (y2 - y1) * (y2 - y1);
    float distancia = sqrt(dif1 + dif2);

    return distancia;
}

vector<float> Speed_F_Normal::vectores_rc(float vertices_talud_[])
{
    vector<float> result = {};

    vector<float> point_middle = contact.machine(vertices_talud_);

    float r1 = (draw.Xd - point_middle[2]) * (draw.Xd - point_middle[2]);
    float r2 = (draw.Yd - point_middle[3]) * (draw.Yd - point_middle[3]);
    float rc = sqrt(r1 + r2);

    // vectores
    float rc_vi = point_middle[2] - draw.Xd;
    float rc_vj = point_middle[3] - draw.Yd;

    float angle = atan(rc_vj / rc_vi) + 3.1416;

    // cout << " " << rc_vi << " " << rc_vj << " " << endl;

    // vectores
    rc_vi = rc * cos(angle);
    rc_vj = rc * sin(angle);
    result.push_back(rc_vi);
    result.push_back(rc_vj);
    // cout << " " << rc_vi << " " << rc_vj << " " << endl;

    return result;
}

vector<float> Speed_F_Normal::velocidad_vec_contacto(float vertices_talud_[])
{
    // arreglos
    vector<float> vectores_rc_ = vectores_rc(vertices_talud_);
    vector<float> vectores_contacto = {/*vc_i , vc_j , w_i , w_j , e_i , e_j*/};

    float vc_i = draw.vx0 - (draw.w * vectores_rc_[1]);
    vectores_contacto.push_back(vc_i);
    float vc_j = draw.vy0 + (draw.w * vectores_rc_[0]);
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

vector<float> Speed_F_Normal::velocidad(float vertices_talud_[])
{
    vector<float> vectores_v_contacto = velocidad_vec_contacto(vertices_talud_);
    vector<float> velocidad = {};

    // vector velocidad
    float sigma_t_i = (vectores_v_contacto[2] * vectores_v_contacto[0] + vectores_v_contacto[3] * vectores_v_contacto[1]) * vectores_v_contacto[2];
    velocidad.push_back(sigma_t_i);
    float sigma_t_j = (vectores_v_contacto[2] * vectores_v_contacto[0] + vectores_v_contacto[3] * vectores_v_contacto[1]) * vectores_v_contacto[3];
    velocidad.push_back(sigma_t_j);
    float sigma_n_i = (vectores_v_contacto[4] * vectores_v_contacto[0] + vectores_v_contacto[5] * vectores_v_contacto[1]) * vectores_v_contacto[4];
    velocidad.push_back(sigma_n_i);
    float sigma_n_j = (vectores_v_contacto[4] * vectores_v_contacto[0] + vectores_v_contacto[5] * vectores_v_contacto[1]) * vectores_v_contacto[5];
    velocidad.push_back(sigma_n_j);

    // cout << " " << vc_i << " " << vc_j << " " << endl;
    // cout << sigma_t_i << " " << sigma_t_j << " " << sigma_n_i << " " << sigma_n_j << endl;

    return velocidad;
}

void Speed_F_Normal::is_negative(float &a_i, float &a_j, float &b_i, float &b_j)
{
    if (a_i < 0)
        a_i = -a_i;
    if (a_j < 0)
        a_j = -a_j;
    if (b_i < 0)
        b_i = -b_i;
    if (b_j < 0)
        b_j = -b_j;
}

float Speed_F_Normal::radio_equivalente()
{
    float Aprima, Bprima;
    float r_star;
    Aprima = 0.5 * ((0.5 * (1 / draw.a + 1 / draw.b) + 0.25 * (1 / draw.a - 1 / draw.b)));
    Bprima = 0.5 * ((0.5 * (1 / draw.a + 1 / draw.b) - 0.25 * (1 / draw.a + 1 / draw.b)));

    r_star = 1 / (sqrt(Aprima * Bprima));

    return r_star;
}

float Speed_F_Normal::young_module()
{
    float E_star;

    E_star = E / (1 - (v * v));
    E_star = E_star * 98.0665;

    return E_star;
}

float Speed_F_Normal::equivalent_shear_modulus()
{
    float G_star;

    float G = E / (2 * (1 + v));
    G_star = G / (2 - v);
    G_star = G_star * 98.0665;

    return G_star;
}

float Speed_F_Normal::gamma_n()
{
    float gama;
    gama = -log(en_coe_restitution) / (sqrt((PI_ * PI_) + (log(en_coe_restitution) * log(en_coe_restitution))));
    return gama;
}

float Speed_F_Normal::gamma_t()
{
    float gama;
    gama = -log(et_coe_restitution) / (sqrt((PI_ * PI_) + (log(et_coe_restitution) * log(et_coe_restitution))));
    return gama;
}

vector<float> Speed_F_Normal::fuerza_normal(float vertices_talud[])
{
    vector<float> fuerza_n;
    vector<float> abs_ = contact.superposition(vertices_talud);
    float module = abs_[1];
    if (abs_[1] < 0)
        module = -module;

    float E_star = young_module();
    float r_star = radio_equivalente();
    float k_n = 2 * E_star * sqrt((r_star * module));
    float kn_module_sigma = k_n * module; // e

    vector<float> velocity_contact = velocidad_vec_contacto(vertices_talud);
    // vectores i , j
    // fuerza resorte
    float kn_module_i = k_n * velocity_contact[4];
    float kn_module_j = k_n * velocity_contact[5];

    // fuerza amortiguadora
    float gama = gamma_n();
    float cn = 2 * gama * sqrt(m_star * k_n);

    vector<float> vector_velocity = velocidad(vertices_talud); /*sigma_point[2]i y sigma_point[3]j pos*/
    float cn_sigma_n_i = cn * vector_velocity[2];
    float cn_sigma_n_j = cn * vector_velocity[3];

    is_negative(kn_module_i, kn_module_j, cn_sigma_n_i, cn_sigma_n_j);

    float FN_i = kn_module_i + cn_sigma_n_i;
    float FN_j = kn_module_j + cn_sigma_n_j;
    float FN = sqrt((FN_i * FN_i) + (FN_j * FN_j));

    fuerza_n.push_back(FN_i);
    fuerza_n.push_back(FN_j);
    fuerza_n.push_back(FN);

    return fuerza_n;
}

float Speed_F_Normal::coeficiente_friccion(float vertices_talud[])
{
    vector<float> FN = fuerza_normal(vertices_talud);
    return mu * FN[2];
}

vector<float> Speed_F_Normal::fuerza_tangencial(float vertices_talud[])
{
    vector<float> abs_ = contact.superposition(vertices_talud);
    float module = abs_[1];
    if (abs_[1] < 0)
        module = -module;

    vector<float> vector_velocity = velocidad(vertices_talud); /*sigma_point[2]i y sigma_point[3]j pos*/
    vector<float> w = velocidad_vec_contacto(vertices_talud);
    float mu_fn = coeficiente_friccion(vertices_talud);

    float mu_fn_i = mu_fn * vector_velocity[0];
    float mu_fn_j = mu_fn * vector_velocity[0];

    float G_star = equivalent_shear_modulus();
    float r_star = radio_equivalente();
    float kt = 8 * G_star * sqrt(r_star * module);

    float gama = gamma_t();
    float F_tangecial_amortiguadora = 2 * gama * sqrt(m_star * kt);
    float F_tangecial_amortiguadora_i = F_tangecial_amortiguadora * vector_velocity[0];
    float F_tangecial_amortiguadora_j = F_tangecial_amortiguadora * vector_velocity[1];

    float step_back_i = 0;
    float step_back_j = 0;

    float k_i_t_deltatime_i = kt * draw.h * vector_velocity[0];
    float k_i_t_deltatime_j = kt * draw.h * vector_velocity[1];

    float actual_fuerza_tangencial_resorte_i = k_i_t_deltatime_i + step_back_i;
    float actual_fuerza_tangencial_resorte_j = k_i_t_deltatime_j + step_back_j;

    actual_fuerza_tangencial_resorte_i = -actual_fuerza_tangencial_resorte_i;
    actual_fuerza_tangencial_resorte_j = -actual_fuerza_tangencial_resorte_j;
    F_tangecial_amortiguadora_i = -F_tangecial_amortiguadora_i;
    F_tangecial_amortiguadora_j = -F_tangecial_amortiguadora_j;

    float Ft_i = actual_fuerza_tangencial_resorte_i + F_tangecial_amortiguadora_i;
    float Ft_j = actual_fuerza_tangencial_resorte_j + F_tangecial_amortiguadora_j;

    float Ft = sqrt((Ft_i * Ft_i) + (Ft_j * Ft_j));

    float rpta;
    vector<float> vector_rpta;

    if (Ft >= mu_fn)
    {
        rpta = mu_fn;
        vector_rpta.push_back(mu_fn_i);
        vector_rpta.push_back(mu_fn_j);
    }
    else
    {
        rpta = Ft;
        vector_rpta.push_back(Ft_i);
        vector_rpta.push_back(Ft_j);
    }

    return vector_rpta;
}

float Speed_F_Normal::momentos(float vertices_talud[])
{
    vector<float> rc = vectores_rc(vertices_talud);
    vector<float> f_n = fuerza_normal(vertices_talud);
    vector<float> f_t = fuerza_tangencial(vertices_talud);

    float f_total_i = f_n[0] + f_t[0];
    float f_total_j = f_n[1] + f_t[1];

    float M_G = (rc[0] * f_total_j) + (rc[1] * f_total_i);

    return M_G;
}
#endif