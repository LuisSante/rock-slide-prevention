#ifndef DRAW_HPP
#define DRAW_HPP

#include <vector>
#include <iostream>
#include <fstream>

#include <cmath>
#ifdef M_PI
#undef M_PI
#endif
#define M_PI 3.14159265358979323846f

using std::vector;
std::ofstream vertex("C:/Users/Usuario/Desktop/hilarios/src/reports/vertex.txt");

class Draw
{
private:
    float a;
    float b;
    int number_of_sections;

public:
    int n;

    Draw();
    Draw(float a, float b, int number_of_sections);
    float degrees_to_radians(float angle);
    void vertex_ellipse(float vertex[]);
    void index_ellipse(unsigned int index[]);

    friend class Move;
    friend class Superposition;
    friend class PointContact;
    friend class Speed_F_Normal;
};

Draw::Draw()
{
    a = 0.0f;
    b = 0.0f;
    number_of_sections = 0;
};

Draw::Draw(float a, float b, int number_of_sections)
{
    this->a = a;
    this->b = b;
    this->number_of_sections = number_of_sections;
}

float Draw::degrees_to_radians(float angle)
{
    return (angle * M_PI) / 180.0f;
}

void Draw::vertex_ellipse(float vertex_data[])
{
    float angle_step = 360.0f / static_cast<float>(number_of_sections);

    vertex_data[0] = 0.0f;
    vertex_data[1] = 0.0f;
    vertex_data[2] = 0.0f;

    vertex_data[3] = 0.0f;
    vertex_data[4] = 0.0f;
    vertex_data[5] = 1.0f;

    float angle = 0.0f;
    for (int i = 1; i < number_of_sections + 1; ++i)
    {
        vertex_data[i * 6 + 0] = a * cosf(degrees_to_radians(angle)); // coordinate X
        vertex_data[i * 6 + 1] = b * sinf(degrees_to_radians(angle)); // coordinate Y
        vertex_data[i * 6 + 2] = 0.0f;                                       // coordinate Z

        vertex_data[i * 6 + 3] = 0.0f;
        vertex_data[i * 6 + 4] = 0.0f;
        vertex_data[i * 6 + 5] = 1.0f;

        angle += angle_step;
    }

    for (int i = 1; i < number_of_sections + 1; ++i)
    {
        vertex << "(" << vertex_data[i * 6 + 0] << " , "
                 << vertex_data[i * 6 + 1] << " , "
                 << vertex_data[i * 6 + 2] << " ) " << std::endl;
    }
}

void Draw::index_ellipse(unsigned int index[])
{
    // llenar el array de index
    int current_index_base = 1;
    for (int j = 0; j < number_of_sections; ++j)
    {
        index[j * 3 + 0] = 0;                      
        index[j * 3 + 1] = current_index_base + 0; // 2do vertex
        index[j * 3 + 2] = current_index_base + 1; // 3er vertex
        ++current_index_base;
    }
    index[(number_of_sections - 1) * 3 + 2] = 1;
}

#endif
