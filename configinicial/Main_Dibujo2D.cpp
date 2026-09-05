// Alumno: Villanueva Figueroa Daniel Kaleb
// Número de cuenta: 320173985
// Fecha de entrega: 28/08/2026
// Práctica 2

#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;


int main() {

	glfwInit();

	//Verificacion de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/


	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Cerdito con Primitivas 2D", NULL, NULL);

	glfwSetFramebufferSizeCallback(window, resize);


	//Verificacion de errores de creacion ventana

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;

		glfwTerminate();

		return EXIT_FAILURE;
	}


	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;


	//Verificacion de errores de inicializacion de glew

	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialise GLEW" << std::endl;

		return EXIT_FAILURE;
	}


	// Imprimimos informacion de OpenGL del sistema

	std::cout << "> Version: "
		<< glGetString(GL_VERSION)
		<< std::endl;

	std::cout << "> Vendor: "
		<< glGetString(GL_VENDOR)
		<< std::endl;

	std::cout << "> Renderer: "
		<< glGetString(GL_RENDERER)
		<< std::endl;

	std::cout << "> SL Version: "
		<< glGetString(GL_SHADING_LANGUAGE_VERSION)
		<< std::endl;


	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);


	Shader ourShader(
		"Shader/core.vs",
		"Shader/core.frag"
	);


	// Set up vertex data (and buffer(s)) and attribute pointers


	float vertices[] = {

		// CABEZA

		 0.0f,  0.05f, 0.0f,     1.0f, 0.70f, 0.72f, // 0 centro

		-0.48f, 0.70f, 0.0f,     1.0f, 0.70f, 0.72f, // 1
		 0.48f, 0.70f, 0.0f,     1.0f, 0.70f, 0.72f, // 2

		 0.62f, 0.45f, 0.0f,     1.0f, 0.70f, 0.72f, // 3
		 0.80f, 0.30f, 0.0f,     1.0f, 0.70f, 0.72f, // 4

		 0.80f,-0.28f, 0.0f,     1.0f, 0.70f, 0.72f, // 5
		 0.60f,-0.55f, 0.0f,     1.0f, 0.70f, 0.72f, // 6

		-0.60f,-0.55f, 0.0f,     1.0f, 0.70f, 0.72f, // 7
		-0.80f,-0.28f, 0.0f,     1.0f, 0.70f, 0.72f, // 8

		-0.80f, 0.30f, 0.0f,     1.0f, 0.70f, 0.72f, // 9
		-0.62f, 0.45f, 0.0f,     1.0f, 0.70f, 0.72f, // 10

		-0.48f, 0.70f, 0.0f,     1.0f, 0.70f, 0.72f, // 11


		// OREJAS

		// Oreja izquierda

		-0.62f, 0.45f, 0.0f,     0.95f, 0.45f, 0.50f, // 12
		-0.48f, 0.70f, 0.0f,     0.95f, 0.45f, 0.50f, // 13
		-0.58f, 0.27f, 0.0f,     0.95f, 0.45f, 0.50f, // 14


		// Oreja derecha

		 0.62f, 0.45f, 0.0f,     0.95f, 0.45f, 0.50f, // 15
		 0.48f, 0.70f, 0.0f,     0.95f, 0.45f, 0.50f, // 16
		 0.58f, 0.27f, 0.0f,     0.95f, 0.45f, 0.50f, // 17


		 // DOBLECES INFERIORES
		 
		 // Doblez izquierdo

		 -0.80f,-0.28f, 0.0f,     0.95f, 0.62f, 0.65f, // 18
		 -0.60f,-0.55f, 0.0f,     0.95f, 0.62f, 0.65f, // 19
		 -0.18f,-0.43f, 0.0f,     0.95f, 0.62f, 0.65f, // 20


		 // Doblez derecho

		  0.80f,-0.28f, 0.0f,     0.95f, 0.62f, 0.65f, // 21
		  0.60f,-0.55f, 0.0f,     0.95f, 0.62f, 0.65f, // 22
		  0.18f,-0.43f, 0.0f,     0.95f, 0.62f, 0.65f, // 23


		  // HOCICO
		  
		   0.00f,-0.16f, 0.0f,     1.0f, 0.82f, 0.84f, // 24 centro

		   0.00f, 0.05f, 0.0f,     1.0f, 0.82f, 0.84f, // 25
		   0.22f,-0.15f, 0.0f,     1.0f, 0.82f, 0.84f, // 26
		   0.00f,-0.36f, 0.0f,     1.0f, 0.82f, 0.84f, // 27
		  -0.22f,-0.15f, 0.0f,     1.0f, 0.82f, 0.84f, // 28

		   0.00f, 0.05f, 0.0f,     1.0f, 0.82f, 0.84f, // 29


		   // OJOS

		   -0.28f, 0.27f, 0.0f,     0.05f, 0.05f, 0.05f, // 30
			0.28f, 0.27f, 0.0f,     0.05f, 0.05f, 0.05f, // 31


			// MEJILLAS

			-0.43f, 0.07f, 0.0f,     1.0f, 0.30f, 0.35f, // 32
			 0.43f, 0.07f, 0.0f,     1.0f, 0.30f, 0.35f, // 33


			 // NARIZ

			 -0.055f,-0.15f, 0.0f,     0.08f, 0.05f, 0.05f, // 34
			  0.055f,-0.15f, 0.0f,     0.08f, 0.05f, 0.05f, // 35


			  // LINEAS DE DOBLECES
			  
			  // Izquierda abajo

			  -0.80f,-0.28f, 0.0f,     0.80f, 0.40f, 0.45f, // 36
			  -0.18f,-0.43f, 0.0f,     0.80f, 0.40f, 0.45f, // 37


			  // Derecha abajo

			   0.80f,-0.28f, 0.0f,     0.80f, 0.40f, 0.45f, // 38
			   0.18f,-0.43f, 0.0f,     0.80f, 0.40f, 0.45f, // 39


			   // Izquierda arriba

			   -0.80f, 0.30f, 0.0f,     0.80f, 0.40f, 0.45f, // 40
			   -0.62f, 0.45f, 0.0f,     0.80f, 0.40f, 0.45f, // 41


			   // Derecha arriba

				0.80f, 0.30f, 0.0f,     0.80f, 0.40f, 0.45f, // 42
				0.62f, 0.45f, 0.0f,     0.80f, 0.40f, 0.45f, // 43


				// CONTORNO DEL HOCICO
				
				 0.00f, 0.05f, 0.0f,     0.85f, 0.50f, 0.55f, // 44
				 0.22f,-0.15f, 0.0f,     0.85f, 0.50f, 0.55f, // 45
				 0.00f,-0.36f, 0.0f,     0.85f, 0.50f, 0.55f, // 46
				-0.22f,-0.15f, 0.0f,     0.85f, 0.50f, 0.55f  // 47

	};

	// Dejamos tambien el arreglo de indices que venia
	// originalmente en el codigo base

	unsigned int indices[] = {

		3, 2, 1,
		0, 1, 3

	};



	GLuint VBO, VAO, EBO;


	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);

	glGenBuffers(1, &EBO);



	// Enlazar Vertex Array Object

	glBindVertexArray(VAO);



	// 2.- Copiamos nuestro arreglo de vertices en un buffer
	// de vertices para que OpenGL lo use

	glBindBuffer(GL_ARRAY_BUFFER, VBO);


	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);



	// 3.- Copiamos nuestro arreglo de indices

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);



	// 4.- Colocamos las caracteristicas de los vertices


	// Posicion

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*)0);

	glEnableVertexAttribArray(0);



	// Color

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));


	glEnableVertexAttribArray(1);



	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0);



	while (!glfwWindowShouldClose(window))
	{

		// Check if any events have been activated
		// (key pressed, mouse moved, etc.)

		glfwPollEvents();



		// Render
		// Clear the colorbuffer


		// Fondo blanco

		glClearColor(
			1.0f,
			1.0f,
			1.0f,
			1.0f
		);


		glClear(GL_COLOR_BUFFER_BIT);
		


		// Draw our figures

		ourShader.Use();


		glBindVertexArray(VAO);

		// CABEZA
		glDrawArrays(GL_TRIANGLE_FAN, 0, 12);

		// OREJAS
		glDrawArrays(GL_TRIANGLES, 12, 6);


		// DOBLECES INFERIORES
		glDrawArrays(GL_TRIANGLES, 18, 6);


		// HOCICO
		glDrawArrays(GL_TRIANGLE_FAN, 24, 6);

		// OJOS
		glPointSize(20);

		glDrawArrays(GL_POINTS,	30,	2);

		// MEJILLAS
		glPointSize(10);


		glDrawArrays(GL_POINTS,	32,	2);



		// NARIZ
		glPointSize(6);


		glDrawArrays(GL_POINTS, 34,	2);



		// LINEAS DEL ORIGAMI
		glLineWidth(2);


		glDrawArrays(GL_LINES, 36, 8);


		// CONTORNO DEL HOCICO

		glDrawArrays(GL_LINE_LOOP, 44, 4);

		
		glBindVertexArray(0);



		// Swap the screen buffers

		glfwSwapBuffers(window);

	}



	glfwTerminate();


	return EXIT_SUCCESS;
}



void resize(GLFWwindow* window, int width, int height)
{

	// Set the Viewport to the size of the created window

	glViewport(0, 0, width, height);


	//glViewport(0, 0, screenWidth, screenHeight);

}