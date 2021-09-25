/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "constants.h"
#include "Entity.h"
#include "Game.h"
#include "ShaderProgram.h"

std::map<std::string, float> inputCallback{
	{"Ymove",0},
	{"Xmove",0},
	{"Zmove", 0},
	{"Zoom",0},
	{"Xoffset", 0 },
	{"Yoffset",0},
	{"shoot",0},
	{"Select",0}
};

float speed_x=0; //angular speed in radians
float speed_y=0; //angular speed in radians
float aspectRatio=1;
 

Game* game;

float lastX = 500 / 2.0f;
float lastY = 500 / 2.0f;
bool firstMouse = true;
float yaw = -90;
float pitch = 0;

glm::vec4 lightSource = glm::vec4(5.0f,10.0f,5.0f,1.0f);
glm::mat4 P = glm::perspective(120.0f * PI / 180.0f, 1.0f, 0.1f, 200.0f);

//Error processing callback procedure
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void keyCallback(GLFWwindow* window,int key,int scancode,int action,int mods) {
	float walkingSpeed = 4;
    if (action==GLFW_PRESS) {
        if (key==GLFW_KEY_LEFT) speed_x=-PI/2;
        if (key==GLFW_KEY_RIGHT) speed_x=PI/2;
        if (key==GLFW_KEY_UP) speed_y=PI/2;
        if (key==GLFW_KEY_DOWN) speed_y=-PI/2;
		if (key == GLFW_KEY_W) inputCallback["Zmove"] = walkingSpeed;
		if (key == GLFW_KEY_A) inputCallback["Xmove"] = -walkingSpeed;
		if (key == GLFW_KEY_S) inputCallback["Zmove"] = -walkingSpeed;
		if (key == GLFW_KEY_D) inputCallback["Xmove"] = walkingSpeed;
		if (key == GLFW_KEY_LEFT_SHIFT) inputCallback["Ymove"] = -walkingSpeed;
		if (key == GLFW_KEY_SPACE) inputCallback["Ymove"] = walkingSpeed;
		if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window,1);
		if (key == GLFW_KEY_Q) inputCallback["Select"] = 1;
		if (key == GLFW_KEY_E) inputCallback["Select"] = -1;
    }
    if (action==GLFW_RELEASE) {
        if (key==GLFW_KEY_LEFT) speed_x=0;
        if (key==GLFW_KEY_RIGHT) speed_x=0;
        if (key==GLFW_KEY_UP) speed_y=0;
        if (key==GLFW_KEY_DOWN) speed_y=0;
		if (key == GLFW_KEY_W) inputCallback["Zmove"] = 0;
		if (key == GLFW_KEY_A) inputCallback["Xmove"] = 0;
		if (key == GLFW_KEY_S) inputCallback["Zmove"] = 0;
		if (key == GLFW_KEY_D) inputCallback["Xmove"] = 0;
		if (key == GLFW_KEY_LEFT_SHIFT) inputCallback["Ymove"] = 0;
		if (key == GLFW_KEY_SPACE) inputCallback["Ymove"] = 0;
		if (key == GLFW_KEY_Q) inputCallback["Select"] = 0;
		if (key == GLFW_KEY_E) inputCallback["Select"] = 0;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.002f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	if (xoffset < 0.01f&& xoffset > -0.01f) xoffset = 0;
	if (yoffset < 0.01f && yoffset > -0.01f) yoffset = 0;

	inputCallback["Xoffset"] = xoffset;
	inputCallback["Yoffset"] = yoffset;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		inputCallback["shoot"] = 1;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		inputCallback["shoot"] = 0;
}

void windowResizeCallback(GLFWwindow* window,int width,int height) {
    if (height==0) return;
    aspectRatio=(float)width/(float)height;
    glViewport(0,0,width,height);
}


//Initialization code procedure
void initOpenGLProgram(GLFWwindow* window) {
	//************Place any code here that needs to be executed once, at the program start************
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window,windowResizeCallback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window,mouse_button_callback);
	glfwSetKeyCallback(window,keyCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	game = new Game(&inputCallback);
	//spDebug = new ShaderProgram("glsl/v_debug.glsl",NULL,"glsl/f_debug.glsl");	
	//"glsl/g_simplest.glsl"
}

//Release resources allocated by the program
void freeOpenGLProgram(GLFWwindow* window) {
	//************Place any code here that needs to be executed once, after the main loop ends************
	delete game;
}

//Drawing procedure
void drawScene(GLFWwindow* window) {
	//************Place any code here that draws something inside the window******************

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto itr : game->world) {
		auto sp = itr->sp;
		sp->use();//activate shading program
	
		//Send parameters to graphics card
		glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
		glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(game->V));

		glUniform4fv(sp->u("lightSource"), 1, glm::value_ptr(lightSource));
		glUniform4fv(sp->u("camPos"), 1, glm::value_ptr(glm::vec4(game->cameraPos, 1)));

		itr->draw();
	}

	//Drawing Hud
	glDisable(GL_DEPTH_TEST);
	game->drawHUD();
	glEnable(GL_DEPTH_TEST);

    glfwSwapBuffers(window); //Copy back buffer to front buffer
}

int main(void)
{
	GLFWwindow* window; //Pointer to object that represents the application window

	glfwSetErrorCallback(error_callback);//Register error processing callback procedure

	if (!glfwInit()) { //Initialize GLFW library
		fprintf(stderr, "Can't initialize GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1000, 1000, "Snakes", NULL, NULL);  //Create a window 500pxx500px titled "OpenGL" and an OpenGL context associated with it.

	if (!window) //If no window is opened then close the program
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Since this moment OpenGL context corresponding to the window is active and all OpenGL calls will refer to this context.
	glfwSwapInterval(1); //During vsync wait for the first refresh

	GLenum err;
	if ((err=glewInit()) != GLEW_OK) { //Initialize GLEW library
		fprintf(stderr, "Can't initialize GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Call initialization procedure

	float dT;
	glfwSetTime(0); //Zero the timer
	//Main application loop
	while (!glfwWindowShouldClose(window)) //As long as the window shouldnt be closed yet...
	{
		dT = glfwGetTime();
        glfwSetTime(0); //Zero the timer
		game->proccesFrame(dT);
		drawScene(window); //Execute drawing procedure
		if (game->end)
			glfwSetWindowShouldClose(window, 1);
		glfwPollEvents(); //Process callback procedures corresponding to the events that took place up to now
	}
	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Delete OpenGL context and the window.
	glfwTerminate(); //Free GLFW resources
	exit(EXIT_SUCCESS);
}
