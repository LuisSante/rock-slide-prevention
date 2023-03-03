#include <iostream>
#include <math.h>
#include "rungek.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
typedef unsigned int uint;
typedef const int cint;

// settings
const uint SCR_WIDTH = 2000;
const uint SCR_HEIGHT = 1000;
glm::mat4 transform = glm::mat4(1.0f);

const float pi = 3.1415;
cint escala = 1000;
//const float g = 9.81;

cint salto_angulos_draw = 10;
cint n_triangulos = 360 / salto_angulos_draw;
//cint pendiente = 2*pi / n_triangulos;

cint size_vertices = 3 * (n_triangulos + 1);
cint size_index = 3 * n_triangulos;

float vertices[size_vertices]; //coordenada de origen 
uint indices[size_index];

cint size_coor_talud = 4;

float talud[size_coor_talud*3];
uint indices_talud[size_coor_talud*3];

//float vertices_giro[size_vertices];

int index_vertices = 3, index_indices = 0, index_indices_value = 1;
double inclinacion = 0;

float t = 0.0;                     // tiempo inicial
float x = 0.0;                     // posición inicial en el eje x
float y = 0.0;                     // posición inicial en el eje y
float vx = 155.8844625;                  // velocidad inicial en el eje x
float vy = 90.0;                  // velocidad inicial en el eje y
float theta = 30.0 * 3.14 / 180.0; // ángulo de giro inicial (en radianes)
float g = 9.81;                    // aceleración gravitatoria (en m/s^2)
float dt = 1e-6;                   // incremento de tiempo// Declarar los arrays para almacenar las posiciones y velocidades en x e y
const int n = 21;
float x_array[n];
float y_array[n]; 
float vx_array[n]; 
float vy_array[n]; 
float theta_array[n];
float delta_pos_x[n];
float delta_pos_y[n];
float delta_tetha[n];


void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
float escala_radio(float radio, float escala);
float radianes(float angulo);
float angulo(float radianes);
float convert_new_coor_x(float x, float y, double grados);
float convert_new_coor_y(float x, float y, int grados);

float getDeltaTime();
//void move();

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

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

const char* vertexShaderSource2 = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}\n\0";


int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _APPLE_
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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


    //codigo

    /*******/

    int a = 20; //radio mayor
    int b = 15; //radio menor
    float origen_x = 0, origen_y = 0;
    x_array[0] = x;
    y_array[0] = y;
    vx_array[0] = vx;
    vy_array[0] = vy;

    //cout<<"Ingrese el radio mayor: "; cin>>a;
    //cout<<"Ingrese el radio menor: "; cin>>b;
    //cout<<"Ingrese una escala para la conversion: "; cin>> escala;
    //cout<<"Ingrese la coordenada x de origen: "; cin>>origen_x;
    //cout<<"Ingrese la coordenada y de origen: "; cin>>origen_y;

    // coordenada x , coordenada y , coordenada z = 0
    
    const float escala_x = escala_radio(a, escala);
    const float escala_y = escala_radio(b, escala);

    vertices[0] = origen_x; 
    vertices[1] = origen_y;
    vertices[2] = 0.0f;

    //llenar array de vertices (x,y,z=0)
    for(int i = 0; i < n_triangulos; i++){
        inclinacion = inclinacion + salto_angulos_draw;
        vertices[index_vertices] = float(escala_x * cos(radianes(inclinacion)));  //coordenada X
        index_vertices++;
        vertices[index_vertices] = float(escala_y * sin(radianes(inclinacion)));  //coordenada Y
        index_vertices++; 
        vertices[index_vertices] = 0.0f;               //coordenada Z
        index_vertices++;
    }

    //llenar el array de indices
    for (int j = 1; j < n_triangulos; j++) {
        indices[index_indices] = 0;//todos los triangulos parten del centro
        index_indices++;
        indices[index_indices] = index_indices_value;//2do vertices
        index_indices++;
        index_indices_value++;
        indices[index_indices] = index_indices_value;//3er vertice
        index_indices++;
    }
    indices[index_indices] = index_indices_value;
    index_indices++;
    indices[index_indices] = 0;
    index_indices++;
    indices[index_indices] = 1;

    /******************************************************************************************************/

    // Calcular las posiciones y velocidades en cada instante de tiempo
    for (int i = 1; i < n; i++)
    {
        // Calcular los valores de k1, k2, k3 y k4 para la actualización de las variables
        float k1_x, k2_x, k3_x, k4_x, k1_y, k2_y, k3_y, k4_y;
        float k1_vx, k2_vx, k3_vx, k4_vx, k1_vy, k2_vy, k3_vy, k4_vy;
        float k1_theta, k2_theta, k3_theta, k4_theta;

        k1_x = vx;
        k1_vx = 0.0;
        k1_y = vy;
        k1_vy = 0.0;
        k1_theta = 0.0;
        movimiento_proyectil(t, x, y, k1_vx, k1_vy, k1_theta, g, a ,b);

        k2_x = vx + k1_vx * dt / 2.0;
        k2_vx = 0.0;
        k2_y = vy + k1_vy * dt / 2.0;
        k2_vy = 0.0;
        k2_theta = 0.0;
        movimiento_proyectil(t + dt / 2.0, x + k1_x * dt / 2.0, y + k1_y * dt / 2.0, k2_vx, k2_vy, k2_theta, g, a ,b);

        k3_x = vx + k2_vx * dt / 2.0;
        k3_vx = 0.0;
        k3_y = vy + k2_vy * dt / 2.0;
        k3_vy = 0.0;
        k3_theta = 0.0;
        movimiento_proyectil(t + dt / 2.0, x + k2_x * dt / 2.0, y + k2_y * dt / 2.0, k3_vx, k3_vy, k3_theta, g, a ,b);
        k4_x = vx + k3_vx * dt;
        k4_vx = 0.0;
        k4_y = vy + k3_vy * dt;
        k4_vy = 0.0;
        k4_theta = 0.0;
        movimiento_proyectil(t + dt, x + k3_x * dt, y + k3_y * dt, k4_vx, k4_vy, k4_theta, g, a ,b);

        // Actualizar las variables con el método Runge-Kutta de cuarto orden
        x += (k1_x + 2.0 * k2_x + 2.0 * k3_x + k4_x) / 6.0;
        vx += (k1_vx + 2.0 * k2_vx + 2.0 * k3_vx + k4_vx) / 6.0;
        y += (k1_y + 2.0 * k2_y + 2.0 * k3_y + k4_y) / 6.0;
        vy += (k1_vy + 2.0 * k2_vy + 2.0 * k3_vy + k4_vy) / 6.0;
        theta += (k1_theta + 2.0 * k2_theta + 2.0 * k3_theta + k4_theta) / 6.0;

        // Almacenar los valores de las posiciones y velocidades en los arrays correspondientes
        x_array[i] = x;
        vx_array[i] = vx;
        y_array[i] = y;
        vy_array[i] = vy;
        theta_array[i] = theta;

        // Actualizar el tiempo
        t += dt;

        // Imprimir los valores de las posiciones y velocidades en cada instante de tiempo
        /*for (int i = 0; i < n; i++)
        {
            cout << "t = " << i * dt << ", x = " << x_array[i] << ", y = " << y_array[i] << ", vx = " << vx_array[i] << ", vy = " << vy_array[i] << ", theta = " << theta_array[i]<< endl;
        }*/
    }

    delta_pos_x[0] = x_array[0];
    delta_pos_y[0] = y_array[0];
    delta_tetha[0] = theta_array[0];
    for(int i = 1; i < n; i++){
        delta_pos_x[i] = (x_array[i] - x_array[i - 1])/100000;
        delta_pos_y[i] = (y_array[i] - y_array[i - 1])/100000;
        delta_tetha[i] = theta_array[i] - theta_array[i]/10;
    }

    /******************************************************************************************************/

    uint VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    float talud_x , talud_y;

    uint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    uint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    uint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);

     // vertex shader
    unsigned int vertexShader2 = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader2, 1, &vertexShaderSource2, NULL);
    glCompileShader(vertexShader2);
    // check for shader compile errors
    int successT;
    char infoLogT[512];
    glGetShaderiv(vertexShader2, GL_COMPILE_STATUS, &successT);
    if (!successT)
    {
        glGetShaderInfoLog(vertexShader2, 512, NULL, infoLogT);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLogT << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader2);
    // check for shader compile errors
    glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &successT);
    if (!successT)
    {
        glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLogT);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLogT << std::endl;
    }
    // link shaders
    unsigned int shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader2);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);
    // check for linking errors
    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &successT);
    if (!successT) {
        glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLogT);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLogT << std::endl;
    }
    glDeleteShader(vertexShader2);
    glDeleteShader(fragmentShader2);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices2[] = {
        -0.06f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 0.0f,
        0.045f, -0.045f, 0.0f,

        0.045f, -0.045f, 0.0f,
        0.9f, -0.045f, 0.0f
    };

    unsigned int VBO2, VAO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    int pos = 0;
    while (!glfwWindowShouldClose(window)){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        processInput(window);

		//render
        glUseProgram(shaderProgram);
        //transform
        //transform = glm::translate(transform, glm::vec3(x_array[pos]/1e8, y_array[pos]/1e8, 0.0f));
        transform = glm::rotate(transform, (float)glm::radians(theta_array[pos]/100), glm::vec3(0.0f, 0.0f, 1.0f));
        pos++;

        if(pos == n) pos = 0;

        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        glBindVertexArray(VAO);
        //glBindVertexArray(VBO_talud);

        glDrawElements(GL_TRIANGLES, size_index, GL_UNSIGNED_INT, 0);

        glUseProgram(shaderProgram2);
        glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_LINES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteProgram(shaderProgram2);

    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow *window){
    //float deltaTime{ getDeltaTime() };
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS){
        //move();
    }
}

float escala_radio(float radio, float escala){
    return radio/escala;
}

float radianes(float angulo){
    return (angulo*pi)/180;
}

float angulo(float radianes){
    return (radianes*180)/pi;
}

float convert_new_coor_x(float x, float y, double grados){
    return float(x*cos(radianes(grados)) - y*sin(radianes(grados)));
}

float convert_new_coor_y(float x, float y, int grados){
    return float(y*cos(radianes(grados)) + x*sin(radianes(grados)));
}

/*void move(){
    for(int i=0; i<n ; i++){
        transform = glm::translate(transform, glm::vec3(delta_pos_x[i], delta_pos_y[i], 0.0f));
        transform = glm::rotate(transform, (float)glm::radians(delta_tetha[i]), glm::vec3(0.0f, 0.0f, 1.0f));
    }    
}*/

float getDeltaTime(){
    static float lastTime{ 0.0f };
    float currentTime{ static_cast<float>(glfwGetTime()) };
    float deltaTime{ currentTime - lastTime };
    lastTime = currentTime;
    return deltaTime;
}