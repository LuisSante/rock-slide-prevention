#include <iostream>
#include <math.h>
#include <chrono> // para la medición del tiempo
#include <thread> // para la gestión de hilos

#include "rungek.hpp"
#include "intersection.hpp"
#include "draw.hpp"
#include "locales.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
typedef unsigned int uint;

// settings
const uint SCR_WIDTH = 4000;
const uint SCR_HEIGHT = 4000;
glm::mat4 transform = glm::mat4(1.0f);

/*dibujar elipse*/
const int escala = 4000;
int index_vertices = 3;
int index_indices = 0;
int index_indices_value = 1;
double inclinacion = 0;
const int salto_angulos_draw = 10;
const int n_triangulos = 360 / salto_angulos_draw;
const int size_vertices = 3 * (n_triangulos + 1);
const int size_index = 3 * n_triangulos;
float vertices[size_vertices]; // coordenada de origen
uint indices[size_index];
// const int escala = 800;
//  const float g = 9.81;
// const int pendiente = 2*pi / n_triangulos;

/*runge kutta*/
float t = 0.0; // tiempo inicial
/*float x = 0.0;                    // posición inicial en el eje x
float y = 0.0;                    // posición inicial en el eje y*/
float vx = 155.8844625;                   // velocidad inicial en el eje x
float vy = 90.0;                  // velocidad inicial en el eje y
float theta = 0.0 * 3.14 / 180.0; // ángulo de giro inicial (en radianes)
const float g = 9.81;             // aceleración gravitatoria (en m/s^2)
float dt = 1;                     // incremento de tiempo// Declarar los arrays para almacenar las posiciones y velocidades en x e y
const int n = 21;
float x_array[n];
float y_array[n];
float vx_array[n];
float vy_array[n];
float theta_array[n];
float delta_pos_x[n];
float delta_pos_y[n];

// control de velocidad de ejecución
int pos = 0;
double lastTime = glfwGetTime();
double deltaTime = 0.0;
double desiredFPS = 60.0;
double frameTime = 1.0 / desiredFPS;

/*coordenadas del talud*/
const int size_coor_talud = 4;
float talud[size_coor_talud * 3];
uint indices_talud[size_coor_talud * 3];

/*calculo del punto medio*/
float inversa[MAX_DIMENSION][MAX_DIMENSION];
float vertices_resta[6], vertices_locales_talud[6];
float x1_, y1_, x2_, y2_;
float x1_elipse_local, y1_elipse_local, x2_elipse_local, y2_elipse_local;
float punto_medio_x, punto_medio_y;

void processInput(GLFWwindow *window)
{
    // float deltaTime{ getDeltaTime() };
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

float escala_radio(float radio, float escala)
{
    return radio / escala;
}

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 aColor;\n"
                                 "uniform mat4 transform; \n"
                                 "out vec3 ourColor;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = transform * vec4(aPos, 1.0);\n"
                                 "   ourColor = aColor;\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "in vec3 ourColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(ourColor, 1.0f);\n"
                                   "}\n\0";

const char *vertexShaderSource_talud = "#version 330 core\n"
                                       "layout (location = 0) in vec3 aPos;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                       "}\0";

const char *fragmentShaderSource_talud = "#version 330 core\n"
                                         "out vec4 FragColor;\n"
                                         "void main()\n"
                                         "{\n"
                                         "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
                                         "}\n\0";

const char *vertexShaderSource_grid = "#version 330 core\n"
                                      "layout (location = 0) in vec2 bPos;\n"
                                      "uniform mat4 model;\n"
                                      "uniform mat4 view;\n"
                                      "uniform mat4 projection;\n"
                                      "void main()\n"
                                      "{\n"
                                      "   gl_Position = projection * view * model * vec4(bPos, 0.0, 1.0);\n"
                                      "}\0";

const char *fragmentShaderSource_grid = "#version 330 core\n"
                                        "out vec4 FragColor;\n"
                                        "uniform vec3 color;\n"
                                        "void main()\n"
                                        "{\n"
                                        "   FragColor = vec4(color, 1.0f);\n"
                                        "}\n\0";

int main()
{
    int a = 50; // radio mayor
    int b = 30; // radio menor
    float origen_x = 0, origen_y = 0;
    x_array[0] = origen_y;
    y_array[0] = origen_x;
    vx_array[0] = vx;
    vy_array[0] = vy;
    // loat a = 2.5, b = 1.5;
    float Xd = 8.045, Yd = 8.045;
    float theta_talud = 10 * 3.1416 / 180;

    // cout<<"Ingrese el radio mayor: "; cin>>a;
    // cout<<"Ingrese el radio menor: "; cin>>b;
    // cout<<"Ingrese una escala para la conversion: "; cin>> escala;
    // cout<<"Ingrese la coordenada x de origen: "; cin>>origen_x;
    // cout<<"Ingrese la coordenada y de origen: "; cin>>origen_y;

    float vertices_talud[] = {
        -0.06f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 0.0f,
        0.045f, -0.045f, 0.0f,

        0.045f, -0.045f, 0.0f,
        0.9f, -0.045f, 0.0f};

    float vertices_talud_[6] = {0.5, 12, 0, 12.5, 1, 0};

    const float escala_x = escala_radio(a, escala);
    const float escala_y = escala_radio(b, escala);

    float matriz_angulos[MAX_DIMENSION][MAX_DIMENSION] = {
        {cos(theta_talud), -sin(theta_talud)},
        {sin(theta_talud), cos(theta_talud)}};

    vertices_elipse(origen_x, origen_y, escala_x, escala_y, inclinacion, salto_angulos_draw, n_triangulos, index_vertices, vertices);
    indices_elipse(n_triangulos, index_indices, index_indices_value, indices);
    move(x_array, y_array, vx_array, vy_array, theta_array, delta_pos_x, delta_pos_y, vx, vy, theta, g, t, dt, origen_x, origen_y, a, b, n);
    calcular_inversa(matriz_angulos, 2, inversa);
    vertices_locales(vertices_resta, vertices_talud_ , vertices_locales_talud, Xd, Yd, x1_elipse_local, y1_elipse_local , x2_elipse_local , y2_elipse_local);
    mult_matriz_array(inversa, x1_elipse_local, y1_elipse_local, x2_elipse_local, y2_elipse_local, x1_, y1_, x2_, y2_);
    machine(matriz_angulos, 2, a, b, x1_, y1_, x2_, y2_, Xd, Yd, punto_medio_x, punto_medio_y);
    cout << punto_medio_x << " " << punto_medio_y << endl;

    /******************************************************************************************************************************/

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _APPLE_
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /******************************************************************************************************/

    uint VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /******************************************************************************************************/

    uint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    uint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    uint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);

    /******************************************************************************************************/
    unsigned int vertexShader_talud = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader_talud, 1, &vertexShaderSource_talud, NULL);
    glCompileShader(vertexShader_talud);

    int success_talud;
    char infoLogT[512];
    glGetShaderiv(vertexShader_talud, GL_COMPILE_STATUS, &success_talud);
    if (!success_talud)
    {
        glGetShaderInfoLog(vertexShader_talud, 512, NULL, infoLogT);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLogT << std::endl;
    }

    unsigned int fragmentShader_talud = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader_talud, 1, &fragmentShaderSource_talud, NULL);
    glCompileShader(fragmentShader_talud);

    glGetShaderiv(fragmentShader_talud, GL_COMPILE_STATUS, &success_talud);
    if (!success_talud)
    {
        glGetShaderInfoLog(fragmentShader_talud, 512, NULL, infoLogT);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLogT << std::endl;
    }

    unsigned int shaderProgram_talud = glCreateProgram();
    glAttachShader(shaderProgram_talud, vertexShader_talud);
    glAttachShader(shaderProgram_talud, fragmentShader_talud);
    glLinkProgram(shaderProgram_talud);

    glGetProgramiv(shaderProgram_talud, GL_LINK_STATUS, &success_talud);
    if (!success_talud)
    {
        glGetProgramInfoLog(shaderProgram_talud, 512, NULL, infoLogT);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLogT << std::endl;
    }
    glDeleteShader(vertexShader_talud);
    glDeleteShader(fragmentShader_talud);

    /******************************************************************************************************/

    unsigned int VBO_talud, VAO_talud;
    glGenVertexArrays(1, &VAO_talud);
    glGenBuffers(1, &VBO_talud);
    glBindVertexArray(VAO_talud);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_talud);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_talud), vertices_talud, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {

        double currentTime = glfwGetTime();
        deltaTime += currentTime - lastTime;
        lastTime = currentTime;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        processInput(window);

        // render
        if (deltaTime >= frameTime)
        {
            glUseProgram(shaderProgram);
            deltaTime -= frameTime;
        }
        if (pos <= n)
        {
            transform = glm::translate(transform, glm::vec3(delta_pos_x[pos] / escala, delta_pos_y[pos] / escala, 0.0f));
            transform = glm::rotate(transform, (float)glm::radians(theta_array[pos]), glm::vec3(0.0f, 0.0f, 1.0f));
            pos++;
        }
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, size_index, GL_UNSIGNED_INT, 0);

        glUseProgram(shaderProgram_talud);
        glBindVertexArray(VAO_talud);
        glDrawArrays(GL_LINES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();

        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glDeleteVertexArrays(1, &VAO_talud);
    glDeleteBuffers(1, &VBO_talud);
    glDeleteProgram(shaderProgram_talud);

    glfwTerminate();

    return 0;
}