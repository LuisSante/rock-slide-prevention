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
    gil::Shader taludShader("talud");
    gil::Shader gridShader("grid");

    std::ofstream output("C:/Users/Usuario/Desktop/hilarios/src/reportes/report.txt");
    std::ofstream rebote("C:/Users/Usuario/Desktop/hilarios/src/reportes/report_rebote.txt");
    /*****************************************************************************/

    // Initial Setup Stuff
    /*****************************************************************************/
    // General Scale
    float scale = 3500.0f;

    // Non-sacled origin => Scaled origin
    float Xd = 0.0f;
    float Yd = 0.5f;

    glm::vec2 pos_init = glm::vec2(Xd, Yd);

    // Begin Point
    // float inicio_x = 0.0f;
    // float inicio_y = 0.5f;

    // Non-scaled radius => Scaled radius
    float radio_mayor = 200.0f;
    float radio_menor = 150.0f;

    float a = radio_mayor / scale;
    float b = radio_menor / scale;

    // Rock Computing Utils [Runge Kutta]
    float vx = 70.9f;
    float vy = 90.0f;
    float theta = 0.0f * M_PI / 180.0f;
    Draw elipse(a, b, NUMBER_OF_SECTIONS);
    /*****************************************************************************/

    // Raw Vertex Data
    /*****************************************************************************/
    // Rock raw data
    float rock_vertex_data[ROCK_VERTEX_DATA_SIZE];
    unsigned int rock_indices[ROCK_INDICES_SIZE];
    elipse.vertices_elipse(rock_vertex_data);
    elipse.indices_elipse(rock_indices);

    // Talud raw data
    float grid_vertex_data[] = {
        -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -1.0f, 0.5f, 0.0f, 1.0f, 0.5f, 0.0f,
        0.5f, -1.0f, 0.0f, 0.5f, 1.0f, 0.0f,
        0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -1.0f, 0.0f, -0.5f, 1.0f, 0.0f,
        -1.0f, -0.5f, 0.0f, 1.0f, -0.5f, 0.0f};

    // Talud raw data
    float talud[SIZE_COORD_GRID * 3];
    float talud_vertex_data[6] = {
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

    unsigned int VAO_talud = 0;
    unsigned int VBO_talud = 0;

    glGenVertexArrays(1, &VAO_talud);
    glGenBuffers(1, &VBO_talud);

    glBindVertexArray(VAO_talud);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_talud);
    glBufferData(GL_ARRAY_BUFFER, sizeof(talud_vertex_data), talud_vertex_data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /*****************************************************************************/

    rebote << " Segundo de impacto \t (Xd,Yd) \t\t\t\t\t Sig_n \t\t\t\t D_Sig_n \t\t\t D_Sig_t \t\t\t\t Fn \t\t Ft \t\t Mg \t" << endl;

    float t0 = 0.0f;
    float tf = 20.0f;
    float MG = 0.0f;
    float w = 0.2f;
    const float h = 1.0f;

    int i = 0;
    float t = 0.0f;

    PuntoContacto point(elipse);
    Speed_F_Normal speed(elipse);

    RungeKutta rk(elipse);

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
        glm::mat4 transform = glm::translate(glm::mat4(1.0f) , glm::vec3(pos_init , 0.0f));

        if (i < tf / h)
        {
            t = t0 + i * h;
            rk.runge_kutta(vx, vy, Xd, Yd, theta, w, speed.M_G, h, t, i, speed.Fnormal, speed.Ftangencial);

            float transform_x = Xd / scale;
            float transform_y = Yd / scale;

            transform = glm::translate(transform, glm::vec3(transform_x, transform_y, 0.0f));
            transform = glm::rotate(transform, (float)theta, glm::vec3(0.0f, 0.0f, 1.0f));

            float outvertices[ROCK_VERTEX_DATA_SIZE];
            memcpy(outvertices, rock_vertex_data, sizeof(rock_vertex_data));
            transformVertices(outvertices, NUMBER_OF_SECTIONS, transform);

            output << theta << endl;
            for (int i = 0; i < NUMBER_OF_SECTIONS + 1; ++i)
            {
                output << "Vertex " << i << ": (" << (outvertices[i * 6 + 0] * scale) << ", " << (outvertices[i * 6 + 1] * scale) << ", " << (outvertices[i * 6 + 2] * scale) << ")" << endl;
            }
            output << endl
                   << endl;

            /*point.superposition(rock_vertex_data[0], rock_vertex_data[1], talud_vertex_data);

            if (point.collision == true)
            {
                speed.momentos(rock_vertex_data[0], rock_vertex_data[1], vx, vy, theta, h, talud_vertex_data);

                rebote << t << "\t\t\t\t\t(" << Xd / scale << "," << Yd / scale << ") \t (" << point.perpendicular.first << "\t" << point.perpendicular.second << ") \t (" << speed.velocidad_sigma[2] << " i " << speed.velocidad_sigma[3] << " j) \t (" << speed.velocidad_sigma[0] << " i " << speed.velocidad_sigma[1] << " j) \t\t " << speed.FN << "\t" << speed.Ft << "\t"
                       << "\t " << speed.M_G << endl;

                return 0;
            }*/

            i++;
        }

        else
        {
            t = t0 + i * h;
            rk.runge_kutta(vx, vy, Xd, Yd, theta, w, MG, h, t, i, speed.Fnormal, speed.Ftangencial);

            transform = glm::translate(transform, glm::vec3(Xd / scale, Yd / scale, 0.0f));
            transform = glm::rotate(transform, (float)theta, glm::vec3(0.0f, 0.0f, 1.0f));

            /*if (point.collision == true)
            {
                speed.momentos(vertices[0], vertices[1], vx, vy, theta, h, talud_vertex_data);
                cout << speed.Fnormal.first << " " << speed.Fnormal.second << " " << speed.Ftangencial.first << " " << speed.Ftangencial.second << endl;

                rebote << "(" << Xd << "," << Yd << ") \t (" << point.perpendicular.first * scale << "\t" << point.perpendicular.second * scale << ") \t (" << speed.velocidad_sigma[2] << " i " << speed.velocidad_sigma[3] << " j) \t (" << speed.velocidad_sigma[0] << " i " << speed.velocidad_sigma[1] << " j) \t\t " << speed.FN << "\t" << speed.Ft << "\t"
                       << "\t " << speed.M_G << endl;
                // rk.runge_kutta(vx, vy, traslate_X , traslate_Y , theta , w , MG , h , t , i);
                MG = speed.M_G;
                // return 0;
            }*/
        }

        gridShader.use();
        glBindVertexArray(VAO_grid);
        glDrawArrays(GL_LINES, 0, 100);

        taludShader.use();
        glBindVertexArray(VAO_talud);
        glDrawArrays(GL_LINES, 0, 100);

        rockShader.use();
        rockShader.setMat4("transform", transform);
        glBindVertexArray(VAO_rock);
        glDrawElements(GL_TRIANGLES, ROCK_VERTEX_DATA_SIZE, GL_UNSIGNED_INT, (const void *)0);

        // Post Tick Calls
        window.swapBuffers();
        timer.tick();

        std::this_thread::sleep_for(std::chrono::milliseconds(600));
    }

    glDeleteVertexArrays(1, &VAO_rock);
    glDeleteBuffers(1, &VBO_rock);
    glDeleteBuffers(1, &EBO_rock);

    glDeleteVertexArrays(1, &VAO_grid);
    glDeleteBuffers(1, &VBO_grid);

    glDeleteVertexArrays(1, &VAO_talud);
    glDeleteBuffers(1, &VBO_talud);

    return 0;
}
