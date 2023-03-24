#ifndef SPEED_HPP
#define SPEED_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <utility>

#include <cmath>

#include "draw.hpp"
#include "punto_contacto.hpp"

#ifdef M_PI
#undef M_PI
#endif
#define M_PI 3.14159265358979323846f

using namespace std;

class Speed_F_Normal
{
private:
    Draw draw;
    PointContact contact;

public:
    float E = 350000.0f;
    float v = 0.275f;
    float m_star = 120.0f;
    float g_young = 9.7639f;
    float en_coe_restitution = 0.75f;
    float et_coe_restitution = 0.59f;
    float mu = 0.25f;

public:
    pair<float, float> F_normal;
    pair<float, float> F_tangential;

public:
    vector<float> velocity_sigma;
    float FN;
    float Ft;
    float M_G;

    Speed_F_Normal();
    Speed_F_Normal(Draw &draw) : draw(draw), contact(draw)
    {
        this->F_normal.first = 0.0f;
        this->F_normal.second = 0.0f;
        this->F_tangential.first = 0.0f;
        this->F_tangential.second = 0.0f;
        this->FN = 0;
        this->Ft = 0;
        this->M_G = 0;

        for (int i = 0; i < velocity_sigma.size(); i++)
        {
            velocity_sigma[i] = 0.0f;
        }
    }

    float euclidean_distance(float &x1, float &y1, float &x2, float &y2);
    vector<float> vector_rc(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float vertex_slope[]);
    vector<float> velocity_vec_contact(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float vertex_slope[]);
    void velocity(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float vertex_slope[]);

    float gamma_n();
    float gamma_t();

    float radio_equivalent();
    float young_module();
    float equivalent_shear_modulus();
    void calculate_normal_force(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float vertex_slope[]);
    float coefficient_friction(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float vertex_slope[]);
    void calculate_tangential_force(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float h, float vertex_slope[]);
    void momentos(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float h, float vertex_slope[]);

    friend class RungeKutta;
};

Speed_F_Normal ::Speed_F_Normal()
{
    F_normal.first = 0.0f;
    F_normal.second = 0.0f;
    F_tangential.first = 0.0f;
    F_tangential.second = 0.0f;
    FN = 0.0f;
    Ft = 0.0f;
    M_G = 0.0f;

    for (int i = 0; i < velocity_sigma.size(); i++)
    {
        velocity_sigma[i] = 0;
    }
}

float Speed_F_Normal::euclidean_distance(float &x1, float &y1, float &x2, float &y2)
{
    float dif1 = (x2 - x1) * (x2 - x1);
    float dif2 = (y2 - y1) * (y2 - y1);
    float distance = sqrt(dif1 + dif2);

    return distance;
}

vector<float> Speed_F_Normal::vector_rc(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float vertex_slope[])
{
    vector<float> result = {};

    vector<float> point_middle = contact.machine(current_center_mass_X, current_center_mass_Y, vertex_slope);

    double r1 = (current_center_mass_X - point_middle[2]) * (current_center_mass_X - point_middle[2]);
    double r2 = (current_center_mass_Y - point_middle[3]) * (current_center_mass_Y - point_middle[3]);
    double rc = sqrt(r1 + r2);
    // cout<<"rc"<<rc * 3500 <<endl;

    // vectores
    float rc_vi = static_cast<double>(point_middle[2] - current_center_mass_X);
    float rc_vj = static_cast<double>(point_middle[3] - current_center_mass_Y);
    // cout<<rc_vi * 3500 <<" "<<rc_vj * 3500<<endl;

    float angle_center = atan(rc_vj / rc_vi) + M_PI;

    // cout << " " << rc_vi << " " << rc_vj << " " << endl;

    // vectores
    rc_vi = rc * cos(angle_center);
    rc_vj = rc * sin(angle_center);
    result.push_back(rc_vi);
    result.push_back(rc_vj);
    // cout << " " << rc_vi * 3500 << " " << rc_vj * 3500<< " " << endl;

    return result;
}

vector<float> Speed_F_Normal::velocity_vec_contact(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float vertex_slope[])
{
    // arreglos
    vector<float> vector_rc_ = vector_rc(current_center_mass_X, current_center_mass_Y, current_velocity_X, current_velocity_Y, angle, vertex_slope);
    vector<float> vectors_contact = {/*vc_i , vc_j , w_i , w_j , e_i , e_j*/};

    // Angle es la velocity angular
    float vc_i = current_velocity_X - (angle * vector_rc_[1]);
    vectors_contact.push_back(vc_i);
    float vc_j = current_velocity_Y + (angle * vector_rc_[0]);
    vectors_contact.push_back(vc_j);

    // vectores unitarios
    float distance = euclidean_distance(vertex_slope[0], vertex_slope[1], vertex_slope[3], vertex_slope[4]);

    float w_i = (vertex_slope[3] - vertex_slope[0]) / distance;
    vectors_contact.push_back(w_i);
    float w_j = (vertex_slope[4] - vertex_slope[1]) / distance;
    vectors_contact.push_back(w_j);
    float e_i = w_j;
    vectors_contact.push_back(e_i);
    float e_j = -w_i;
    vectors_contact.push_back(e_j);

    // cout << w_i << " " << w_j << " " << e_i << " " << e_j << endl;

    return vectors_contact;
}

void Speed_F_Normal::velocity(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float vertex_slope[])
{
    vector<float> vectors_v_contact = velocity_vec_contact(current_center_mass_X, current_center_mass_Y, current_velocity_X, current_velocity_Y, angle, vertex_slope);

    // vector velocity
    float sigma_t_i = (vectors_v_contact[2] * vectors_v_contact[0] + vectors_v_contact[3] * vectors_v_contact[1]) * vectors_v_contact[2];
    velocity_sigma.push_back(sigma_t_i);
    float sigma_t_j = (vectors_v_contact[2] * vectors_v_contact[0] + vectors_v_contact[3] * vectors_v_contact[1]) * vectors_v_contact[3];
    velocity_sigma.push_back(sigma_t_j);
    float sigma_n_i = (vectors_v_contact[4] * vectors_v_contact[0] + vectors_v_contact[5] * vectors_v_contact[1]) * vectors_v_contact[4];
    velocity_sigma.push_back(sigma_n_i);
    float sigma_n_j = (vectors_v_contact[4] * vectors_v_contact[0] + vectors_v_contact[5] * vectors_v_contact[1]) * vectors_v_contact[5];
    velocity_sigma.push_back(sigma_n_j);

    // return velocity_sigma;
}

float Speed_F_Normal::radio_equivalent()
{
    float A_prima = 0, B_prima = 0;
    float r_star;

    A_prima = 0.5f * (1 / (draw.a ));
    B_prima = 0.5f * (1 / (draw.b ));

    r_star = 1 / (2 * sqrt(A_prima * B_prima));

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

void Speed_F_Normal::calculate_normal_force(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float vertex_slope[])
{
    contact.superposition(current_center_mass_X, current_center_mass_Y, vertex_slope);
    float module = contact.perpendicular.second;
    if (module < 0)
        module = -module;

    float E_star = young_module();
    float r_star = radio_equivalent();
    float k_n = 2 * E_star * sqrt((r_star * module));
    float kn_module_sigma = k_n * module; // e
    // cout << "modulo: " << module << endl;
    // cout << "E_star " << E_star << endl;
    // cout << "r_star " << r_star << endl;
    // cout << "kn: " << k_n << endl;
    // cout << "K_n_module_sigma: " << kn_module_sigma << endl;

    vector<float> velocity_contact = velocity_vec_contact(current_center_mass_X, current_center_mass_Y, current_velocity_X, current_velocity_Y, angle, vertex_slope);
    // vectores i , j
    // fuerza resort
    float kn_module_i = kn_module_sigma * velocity_contact[4];
    float kn_module_j = kn_module_sigma * velocity_contact[5];

    // fuerza amortiguadora
    float gama = gamma_n();
    float cn = 2 * gama * sqrt(m_star * k_n);

    velocity(current_center_mass_X, current_center_mass_Y, current_velocity_X, current_velocity_Y, angle, vertex_slope); /*sigma_point[2]i y sigma_point[3]j pos*/

    float cn_sigma_n_i = cn * velocity_sigma[2];
    float cn_sigma_n_j = cn * velocity_sigma[3];

    kn_module_i = -kn_module_i;
    kn_module_j = -kn_module_j;
    cn_sigma_n_i = -cn_sigma_n_i;
    cn_sigma_n_j = -cn_sigma_n_j;

    float FN_i = kn_module_i + cn_sigma_n_i;
    float FN_j = kn_module_j + cn_sigma_n_j;

    F_normal.first = FN_i;
    F_normal.second = FN_j;

    /*cout << "CLASE SPEED "
         << "FN_I : " << FN_i << " Fn_j : " << FN_j << endl;*/

    FN = sqrt((FN_i * FN_i) + (FN_j * FN_j));
}

float Speed_F_Normal::coefficient_friction(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float vertex_slope[])
{
    calculate_normal_force(current_center_mass_X, current_center_mass_Y, current_velocity_X, current_velocity_Y, angle, vertex_slope);
    return mu * FN;
}

void Speed_F_Normal::calculate_tangential_force(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float h, float vertex_slope[])
{
    contact.superposition(current_center_mass_X, current_center_mass_Y, vertex_slope);
    float module = contact.perpendicular.second;
    if (module < 0)
        module = -module;

    velocity(current_center_mass_X, current_center_mass_Y, current_velocity_X, current_velocity_Y, angle, vertex_slope); /*sigma_point[2]i y sigma_point[3]j pos*/
    vector<float> w = velocity_vec_contact(current_center_mass_X, current_center_mass_Y, current_velocity_X, current_velocity_Y, angle, vertex_slope);
    float mu_fn = coefficient_friction(current_center_mass_X, current_center_mass_Y, current_velocity_X, current_velocity_Y, angle, vertex_slope);

    float mu_fn_i = mu_fn * -w[2];
    float mu_fn_j = mu_fn * -w[3];

    float G_star = equivalent_shear_modulus();
    float r_star = radio_equivalent();
    float kt = 8 * G_star * sqrt(r_star * module);
    float gama = gamma_t();

    // cout << "modulo: " << module << endl;
    // cout << "G_star " << G_star << endl;
    // cout << "r_star " << r_star << endl;
    // cout << "kt: " << kt << endl;
    // cout << "gama: " << gama << endl;

    float F_tangential_buffer = 2 * gama * sqrt(m_star * kt);
    float F_tangential_buffer_i = F_tangential_buffer * velocity_sigma[0];
    float F_tangential_buffer_j = F_tangential_buffer * velocity_sigma[1];

    float step_back_i = 0; // Fuerza tangencial elastica en i
    float step_back_j = 0; // Fuerza tangencial elastica en j

    float k_i_t_delta_time_i = kt * h * velocity_sigma[0];
    float k_i_t_delta_time_j = kt * h * velocity_sigma[1];

    float actual_calculate_tangential_force_resort_i = k_i_t_delta_time_i + step_back_i;
    float actual_calculate_tangential_force_resort_j = k_i_t_delta_time_j + step_back_j;

    actual_calculate_tangential_force_resort_i = -actual_calculate_tangential_force_resort_i;
    actual_calculate_tangential_force_resort_j = -actual_calculate_tangential_force_resort_j;

    F_tangential_buffer_i = -F_tangential_buffer_i;
    F_tangential_buffer_j = -F_tangential_buffer_j;

    float Ft_i = actual_calculate_tangential_force_resort_i + F_tangential_buffer_i;
    float Ft_j = actual_calculate_tangential_force_resort_j + F_tangential_buffer_j;

    Ft = sqrt((Ft_i * Ft_i) + (Ft_j * Ft_j));

    // cout << actual_calculate_tangential_force_resort_i << actual_calculate_tangential_force_resort_j << endl;
    // cout << F_tangential_buffer_i << " " << F_tangential_buffer_j << endl;
    // cout << Ft << endl;

    // float rpta;

    if (Ft >= mu_fn)
    {
        Ft = mu_fn;
        F_tangential.first = mu_fn_i;
        F_tangential.second = mu_fn_j;
        // fuerzas << " Ft_i : " << Ft_i << " Ft_j : " << Ft_j << endl;
    }
    else
    {
        F_tangential.first = Ft_i;
        F_tangential.second = Ft_j;;
        // fuerzas << " Ft_i : " << Ft_i << " Ft_j : " << Ft_j << endl;
    }

    /*cout << "CLASE SPEED "
         << "Ft_i : " << Ft_i << " Ft_j : " << Ft_j << endl;*/
}

void Speed_F_Normal::momentos(float current_center_mass_X, float current_center_mass_Y, float current_velocity_X, float current_velocity_Y, float angle, float h, float vertex_slope[])
{
    vector<float> rc = vector_rc(current_center_mass_X, current_center_mass_Y, current_velocity_X, current_velocity_Y, angle, vertex_slope);
    calculate_normal_force(current_center_mass_X, current_center_mass_Y, current_velocity_X, current_velocity_Y, angle, vertex_slope);
    calculate_tangential_force(current_center_mass_X, current_center_mass_Y, current_velocity_X, current_velocity_Y, angle, h, vertex_slope);

    float f_total_i = F_normal.first + F_tangential.first;
    float f_total_j = F_normal.second + F_tangential.second;

    M_G = (rc[0] * f_total_j) - (rc[1] * f_total_i);
}

#endif