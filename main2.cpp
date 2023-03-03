#include <iostream>
#include <cmath>

using namespace std;

double f(double t, double x, double y, double vx, double vy) {
    double m = 120.0; // masa del objeto
    double F = 0.0; // fuerza neta que actúa sobre el objeto
    double ax = F/m; // aceleración en el eje x
    return ax; // devuelve la aceleración en el eje x
}

double g(double t, double x, double y, double vx, double vy) {
    double m = 120.0; // masa del objeto
    double g = 9.8;
    double F = m*-g; // fuerza neta que actúa sobre el objeto
    double ay = F/m; // aceleración en el eje y
    return ay; // devuelve la aceleración en el eje y
}

void rungeKutta(double t0, double x0, double y0, double vx0, double vy0, double tf, double h, double *t, double *x, double *y, double *vx, double *vy, int n) {
    double k1x, k2x, k3x, k4x;
    double k1y, k2y, k3y, k4y;
    double k1vx, k2vx, k3vx, k4vx;
    double k1vy, k2vy, k3vy, k4vy;
    double ti = t0;
    double xi = x0;
    double yi = y0;
    double vxi = vx0;
    double vyi = vy0;
    
    for (int i = 0; i < n; i++) {
        t[i] = ti;
        x[i] = xi;
        y[i] = yi;
        vx[i] = vxi;
        vy[i] = vyi;
        
        k1x = h * vxi;
        k1y = h * vyi;
        k1vx = h * f(ti, xi, yi, vxi, vyi);
        k1vy = h * g(ti, xi, yi, vxi, vyi);
        
        k2x = h * (vxi + k1vx/2);
        k2y = h * (vyi + k1vy/2);
        k2vx = h * f(ti + h/2, xi + k1x/2, yi + k1y/2, vxi + k1vx/2, vyi + k1vy/2);
        k2vy = h * g(ti + h/2, xi + k1x/2, yi + k1y/2, vxi + k1vx/2, vyi + k1vy/2);
        
        k3x = h * (vxi + k2vx/2);
        k3y = h * (vyi + k2vy/2);
        k3vx = h * f(ti + h/2, xi + k2x/2, yi + k2y/2, vxi + k2vx/2, vyi + k2vy/2);
        k3vy = h * g(ti + h/2, xi + k2x/2, yi + k2y/2, vxi + k2vx/2, vyi + k2vy/2);

        k4x = h * (vxi + k3vx/2);
        k4y = h * (vyi + k3vy/2);
        k4vx = h * f(ti + h, xi + k3x, yi + k3y, vxi + k3vx, vyi + k3vy);
        k4vy = h * g(ti + h, xi + k3x, yi + k3y, vxi + k3vx, vyi + k3vy);

        ti += h;
        xi += (k1x + 2*k2x + 2*k3x + k4x)/6;
        yi += (k1y + 2*k2y + 2*k3y + k4y)/6;
        vxi += (k1vx + 2*k2vx + 2*k3vx + k4vx)/6;
        vyi += (k1vy + 2*k2vy + 2*k3vy + k4vy)/6;
    }
}

int main() {
    double t0 = 0.0; // tiempo inicial
    double x0 = 0.0; // posición inicial en el eje x
    double y0 = 0.0; // posición inicial en el eje y
    double vx0 = 155.9; // velocidad inicial en el eje x
    double vy0 = 90; // velocidad inicial en el eje y
    double tf = 20.0; // tiempo final
    double h = 1; // paso de integración

    int n = (tf - t0)/h + 1; // número de puntos

    double *t = new double[n]; // array para el tiempo
    double *x = new double[n]; // array para la posición en el eje x
    double *y = new double[n]; // array para la posición en el eje y
    double *vx = new double[n]; // array para la velocidad en el eje x
    double *vy = new double[n]; // array para la velocidad en el eje y

    rungeKutta(t0, x0, y0, vx0, vy0, tf, h, t, x, y, vx, vy, n); // llama a la función rungeKutta

    // imprime los resultados
    for (int i = 0; i < n; i++) {
        cout << "t = " << t[i] << " vx= " << vx[i] << "   x= " << x[i] << " vy = " << vy[i] << "   y = " << y[i] << endl;
    }

    // libera la memoria de los arrays
    delete[] t;
    delete[] x;
    delete[] y;
    delete[] vx;
    delete[] vy;
    return 0;
}   