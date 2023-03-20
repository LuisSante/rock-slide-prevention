#ifndef SPEED_HPP
#define SPEED_HPP

#include <iostream>
#include <vector>
#include <fstream>

#include <cmath>

#include "draw.hpp"
#include "punto_contacto.hpp"

#ifdef M_PI
    #undef M_PI
#endif
#define M_PI 3.14159265358979323846f

ofstream fuerzas("src/fuerzas_externas.txt");

using namespace std;

class Speed_F_Normal
{
private:
    Draw draw;
    PuntoContacto contact;
    /*float sigma_n_i;
    float sigma_n_j;*/

    float E = 350000.0f;
    float v = 0.275f;
    float m_star = 120.0f;
    float g_young = 9.7639f;
    float en_coe_restitution = 0.75f;
    float et_coe_restitution = 0.59f;
    float mu = 0.25f;

    // Fuerzas que van a cambiar la velocidad , traslacion y rotacion
    float FN_i;
    float FN_j;
    float Ft_i;
    float Ft_j;


public:
    Speed_F_Normal();
    Speed_F_Normal(Draw draw) : draw(draw), contact(draw) {
        this -> FN_i = 0.0f;
        this -> FN_j = 0.0f;
        this -> Ft_i = 0.0f;
        this -> Ft_j = 0.0f;
    }

    float distancia_euclidiana(float x1, float y1, float x2, float y2);
    vector<float> vectores_rc(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float vertices_talud[]);
    vector<float> velocidad_vec_contacto(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float vertices_talud[]);
    vector<float> velocidad(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float vertices_talud[]);

    float gamma_n();
    float gamma_t();

    float radio_equivalente();
    float young_module();
    float equivalent_shear_modulus();
    float fuerza_normal(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float vertices_talud[]);
    float coeficiente_friccion(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float vertices_talud[]);
    float fuerza_tangencial(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float vertices_talud[]);
    float momentos(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float vertices_talud[]);

    friend class RungeKutta;
};

Speed_F_Normal :: Speed_F_Normal()
{
    FN_i = 0;
    FN_j = 0;
    Ft_i = 0;
    Ft_j = 0;
}

float Speed_F_Normal::distancia_euclidiana(float x1, float y1, float x2, float y2)
{
    float dif1 = (x2 - x1) * (x2 - x1);
    float dif2 = (y2 - y1) * (y2 - y1);
    float distancia = sqrt(dif1 + dif2);

    return distancia;
}

vector<float> Speed_F_Normal::vectores_rc(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float vertices_talud[])
{
    vector<float> result = {};

    vector<float> point_middle = contact.machine(current_center_mass_X, current_center_mass_Y, vertices_talud);

    float r1 = (current_center_mass_X - point_middle[2]) * (current_center_mass_X - point_middle[2]);
    float r2 = (current_center_mass_Y - point_middle[3]) * (current_center_mass_Y - point_middle[3]);
    float rc = sqrt(r1 + r2);

    // vectores
    float rc_vi = point_middle[2] - current_center_mass_X;
    float rc_vj = point_middle[3] - current_center_mass_Y;

    float angle_center = atan(rc_vj / rc_vi) + M_PI;

    // cout << " " << rc_vi << " " << rc_vj << " " << endl;

    // vectores
    rc_vi = rc * cos(angle_center);
    rc_vj = rc * sin(angle_center);
    result.push_back(rc_vi);
    result.push_back(rc_vj);
    // cout << " " << rc_vi << " " << rc_vj << " " << endl;

    return result;
}

vector<float> Speed_F_Normal::velocidad_vec_contacto(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float vertices_talud[])
{
    // arreglos
    vector<float> vectores_rc_ = vectores_rc(current_center_mass_X, current_center_mass_Y, current_velocity_X, current_velocity_Y, angle, vertices_talud);
    vector<float> vectores_contacto = {/*vc_i , vc_j , w_i , w_j , e_i , e_j*/};

    // Angle es la velocidad angular
    float vc_i = current_velocity_X - (angle * vectores_rc_[1]);
    vectores_contacto.push_back(vc_i);
    float vc_j = current_velocity_Y + (angle * vectores_rc_[0]);
    vectores_contacto.push_back(vc_j);

    // vectores unitarios
    float distance = distancia_euclidiana(vertices_talud[0], vertices_talud[1], vertices_talud[3], vertices_talud[4]);

    float w_i = (vertices_talud[3] - vertices_talud[0]) / distance;
    vectores_contacto.push_back(w_i);
    float w_j = (vertices_talud[4] - vertices_talud[1]) / distance;
    vectores_contacto.push_back(w_j);
    float e_i = w_j;
    vectores_contacto.push_back(e_i);
    float e_j = -w_i;
    vectores_contacto.push_back(e_j);

    return vectores_contacto;
}

vector<float> Speed_F_Normal::velocidad(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float vertices_talud[])
{
    vector<float> vectores_v_contacto = velocidad_vec_contacto(current_center_mass_X, current_center_mass_Y, current_velocity_X, current_velocity_Y, angle, vertices_talud);
    vector<float> velocidad_sigma;

    // vector velocidad
    float sigma_t_i = (vectores_v_contacto[2] * vectores_v_contacto[0] + vectores_v_contacto[3] * vectores_v_contacto[1]) * vectores_v_contacto[2];
    velocidad_sigma.push_back(sigma_t_i);
    float sigma_t_j = (vectores_v_contacto[2] * vectores_v_contacto[0] + vectores_v_contacto[3] * vectores_v_contacto[1]) * vectores_v_contacto[3];
    velocidad_sigma.push_back(sigma_t_j);
    float sigma_n_i = (vectores_v_contacto[4] * vectores_v_contacto[0] + vectores_v_contacto[5] * vectores_v_contacto[1]) * vectores_v_contacto[4];
    velocidad_sigma.push_back(sigma_n_i);
    float sigma_n_j = (vectores_v_contacto[4] * vectores_v_contacto[0] + vectores_v_contacto[5] * vectores_v_contacto[1]) * vectores_v_contacto[5];
    velocidad_sigma.push_back(sigma_n_j);

    return velocidad_sigma;
}

float Speed_F_Normal::radio_equivalente()
{
    float Aprima = 0, Bprima = 0;
    float r_star;
    /*Aprima = 0.5 * ((0.5 * (1 / draw.a + 1 / draw.b) + 0.25 * (1 / draw.a - 1 / draw.b)));
    Bprima = 0.5 * ((0.5 * (1 / draw.a + 1 / draw.b) - 0.25 * (1 / draw.a + 1 / draw.b)));*/

    Aprima = 0.5f * (1 / draw.a);
    Bprima = 0.5f * (1 / draw.b);

    r_star = 1 / (2 * sqrt(Aprima * Bprima));

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
    gama = (-log(en_coe_restitution)) / (sqrt((M_PI * M_PI) + (log(en_coe_restitution) * log(en_coe_restitution))));
    return gama;
}

float Speed_F_Normal::gamma_t()
{
    float gama;
    gama = -log(et_coe_restitution) / (sqrt((M_PI * M_PI) + (log(et_coe_restitution) * log(et_coe_restitution))));
    return gama;
}

float Speed_F_Normal::fuerza_normal(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float vertices_talud[])
{
    vector<float> abs_ = contact.superposition(current_center_mass_X, current_center_mass_Y, vertices_talud);
    float module = abs_[1];
    if (abs_[1] < 0)
        module = -module;

    float E_star = young_module();
    float r_star = radio_equivalente();
    float k_n = 2 * E_star * sqrt((r_star * module));
    float kn_module_sigma = k_n * module; // e

    vector<float> velocity_contact = velocidad_vec_contacto(current_center_mass_X, current_center_mass_Y, current_velocity_X, current_velocity_Y, angle, vertices_talud);
    // vectores i , j
    // fuerza resorte
    float kn_module_i = kn_module_sigma * velocity_contact[4];
    float kn_module_j = kn_module_sigma * velocity_contact[5];

    // fuerza amortiguadora
    float gama = gamma_n();
    float cn = 2 * gama * sqrt(m_star * k_n);

    vector<float> vector_velocity = velocidad(current_center_mass_X, current_center_mass_Y, current_velocity_X, current_velocity_Y, angle, vertices_talud); /*sigma_point[2]i y sigma_point[3]j pos*/

    float cn_sigma_n_i = cn * vector_velocity[2];
    float cn_sigma_n_j = cn * vector_velocity[3];

    kn_module_i = -kn_module_i;
    kn_module_j = -kn_module_j;
    cn_sigma_n_i = -cn_sigma_n_i;
    cn_sigma_n_j = -cn_sigma_n_j;

    FN_i = kn_module_i + cn_sigma_n_i;
    FN_j = kn_module_j + cn_sigma_n_j;
    float FN = sqrt((FN_i * FN_i) + (FN_j * FN_j));

    return FN;
}

float Speed_F_Normal::coeficiente_friccion(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float vertices_talud[])
{
    float FN = fuerza_normal(current_center_mass_X, current_center_mass_Y, current_velocity_X, current_velocity_Y, angle, vertices_talud);
    fuerzas << "FN_i : " <<FN_i << " FN_j : " << FN_j << " ";
    return mu * FN;
}

float Speed_F_Normal::fuerza_tangencial(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float vertices_talud[])
{
    vector<float> abs_ = contact.superposition(current_center_mass_X, current_center_mass_Y, vertices_talud);
    float module = abs_[1];
    if (abs_[1] < 0)
        module = -module;

    vector<float> vector_velocity = velocidad(current_center_mass_X, current_center_mass_Y, current_velocity_X, current_velocity_Y, angle, vertices_talud); /*sigma_point[2]i y sigma_point[3]j pos*/
    vector<float> w = velocidad_vec_contacto(current_center_mass_X, current_center_mass_Y, current_velocity_X, current_velocity_Y, angle, vertices_talud);
    float mu_fn = coeficiente_friccion(current_center_mass_X, current_center_mass_Y, current_velocity_X, current_velocity_Y, angle, vertices_talud);

    float mu_fn_i = mu_fn * -w[2];
    float mu_fn_j = mu_fn * -w[3];

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

    // PROBLEMA ESTA AQUI
    Ft_i = actual_fuerza_tangencial_resorte_i + F_tangecial_amortiguadora_i;
    Ft_j = actual_fuerza_tangencial_resorte_j + F_tangecial_amortiguadora_j;

    float Ft = sqrt((Ft_i * Ft_i) + (Ft_j * Ft_j));

    float rpta;

    if (Ft >= mu_fn)
    {
        rpta = mu_fn;
        Ft_i = mu_fn_i;
        Ft_j = mu_fn_j;
        fuerzas << " Ft_i : " <<Ft_i << " Ft_j : " << Ft_j << endl;
    }
    else
    {
        rpta = Ft;
        fuerzas << " Ft_i : " <<Ft_i << " Ft_j : " << Ft_j << endl;
    }

    return Ft;
}

float Speed_F_Normal::momentos(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float vertices_talud[])
{
    vector<float> rc = vectores_rc(current_center_mass_X, current_center_mass_Y, current_velocity_X, current_velocity_Y, angle, vertices_talud);
    float f_n = fuerza_normal(current_center_mass_X, current_center_mass_Y, current_velocity_X, current_velocity_Y, angle, vertices_talud);
    float f_t = fuerza_tangencial(current_center_mass_X, current_center_mass_Y, current_velocity_X, current_velocity_Y, angle, vertices_talud);

    float f_total_i = FN_i + Ft_i;
    float f_total_j = FN_j + Ft_j;

    float M_G = (rc[0] * f_total_j) - (rc[1] * f_total_i);

    return M_G;
}

#endif