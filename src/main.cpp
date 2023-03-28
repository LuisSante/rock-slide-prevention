/*****************************************************************************/

#include <HSGIL/hsgil.hpp>

#include <chrono>
#include <thread>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <utility>

#include <cmath>
#include <cstdio>
#include <cstring>

#include "draw.hpp"
#include "Rk.hpp"
#include "punto_contacto.hpp"
#include "speed_f_normal.hpp"

using std::vector;

// Window Size Settings
constexpr unsigned int SCR_WIDTH = 4000;
constexpr unsigned int SCR_HEIGHT = 4000;

constexpr unsigned int SIZE_COORD_GRID = 4;

constexpr unsigned int NUMBER_OF_SECTIONS = 18;
constexpr unsigned int ROCK_VERTEX_DATA_SIZE = (NUMBER_OF_SECTIONS + 1) * 6;
constexpr unsigned int ROCK_INDICES_SIZE = NUMBER_OF_SECTIONS * 3;

// Seed
constexpr int DATA = 997;
constexpr long long B = 314159239;
constexpr long long C = 453806245;
constexpr int init = 36;
constexpr double mean = 0.25;
constexpr double standard_deviation = 0.05;

/*****************************************************************************/
void transformVertices(float *vertices, int numVertices, const glm::mat4 &transform)
{
    for (int i = 0; i < numVertices + 1; ++i)
    {
        glm::vec4 transformedVertex = transform * glm::vec4(vertices[i * 6 + 0],
                                                            vertices[i * 6 + 1],
                                                            vertices[i * 6 + 2],
                                                            1.0f);
        vertices[i * 6 + 0] = transformedVertex.x;
        vertices[i * 6 + 1] = transformedVertex.y;
        vertices[i * 6 + 2] = transformedVertex.z;
    }
}

/*****************************************************************************/
int main()
{
    // Window Setup
    /*****************************************************************************/
    gil::RenderingWindow window(SCR_WIDTH, SCR_HEIGHT, "Rendering");
    if (!window.isReady())
    {
        return -1;
    }

    gil::InputHandler inputHandler;
    window.setInputHandler(inputHandler);

    gil::Shader rockShader("rock");
    gil::Shader slopeShader("slope");
    gil::Shader gridShader("grid");

    std::ofstream report("C:/Users/Usuario/Desktop/hilarios/src/reportes/report.txt");
    std::ofstream rebound("C:/Users/Usuario/Desktop/hilarios/src/reportes/report_rebound.txt");
    std::ofstream Frame_Rk("C:/Users/Usuario/Desktop/hilarios/src/reportes/frame_RK.txt");
    /*****************************************************************************/

    // Initial Setup Stuff
    /*****************************************************************************/
    // General Scale
    float scale = 3500.0f;
    float center_mass_x = 0.0f;
    float center_mass_y = 0.5f;

    glm::vec2 pos_init = glm::vec2(center_mass_x, center_mass_y);

    // Non-scaled radius => Scaled radius
    float radio_major = 20.0f;
    float radio_minor = 15.0f;

    float a = radio_major / scale;
    float b = radio_minor / scale;

    // Rock Computing Utils [Runge Kutta]
    float vx = 20.9f;
    float vy = 90.0f;
    float theta = 0.0f * M_PI / 180.0f;
    Draw ellipse(a, b, NUMBER_OF_SECTIONS);
    /*****************************************************************************/

    // Raw Vertex Data
    /*****************************************************************************/
    // Rock raw data
    float rock_vertex_data[ROCK_VERTEX_DATA_SIZE];
    unsigned int rock_indices[ROCK_INDICES_SIZE];
    ellipse.vertex_ellipse(rock_vertex_data);
    ellipse.index_ellipse(rock_indices);

    // slope raw data
    float grid_vertex_data[] = {
        -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -1.0f, 0.5f, 0.0f, 1.0f, 0.5f, 0.0f,
        0.5f, -1.0f, 0.0f, 0.5f, 1.0f, 0.0f,
        0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -1.0f, 0.0f, -0.5f, 1.0f, 0.0f,
        -1.0f, -0.5f, 0.0f, 1.0f, -0.5f, 0.0f};

    // slope raw data
    float slope[SIZE_COORD_GRID * 3];
    float slope_vertex_data[6] = {
        0.7f, 0.0f, 0.0f, 0.0f, 0.41f, 0.0f};

    /*****************************************************************************/

    unsigned int VAO_rock = 0;
    unsigned int VBO_rock = 0;
    unsigned int EBO_rock = 0;

    glGenVertexArrays(1, &VAO_rock);
    glGenBuffers(1, &VBO_rock);
    glGenBuffers(1, &EBO_rock);

    glBindVertexArray(VAO_rock);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_rock);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rock_vertex_data), rock_vertex_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_rock);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rock_indices), rock_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void *)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /*****************************************************************************/

    unsigned int VAO_grid = 0;
    unsigned int VBO_grid = 0;

    glGenVertexArrays(1, &VAO_grid);
    glGenBuffers(1, &VBO_grid);

    glBindVertexArray(VAO_grid);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_grid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grid_vertex_data), grid_vertex_data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /*****************************************************************************/

    unsigned int VAO_slope = 0;
    unsigned int VBO_slope = 0;

    glGenVertexArrays(1, &VAO_slope);
    glGenBuffers(1, &VBO_slope);

    glBindVertexArray(VAO_slope);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_slope);
    glBufferData(GL_ARRAY_BUFFER, sizeof(slope_vertex_data), slope_vertex_data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /*****************************************************************************/

    float t0 = 0.0f;
    float tf = 50.0f;
    float MG = 0.0f;
    float w = 0.2f;
    static const float h = 0.01f;

    int i = 0;
    float t = 0.0f;

    PointContact point(ellipse);
    Speed_F_Normal speed(ellipse);

    RungeKutta rk(ellipse);

    gil::Timer timer(true);

    while (window.isActive())
    {
        // Pre Tick calls
        window.pollEvents();
        if (inputHandler.onKeyReleased(gil::KEY_ESCAPE))
        {
            window.close();
            continue;
        }

        // OpenGL clearing functions
        glClearColor(0.2f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Rendering Stuff
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos_init, 0.0f));

        if (i < tf / h)
        {
            t = t0 + i * h;

            Frame_Rk << "T: " << t << " (center_mass_x,center_mass_y): (" << center_mass_x << " , " << center_mass_y << ") vx: " << vx << " vy: " << vy << " giro (rads): " << theta << " Mg: " << speed.M_G << " F_normal: (" << speed.F_normal.first << " , " << speed.F_normal.second << ") F_tangential : (" << speed.F_tangential.first << " , " << speed.F_tangential.second << ")" << endl;

            rk.runge_kutta(vx, vy, center_mass_x, center_mass_y, theta, w, speed.M_G, h, t, i, speed.F_normal, speed.F_tangential);

            float transform_x = center_mass_x / scale;
            float transform_y = center_mass_y / scale;

            transform = glm::translate(transform, glm::vec3(transform_x, transform_y, 0.0f));
            transform = glm::rotate(transform, (float)theta, glm::vec3(0.0f, 0.0f, 1.0f));

            float out_vertex[ROCK_VERTEX_DATA_SIZE];
            memcpy(out_vertex, rock_vertex_data, sizeof(rock_vertex_data));
            transformVertices(out_vertex, NUMBER_OF_SECTIONS, transform);

            report << theta << endl;
            for (int i = 0; i < NUMBER_OF_SECTIONS + 1; ++i)
            {
                report << "Vertex " << i << ": (" << (out_vertex[i * 6 + 0]) << ", " << (out_vertex[i * 6 + 1]) << ", " << (out_vertex[i * 6 + 2]) << ")" << endl;
            }
            report << endl
                   << endl;

            point.superposition(out_vertex[0], out_vertex[1], slope_vertex_data);

            if (point.collision == true)
            {
                // I pass you vertices of the virtual world
                speed.momentos(out_vertex[0], out_vertex[1], vx, vy, theta, h, slope_vertex_data);

                rebound << " Second: " << t << "\t\t\t\t\t (X,Y): (" << out_vertex[0] << "," << out_vertex[1] << ") \t Sig_n: (" << point.perpendicular.first << "\t" << point.perpendicular.second << ") \t D_Sig_n : (" << speed.velocity_sigma[2] << " i " << speed.velocity_sigma[3] << " j) \t D_Sig_t: (" << speed.velocity_sigma[0] << " i " << speed.velocity_sigma[1] << " j) \t\t F_normal : " << speed.FN << "\t (" << speed.F_normal.first << " i, " << speed.F_normal.second << " j) \t F_tangential: " << speed.Ft << " \t (" << speed.F_tangential.first << "i , " << speed.F_tangential.second << ") \t"
                        << "\t Mg : " << speed.M_G << endl;
                //return 0;
            }

            i++;
        }

        else if (i == tf / h)
        {
            t = t0 + i * h;
            rk.runge_kutta(vx, vy, center_mass_x, center_mass_y, theta, w, MG, h, t, i, speed.F_normal, speed.F_tangential);

            transform = glm::translate(transform, glm::vec3(center_mass_x / scale, center_mass_y / scale, 0.0f));
            transform = glm::rotate(transform, (float)theta, glm::vec3(0.0f, 0.0f, 1.0f));

            float out_vertex[ROCK_VERTEX_DATA_SIZE];
            memcpy(out_vertex, rock_vertex_data, sizeof(rock_vertex_data));
            transformVertices(out_vertex, NUMBER_OF_SECTIONS, transform);

            if (point.collision == true)
            {

                speed.momentos(out_vertex[0], out_vertex[1], vx, vy, theta, h, slope_vertex_data);

                rebound << "(" << center_mass_x << "," << center_mass_y << ") \t (" << point.perpendicular.first * scale << "\t" << point.perpendicular.second * scale << ") \t (" << speed.velocity_sigma[2] << " i " << speed.velocity_sigma[3] << " j) \t (" << speed.velocity_sigma[0] << " i " << speed.velocity_sigma[1] << " j) \t\t " << speed.FN << "\t" << speed.Ft << "\t"
                        << "\t " << speed.M_G << endl;
                MG = speed.M_G;
                //return 0;
            }
        }

        gridShader.use();
        glBindVertexArray(VAO_grid);
        glDrawArrays(GL_LINES, 0, 100);

        slopeShader.use();
        glBindVertexArray(VAO_slope);
        glDrawArrays(GL_LINES, 0, 100);

        rockShader.use();
        rockShader.setMat4("transform", transform);
        glBindVertexArray(VAO_rock);
        glDrawElements(GL_TRIANGLES, ROCK_VERTEX_DATA_SIZE, GL_UNSIGNED_INT, (const void *)0);

        // Post Tick Calls
        window.swapBuffers();
        timer.tick();

        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    glDeleteVertexArrays(1, &VAO_rock);
    glDeleteBuffers(1, &VBO_rock);
    glDeleteBuffers(1, &EBO_rock);

    glDeleteVertexArrays(1, &VAO_grid);
    glDeleteBuffers(1, &VBO_grid);

    glDeleteVertexArrays(1, &VAO_slope);
    glDeleteBuffers(1, &VBO_slope);

    return 0;
}
