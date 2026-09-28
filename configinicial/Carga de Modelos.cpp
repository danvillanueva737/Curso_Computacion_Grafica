/*
Práctica 6.
Nombre: Daniel Kaleb Villanueva Figueroa
Número de cuenta: 320173985
Fecha: 25/09/2026
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


// Camera
Camera camera(glm::vec3(0.0f, 1.5f, 6.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;



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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Práctica 6. Carga de modelos y camara sintetica. Daniel Villanueva", nullptr, nullptr);

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

        // Clear the colorbuffer
        glClearColor(0.85f, 0.80f, 0.70f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

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
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"),1,GL_FALSE,glm::value_ptr(model));
        dog.Draw(shader);


        // ======================================
        // ÁRBOLES - GRUPO 1
        // ======================================
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.8f, -0.40f, -3.2f));
        model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.16f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        tree.Draw(shader);


        // ======================================
        // ÁRBOLES - GRUPO 2
        // ======================================
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.8f, -0.40f, -3.6f));
        model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.16f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        tree.Draw(shader);


        // ======================================
        // ÁRBOLES - GRUPO 3
        // ======================================
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.2f, -0.40f, -3.4f));
        model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.13f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        tree.Draw(shader);


        // ======================================
        // ÁRBOLES - GRUPO 4
        // ======================================
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0f, -0.40f, -3.6f));
        model = glm::rotate(model, glm::radians(-35.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.13f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        tree.Draw(shader);


        // ======================================
        // ÁRBOLES - COSTADO IZQUIERDO
        // ======================================
        model = glm::mat4(1.0f);
        model = glm::translate(
            model,
            glm::vec3(-3.5f, -0.40f, -0.5f)
        );
        model = glm::rotate(
            model,
            glm::radians(90.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
        model = glm::scale(
            model,
            glm::vec3(0.13f)
        );
        glUniformMatrix4fv(
            glGetUniformLocation(shader.Program, "model"),
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );
        tree.Draw(shader);

        
        // ======================================
        // ÁRBOLES - COSTADO IZQUIERDO 2
        // ======================================
        model = glm::mat4(1.0f);

        model = glm::translate(
            model,
            glm::vec3(-4.6f, -0.40f, -1.8f)
        );

        model = glm::rotate(
            model,
            glm::radians(90.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        model = glm::scale(
            model,
            glm::vec3(0.13f)
        );

        glUniformMatrix4fv(
            glGetUniformLocation(shader.Program, "model"),
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        tree.Draw(shader);


        // ======================================
        // ÁRBOLES - COSTADO DERECHO
        // ======================================
        model = glm::mat4(1.0f);

        model = glm::translate(
            model,
            glm::vec3(3.5f, -0.40f, -0.5f)
        );

        model = glm::rotate(
            model,
            glm::radians(-90.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        model = glm::scale(
            model,
            glm::vec3(0.13f)
        );

        glUniformMatrix4fv(
            glGetUniformLocation(shader.Program, "model"),
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        tree.Draw(shader);


        // ======================================
        // ÁRBOLES - COSTADO DERECHO 2
        // ======================================
        model = glm::mat4(1.0f);

        model = glm::translate(
            model,
            glm::vec3(4.6f, -0.40f, -1.8f)
        );

        model = glm::rotate(
            model,
            glm::radians(-90.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        model = glm::scale(
            model,
            glm::vec3(0.13f)
        );

        glUniformMatrix4fv(
            glGetUniformLocation(shader.Program, "model"),
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        tree.Draw(shader);


        // ======================================
        // PISO
        // ======================================
        model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(0.0f, -0.55f, 0.0f));
        model = glm::scale(model,glm::vec3(0.08f, 0.10f, 0.08f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"),1,GL_FALSE,glm::value_ptr(model));
        ground.Draw(shader);


        // ======================================
        // BANCA
        // ======================================
        model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(2.5f, -0.40f, -0.5f));
        model = glm::scale(model,glm::vec3(0.75f, 0.75f, 0.75f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"),1,GL_FALSE,glm::value_ptr(model));
        bench.Draw(shader);


        // ======================================
        // CASA DEL PERRO
        // ======================================
        model = glm::mat4(1.0f);

        model = glm::translate(
            model,
            glm::vec3(-2.0f, -0.40f, -0.8f)
        );

        model = glm::scale(
            model,
            glm::vec3(0.35f)
        );

        glUniformMatrix4fv(
            glGetUniformLocation(shader.Program, "model"),
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        dogHouse.Draw(shader);



        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
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