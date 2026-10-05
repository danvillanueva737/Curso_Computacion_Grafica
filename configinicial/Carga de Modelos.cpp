/*
Práctica 8.
Nombre: Daniel Kaleb Villanueva Figueroa
Número de cuenta: 320173985
Fecha: 05/10/2026
*/


// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

float orbitAngle = 0.0f;
float orbitControlSpeed = 0.8f;

// Camera
Camera camera(glm::vec3(0.0f, 1.5f, 6.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// ======================================
// ILUMINACIÓN
// ======================================

glm::vec3 sunPos(0.0f, 0.0f, 0.0f);
glm::vec3 moonPos(0.0f, 0.0f, 0.0f);

const float PI = 3.14159265f;

// Parámetros de la órbita
float orbitRadius = 8.0f;
float orbitHeight = 6.0f;
float orbitSpeed = 0.30f;


int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Práctica 8. Daniel Villanueva", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lightModelShader("Shader/lamp.vs", "Shader/lamp.frag");

    // Load models
    
    //Se carga al perrito
    Model dog((char*)"Models/RedDog.obj");
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    
    //Se carga árboles
    Model tree((char*)"Models/tree1.obj");

    //Se carga piso
    Model ground((char*)"Models/desert ground-obj.obj");

    //Se carga la Banca
    Model bench((char*)"Models/Bench/PublicBench.obj");

    //Se carga la Casa de perro
    Model dogHouse((char*)"Models/DogHouse/niche.obj");

    //Se carga el Sol
    Model celestialBody((char*)"Models/Sun/Sun.obj");

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // ======================================
 // ÁNGULO DE LA ÓRBITA
 // ======================================

        float sunAngle = orbitAngle;


        // ======================================
        // ÓRBITA DEL SOL Y LA LUNA
        // ======================================

        // SOL
        sunPos.x = cos(sunAngle) * orbitRadius;
        sunPos.y = sin(sunAngle) * orbitHeight;
        sunPos.z = 0.0f;

        // LUNA
        moonPos.x = cos(sunAngle + PI) * orbitRadius;
        moonPos.y = sin(sunAngle + PI) * orbitHeight;
        moonPos.z = 0.0f;


        // ======================================
        // ALTURA DEL SOL Y LA LUNA
        // ======================================

        // Valores aproximadamente entre -1 y 1
        float sunHeight =
            sunPos.y / orbitHeight;

        float moonHeight =
            moonPos.y / orbitHeight;


        // ======================================
        // INTENSIDAD GRADUAL
        // ======================================

        float sunIntensity =
            glm::smoothstep(
                -0.15f,
                0.25f,
                sunHeight
            );

        float moonIntensity =
            glm::smoothstep(
                -0.15f,
                0.25f,
                moonHeight
            );


        // ======================================
        // COLORES DEL CIELO
        // ======================================

        glm::vec3 daySky(
            0.53f,
            0.75f,
            0.95f
        );

        glm::vec3 sunsetSky(
            0.90f,
            0.45f,
            0.20f
        );

        glm::vec3 nightSky(
            0.01f,
            0.02f,
            0.08f
        );

        glm::vec3 skyColor;


        // ======================================
        // TRANSICIÓN DEL CIELO
        // ======================================

        if (sunHeight < 0.0f)
        {
            // NOCHE -> AMANECER

            float t =
                glm::smoothstep(
                    -0.35f,
                    0.0f,
                    sunHeight
                );

            skyColor =
                glm::mix(
                    nightSky,
                    sunsetSky,
                    t
                );
        }
        else
        {
            // AMANECER -> DÍA

            float t =
                glm::smoothstep(
                    0.0f,
                    0.35f,
                    sunHeight
                );

            skyColor =
                glm::mix(
                    sunsetSky,
                    daySky,
                    t
                );
        }


        // ======================================
        // LIMPIAR PANTALLA
        // ======================================

        glClearColor(
            skyColor.r,
            skyColor.g,
            skyColor.b,
            1.0f
        );

        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT
        );


        // ======================================
        // SHADER DE ILUMINACIÓN
        // ======================================

        lightingShader.Use();
        glm::mat4 view = camera.GetViewMatrix();

        // Matriz de proyección
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program,"projection"),1,GL_FALSE,glm::value_ptr(projection));


        // Matriz de vista
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program,"view"),1,GL_FALSE,glm::value_ptr(view));


        // ======================================
        // POSICIÓN DE LA CÁMARA
        // ======================================

        glUniform3f(glGetUniformLocation(lightingShader.Program,"viewPos"),camera.GetPosition().x,camera.GetPosition().y,camera.GetPosition().z);


        // ======================================
        // LUZ DEL SOL
        // ======================================

        // Posición
        glUniform3f(glGetUniformLocation(lightingShader.Program,"sunLight.position"),sunPos.x,sunPos.y,sunPos.z);
        // Ambiental
        glUniform3f(glGetUniformLocation(lightingShader.Program,"sunLight.ambient"),0.30f,0.28f,0.22f);
        // Difusa
        glUniform3f(glGetUniformLocation(lightingShader.Program,"sunLight.diffuse"),1.00f,0.85f,0.60f);
        // Especular
        glUniform3f(glGetUniformLocation(lightingShader.Program,"sunLight.specular"),0.90f,0.80f,0.65f);
        // Intensidad
        glUniform1f(glGetUniformLocation(lightingShader.Program,"sunLight.intensity"),sunIntensity);


        // ======================================
        // LUZ DE LA LUNA
        // ======================================

        // Posición
        glUniform3f(
            glGetUniformLocation(
                lightingShader.Program,
                "moonLight.position"
            ),
            moonPos.x,
            moonPos.y,
            moonPos.z
        );

        // Ambiental
        glUniform3f(
            glGetUniformLocation(
                lightingShader.Program,
                "moonLight.ambient"
            ),
            0.08f,
            0.10f,
            0.18f
        );

        // Difusa
        glUniform3f(
            glGetUniformLocation(
                lightingShader.Program,
                "moonLight.diffuse"
            ),
            0.22f,
            0.32f,
            0.60f
        );

        // Especular
        glUniform3f(
            glGetUniformLocation(
                lightingShader.Program,
                "moonLight.specular"
            ),
            0.35f,
            0.45f,
            0.75f
        );

        // Intensidad
        glUniform1f(
            glGetUniformLocation(
                lightingShader.Program,
                "moonLight.intensity"
            ),
            moonIntensity * 1.4f
        );

        // ======================================
        // PROPIEDADES DEL MATERIAL
        // ======================================

        glUniform3f(glGetUniformLocation(lightingShader.Program,"material.ambient"),1.0f,1.0f,1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program,"material.diffuse"),1.0f,1.0f,1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program,"material.specular"),0.5f,0.5f,0.5f);
        glUniform1f(glGetUniformLocation(lightingShader.Program,"material.shininess"),32.0f);


        //======================
        //Checkpoint
        //=======================
        /*
        // Draw the loaded model
        glm::mat4 model(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(shader);

        model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(shader);
        */

        // ======================================
        // PERRITO - PRUEBA
        // ======================================

        glm::mat4 model(1.0f);
        model = glm::translate(model,glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model,glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program,"model"),1,GL_FALSE,glm::value_ptr(model));
        dog.Draw(lightingShader);


        // ======================================
        // ÁRBOLES - GRUPO 1
        // ======================================
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.8f, -0.40f, -3.2f));
        model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.16f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        tree.Draw(lightingShader);


        // ======================================
        // ÁRBOLES - GRUPO 2
        // ======================================
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.8f, -0.40f, -3.6f));
        model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.16f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        tree.Draw(lightingShader);


        // ======================================
        // ÁRBOLES - GRUPO 3
        // ======================================
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.2f, -0.40f, -3.4f));
        model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.13f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        tree.Draw(lightingShader);


        // ======================================
        // ÁRBOLES - GRUPO 4
        // ======================================
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0f, -0.40f, -3.6f));
        model = glm::rotate(model, glm::radians(-35.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.13f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        tree.Draw(lightingShader);


        // ======================================
        // ÁRBOLES - COSTADO IZQUIERDO
        // ======================================
        model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(-3.5f, -0.40f, -0.5f));
        model = glm::rotate(model,glm::radians(90.0f),glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model,glm::vec3(0.13f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"),1,GL_FALSE,glm::value_ptr(model));
        tree.Draw(lightingShader);

        
        // ======================================
        // ÁRBOLES - COSTADO IZQUIERDO 2
        // ======================================
        model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(-4.6f, -0.40f, -1.8f));
        model = glm::rotate(model,glm::radians(90.0f),glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model,glm::vec3(0.13f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"),1,GL_FALSE,glm::value_ptr(model));
        tree.Draw(lightingShader);


        // ======================================
        // ÁRBOLES - COSTADO DERECHO
        // ======================================
        model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(3.5f, -0.40f, -0.5f));
        model = glm::rotate(model,glm::radians(-90.0f),glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model,glm::vec3(0.13f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"),1,GL_FALSE,glm::value_ptr(model));
        tree.Draw(lightingShader);


        // ======================================
        // ÁRBOLES - COSTADO DERECHO 2
        // ======================================
        model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(4.6f, -0.40f, -1.8f));
        model = glm::rotate(model,glm::radians(-90.0f),glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model,glm::vec3(0.13f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"),1,GL_FALSE,glm::value_ptr(model));
        tree.Draw(lightingShader);


        // ======================================
        // PISO
        // ======================================
        model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(0.0f, -0.55f, 0.0f));
        model = glm::scale(model,glm::vec3(0.08f, 0.10f, 0.08f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"),1,GL_FALSE,glm::value_ptr(model));
        ground.Draw(lightingShader);


        // ======================================
        // BANCA
        // ======================================
        model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(2.5f, -0.40f, -0.5f));
        model = glm::scale(model,glm::vec3(0.75f, 0.75f, 0.75f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"),1,GL_FALSE,glm::value_ptr(model));
        bench.Draw(lightingShader);


        // ======================================
        // CASA DEL PERRO
        // ======================================
        model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(-2.0f, -0.40f, -0.8f));
        model = glm::scale(model,glm::vec3(0.35f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"),1,GL_FALSE,glm::value_ptr(model));
        dogHouse.Draw(lightingShader);


        // ======================================
        // REPRESENTACIÓN VISUAL DEL SOL Y LA LUNA
        // ======================================

        lightModelShader.Use();

        // projection
        glUniformMatrix4fv(
            glGetUniformLocation(lightModelShader.Program, "projection"),
            1,
            GL_FALSE,
            glm::value_ptr(projection)
        );

        // view
        glUniformMatrix4fv(
            glGetUniformLocation(lightModelShader.Program, "view"),
            1,
            GL_FALSE,
            glm::value_ptr(view)
        );



        // ======================================
        // SOL
        // ======================================

        glm::mat4 sunModelMat = glm::mat4(1.0f);

        sunModelMat = glm::translate(
            sunModelMat,
            sunPos
        );

        sunModelMat = glm::scale(
            sunModelMat,
            glm::vec3(0.45f)
        );

        glUniformMatrix4fv(
            glGetUniformLocation(lightModelShader.Program, "model"),
            1,
            GL_FALSE,
            glm::value_ptr(sunModelMat)
        );

        // Naranja brillante
        glUniform3f(
            glGetUniformLocation(lightModelShader.Program, "lightColor"),
            1.0f,
            0.65f,
            0.10f
        );

        celestialBody.Draw(lightModelShader);



        // ======================================
        // LUNA
        // ======================================

        glm::mat4 moonModelMat = glm::mat4(1.0f);

        moonModelMat = glm::translate(
            moonModelMat,
            moonPos
        );

        moonModelMat = glm::scale(
            moonModelMat,
            glm::vec3(0.30f)
        );

        glUniformMatrix4fv(
            glGetUniformLocation(lightModelShader.Program, "model"),
            1,
            GL_FALSE,
            glm::value_ptr(moonModelMat)
        );

        // Azul frío
        glUniform3f(
            glGetUniformLocation(lightModelShader.Program, "lightColor"),
            0.55f,
            0.75f,
            1.0f
        );

        celestialBody.Draw(lightModelShader);



        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Cámara
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        camera.ProcessKeyboard(FORWARD, deltaTime);

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        camera.ProcessKeyboard(BACKWARD, deltaTime);

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        camera.ProcessKeyboard(LEFT, deltaTime);

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        camera.ProcessKeyboard(RIGHT, deltaTime);


    // ======================================
    // CONTROL SOL / LUNA
    // ======================================

    if (keys[GLFW_KEY_O])
    {
        orbitAngle +=
            orbitControlSpeed *
            deltaTime;
    }

    if (keys[GLFW_KEY_L])
    {
        orbitAngle -=
            orbitControlSpeed *
            deltaTime;
    }


    // Mantener ángulo entre 0 y 2PI
    if (orbitAngle > 2.0f * PI)
    {
        orbitAngle -=
            2.0f * PI;
    }

    if (orbitAngle < 0.0f)
    {
        orbitAngle +=
            2.0f * PI;
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }

}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}