#ifndef F_NORMAL_HPP
#define F_NORMAL_HPP

#include <iostream>
#include <math.h>
#include <vector>

#include "draw.hpp"
#include "speed.hpp"

using namespace std;

const float PI_ = 3.1415;

class FuerzaNormal
{

private:
    Draw draw;
    float r_star;
    float E_star;
    float G_star;
    float gama;
    float E = 350000;
    float v = 0.275;
    float m = 120;
    float g_young = 9.7639;
    float en_coe_restitution = 0.75;

public:
    FuerzaNormal();
    // FuerzaNormal(Draw draw);
    void radio_equivalente();
    void young_module();
    void equivalent_shear_modulus();
    float fuerza();
    void modulo();
};

FuerzaNormal::FuerzaNormal()
{
    r_star = 0;
    E_star = 0;
    G_star = 0;
    gama = 0;
}

void FuerzaNormal::radio_equivalente()
{
    float Aprima, Bprima;
    Aprima = 0.5 * ((0.5 * (1 / draw.a + 1 / draw.b) + 0.25 * (1 / draw.a - 1 / draw.b)));
    Bprima = 0.5 * ((0.5 * (1 / draw.a + 1 / draw.b) - 0.25 * (1 / draw.a + 1 / draw.b)));

    r_star = 1 / (sqrt(Aprima * Bprima));
}

void FuerzaNormal::young_module()
{
    E_star = E / (1 - (v * v));
    E_star = E_star * 98.0665;
}

void FuerzaNormal::equivalent_shear_modulus()
{
    float G = E / (2 * (1 + v));
    G_star = G / (2 - v);
    G_star = G_star * 98.0665;
}

float FuerzaNormal::fuerza()
{
    gama = -log(en_coe_restitution) / (sqrt((PI_ * PI_) + (log(en_coe_restitution) * log(en_coe_restitution))));
    return gama;
}

void FuerzaNormal::modulo()
{   
    
}

#endif