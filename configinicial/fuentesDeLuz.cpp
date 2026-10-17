// Práctica 9. Fuentes de Luz 
// Autor: Villanueva Figueroa Daniel Kaleb
// Número de cuenta: 320173985
// Fecha: 15/10/2026

#include <iostream>
#include <cmath>
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera camera(glm::vec3(0.0f, 1.5f, 9.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
// Una bandera por fuente: direccional, amarilla, azul, verde, reflector.
bool lucesEncendidas[5] = { true, true, true, true, true };


// ======================================
// PRACTICA 9 - POSICIONES DE LAS LUCES
// ======================================

glm::vec3 pointLightPositions[] = {
	glm::vec3(-2.8f, 1.8f, 0.5f),  // amarilla - casa
	glm::vec3(2.8f, 1.8f, 0.5f),  // azul - banca
	glm::vec3(0.0f, 2.2f, -2.5f), // verde - arboles
	glm::vec3(0.0f, 0.0f, 0.0f)   // desactivada
};


float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



// Controles de las luces: teclas 1-5 y 0 para restaurar todas.


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Práctica 9. Fuentes de luz. Daniel Villanueva", nullptr, nullptr);

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
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	std::cout << "Controles: 1 direccional | 2 amarilla | 3 azul | 4 verde | 5 reflector | 0 todas ON\n";



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	// ======================================
	// PRACTICA 9 - CARGA DE MODELOS
	// ======================================

		// Perrito
	Model dog((char*)"Models/RedDog.obj");
	// Arboles
	Model tree((char*)"Models/tree1.obj");
	// Piso
	Model ground((char*)"Models/desert ground-obj.obj");
	// Banca
	Model bench((char*)"Models/Bench/PublicBench.obj");
	// Casa del perro
	Model dogHouse((char*)"Models/DogHouse/niche.obj");
	// Esfera para futuras fuentes de luz
	Model Ball((char*)"Models/ball.obj");




	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);



		//Load Model


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);



		// ======================================
		// PRACTICA 9 - CINCO FUENTES DE LUZ
		// ======================================

			// Funciones auxiliares para uniformes
		auto setVec3 = [&](const std::string& name, const glm::vec3& value)
			{
				glUniform3fv(glGetUniformLocation(lightingShader.Program, name.c_str()), 1, glm::value_ptr(value));
			};

		auto setFloat = [&](const std::string& name, float value)
			{
				glUniform1f(glGetUniformLocation(lightingShader.Program, name.c_str()), value);
			};

		// ======================================
		// 1. LUZ DIRECCIONAL - GENERAL
		// ======================================

		setVec3("dirLight.direction", glm::vec3(-0.3f, -1.0f, -0.4f));
		float intensidadDir = lucesEncendidas[0] ? 1.0f : 0.0f;
		setVec3("dirLight.ambient", glm::vec3(0.12f) * intensidadDir);
		setVec3("dirLight.diffuse", glm::vec3(0.22f) * intensidadDir);
		setVec3("dirLight.specular", glm::vec3(0.20f) * intensidadDir);


		// ======================================
		// 2, 3 Y 4. LUCES PUNTUALES
		// ======================================

		glm::vec3 pointColors[4] = {

			// Amarillo calido
			glm::vec3(1.0f, 0.75f, 0.30f),

			// Azul
			glm::vec3(0.25f, 0.50f, 1.0f),

			// Verde
			glm::vec3(0.25f, 1.0f, 0.45f),

			// Desactivada
			glm::vec3(0.0f)
		};

		for (int i = 0; i < 4; i++)
		{
			std::string name = "pointLights[" + std::to_string(i) + "].";

			setVec3(name + "position", pointLightPositions[i]);

			// El cuarto elemento (i=3) se mantiene desactivado.
			float intensidadPuntual = (i < 3 && lucesEncendidas[i + 1]) ? 1.0f : 0.0f;
			glm::vec3 colorActivo = pointColors[i] * intensidadPuntual;
			setVec3(name + "ambient", colorActivo * 0.02f);
			setVec3(name + "diffuse", colorActivo);
			setVec3(name + "specular", colorActivo * 0.7f);

			setFloat(name + "constant", 1.0f);
			setFloat(name + "linear", 0.14f);
			setFloat(name + "quadratic", 0.07f);
		}


		// ======================================
		// 5. SPOTLIGHT - PERRITO
		// ======================================

		// Posicion fija del reflector
		glm::vec3 spotPos(0.0f, 3.0f, 2.5f);

		// Apunta hacia el perrito
		glm::vec3 spotDirection = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - spotPos);
		setVec3("spotLight.position", spotPos);
		setVec3("spotLight.direction", spotDirection);

		// Color blanco calido
		float intensidadSpot = lucesEncendidas[4] ? 1.0f : 0.0f;
		setVec3("spotLight.ambient", glm::vec3(0.01f) * intensidadSpot);
		setVec3("spotLight.diffuse", glm::vec3(1.0f, 0.92f, 0.75f) * intensidadSpot);
		setVec3("spotLight.specular", glm::vec3(1.0f) * intensidadSpot);
		setFloat("spotLight.constant", 1.0f);
		setFloat("spotLight.linear", 0.09f);
		setFloat("spotLight.quadratic", 0.032f);

		// Apertura del reflector
		setFloat("spotLight.cutOff", glm::cos(glm::radians(16.0f)));
		setFloat("spotLight.outerCutOff", glm::cos(glm::radians(23.0f)));



		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");


		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));



		// ======================================
		// PRACTICA 9 - ESCENARIO DEL PERRITO
		// ======================================

		glm::mat4 model(1.0f);
		// Configuracion para modelos opacos
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "objectAlpha"), 1.0f);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		// ======================================
		// 1. PERRITO
		// ======================================

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		dog.Draw(lightingShader);


		// ======================================
		// 2. ARBOLES
		// ======================================

		// Posiciones originales de la Practica 6
		glm::vec3 treePositions[] = {
			glm::vec3(-2.8f, -0.40f, -3.2f),
			glm::vec3(-0.8f, -0.40f, -3.6f),
			glm::vec3(1.2f, -0.40f, -3.4f),
			glm::vec3(3.0f, -0.40f, -3.6f),
			glm::vec3(-3.5f, -0.40f, -0.5f),
			glm::vec3(-4.6f, -0.40f, -1.8f),
			glm::vec3(3.5f, -0.40f, -0.5f),
			glm::vec3(4.6f, -0.40f, -1.8f)
		};

		float treeRotations[] = {
			15.0f, -20.0f, 30.0f, -35.0f,
			90.0f, 90.0f, -90.0f, -90.0f
		};

		float treeScales[] = {
			0.16f, 0.16f, 0.13f, 0.13f,
			0.13f, 0.13f, 0.13f, 0.13f
		};

		for (int i = 0; i < 8; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, treePositions[i]);
			model = glm::rotate(model, glm::radians(treeRotations[i]), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(treeScales[i]));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			tree.Draw(lightingShader);
		}


		// ======================================
		// 3. PISO
		// ======================================

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.55f, 0.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.10f, 0.08f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ground.Draw(lightingShader);


		// ======================================
		// 4. BANCA
		// ======================================

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.5f, -0.40f, -0.5f));
		model = glm::scale(model, glm::vec3(0.75f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		bench.Draw(lightingShader);


		// ======================================
		// 5. CASA DEL PERRO (TRANSPARENTE)
		// ======================================

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		glUniform1f(glGetUniformLocation(lightingShader.Program, "objectAlpha"), 0.45f);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.0f, -0.40f, -0.8f));
		model = glm::scale(model, glm::vec3(0.35f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		dogHouse.Draw(lightingShader);


		// ======================================
		// RESTAURAR ESTADO DE OPENGL
		// ======================================

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "objectAlpha"), 1.0f);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		glBindVertexArray(0);



		// ======================================
		// CHECKPOINT 5 - FAROLAS DEL PARQUE
		// ======================================

		// Restaurar estado
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);

		// Usar shader de cubos
		lampShader.Use();

		// Matriz de vista
		glUniformMatrix4fv(
			glGetUniformLocation(lampShader.Program, "view"),
			1, GL_FALSE, glm::value_ptr(view)
		);

		// Matriz de proyeccion
		glUniformMatrix4fv(
			glGetUniformLocation(lampShader.Program, "projection"),
			1, GL_FALSE, glm::value_ptr(projection)
		);

		// Colores de las luces puntuales
		glm::vec3 lampColors[3] = {
			glm::vec3(1.0f, 0.75f, 0.30f), // amarilla
			glm::vec3(0.25f, 0.50f, 1.0f), // azul
			glm::vec3(0.25f, 1.0f, 0.45f)  // verde
		};

		// ======================================
		// 1, 2 y 3. FAROLAS (POSTE + CUBO LUMINOSO)
		// ======================================

		for (int i = 0; i < 3; i++)
		{
			// Poste metalico desde el suelo hasta la bombilla.
			const float groundY = -0.45f;
			const float topY = pointLightPositions[i].y - 0.10f;
			const float poleHeight = topY - groundY;
			const float poleCenterY = (groundY + topY) * 0.5f;

			glm::mat4 poleModel(1.0f);
			poleModel = glm::translate(poleModel, glm::vec3(
				pointLightPositions[i].x, poleCenterY, pointLightPositions[i].z
			));
			poleModel = glm::scale(poleModel, glm::vec3(0.09f, poleHeight, 0.09f));

			glUniformMatrix4fv(
				glGetUniformLocation(lampShader.Program, "model"),
				1, GL_FALSE, glm::value_ptr(poleModel)
			);
			glUniform3f(
				glGetUniformLocation(lampShader.Program, "lampColor"),
				0.30f, 0.32f, 0.35f
			);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);

			// Cubo de color que representa la fuente puntual.
			glm::mat4 cubeModel(1.0f);

			cubeModel = glm::translate(
				cubeModel,
				pointLightPositions[i]
			);

			cubeModel = glm::scale(
				cubeModel,
				glm::vec3(0.25f)
			);

			glUniformMatrix4fv(
				glGetUniformLocation(lampShader.Program, "model"),
				1, GL_FALSE, glm::value_ptr(cubeModel)
			);

			// La bombilla se atenúa si su luz está apagada.
			glm::vec3 colorBombilla = lampColors[i] *
				(lucesEncendidas[i + 1] ? 1.0f : 0.08f);
			glUniform3fv(
				glGetUniformLocation(lampShader.Program, "lampColor"),
				1, glm::value_ptr(colorBombilla)
			);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}

		// ======================================
		// 4. CUBO DEL SPOTLIGHT
		// ======================================

		// La representacion coincide con la posicion real del reflector.

		glm::mat4 spotCubeModel(1.0f);

		spotCubeModel = glm::translate(
			spotCubeModel,
			spotPos
		);

		spotCubeModel = glm::scale(
			spotCubeModel,
			glm::vec3(0.28f)
		);

		glUniformMatrix4fv(
			glGetUniformLocation(lampShader.Program, "model"),
			1, GL_FALSE, glm::value_ptr(spotCubeModel)
		);

		// Blanco cálido
		glm::vec3 colorReflector = glm::vec3(1.0f, 0.92f, 0.75f) *
			(lucesEncendidas[4] ? 1.0f : 0.08f);
		glUniform3fv(
			glGetUniformLocation(lampShader.Program, "lampColor"),
			1, glm::value_ptr(colorReflector)
		);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// ======================================
		// CHECKPOINT 6 - SOPORTE DEL REFLECTOR
		// ======================================
		// Cubos alargados: poste vertical y brazo horizontal.
		const float posteX = spotPos.x + 1.25f;
		const float posteZ = spotPos.z;
		const float sueloY = -0.45f;
		const float alturaPoste = spotPos.y - sueloY;

		glm::mat4 posteReflector(1.0f);
		posteReflector = glm::translate(posteReflector,
			glm::vec3(posteX, (sueloY + spotPos.y) * 0.5f, posteZ));
		posteReflector = glm::scale(posteReflector,
			glm::vec3(0.12f, alturaPoste, 0.12f));

		glUniform3f(glGetUniformLocation(lampShader.Program, "lampColor"),
			0.30f, 0.32f, 0.35f);
		glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"),
			1, GL_FALSE, glm::value_ptr(posteReflector));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glm::mat4 brazoReflector(1.0f);
		brazoReflector = glm::translate(brazoReflector,
			glm::vec3((posteX + spotPos.x) * 0.5f, spotPos.y, spotPos.z));
		brazoReflector = glm::scale(brazoReflector,
			glm::vec3(1.25f, 0.12f, 0.12f));
		glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"),
			1, GL_FALSE, glm::value_ptr(brazoReflector));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
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

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.001f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.001f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.001f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.001f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.001f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.001f;
	}

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS) keys[key] = true;
		else if (action == GLFW_RELEASE) keys[key] = false;
	}

	// Cambiar SOLO una vez por pulsación, no en cada frame.
	if (action == GLFW_PRESS)
	{
		if (key >= GLFW_KEY_1 && key <= GLFW_KEY_5)
		{
			int indice = key - GLFW_KEY_1;
			lucesEncendidas[indice] = !lucesEncendidas[indice];
			std::cout << "Luz " << (indice + 1) << ": "
				<< (lucesEncendidas[indice] ? "ENCENDIDA" : "APAGADA") << std::endl;
		}
		else if (key == GLFW_KEY_0)
		{
			for (int i = 0; i < 5; ++i) lucesEncendidas[i] = true;
			std::cout << "TODAS LAS LUCES ENCENDIDAS" << std::endl;
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