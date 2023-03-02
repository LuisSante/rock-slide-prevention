#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

/*
* CURIOSIDAD: cuando el gallo se mueve cambia de colores
* CURIOSIDAD 2: el gallo se puede movar en diagonal
* 
tecla up : mueve la imagen hacia arriba
tecla down: mueve la imagen hacia abajo
tecla left: mueve la imagen hacia la izquiera
tecla right: mueve la imagen hacia la derecha

tecla numero 1: rota la imagen con respecto a su origen
tecla numero 2: rota la imagen con respecto a su eje X
tecla numero 3: rota la imagen con respecto a su eje Y

tecla C: la imagen crece en una escala indicada
tecla E: la imagen se encoge en una escala indicada

tecla O: la imagen se refleja con respecto a su origen
tecla X: la imagen se refleja con respecto a su al eje X
tecla Y: la imagen se refleja con respecto a su al eje Y
*/

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0f);\n"
"}\n\0";


//UN GALLO
float vertices[] = {
    -0.6f , -0.14f ,0.0f,//D
    -0.57f , 0.54f  ,0.0f,//C
    -0.4f  , 0.26f  ,0.0f,//E

    -0.03f , 0.27f  ,0.0f,//H
    -0.37f , 0.27f  ,0.0f,//G
    -0.55f , 0.55f  ,0.0f,//F

    -0.03f , 0.27f  ,0.0f,//H
    -0.55f , 0.55f  ,0.0f,//F
    -0.22f , 0.55f  ,0.0f,//I

    -0.36f , 0.25f  ,0.0f,//J
    -0.12f , -0.02f ,0.0f,//K
    -0.03f , 0.24f  ,0.0f,//L

    -0.09f , -0.04f ,0.0f,//M
    0.01f , 0.25f  ,0.0f,//N
    0.14f  , 0.04f  ,0.0f,//A1

    0.01f , 0.25f  ,0.0f,//N
    0.31f  , 0.24f  ,0.0f,//O
    0.14f  , 0.04f  ,0.0f,//A1

    0.31f  , 0.24f  ,0.0f,//O
    0.4f  , -0.05f ,0.0f,//P
    0.14f  , 0.04f  ,0.0f,//A1

    0.4f  , -0.05f ,0.0f,//P
    0.15f  , -0.23f ,0.0f,//Q
    0.14f  , 0.04f  ,0.0f,//A1

    0.13f  , -0.25f ,0.0f,//R
    0.17f  , -0.25f ,0.0f,//S
    0.13f  , -0.56f ,0.0f,//T

    0.17f  , -0.25f ,0.0f,//S
    0.13f  , -0.56f ,0.0f,//T
    0.17f  , -0.56f ,0.0f,//U

    -0.04f , -0.6f ,0.0f,//V
    0.15f , -0.57f ,0.0f,//W
    0.31f  , -0.6f  ,0.0f,//Z

    0.02f  , 0.27f  ,0.0f,//B1
    0.3f   , 0.27f  ,0.0f,//C1
    0.3f   , 0.71f  ,0.0f,//D1

    0.32f  , 0.37f  ,0.0f,//E1
    0.47f  , 0.38f  ,0.0f,//F1
    0.39f  , 0.55f  ,0.0f,//Q1

    0.32f  , 0.72f  ,0.0f,//G1
    0.32f  , 0.57f  ,0.0f,//H1
    0.47f  , 0.57f  ,0.0f,//I1

    0.32f  , 0.72f  ,0.0f,//G1
    0.47f  , 0.57f  ,0.0f,//I1
    0.47f  , 0.72f  ,0.0f,//J1

    0.49f  , 0.68f  ,0.0f,//K1
    0.49f  , 0.57f  ,0.0f,//L1
    0.59f  , 0.57f  ,0.0f,//M1

    0.17f  , 0.73f  ,0.0f,//N1
    0.25f  , 0.93f  ,0.0f,//O1
    0.46f  , 0.73f  ,0.0f//P1
};


//UN PEZ
/*float vertices[] = {
    -0.19f , -0.04f , 0.0f,//C
    -0.01f , -0.03f , 0.0f,//D
    -0.11f , -0.12f , 0.0f,//E

    -0.19f , -0.04f , 0.0f,//C
    -0.01f , -0.03f , 0.0f,//D
    -0.1f , 0.06f, 0.0f,//F

    -0.11f , -0.12f , 0.0f,//E
    -0.12f , -0.1f , 0.0f,//G
    -0.13f , -0.23f , 0.0f,//H

    -0.13f , -0.23f , 0.0f,//H
    -0.11f , -0.12f , 0.0f,//E
    -0.1f , -0.22f , 0.0f,//I

    -0.13f , -0.23f , 0.0f,//H
    -0.1f , -0.22f , 0.0f,//I
    -0.07f , -0.32f , 0.0f,//J

    -0.01f , -0.03f , 0.0f,//D
    -0.11f , -0.12f , 0.0f,//E
    0.0f , -0.2f , 0.0f,//K

    -0.11f , -0.12f , 0.0f,//E
    0.0f , -0.2f , 0.0f,//K
    -0.02f , -0.27f , 0.0f,//L

    0.0f , -0.2f , 0.0f,//K
    -0.02f , -0.27f , 0.0f,//L
    0.18f , -0.36f , 0.0f,//M

    -0.01f , -0.03f , 0.0f,//D
    0.0f , -0.2f , 0.0f,//K
    0.1f , -0.04f , 0.0f,//N

    0.1f , -0.04f , 0.0f,//N
    0.06f , -0.1f , 0.0f,//O
    0.17f , -0.15 , 0.0f,//P

    -0.01f , -0.03f , 0.0f,//D
    0.1f , -0.04f , 0.0f,//N
    0.0f , 0.12f , 0.0f,//Q

    -0.1f , 0.06f, 0.0f,//F 
    0.0f , 0.12f , 0.0f,//Q
    -0.01f , -0.03f , 0.0f,//D

    -0.1f , 0.06f, 0.0f,//F 
    0.0f , 0.12f , 0.0f,//Q
    -0.02f , 0.19f , 0.0f,//T

    0.1f , -0.04f , 0.0f,//N
    0.07f , 0.01f , 0.0f,//R
    0.17f , 0.06f , 0.0f,//S

    0.0f , 0.12f , 0.0f,//Q
    -0.02f , 0.19f , 0.0f,//T
    0.18f , 0.29f , 0.0f//U	

};*/

const int size_ = sizeof(vertices) / sizeof(vertices[0]);

float** create_matrix();
void initilize(float** m);
void vector_column_vextex(float vertices[], int size_);
void produc_vec_column(float** matrix, float vertices[]);

void traslate(float traslate_x, float traslate_y);
void _scale(float escala);
void rotate_x(float angulo);
void rotate_y(float angulo);
void rotate_origen(float angulo);
void reflect_origin();
void reflect_x_axis();
void reflect_y_axis();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float rValue = 0.0f, gValue = 0.0f, bValue = 0.0f;

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Laboratorio 08", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    float cColor = 0.0f, cColorInc = 0.0001f;
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    //vectores columna
    vector_column_vextex(vertices, size_);
    int numTriangles = 100;

    // Vertex Array Object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex Buffer Object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
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


    while (!glfwWindowShouldClose(window)) {
        glClearColor(rValue, gValue, bValue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        processInput(window);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, numTriangles);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {//mover hacia arriba
        traslate(0, 0.008);
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {//mover hacia abajo
        traslate(0, -0.008);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {//mover hacia la izquiera
        traslate(-0.008, 0);
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {//mover hacia la derecha
        traslate(0.008, 0);
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {//rota con respecto a su origen
        rotate_origen(12.5);
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {//rota con respecto a X
        rotate_x(12.5);
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {//rota con respecto a Y
        rotate_y(12.5);
    }

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {//crece
        _scale(2);
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {//encoge
        _scale(0.2);
    }

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {//refleja con respecto al origen
        reflect_origin();
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {//refleja con respecto a X
        reflect_x_axis();
    }

    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {//refleja con respecto a Y
        reflect_y_axis();
    }
}


float** create_matrix() {
    float** scale_ = new float* [3];
    for (int i = 0; i < 3; i++) {
        scale_[i] = new float[3]{ 0 };
    }
    return scale_;
}

void initilize(float** m) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == j)
                m[i][j] = 1;
            else
                m[i][j] = 0;
        }
    }
}

void produc_vec_column(float** matrix, float vertices[]) {
    float* output = new float[size_] {0};
    int i = 0, pos_output = 0, pos_ver = 0;
    while (pos_output < size_) {
        for (int j = 0; j < 3; j++)
            output[pos_output] += matrix[i][j] * vertices[pos_ver + j];

        pos_output++; i++;
        if (i == 3) { i = 0; pos_ver += 3; }
    }

    for (int i = 0; i < size_; i++)
        vertices[i] = output[i];
}

void vector_column_vextex(float vertices[], int size_) {
    int pos = 2;
    if (size_ == 3)
        vertices[2] = 1;
    else {
        for (int i = 0; i < size_; i++) {
            if (i == pos) {
                vertices[i] = 1;
                pos += 3;
            }
        }
    }
}

void _scale(float escala) {
    auto _scale_ = create_matrix();
    initilize(_scale_);
    _scale_[0][0] = _scale_[0][0] * escala;
    _scale_[1][1] = _scale_[1][1] * escala;
    produc_vec_column(_scale_, vertices);
}

void rotate_x(float angulo) {
    auto _rotate_x = create_matrix();
    initilize(_rotate_x);
    _rotate_x[0][1] = tan(angulo);
    produc_vec_column(_rotate_x, vertices);
}

void rotate_y(float angulo) {
    auto _rotate_y = create_matrix();
    initilize(_rotate_y);
    _rotate_y[1][0] = tan(angulo);
    produc_vec_column(_rotate_y, vertices);
}

void rotate_origen(float angulo) {
    auto _rotate_origen = create_matrix();
    initilize(_rotate_origen);
    _rotate_origen[0][0] = cos(angulo);
    _rotate_origen[1][1] = cos(angulo);
    _rotate_origen[0][1] = sin(angulo);
    _rotate_origen[1][0] = -sin(angulo);
    produc_vec_column(_rotate_origen, vertices);
}

void traslate(float traslate_x, float traslate_y) {
    float** _traslate_ = create_matrix();
    initilize(_traslate_);
    _traslate_[0][2] = traslate_x;
    _traslate_[1][2] = traslate_y;

    produc_vec_column(_traslate_, vertices);
}

void reflect_origin() {
    float** origin = create_matrix();
    initilize(origin);
    origin[0][0] = -1;
    origin[1][1] = -1;

    produc_vec_column(origin, vertices);
}

void reflect_x_axis() {
    float** reflect_x = create_matrix();
    initilize(reflect_x);
    reflect_x[1][1] = -1;

    produc_vec_column(reflect_x, vertices);
}

void reflect_y_axis() {
    float** reflect_y = create_matrix();
    initilize(reflect_y);
    reflect_y[0][0] = -1;

    produc_vec_column(reflect_y, vertices);
}