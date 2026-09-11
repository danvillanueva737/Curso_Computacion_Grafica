//Práctica 4 Modelado Geométrico
//Autor: Daniel Villanueva Figueroa
//Fecha: 10/09/26

#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Shaders
#include "Shader.h"

void Inputs(GLFWwindow *window);


const GLint WIDTH = 800, HEIGHT = 600;
float movX = 0.0f;
float movY = 0.0f;
float movZ = -8.0f;
float rot = 0.0f;
int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Villanueva Figueroa Daniel Kaleb", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificaci�n de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}


	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);


	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers

	

	// use with Perspective Projection
	float vertices[] = {
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,//Front
		0.5f, -0.5f, 0.5f,  1.0f, 0.0f,0.0f,
		0.5f,  0.5f, 0.5f,  1.0f, 0.0f,0.0f,
		0.5f,  0.5f, 0.5f,  1.0f, 0.0f,0.0f,
		-0.5f,  0.5f, 0.5f, 1.0f, 0.0f,0.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,
		
	    -0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,//Back
		 0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,
		 0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
		 0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	    -0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	    -0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,
		
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  -0.5f, 0.5f, 0.0f, 0.0f,1.0f,
      
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 1.0f,0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
		
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f,1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,
		
		-0.5f,  0.5f, -0.5f, 1.0f, 0.2f,0.5f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.2f,0.5f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.2f,0.5f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.2f,0.5f,
		-0.5f,  0.5f,  0.5f, 1.0f, 0.2f,0.5f,
		-0.5f,  0.5f, -0.5f, 1.0f, 0.2f,0.5f,
	};




	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//Lo desactivamos para poder darle un color diferente a cada cubo
	glDisableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	
	glm::mat4 projection=glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	//projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);//Izq,Der,Fondo,Alto,Cercania,Lejania
	while (!glfwWindowShouldClose(window))
	{

		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glm::mat4 model = glm::mat4(1);
		glm::mat4 view = glm::mat4(1);


		view = glm::translate(view, glm::vec3(movX, movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");


		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


		glBindVertexArray(VAO);

		// CUERPO DE LA VACA
		
		model = glm::mat4(1.0f);

		//Posicion
		model = glm::translate(model, glm::vec3(0.3f, 0.5f, 0.0f));

		//Tamaño
		model = glm::scale(model, glm::vec3(3.0f, 1.4f, 1.4f));

		//Color cafe
		glVertexAttrib3f(1, 0.25f, 0.15f, 0.08f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);


		
		// CABEZA
		
		model = glm::mat4(1.0f);

		//Posicion
		model = glm::translate(model, glm::vec3(-1.65f, 0.45f, 0.0f));

		//Tamaño
		model = glm::scale(model, glm::vec3(1.0f, 1.25f, 1.15f));

		//Color cafe
		glVertexAttrib3f(1, 0.27f, 0.17f, 0.09f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);


		// HOCICO
		model = glm::mat4(1.0f);

		//Posicion
		model = glm::translate(model, glm::vec3(-2.23f, 0.27f, 0.0f));
		
		//Tamaño
		model = glm::scale(model, glm::vec3(0.42f, 0.52f, 0.82f));

		//Color gris/cafe claro
		glVertexAttrib3f(1, 0.45f, 0.40f, 0.35f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);


		
		// PATA DELANTERA 1
		
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(-0.85f, -0.85f, 0.48f));

		model = glm::scale(model, glm::vec3(0.40f, 1.55f, 0.40f));

		glVertexAttrib3f(1, 0.22f, 0.13f, 0.07f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);



		// PATA DELANTERA 2
		
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(-0.85f, -0.85f, -0.48f));

		model = glm::scale(model, glm::vec3(0.40f, 1.55f, 0.40f));

		glVertexAttrib3f(1, 0.22f, 0.13f, 0.07f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);


		// PATA TRASERA 1

		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(1.25f, -0.85f, 0.48f));

		model = glm::scale(model, glm::vec3(0.40f, 1.55f, 0.40f));

		glVertexAttrib3f(1, 0.22f, 0.13f, 0.07f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);


		// PATA TRASERA 2

		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(1.25f, -0.85f, -0.48f));

		model = glm::scale(model, glm::vec3(0.40f, 1.55f, 0.40f));

		glVertexAttrib3f(1, 0.22f, 0.13f, 0.07f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);


				 
		// PEZUÑA DELANTERA 1

		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(-0.85f, -1.72f, 0.48f));

		model = glm::scale(model, glm::vec3(0.42f, 0.22f, 0.42f));

		glVertexAttrib3f(1, 0.04f, 0.03f, 0.02f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		 
		
		// PEZUÑA DELANTERA 2
		
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(-0.85f, -1.72f, -0.48f));

		model = glm::scale(model, glm::vec3(0.42f, 0.22f, 0.42f));

		glVertexAttrib3f(1, 0.04f, 0.03f, 0.02f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);




		// PEZUÑA TRASERA 1
		
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(1.25f, -1.72f, 0.48f));

		model = glm::scale(model, glm::vec3(0.42f, 0.22f, 0.42f));

		glVertexAttrib3f(1, 0.04f, 0.03f, 0.02f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);




		// PEZUÑA TRASERA 2
		
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(1.25f, -1.72f, -0.48f));

		model = glm::scale(model, glm::vec3(0.42f, 0.22f, 0.42f));

		glVertexAttrib3f(1, 0.04f, 0.03f, 0.02f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);


		

		// OREJA 1
		
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(-1.65f, 1.15f, 0.68f));

		model = glm::scale(model, glm::vec3(0.30f, 0.20f, 0.30f));

		glVertexAttrib3f(1, 0.50f, 0.28f, 0.30f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);



		// OREJA 2
		
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(-1.65f, 1.15f, -0.68f));

		model = glm::scale(model, glm::vec3(0.30f, 0.20f, 0.30f));

		glVertexAttrib3f(1, 0.50f, 0.28f, 0.30f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);


		
		// INTERIOR OREJA 1

		model = glm::mat4(1.0f);
		
		model = glm::translate(model, glm::vec3(-1.65f, 1.15f, 0.68f));
		
		model = glm::scale(model, glm::vec3(0.16f, 0.11f, 0.16f));
		
		glVertexAttrib3f(1, 0.75f, 0.50f, 0.55f);
		
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		
		glDrawArrays(GL_TRIANGLES, 0, 36);


		
		// INTERIOR OREJA 2
		
		model = glm::mat4(1.0f);
		
		model = glm::translate(model, glm::vec3(-1.65f, 1.15f, -0.68f));
		
		model = glm::scale(model, glm::vec3(0.16f, 0.11f, 0.16f));
		
		glVertexAttrib3f(1, 0.75f, 0.50f, 0.55f);
		
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// CUERNO 1
		
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(-1.65f, 1.42f, 0.38f));

		model = glm::scale(model, glm::vec3(0.15f, 0.40f, 0.15f));

		glVertexAttrib3f(1, 0.65f, 0.65f, 0.65f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);



		// CUERNO 2

		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(-1.65f, 1.42f, -0.38f));

		model = glm::scale(model, glm::vec3(0.15f, 0.40f, 0.15f));

		glVertexAttrib3f(1, 0.65f, 0.65f, 0.65f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		

		//======================================================
// OJO IZQUIERDO - BASE BLANCA
//======================================================
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.12f, 0.74f, 0.43f));
		model = glm::scale(model, glm::vec3(0.10f, 0.18f, 0.24f));
		glVertexAttrib3f(1, 1.0f, 1.0f, 1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		
		//======================================================
		// OJO IZQUIERDO - PUPILA NEGRA
		//======================================================
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.17f, 0.78f, 0.50f));
		model = glm::scale(model, glm::vec3(0.05f, 0.15f, 0.10f));
		glVertexAttrib3f(1, 0.0f, 0.0f, 0.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		
		//======================================================
		// OJO DERECHO - BASE BLANCA
		//======================================================
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.12f, 0.74f, -0.43f));
		model = glm::scale(model, glm::vec3(0.10f, 0.18f, 0.24f));
		glVertexAttrib3f(1, 1.0f, 1.0f, 1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		
		//======================================================
		// OJO DERECHO - PUPILA NEGRA
		//======================================================
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.17f, 0.78f, -0.50f));
		model = glm::scale(model, glm::vec3(0.05f, 0.15f, 0.10f));
		glVertexAttrib3f(1, 0.0f, 0.0f, 0.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// FOSA NASAL 1
		
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(-2.49f, 0.28f, 0.25f));

		model = glm::scale(model, glm::vec3(0.04f, 0.08f, 0.08f));

		glVertexAttrib3f(1, 0.05f, 0.03f, 0.03f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);



		// FOSA NASAL 2
		
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(-2.49f, 0.28f, -0.25f));

		model = glm::scale(model, glm::vec3(0.04f, 0.08f, 0.08f));

		glVertexAttrib3f(1, 0.05f, 0.03f, 0.03f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		
		
		// MANCHA DE LA CARA LADO IZQUIERDO
		
		model = glm::mat4(1.0f);
		
		model = glm::translate(model, glm::vec3(-1.92f, 0.88f, 0.52f));
		
		model = glm::scale(model, glm::vec3(0.18f, 0.32f, 0.28f));
		
		glVertexAttrib3f(1, 0.72f, 0.72f, 0.72f);
		
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		
		glDrawArrays(GL_TRIANGLES, 0, 36);


		
		// MANCHA DE LA CARA LADO DERECHO
		
		model = glm::mat4(1.0f);
		
		model = glm::translate(model, glm::vec3(-1.92f, 0.88f, -0.52f));
		
		model = glm::scale(model, glm::vec3(0.18f, 0.32f, 0.28f));
		
		glVertexAttrib3f(1, 0.72f, 0.72f, 0.72f);
		
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		

		
		// MANCHA SUPERIOR DE LA CABEZA

		model = glm::mat4(1.0f);
		
		model = glm::translate(model, glm::vec3(-1.55f, 1.10f, 0.0f));
		
		model = glm::scale(model, glm::vec3(0.45f, 0.04f, 0.60f));
		
		glVertexAttrib3f(1, 0.75f, 0.75f, 0.75f);
		
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		
		glDrawArrays(GL_TRIANGLES, 0, 36);
		


		// MANCHA CARA IZQUIERDA GRANDE

		model = glm::mat4(1.0f);
		
		model = glm::translate(model, glm::vec3(-1.88f, 0.88f, 0.53f));
		
		model = glm::scale(model, glm::vec3(0.20f, 0.34f, 0.24f));
		
		glVertexAttrib3f(1, 0.82f, 0.82f, 0.82f);
		
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		
		glDrawArrays(GL_TRIANGLES, 0, 36);


		
		// MANCHA CARA DERECHA GRANDE

		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(-1.88f, 0.88f, -0.53f));

		model = glm::scale(model, glm::vec3(0.20f, 0.34f, 0.24f));

		glVertexAttrib3f(1, 0.82f, 0.82f, 0.82f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);


		
		// MANCHA SUPERIOR DE LA CABEZA

		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(-1.55f, 1.10f, 0.0f));

		model = glm::scale(model, glm::vec3(0.55f, 0.04f, 0.70f));

		glVertexAttrib3f(1, 0.85f, 0.85f, 0.85f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		//======================================================
// MANCHA SUPERIOR 1
//======================================================
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.55f, 1.22f, 0.10f));
		model = glm::scale(model, glm::vec3(0.90f, 0.04f, 0.75f));
		glVertexAttrib3f(1, 0.84f, 0.84f, 0.84f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//======================================================
// MANCHA SUPERIOR 2
//======================================================
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.35f, 1.22f, -0.05f));
		model = glm::scale(model, glm::vec3(0.95f, 0.04f, 0.65f));
		glVertexAttrib3f(1, 0.78f, 0.78f, 0.78f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//======================================================
// MANCHA SUPERIOR 3
//======================================================
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.15f, 1.22f, 0.28f));
		model = glm::scale(model, glm::vec3(0.70f, 0.04f, 0.50f));
		glVertexAttrib3f(1, 0.82f, 0.82f, 0.82f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//======================================================
// MANCHA COSTADO DERECHO EXTRA 1
//======================================================
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.55f, 0.52f, 0.71f));
		model = glm::scale(model, glm::vec3(0.35f, 0.25f, 0.04f));
		glVertexAttrib3f(1, 0.72f, 0.72f, 0.72f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		//======================================================
// MANCHA COSTADO DERECHO EXTRA 2
//======================================================
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.15f, 0.20f, 0.71f));
		model = glm::scale(model, glm::vec3(0.30f, 0.22f, 0.04f));
		glVertexAttrib3f(1, 0.70f, 0.70f, 0.70f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		//======================================================
// MANCHA COSTADO IZQUIERDO EXTRA 1
//======================================================
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.95f, 0.38f, -0.71f));
		model = glm::scale(model, glm::vec3(0.42f, 0.30f, 0.04f));
		glVertexAttrib3f(1, 0.68f, 0.68f, 0.68f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//======================================================
// MANCHA COSTADO IZQUIERDO EXTRA 2
//======================================================
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.25f, 0.58f, -0.71f));
		model = glm::scale(model, glm::vec3(0.48f, 0.28f, 0.04f));
		glVertexAttrib3f(1, 0.76f, 0.76f, 0.76f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		//======================================================
// MANCHA DERECHA 6 - PARTE TRASERA SUPERIOR
//======================================================
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.45f, 0.72f, 0.71f));
		model = glm::scale(model, glm::vec3(0.38f, 0.25f, 0.04f));
		glVertexAttrib3f(1, 0.68f, 0.68f, 0.68f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//======================================================
		// MANCHA DERECHA 7 - CONTINUACION TRASERA
		//======================================================
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.57f, 0.50f, 0.71f));
		model = glm::scale(model, glm::vec3(0.24f, 0.22f, 0.04f));
		glVertexAttrib3f(1, 0.68f, 0.68f, 0.68f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		//======================================================
// MANCHA DERECHA 8 - SUPERIOR DELANTERA
//======================================================
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.75f, 0.82f, 0.71f));
		model = glm::scale(model, glm::vec3(0.42f, 0.25f, 0.04f));
		glVertexAttrib3f(1, 0.76f, 0.76f, 0.76f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//======================================================
		// MANCHA DERECHA 9 - CONTINUACION INFERIOR
		//======================================================
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.60f, 0.62f, 0.71f));
		model = glm::scale(model, glm::vec3(0.25f, 0.22f, 0.04f));
		glVertexAttrib3f(1, 0.76f, 0.76f, 0.76f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//======================================================
// MANCHA IZQUIERDA 5 - ZONA CENTRAL SUPERIOR
//======================================================
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.20f, 0.78f, -0.71f));
		model = glm::scale(model, glm::vec3(0.55f, 0.28f, 0.04f));
		glVertexAttrib3f(1, 0.72f, 0.72f, 0.72f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//======================================================
		// MANCHA IZQUIERDA 6 - EXTENSION CENTRAL
		//======================================================
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.43f, 0.57f, -0.71f));
		model = glm::scale(model, glm::vec3(0.32f, 0.22f, 0.04f));
		glVertexAttrib3f(1, 0.72f, 0.72f, 0.72f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//======================================================
		// MANCHA IZQUIERDA 7 - EXTENSION LATERAL
		//======================================================
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.08f, 0.60f, -0.71f));
		model = glm::scale(model, glm::vec3(0.25f, 0.20f, 0.04f));
		glVertexAttrib3f(1, 0.72f, 0.72f, 0.72f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//======================================================
		// MANCHA IZQUIERDA 8 - TRASERA SUPERIOR
		//======================================================
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.42f, 0.82f, -0.71f));
		model = glm::scale(model, glm::vec3(0.35f, 0.25f, 0.04f));
		glVertexAttrib3f(1, 0.64f, 0.64f, 0.64f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//======================================================
		// MANCHA IZQUIERDA 9 - CONTINUACION TRASERA
		//======================================================
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.55f, 0.62f, -0.71f));
		model = glm::scale(model, glm::vec3(0.22f, 0.20f, 0.04f));
		glVertexAttrib3f(1, 0.64f, 0.64f, 0.64f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// MANCHA LATERAL 1 Der
		
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.10f, 0.65f, 0.71f));

		model = glm::scale(model, glm::vec3(0.70f, 0.50f, 0.04f));

		glVertexAttrib3f(1, 0.55f, 0.55f, 0.55f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);


		
		// MANCHA LATERAL 2 Der
		
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(-0.70f, 0.18f, 0.71f));

		model = glm::scale(model, glm::vec3(0.50f, 0.36f, 0.04f));

		glVertexAttrib3f(1, 0.55f, 0.55f, 0.55f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);



		// MANCHA LATERAL 3 Der

		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(1.0f, 0.20f, 0.71f));

		model = glm::scale(model, glm::vec3(0.55f, 0.38f, 0.04f));

		glVertexAttrib3f(1, 0.40f, 0.40f, 0.40f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);



		// MANCHA LATERAL 4 Izq
		
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(-0.7f, 0.20f, -0.7f));

		model = glm::scale(model, glm::vec3(0.55f, 0.38f, 0.04f));

		glVertexAttrib3f(1, 0.40f, 0.40f, 0.40f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);



		// MANCHA LATERAL 5 Izq
		
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.7f, 0.10f, -0.7f));

		model = glm::scale(model, glm::vec3(0.33f, 0.22f, 0.04f));

		glVertexAttrib3f(1, 0.40f, 0.40f, 0.40f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);



		// MANCHA SUPERIOR

		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.15f, 1.22f, 0.10f));

		model = glm::scale(model, glm::vec3(0.90f, 0.04f, 0.60f));

		glVertexAttrib3f(1, 0.65f, 0.65f, 0.65f);


		
		// UBRE
		
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.75f, -0.28f, 0.0f));

		model = glm::scale(model, glm::vec3(0.65f, 0.25f, 0.65f));

		glVertexAttrib3f(1, 0.65f, 0.38f, 0.40f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		// TETILLA 1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.55f, -0.48f, 0.20f));
		model = glm::scale(model, glm::vec3(0.10f, 0.25f, 0.10f));
		glVertexAttrib3f(1, 0.65f, 0.38f, 0.40f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// TETILLA 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.55f, -0.48f, -0.20f));
		model = glm::scale(model, glm::vec3(0.10f, 0.25f, 0.10f));
		glVertexAttrib3f(1, 0.65f, 0.38f, 0.40f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// TETILLA 3
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.95f, -0.48f, 0.20f));
		model = glm::scale(model, glm::vec3(0.10f, 0.25f, 0.10f));
		glVertexAttrib3f(1, 0.65f, 0.38f, 0.40f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// TETILLA 4
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.95f, -0.48f, -0.20f));
		model = glm::scale(model, glm::vec3(0.10f, 0.25f, 0.10f));
		glVertexAttrib3f(1, 0.65f, 0.38f, 0.40f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Cola

		// Parte principal
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(1.88f, 0.18f, 0.0f));

		model = glm::scale(model, glm::vec3(0.18f, 0.70f, 0.18f));

		glVertexAttrib3f(1, 0.22f, 0.13f, 0.07f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);


		// Punta de la cola
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(1.88f, -0.23f, 0.0f));

		model = glm::scale(model, glm::vec3(0.26f, 0.25f, 0.26f));

		glVertexAttrib3f(1, 0.05f, 0.03f, 0.02f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);





		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);
				
		glBindVertexArray(0);
		// Swap the screen buffers
		glfwSwapBuffers(window);
		}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;
 }

 void Inputs(GLFWwindow *window) {
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		 glfwSetWindowShouldClose(window, true);
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		 movX += 0.02f;
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		 movX -= 0.02f;
	 if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		 movY += 0.02f;
	 if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		 movY -= 0.02f;
	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		 movZ -= 0.02f;
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		 movZ += 0.02f;
	 if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		 rot += 0.1f;
	 if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		 rot -= 0.1f;
 }