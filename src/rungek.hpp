#ifndef RUNGE_K_HPP
#define RUNGE_K_HPP

#include <iostream>
#include <cmath>

using namespace std;

// Definir la función que describe el movimiento del proyectil
void movimiento_proyectil(double t, double x, double y, double &vx, double &vy, double &theta, double g, int a , int b_)
{
    // Calcular las aceleraciones en los ejes x e y
    double ax = 0.0;
    double ay = -g;

    // Calcular la velocidad angular en el plano xy
    //double w = vx * std::sin(theta) + vy * std::cos(theta);
    //double w = vx * std::sin(theta) + vy * std::cos(theta);
    double w = 0.2 ;

    // Calcular la fuerza de resistencia del aire
    double b = 0.000; // coeficiente de resistencia del aire
    double Fx = -b * vx * std::sqrt(vx * vx + vy * vy);
    double Fy = -b * vy * std::sqrt(vx * vx + vy * vy);
    
    // Calcular el momento resultante en el plano xy
    double I = (0.25 * 3.14) * (a*b_) * (a*a + b_*b_); // momento de inercia del proyectil (en kg*m^2)
    double M = 0; // momento resultante

    // Actualizar la velocidad en el eje x
    double k1_vx = ax + Fx / I;
    double k2_vx = ax + Fx / I;
    double k3_vx = ax + Fx / I;
    double k4_vx = ax + Fx / I;
    vx += (k1_vx + 2.0 * k2_vx + 2.0 * k3_vx + k4_vx) / 6.0;

    // Actualizar la velocidad en el eje y
    double k1_vy = ay + Fy / I;
    double k2_vy = ay + Fy / I;
    double k3_vy = ay + Fy / I;
    double k4_vy = ay + Fy / I;
    vy += (k1_vy + 2.0 * k2_vy + 2.0 * k3_vy + k4_vy) / 6.0;

    // Actualizar la posición en el eje x
    double k1_x = vx;
    double k2_x = vx + k1_vx / 2.0;
    double k3_x = vx + k2_vx / 2.0;
    double k4_x = vx + k3_vx;
    x += (k1_x + 2.0 * k2_x + 2.0 * k3_x + k4_x) / 6.0;

    // Actualizar la posición en el eje y
    double k1_y = vy;
    double k2_y = vy + k1_vy / 2.0;
    double k3_y = vy + k2_vy / 2.0;
    double k4_y = vy + k3_vy;
    y += (k1_y + 2.0 * k2_y + 2.0 * k3_y + k4_y) / 6.0;

    // Actualizar el ángulo de giro del proyectil
    //suma W ya que es ek resultado del primer RK
    double k1_theta = w;
    double k2_theta = w + M / I;
    double k3_theta = w + M / I;
    double k4_theta = w + M / I;
    theta += (k1_theta + 2.0 * k2_theta + 2.0 * k3_theta + k4_theta) / 6.0;
}

#endif