#include <GL/glew.h>
#include <Windows.h>
#include <GL/glut.h>
#include "shader.h"
#include <iostream>
#include "glm.hpp"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Arcball.h"

bool stateSwitch = false;
GLuint vertexID;
GLuint vao;
GLuint posLocID;
GLuint normalLocID;
GLuint texLocID;
GLuint vShader, fShader, programHandle;
const GLuint WIDTH = 400, HEIGHT = 400;

GLfloat lastX = 0.0f;
GLfloat lastY = 0.0f;
bool keys[1024];
Arcball arcball(WIDTH / 2, glm::vec2(WIDTH / 2, HEIGHT / 2));

GLfloat vertices[] = {
	//vertex    //normal    //texcoord
	-1, -1, -1,  0,  0, -1,  0, 0,
	-1,  1, -1,  0,  0, -1,  1, 0,
	 1,  1, -1,  0,  0, -1,  1, 1,
	 1, -1, -1,  0,  0, -1,  0, 1,

	-1, -1,  1,  0,  0,  1,  0, 0,
	 1, -1,  1,  0,  0,  1,  1, 0,
	 1,  1,  1,  0,  0,  1,  1, 1,
	-1,  1,  1,  0,  0,  1,  0, 1,

	-1, -1, -1, -1,  0,  0,  0, 0,
	-1, -1,  1, -1,  0,  0,  1, 0,
	-1,  1,  1, -1,  0,  0,  1, 1,
	-1,  1, -1, -1,  0,  0,  0, 1,

	 1,  1,  1,  1,  0,  0,  0, 0,
	 1, -1,  1,  1,  0,  0,  1, 0,
	 1, -1, -1,  1,  0,  0,  1, 1,
	 1,  1, -1,  1,  0,  0,  0, 1,
	  
	 1,  1,  1,  0,  1,  0,  0, 0,
	 1,  1, -1,  0,  1,  0,  1, 0,
	-1,  1, -1,  0,  1,  0,  1, 1,
	-1,  1,  1,  0,  1,  0,  0, 1,

	-1, -1, -1,  0, -1,  0,  0, 0,
	 1, -1, -1,  0, -1,  0,  1, 0,
	 1, -1,  1,  0, -1,  0,  1, 1,
	-1, -1,  1,  0, -1,  0,  0, 1,
};

void printInfoLog(GLuint shader)
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 1) {
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(shader, infologLength,
			&charsWritten, infoLog);
		printf("%s \n", infoLog);
		printf("***********************************************************\n");
		free(infoLog);
	}
}

void initVBO()//vbo的生成与vao的绑定
{
	//create vertex buffers
	glGenBuffers(1, &vertexID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//创建顶点数组
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
		glEnableVertexAttribArray(posLocID);//默认情况下，着色器(shader)里对应属性位置(location)的顶点访问都是关闭的，我们要通过设置glEnableVertexAttribArray来设置其允许访问
		glBindBuffer(GL_ARRAY_BUFFER, vertexID);
		glVertexAttribPointer(posLocID, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
}

void initGLSL()
{
	//create & compile a vertex shader
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vertl, NULL);
	glCompileShader(vShader);
	printInfoLog(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fragl, NULL);
	glCompileShader(fShader);
	printInfoLog(fShader);

	//create a glslhandle & attach shaders
	programHandle = glCreateProgram();
	glAttachShader(programHandle, vShader);
	glAttachShader(programHandle, fShader);
	glLinkProgram(programHandle);

	GLint success;
	glGetProgramiv(programHandle,
		GL_LINK_STATUS,
		&success);
	if (!success) {
		printf("Shader could not be linked!\n ");
	}

	//bind attribute
	posLocID = glGetAttribLocation(programHandle, "vertexPosition");
}

void init()
{
	//gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//glClearColor(0.0, 0.0, 0.0, 0.0);
	initGLSL();
	initVBO();
}

void display()
{
	static int frame = 0;
	std::cout << frame++ << std::endl;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(programHandle);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);


	//use vaoID & draw quads
	glBindVertexArray(vao);

	glm::mat4 view = glm::lookAt(glm::vec3(0,0,9), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 projection = glm::perspective(GLfloat(60.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
	glm::mat4 model = arcball.GetArcballMatrix();
	GLint modelLoc = glGetUniformLocation(programHandle, "model");
	GLint viewLoc = glGetUniformLocation(programHandle, "view");
	GLint projLoc = glGetUniformLocation(programHandle, "projection");
	GLint viewPosLoc = glGetUniformLocation(programHandle, "viewPos");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_QUADS, 0, 24);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	//glFlush();
	glutSwapBuffers();
}

void reshape(int width, int height)
{
	width = WIDTH;
	height = HEIGHT;
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN){
		arcball.mouse_down(LEFTBUTTON_DOWN);
		lastX = (GLfloat)x - WIDTH / 2;
		lastY = -((GLfloat)y - HEIGHT / 2);
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
		arcball.mouse_up(LEFTBUTTON_UP);
	}
	if (button == GLUT_RIGHT_BUTTON&&state == GLUT_DOWN){
		arcball.mouse_down(RIGHTBUTTON_DOWN);
		lastX = (GLfloat)x - WIDTH / 2;
		lastY = -((GLfloat)y - HEIGHT / 2);
	}
	if(button == GLUT_RIGHT_BUTTON&&state == GLUT_UP){
		arcball.mouse_down(RIGHTBUTTON_UP);
	}
	glutPostRedisplay();
}

void motion(int x, int y)
{
	arcball.mouse_motion(lastX, lastY, (GLfloat)x - WIDTH / 2, -((GLfloat)y - HEIGHT / 2));
	lastX = (GLfloat)x - WIDTH / 2;
	lastY = -((GLfloat)y - HEIGHT / 2);
	std::cout<<lastX<<std::endl;
	glutPostRedisplay();
}

void keyboarddown(unsigned char key, int x, int y)
{
	keys[key] = true;
}

void keyboardup(unsigned char key, int x, int y)
{
	keys[key] = false;
}

void idle()
{
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("arcball");
	GLenum err = glewInit();
	if (err != GLEW_OK){
		std::cout << "fail" << std::endl;
		return -1;
	}
	glViewport(0, 0, WIDTH, HEIGHT);

	//多重采样 抗锯齿
	GLint bufs, samples;   //支持多重采样时，值不能为零
	glGetIntegerv(GL_SAMPLE_BUFFERS, &bufs);
	glGetIntegerv(GL_SAMPLES, &samples);
	glEnable(GL_MULTISAMPLE);
	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* biaoshifu = glGetString(GL_RENDERER);
	const GLubyte* OpenGLVersion = glGetString(GL_VERSION);
	const GLubyte* gluVersion = gluGetString(GLU_VERSION);
	std::cout << "GL_VENDER: " << name << std::endl
		<< "GL_RENDERER: " << biaoshifu << std::endl
		<< "GL_VERSION: " << OpenGLVersion << std::endl
		<< "gluVertion: " << gluVersion << std::endl
		<< "============================" << std::endl;
	init();
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboarddown);
	glutKeyboardUpFunc(keyboardup);
	glutReshapeFunc(reshape);
	//glutIdleFunc(idle);

	glutMainLoop();

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vertexID);
	glDeleteProgram(programHandle);
	glDeleteShader(vShader);
	glDeleteShader(fShader);
	return 0;
}