#ifndef RUNGE_K_HPP
#define RUNGE_K_HPP

#include <iostream>
#include <cmath>

using namespace std;

// Definir la función que describe el movimiento del proyectil
void movimiento_proyectil(float t, float x, float y, float &vx, float &vy, float &theta, float g, int a , int b_)
{
    // Calcular las aceleraciones en los ejes x e y
    float ax = 0.0;
    float ay = -g;

    // Calcular la velocidad angular en el plano xy
    //float w = vx * std::sin(theta) + vy * std::cos(theta);
    //float w = vx * std::sin(theta) + vy * std::cos(theta);
    float w = 0.4 ;

    // Calcular la fuerza de resistencia del aire
    float b = 0.000; // coeficiente de resistencia del aire
    float Fx = -b * vx * std::sqrt(vx * vx + vy * vy);
    float Fy = -b * vy * std::sqrt(vx * vx + vy * vy);
    
    // Calcular el momento resultante en el plano xy
    float I = (0.25 * 3.14) * (a*b_) * (a*a + b_*b_); // momento de inercia del proyectil (en kg*m^2)
    float M = 0; // momento resultante

    // Actualizar la velocidad en el eje x
    float k1_vx = ax + Fx / I;
    float k2_vx = ax + Fx / I;
    float k3_vx = ax + Fx / I;
    float k4_vx = ax + Fx / I;
    vx += (k1_vx + 2.0 * k2_vx + 2.0 * k3_vx + k4_vx) / 6.0;

    // Actualizar la velocidad en el eje y
    float k1_vy = ay + Fy / I;
    float k2_vy = ay + Fy / I;
    float k3_vy = ay + Fy / I;
    float k4_vy = ay + Fy / I;
    vy += (k1_vy + 2.0 * k2_vy + 2.0 * k3_vy + k4_vy) / 6.0;

    // Actualizar la posición en el eje x
    float k1_x = vx;
    float k2_x = vx + k1_vx / 2.0;
    float k3_x = vx + k2_vx / 2.0;
    float k4_x = vx + k3_vx;
    x += (k1_x + 2.0 * k2_x + 2.0 * k3_x + k4_x) / 6.0;

    // Actualizar la posición en el eje y
    float k1_y = vy;
    float k2_y = vy + k1_vy / 2.0;
    float k3_y = vy + k2_vy / 2.0;
    float k4_y = vy + k3_vy;
    y += (k1_y + 2.0 * k2_y + 2.0 * k3_y + k4_y) / 6.0;

    // Actualizar el ángulo de giro del proyectil
    //suma W ya que es ek resultado del primer RK
    float k1_theta = w;
    float k2_theta = w + M / I;
    float k3_theta = w + M / I;
    float k4_theta = w + M / I;
    theta += (k1_theta + 2.0 * k2_theta + 2.0 * k3_theta + k4_theta) / 6.0;
}

#endif