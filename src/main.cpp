/* Header Files */
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "camera.h"
#include "shader.h"

using namespace std;
using namespace glm;

Camera camera(vec3(1.0f, 1.0f, 4.0f));

/* global variables */
const auto edge_length = 0.4f;          // edge length of the prism
const unsigned int SCR_WIDTH = 800;     // width
const unsigned int SCR_HEIGHT = 800;    // height
const char *SCR_TITLE = "Assignment0";  // window title

vec3 camera_position = vec3(0.0f, 0.0f, 3.0f);
vec3 new_camera_position = camera_position;
vec3 new_camera_target = vec3(0.0f, 0.0f, 0.0f);
vec3 new_camera_up = vec3(0.0f, 1.0f, 0.0f);

vec3 camera_front = vec3(0.0f, 0.0f, -1.0f);
vec3 camera_up = vec3(0.0f, 1.0f, 0.0f);
vec3 prism_center = vec3(0.0f, 0.0f, 0.0f);

mat4 transform = mat4(1.0f);
mat4 view = mat4(1.0f);

bool rotation = false;
bool turn_table = false;
bool displaced = false;

/* helper functions */

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
// to process the keys pressed  ( input given to the OpenGl windows )
void processInput(GLFWwindow *window);
//
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

		vertex.push_back(edge_length * (cos(beta)));
		vertex.push_back(edge_length * (sin(beta)));
		vertex.push_back(z_coord);
		vertex.push_back(1.0f);
		vertex.push_back(1.0f);
		vertex.push_back(0.0f);

		// vertex 3 for the front triangle

		vertex.push_back(edge_length * (cos(gamma)));
		vertex.push_back(edge_length * (sin(gamma)));
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

		vertex.push_back(edge_length * (cos(beta)));
		vertex.push_back(edge_length * (sin(beta)));
		vertex.push_back(z_coord - 0.5f);
		vertex.push_back(1.0f);
		vertex.push_back(1.0f);
		vertex.push_back(0.0f);

		// vertex 3 for the back triangle

		vertex.push_back(edge_length * (cos(gamma)));
		vertex.push_back(edge_length * (sin(gamma)));
		vertex.push_back(z_coord - 0.5f);
		vertex.push_back(1.0f);
		vertex.push_back(1.0f);
		vertex.push_back(0.0f);

		// face

		// vertex 1

		vertex.push_back(edge_length * (cos(beta)));
		vertex.push_back(edge_length * (sin(beta)));
		vertex.push_back(z_coord);
		vertex.push_back(cos1);
		vertex.push_back(sin1);
		vertex.push_back(sin12);

		// vertex 2

		vertex.push_back(edge_length * (cos(gamma)));
		vertex.push_back(edge_length * (sin(gamma)));
		vertex.push_back(z_coord);
		vertex.push_back(cos1);
		vertex.push_back(sin1);
		vertex.push_back(sin12);

		// vertex 3

		vertex.push_back(edge_length * (cos(beta)));
		vertex.push_back(edge_length * (sin(beta)));
		vertex.push_back(z_coord - 0.5f);
		vertex.push_back(cos1);
		vertex.push_back(sin1);
		vertex.push_back(sin12);

		// upper face vertex 1
		vertex.push_back(edge_length * (cos(beta)));
		vertex.push_back(edge_length * (sin(beta)));
		vertex.push_back(z_coord - 0.5f);
		vertex.push_back(cos1);
		vertex.push_back(sin1);
		vertex.push_back(sin12);

		// upper face vertex 2

		vertex.push_back(edge_length * (cos(gamma)));
		vertex.push_back(edge_length * (sin(gamma)));
		vertex.push_back(z_coord - 0.5f);
		vertex.push_back(cos1);
		vertex.push_back(sin1);
		vertex.push_back(sin12);

		// lower face vertex 3

		vertex.push_back(edge_length * (cos(gamma)));
		vertex.push_back(edge_length * (sin(gamma)));
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
	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		turn_table = !turn_table;
	}
}

int main(int argc, char *argv[]) {
	// glfw: initialize and configure
	int n;
	if (argc == 2) {
		n = atoi(argv[1]);
	} else
		return -1;

	initialise_screen();
	auto window = create_window();

	// build and compile our shader program
	Shader ourShader("../src/vertex.shader", "../src/fragment.shader");

	vector<float> vertex;
	make_polygon(n, vertex, 0.25f);

	unsigned int VBO, VAO;
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

	// You can unbind the VAO afterwards so other VAO calls won't accidentally
	// modify this VAO, but this rarely happens. Modifying other VAOs requires a
	// call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
	// VBOs) when it's not directly necessary.
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
	// render loop
	glfwSetKeyCallback(window, toggle_key);
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// set the view matrix ( which sets the location the camera is looking
		// at )
		view = lookAt(new_camera_position, new_camera_target, new_camera_up);

		mat4 projection = mat4(1.0f);
		projection = perspective(radians(40.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		ourShader.use();

		// ROtation 

		if (rotation) {
			// apply translate_1 to bring back the object to origin
			mat4 translate_1 = mat4(1.0f);
			translate_1 = translate(translate_1, (-1.0f) * prism_center);
			// apply the rotation transformation
			mat4 rot = mat4(1.0f);
			rot = rotate(rot, 0.01f, vec3(1.0f, 0.0f, 0.0f));
			// apply translation back to it's original position
			mat4 translate_2 = mat4(1.0f);
			translate_2 = translate(translate_2, prism_center);
			// update the final transform matrix
			transform = translate_2 * rot * translate_1 * transform;
			// update the prism center , applying the transformations
			vec4 tmp = vec4(prism_center, 1.0f);
			tmp = translate_2 * rot * translate_1 * tmp;
			prism_center = vec3(tmp);
		}

		// Turn Table 
		if (turn_table) {
			// set the camera_target
			new_camera_target = prism_center;
			float camX = prism_center.x;
			float camZ = prism_center.z;
			//
			new_camera_position.x = new_camera_position.x - camX;
			new_camera_position.z = new_camera_position.z - camZ;
			mat4 rot = mat4(1.0f);
			// rotate the camera
			rot = rotate(rot, radians(1.0f), vec3(0.0f, 1.0f, 0.0f));
			vec4 pos = vec4(new_camera_position, 1.0f);
			pos = rot * pos;
			new_camera_position = vec3(pos);
			new_camera_position.x = new_camera_position.x + camX;
			new_camera_position.z = new_camera_position.z + camZ;
		}

		ourShader.setMat4("transform", transform);
		ourShader.setMat4("view", view);
		ourShader.setMat4("projection", projection);

		glBindVertexArray(VAO);
		// seeing as we only have a single VAO there's
		// no need to bind it every time, but we'll do
		// so to keep things a bit more organized
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
void processInput(GLFWwindow *window) {
	float camera_speed = 0.05f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// Flying Camera

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		new_camera_target = prism_center;
		new_camera_position =
			new_camera_position - camera_speed * vec3(0.0f, 0.0f, 1.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		new_camera_target = prism_center;
		new_camera_position =
			new_camera_position - camera_speed * vec3(1.0f, 0.0f, 0.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		new_camera_target = prism_center;
		new_camera_position =
			new_camera_position + camera_speed * vec3(1.0f, 0.0f, 0.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		new_camera_target = prism_center;
		new_camera_position =
			new_camera_position + camera_speed * vec3(0.0f, 0.0f, 1.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		new_camera_target = prism_center;
		new_camera_position =
			new_camera_position + camera_speed * vec3(0.0f, 1.0f, 0.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		new_camera_target = prism_center;
		new_camera_position =
			new_camera_position - camera_speed * vec3(0.0f, 1.0f, 0.0f);
	}

	// Prism Fashion Show
	// 0,0,1
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
		new_camera_position = vec3(0.0f, 0.0f, 1.0f);
		new_camera_target = prism_center;
	}
	// 1,0,1
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
		new_camera_position = vec3(1.0f, 0.0f, 1.0f);
		new_camera_target = prism_center;
	}

	// Object Translation
	// left
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		vec3 direction = (camera_speed)*normalize(
			vec3(new_camera_target - new_camera_position));
		vec3 right =
			normalize(cross(direction, new_camera_up)) * (-camera_speed);
		prism_center = prism_center + right;
		mat4 trans = glm ::mat4(1.0f);
		trans = translate(trans, right);
		transform = trans * transform;
	}
	// right
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
		vec3 direction = vec3(new_camera_target - new_camera_position);
		vec3 right = camera_speed * normalize(cross(direction, new_camera_up));
		// also translate prism center
		prism_center = prism_center + right;
		// now translate the transform
		mat4 trans = glm ::mat4(1.0f);
		trans = translate(trans, right);
		transform = trans * transform;
	}
	// up
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
		vec3 direction = vec3(new_camera_target - new_camera_position);
		vec3 right = normalize(cross(direction, new_camera_up));
		vec3 up = camera_speed * normalize(cross(right, direction));
		prism_center = prism_center + up;
		mat4 trans = glm ::mat4(1.0f);
		trans = translate(trans, up);
		transform = trans * transform;
	}
	// down
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		vec3 direction = vec3(new_camera_target - new_camera_position);
		vec3 right = normalize(cross(direction, new_camera_up));
		vec3 up = (-camera_speed) * normalize(cross(right, direction));
		prism_center = prism_center + up;
		mat4 trans = glm ::mat4(1.0f);
		trans = translate(trans, up);
		transform = trans * transform;
	}
	// towards
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		vec3 direction =
			(-camera_speed) *
			normalize(vec3(new_camera_target - new_camera_position));
		// also translate prism center
		prism_center = prism_center + direction;
		// now translate the transform
		mat4 tmp = mat4(1.0f);
		tmp = translate(tmp, direction);
		transform = tmp * transform;
	}
	// away
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		vec3 direction =
			camera_speed *
			normalize(vec3(new_camera_target - new_camera_position));
		prism_center = prism_center + direction;
		mat4 tmp = mat4(1.0f);
		tmp = translate(tmp, direction);
		transform = tmp * transform;
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
