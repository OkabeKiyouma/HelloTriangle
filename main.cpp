#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<iostream>
#include<fstream>
#include<sstream>

#define SCREEN_WIDTH 1200
//#define SCREEN_HEIGHT ((SCREEN_WIDTH)*9/16)
#define SCREEN_HEIGHT 1200


float buffer[] = {
	//positions			  //color			   //texture cordinates
	0.f,   0.f,   0.5f,	  1.f,   0.f,   0.f,   0.0f,   0.0f,   0.5f,
	1.f,   0.f,    1.f,	  0.f,   1.f,   0.f,    1.f,    0.f,    1.f,
	1.f,   1.f,    1.f,	  0.f,   1.f,   0.f,    1.f,    1.f,    1.f,
	0.f,   1.f,  0.75f,	  0.f,   0.f,   1.f,    0.f,    1.f,   0.5f
};

unsigned int element[] = {
	0,	1,	2,
	0,	2,	3
};

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){

}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static void error_callback(int error, const char* description) {
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

unsigned int compile_shader(const char* filename, GLenum type) {
	std::string tmp;
	std::ifstream infile;
	std::stringstream tmpstream;
	infile.open(filename);
	tmpstream << infile.rdbuf();
	infile.close();
	int success;

	unsigned int sh = glCreateShader(type);
	tmp = tmpstream.str();
	const char* const a = tmp.c_str();
	glShaderSource(sh, 1, &a, NULL);
	glCompileShader(sh);
	glGetShaderiv(sh, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		printf("ERROR: Compilation failed for %s\n", filename);
		char infolog[1024];
		glGetShaderInfoLog(sh, 1024, NULL, infolog);
		printf("%s\n", infolog);
	}
	return sh;
}

int main(int argc, char** argv) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OPENGL Window", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create glfw window";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetErrorCallback(error_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//std::cout<<glGetString(GL_VERSION);
	
	//get number of variables of (4size like vec) in GPU
	/*int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;*/

	/*VAO(Vertex Array Object) creation
	 Each VBO(Vertex Buffer Object) is a block of data(buffer) loaded in GPU which has various attributes set by glVertexAttribPointer which tells GPU how to access these data.
	 Every time draw call glDrawArrays is called, the buffer data must be bound to context and attribute should be set.
	 This process is trivialized with the help of VAO
	 A VBO can be bound with VAO which stores its attributes. The binding is done by simply binding array to current context.
	 Now on each draw call, simply binding VAO and VBO suffices.
	 On that note, multiple VAO can be bound to different VBO and used in unision whenever required.
	 Same VBO can be bound to multiple VAO.
	 */
	
	GLuint vaoid; //vertex array object
	glGenVertexArrays(1, &vaoid);
	glBindVertexArray(vaoid);

	//Vertex Buffer Object (VBO) 
	GLuint bufferid;
	glGenBuffers(1, &bufferid);	
	glBindBuffer(GL_ARRAY_BUFFER, bufferid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_DYNAMIC_DRAW); 

	GLuint elementid;
	glGenBuffers(1, &elementid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(element), element, GL_STATIC_DRAW);

	int theight, twidth, theight1, twidth1, tcomp1, tcomp; //for image loading
	stbi_set_flip_vertically_on_load(1);
	stbi_uc* tex = stbi_load("res/brick.png", &twidth, &theight, &tcomp, 0);
	if (tex == NULL) {
		printf("ERROR! Couldn't open the file brick.png");
	}
	stbi_uc* tex2 = stbi_load("res/test.jpg", &twidth1, &theight1, &tcomp1, 0);
	if (tex2 == NULL) {
		printf("ERROR! Couldn't open the file test.jpg");
	}

	GLuint textureid;
	glGenTextures(1, &textureid);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureid);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tex);
	glGenerateMipmap(GL_TEXTURE_2D);

	if (tex)
		stbi_image_free(tex);
	glGenerateMipmap(GL_TEXTURE_2D);

	GLuint textureid2;
	glGenTextures(1, &textureid2);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, textureid2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, twidth1, theight1, 0, GL_RGB, GL_UNSIGNED_BYTE, tex2);

	if (tex2)
		stbi_image_free(tex2);
	glGenerateMipmap(GL_TEXTURE_2D);

	GLuint vshaderid;
	vshaderid = compile_shader("shader/vertexshader.glsl",GL_VERTEX_SHADER);
	GLuint fshaderid;
	fshaderid = compile_shader("shader/fragmentshader.glsl", GL_FRAGMENT_SHADER);

	int success;
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vshaderid);
	glAttachShader(shaderProgram, fshaderid);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		printf("ERROR: Link failed %d\n", __LINE__);
		char infolog[1024];
		glGetShaderInfoLog(fshaderid, 1024, NULL, infolog);
		printf("%s\n", infolog);
	}
	glValidateProgram(shaderProgram);
	glUseProgram(shaderProgram);
	glDeleteShader(vshaderid);
	glDeleteShader(fshaderid);

	glUniform1i(glGetUniformLocation(shaderProgram, "tsamp1"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "tsamp2"), 1);

	glVertexAttribPointer(
		0,  //layout location of vertex in vshader eg layout (location = 0) 
		3, //number of data to be processed consecutively for a vertex attribute, here vec3(x,y,z) coordinates needs to be loaded, hence 3
		GL_FLOAT, // type of data to be processed
		GL_FALSE, //set true to normalize from -1 to 1
		9 * sizeof(float), //stride i.e difference between two consecutive vertex data
		(void*)0 //offset of buffer in this case, its start from beginning
	);
	glVertexAttribPointer(
		1,  //layout location of vertex in vshader eg layout (location = 0) 
		3, //number of data to be processed consecutively for a vertex attribute, here vec3(x,y,z) coordinates needs to be loaded, hence 3
		GL_FLOAT, // type of data to be processed
		GL_FALSE, //set true to normalize from -1 to 1
		9 * sizeof(float), //stride i.e difference between two consecutive vertex data
		(void*)(3 * sizeof(float)) //offset of buffer in this case, its start from beginning
	);
	glVertexAttribPointer(
		2,  //layout location of vertex in vshader eg layout (location = 0) 
		2, //number of data to be processed consecutively for a vertex attribute, here vec3(x,y,z) coordinates needs to be loaded, hence 3
		GL_FLOAT, // type of data to be processed
		GL_FALSE, //set true to normalize from -1 to 1
		9 * sizeof(float), //stride i.e difference between two consecutive vertex data
		(void*)(6 * sizeof(float)) //offset of buffer in this case, its start from beginning
	);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		//glDrawArrays(GL_TRIANGLES, 0, 3); //GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP, GL_TRIANGLE_STRIP
										  //GL_TRIANGLE_FAN,
		glBindVertexArray(vaoid);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementid);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
	}
	glDeleteTextures(1, &textureid);
	glDeleteTextures(1, &textureid2);
	glfwDestroyWindow(window);
	glfwTerminate();
	//// define the function's prototype
	//typedef void (*GL_GENBUFFERS) (GLsizei, GLuint*);
	//// find the function and assign it to a function pointer
	//GL_GENBUFFERS glGenBuffers = (GL_GENBUFFERS)glfwGetProcAddress("glGenBuffers");
	//// function can now be called as normal
	//unsigned int buffer;
	//glGenBuffers(1, &buffer);

}