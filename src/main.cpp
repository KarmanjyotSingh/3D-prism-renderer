#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "camera.h"
#include "shader.h"
using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

bool displaced = false;
// settings
// Set the window parameters
const unsigned int SCR_WIDTH = 800;     // width
const unsigned int SCR_HEIGHT = 800;    // height
const char *SCR_TITLE = "Assignment0";  // window title

bool rotation;    // boolean variable for toggling the rotation
bool turn_table;  // boolean variable for toggling the turn table feature

#define EDGE_LENGTH 0.4f  // edge length of the polygon

// camera

Camera camera(glm::vec3(1.0f, 1.0f, 4.0f));

// camera position
glm ::vec3 camera_position = glm ::vec3(0.0f, 0.0f, 3.0f);
// prism center
glm ::vec3 camera_target = glm ::vec3(0.0f, 0.0f, 0.0f);
// the up camera vector
glm ::vec3 camera_up = glm ::vec3(0.0f, 1.0f, 0.0f);
// the front camera  vector ( direction camera is pointing to ( or facing ) )
glm ::vec3 camera_front = camera_target - camera_position;
// the right camera vector ( the +ve x axis of the camera frame )
glm ::vec3 camera_right = glm ::vec3(1.0f, 0.0f, 0.0f);

glm ::vec4 prism_center = glm ::vec4(camera_target, 1.0f);
// helper functions

// for initialising the screen

void initialise_screen() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

// for creating the opengl window

GLFWwindow *create_window() {
	GLFWwindow *window =
		glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, SCR_TITLE, NULL, NULL);
	if (window == NULL) {
		std ::cout << "Failed to create GLFW window" << std ::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std ::cout << "Failed to initialize GLAD" << std ::endl;
		return NULL;
	}
	return window;
}

// for drawing the polygon

void make_polygon(int n, vector<float> &vertex, float z_coord) {
	// to make a regular polygon , each side subtends an angle of 360/n at the
	// center of the circle
	float alpha = (360.0f) / ((float)n);

	for (auto i = 0; i < n; i++) {
		float beta = glm ::radians(alpha * (float)i);
		float gamma = glm ::radians(alpha * (float)(i + 1));
		float cos1 = glm ::cos(beta);
		float sin1 = glm ::sin(beta);
		float sin12 = glm ::sin(gamma / 2.0f);

		// using basic coordinate geometry to specify the coordinates of the
		// points

		// Vertex 1 for each front triangle

		vertex.push_back(0);
		vertex.push_back(0);
		vertex.push_back(z_coord);
		vertex.push_back(1.0f);
		vertex.push_back(1.0f);
		vertex.push_back(0.0f);

		// Vertex 2 for the front triangle

		vertex.push_back(EDGE_LENGTH * (glm::cos(beta)));
		vertex.push_back(EDGE_LENGTH * (glm::sin(beta)));
		vertex.push_back(z_coord);
		vertex.push_back(1.0f);
		vertex.push_back(1.0f);
		vertex.push_back(0.0f);

		// vertex 3 for the front triangle

		vertex.push_back(EDGE_LENGTH * (glm::cos(gamma)));
		vertex.push_back(EDGE_LENGTH * (glm::sin(gamma)));
		vertex.push_back(z_coord);
		vertex.push_back(1.0f);
		vertex.push_back(1.0f);
		vertex.push_back(0.0f);

		// vertex 1 for the back triangle

		vertex.push_back(0);
		vertex.push_back(0);
		vertex.push_back(z_coord - 0.5f);
		vertex.push_back(1.0f);
		vertex.push_back(1.0f);
		vertex.push_back(0.0f);

		// Vertex 2 for the back triangle

		vertex.push_back(EDGE_LENGTH * (glm::cos(beta)));
		vertex.push_back(EDGE_LENGTH * (glm::sin(beta)));
		vertex.push_back(z_coord - 0.5f);
		vertex.push_back(1.0f);
		vertex.push_back(1.0f);
		vertex.push_back(0.0f);

		// vertex 3 for the back triangle

		vertex.push_back(EDGE_LENGTH * (glm::cos(gamma)));
		vertex.push_back(EDGE_LENGTH * (glm::sin(gamma)));
		vertex.push_back(z_coord - 0.5f);
		vertex.push_back(1.0f);
		vertex.push_back(1.0f);
		vertex.push_back(0.0f);

		// face

		// vertex 1

		vertex.push_back(EDGE_LENGTH * (glm::cos(beta)));
		vertex.push_back(EDGE_LENGTH * (glm::sin(beta)));
		vertex.push_back(z_coord);
		vertex.push_back(cos1);
		vertex.push_back(sin1);
		vertex.push_back(sin12);

		// vertex 2

		vertex.push_back(EDGE_LENGTH * (glm::cos(gamma)));
		vertex.push_back(EDGE_LENGTH * (glm::sin(gamma)));
		vertex.push_back(z_coord);
		vertex.push_back(cos1);
		vertex.push_back(sin1);
		vertex.push_back(sin12);

		// vertex 3

		vertex.push_back(EDGE_LENGTH * (glm::cos(beta)));
		vertex.push_back(EDGE_LENGTH * (glm::sin(beta)));
		vertex.push_back(z_coord - 0.5f);
		vertex.push_back(cos1);
		vertex.push_back(sin1);
		vertex.push_back(sin12);

		// upper face vertex 1
		vertex.push_back(EDGE_LENGTH * (glm::cos(beta)));
		vertex.push_back(EDGE_LENGTH * (glm::sin(beta)));
		vertex.push_back(z_coord - 0.5f);
		vertex.push_back(cos1);
		vertex.push_back(sin1);
		vertex.push_back(sin12);

		// upper face vertex 2

		vertex.push_back(EDGE_LENGTH * (glm::cos(gamma)));
		vertex.push_back(EDGE_LENGTH * (glm::sin(gamma)));
		vertex.push_back(z_coord - 0.5f);
		vertex.push_back(cos1);
		vertex.push_back(sin1);
		vertex.push_back(sin12);

		// lower face vertex 3

		vertex.push_back(EDGE_LENGTH * (glm::cos(gamma)));
		vertex.push_back(EDGE_LENGTH * (glm::sin(gamma)));
		vertex.push_back(z_coord);
		vertex.push_back(cos1);
		vertex.push_back(sin1);
		vertex.push_back(sin12);
	}
}

// for toggle key input

void toggle_key(GLFWwindow *window, int key, int scancode, int action,
				int mods) {
	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		// rotate -> stop rotation
		rotation = !rotation;
		//
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		turn_table = !turn_table;
	}
}

glm::mat4 transform;  // identity matrix
void reset();

int main(int argc, char *argv[]) {
	int n = 0;

	switch (argc) {
		case 2:
			n = atoi(argv[1]);
			break;
		default:
			cout << "Incorrect Number of Arguments !" << endl;
			return -1;
			break;
	}

	initialise_screen();

	auto window = create_window();

	if (window == NULL)
		return -1;

	// the main vertex arrays , storing all the vertexes of the prism
	vector<float> vertex;

	// The OpenGL vectors
	unsigned int VBO, VAO, EBO;

	make_polygon(n, vertex, 0.25f);

	// build and compile our shader program
	Shader ourShader("../src/vertex.shader", "../src/fragment.shader");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s),
	// and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex.size(), vertex.data(),
				 GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
						  (void *)0);

	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
						  (void *)(3 * sizeof(float)));

	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered
	// VBO as the vertex attribute's bound vertex buffer object so afterwards we
	// can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally
	// modify this VAO, but this rarely happens. Modifying other VAOs requires a
	// call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
	// VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// enable depth testing for 3d
	glEnable(GL_DEPTH_TEST);
	// render loop
	// initialise the transform matrix
	transform = glm::mat4(1.0f);
	rotation = false;
	turn_table = false;

	while (!glfwWindowShouldClose(window)) {
		// print stuff for better understanding
		cout << "Prism Center : " << prism_center.x << " " << prism_center.y
			 << " " << prism_center.z << endl;

		printf("Camera Position: %0.3f %0.3f %0.3f\n", camera_position.x,
			   camera_position.y, camera_position.z);
		// camera_target =
		// 	glm ::vec3(prism_center.x, prism_center.y, prism_center.z);
		printf("Camera Target: %0.3f %0.3f %0.3f\n", camera_target.x,
			   camera_target.y, camera_target.z);
		// cout << vertex[0] << endl;

		// input
		processInput(window);
		// For Toggling effect
		glfwSetKeyCallback(window, toggle_key);
		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// make transformations
		//	transform = glm::mat4(1.0f);

		ourShader.use();

		// part B : 4 : Prism , let's go for a spin

		if (rotation) {
			//reset();
			transform =
				glm::rotate(transform, 0.05f, glm::vec3(0.0f, 0.0f, 1.0f));
			// rotation about the x-axis
		}

		ourShader.setMat4("transform", transform);

		// view matrix

		glm::mat4 view =
			glm ::lookAt(camera_position, camera_target, camera_up);

		// part B : 5 : Oh! How the turntables?

		if (turn_table) {
			reset();
			auto radius_vector =
				camera_position -
				glm::vec3(prism_center.x, prism_center.y, prism_center.z);
			float radius = glm ::dot(radius_vector, radius_vector);
			radius = sqrt(radius);

			float camX = prism_center.x + cos(0.5f * glfwGetTime()) * radius;
			float camZ = prism_center.z + sin(0.5f * glfwGetTime()) * radius;

			camera_position.x = camX;
			camera_position.y = prism_center.y;
			camera_position.z = camZ;
			// glm::mat4 view;
			// view =
			// 	glm::lookAt(glm::vec3(camX, 0.0, camZ),
			// 				glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		}

		ourShader.setMat4("view", view);

		// projection matrix
		glm::mat4 projection = glm::perspective(
			glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT,
			0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		glBindVertexArray(VAO);  // seeing as we only have a single VAO there's
		// no need to bind it every time, but we'll do
		// so to keep things a bit more organized
		// draw the triangles
		glDrawArrays(GL_TRIANGLES, 0, n * 3 * 2 * 2);

		// glBindVertexArray(0); // no need to unbind it every time

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse
		// moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// used for continuous input
void reset() {
	if (displaced) {
		//	displaced = false;
		transform = glm ::mat4(1.0f);
		transform = glm ::translate(
			transform,
			-1.0f * glm ::vec3(prism_center.x, prism_center.y, prism_center.z));

		prism_center = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		// auto temp = transform * prism_center;
		// prism_center = temp;
		cout << "Prism Center " << prism_center.x << " " << prism_center.y
			 << " " << prism_center.z << endl;

		camera_front = glm ::vec3(0.0f, -1.0f, 0.0f);
		// camera_position = glm::vec3(0.0f, 0.0f, 3.0f);
		// camera_target = glm ::vec3(0.0f, 0.0f, 0.0f);
		camera_target = glm ::vec3(prism_center.x - camera_position.x,
								   prism_center.y - camera_position.y,
								   prism_center.z - camera_position.z);
		camera_up = glm ::vec3(0.0f, 1.0f, 0.0f);
		camera_front = camera_target - camera_position;
		camera_right = glm ::vec3(1.0f, 0.0f, 0.0f);
	}
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	float camera_speed = 0.01f;
	//	transform = glm ::mat4(1.0f);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		// reset();
		camera_position += camera_speed * camera_front;
		reset();
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera_position -= camera_speed * camera_front;
		reset();
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera_position -=
			glm ::normalize(glm ::cross(camera_front, camera_up)) *
			camera_speed;
		reset();
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera_position +=
			glm ::normalize(glm ::cross(camera_front, camera_up)) *
			camera_speed;
		reset();
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera_position += camera_up * camera_speed;
		reset();
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		camera_position -= camera_up * camera_speed;
		reset();
	}

	// part 2 : Object Translation
	// Do the matrix Translation  ( Transformation )
	// ( down )
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		displaced = true;
		// cout << "Prism Center Original 2: " << prism_center.x << " "
		// 	 << prism_center.y << " " << prism_center.z << endl;
		//	camera_up = glm ::normalize(glm ::cross(camera_up, camera_front));
		transform = glm ::translate(transform, -0.05f * camera_up);
		prism_center = transform * prism_center;
	}
	// ( left )
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		displaced = true;
		transform = glm ::translate(
			transform, -0.005f * glm ::cross(camera_up, camera_front));
		prism_center = transform * prism_center;
		// cout << "Prism Center : " << prism_center.x << " " << prism_center.y
		// 	 << " " << prism_center.z << endl;
	}

	// right
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
		displaced = true;
		transform = glm ::translate(
			transform,
			0.05f * glm ::normalize(glm ::cross(camera_up, camera_front)));
		prism_center = transform * prism_center;
		//		cout << "Prism Center : " << prism_center.x << " " <<
		// prism_center.y
		//			 << " " << prism_center.z << endl;
	}
	// up
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
		displaced = true;
		transform = glm ::translate(transform, 0.05f * camera_up);
		prism_center = transform * prism_center;
		//	cout << "Prism Center : " << prism_center.x << " " << prism_center.y
		//		 << " " << prism_center.z << endl;
	}
	// towards
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		displaced = true;
		transform = glm ::translate(transform, 0.005f * camera_front);
		prism_center = transform * prism_center;
		//	cout << "Prism Center : " << prism_center.x << " " << prism_center.y
		//		 << " " << prism_center.z << endl;
	}
	// away
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		displaced = true;
		transform = glm ::translate(transform, -0.005f * camera_front);
		prism_center = transform * prism_center;
		//	cout << "Prism Center : " << prism_center.x << " " << prism_center.y
		//		 << " " << prism_center.z << endl;
	}

	// Camera Prism-Fashion Show
	// 0 key
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
		displaced = true;
		reset();
		camera_position = glm ::vec3(1.0f, 1.0f, 0.0f);
	}
	// 9 key
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
		displaced = true;
		reset();
		transform = glm ::translate(transform, glm ::vec3(0.0f, 0.0f, -0.25f));
		camera_position = glm ::vec3(0.0f, 0.0f, 5.0f);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width
	// and height will be significantly larger than specified on retina
	// displays.
	glViewport(0, 0, width, height);
}
