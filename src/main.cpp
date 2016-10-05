#include <cstdio>
#include <cstdlib>

#include <string>
#include <chrono>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION //TODO: remover essa merda u.u
#include "stb_image.h"
#include "glad/glad.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ns/spritebuffer.h"
#include "ns/textureinfo.h"
#include "ns/vertex.h"
#include "ns/text.h"

#define DEBUG_SHADER true

#define vertexShaderCode\
		"#version 150\n"\
		\
        "in vec2 position;"\
        "in vec2 texcoord;"\
        "in vec4 color;"\
		\
        "out vec4 Color;"\
		"out vec2 Texcoord;"\
        \
        "uniform mat4 view;"\
        "uniform mat4 proj;"\
		\
		"void main() {"\
		"	Color = color;"\
		"	Texcoord = texcoord;"\
        "	gl_Position = proj * view * vec4(position, 0.0, 1.0);"\
		"}"

#define fragmentShaderCode\
		"#version 150\n"\
		\
        "in vec2 Texcoord;"\
        "in vec4 Color;"\
		\
		"out vec4 outColor;"\
		\
        "uniform sampler2D tex;"\
		\
        "void main() {"\
        "	outColor = texture(tex, Texcoord) * Color;"\
		"}"

#define CHECK_GL {\
    GLenum error = glGetError();\
    if (error != GL_NO_ERROR) printf("%x\n", error);\
    assert(error == GL_NO_ERROR);\
}

void check(bool cond, const std::string &error) {
	if (!cond) {
		fprintf(stderr, "%s\n", error.c_str());
		exit(-1);
	}
}

template <typename T>
T stepTowards(T initial, T target, T step) {
    if (initial < target) {
        initial += step;
        if (initial > target)
            return target;
        return initial;
    } else if (initial > target) {
        initial -= step;
        if (initial < target)
            return target;
        return initial;
    } else {
        return initial;
    }
}

GLFWwindow* createWindow(int w, int h, const std::string &windowName, bool resizable) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	GLFWwindow *window = glfwCreateWindow(w, h, windowName.c_str(), nullptr, nullptr);
	check(window, "GLFW error");

	glfwMakeContextCurrent(window);

	return window;
}

GLuint compileShader(const char *code, GLenum shaderType) {
	const GLchar *vsource[] { code };

	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, vsource, nullptr);
	glCompileShader(shader);

	if (DEBUG_SHADER) {
		char buffer[512];
		glGetShaderInfoLog(shader, 512, nullptr, buffer);

		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		check(status == GL_TRUE, std::string("Shader error:\n") + buffer);

		if (strcmp(buffer, "")) {
			printf("Shader warning:\n%s", buffer);
		}
	}

	return shader;
}

void gladInit() {
	bool gladStatus = gladLoadGL();
	check(gladStatus, "Glad error");
}

void cursorPositionCallback(GLFWwindow *window, double x, double y) {
	(void)window;
	printf("x%lf y%lf\n", x, y);
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
	(void)mods;
	if (button == 0 && action == 1) {
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		printf("clicked x%lf y%lf\n", x, y);
	}
}

int main() {
    int width = 800, height = 600;
    setvbuf(stdout, nullptr, _IONBF, 0);

    glfwInit();

	//Window
    GLFWwindow *window = createWindow(width, height, "playground", false);
	//glfwSetCursorPosCallback(window, cursorPositionCallback);
	//glfwSetMouseButtonCallback(window, mouseButtonCallback);

	//Glad
    gladInit();

    //glEnable(GL_DEPTH_TEST);

	//Input
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	//Shader
	GLuint vertexShader = compileShader(vertexShaderCode, GL_VERTEX_SHADER);
	GLuint fragmentShader = compileShader(fragmentShaderCode, GL_FRAGMENT_SHADER);

	GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    ns::SpriteBuffer sb;
    glBindBuffer(GL_ARRAY_BUFFER, sb.vbo.id);

    ns::SpriteBufferIndices indices;

    ns::TextureInfo spriteTI = ns::loadTexture("img.jpg");
    ns::TextureInfo fontTI = ns::loadTexture("font.bmp");
    ns::FontInfo fontInfo {'a', 32, 32, 0, 0, 2, 2};

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    CHECK_GL;

    //needs VAO
    ns::Vertex::setupVertexAttribs(
                glGetAttribLocation(shaderProgram, "position"),
                glGetAttribLocation(shaderProgram, "texcoord"),
                glGetAttribLocation(shaderProgram, "color"));
    CHECK_GL;

    //MVP
    GLint uniView = glGetUniformLocation(shaderProgram, "view");
    GLint uniProj = glGetUniformLocation(shaderProgram, "proj");

    glm::mat4 view;
    glm::mat4 proj;

    view = glm::mat4();
    glUniformMatrix4fv(uniView, 1, false, glm::value_ptr(view));

    proj = glm::ortho(-width/2.0f, width/2.0f, height/2.0f, -height/2.0f, 0.0f, 1.0f);
    glUniformMatrix4fv(uniProj, 1, false, glm::value_ptr(proj));

    struct Character {
        bool ai;
        ns::IntRect sprRct;
        ns::vec2 pos;
        ns::vec2 speed;
        ns::Complex deg;
        float mass;
        float agi;
        float drag;
        float fric;
    };

    std::vector<std::unique_ptr<Character>> chars;

    Character d;
    d.ai = false;
    d.sprRct = {0, 512, 512, 512};
    d.mass = 1;
    d.agi = 16;
    d.drag = 1;
    d.fric = 16;
    chars.push_back(std::make_unique<Character>(d));

    Character &doge = *chars.front();

    Character mob;
    mob.ai = true;
    mob.sprRct = {0, 0, 512, 512};

    mob.mass = 5;
    mob.agi = 1;
    mob.drag = 0;
    mob.fric = 0;
    chars.push_back(std::make_unique<Character>(mob));

    mob.mass = 1;
    mob.agi = 2;
    mob.drag = 0;
    mob.fric = 0;
    chars.push_back(std::make_unique<Character>(mob));

    mob.mass = 1;
    mob.agi = 1;
    mob.drag = 1;
    mob.fric = 1;
    chars.push_back(std::make_unique<Character>(mob));

    mob.mass = 1;
    mob.agi = 2;
    mob.drag = 1;
    mob.fric = 1;
    chars.push_back(std::make_unique<Character>(mob));

    double t_last = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        CHECK_GL;
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
        }

		double t_time = glfwGetTime();
        float t_delta = t_time - t_last;
        t_last = t_time;

        glBindTexture(GL_TEXTURE_2D, spriteTI.handle.id);
        sb.texture_size[0] = spriteTI.width;
        sb.texture_size[1] = spriteTI.height;

        for (auto &ptr : chars) {
            Character &c = *ptr;

            //calculate direction
            ns::vec2 dir;

            if (!c.ai) {
                if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                    dir += {0.0f, -1.0f};
                } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                    dir += {0.0f, 1.0f};
                }

                if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                    dir += {-1.0f, 0.0f};
                } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                    dir += {1.0f, 0.0f};
                }
            } else {
                dir = doge.pos - c.pos;
            }

            float length = ns::length(dir);

            //calculate speed
            if (dir[0] != 0)
                c.speed[0] += 2 * c.agi/c.mass * dir[0]/length * t_delta;
            if (dir[1] != 0)
                c.speed[1] += 2 * c.agi/c.mass * dir[1]/length * t_delta;

            //calculate drag
            float dragX = 0.5f * c.speed[0] * c.speed[0] * c.drag;
            float dragY = 0.5f * c.speed[1] * c.speed[1] * c.drag;

            //calculate friction
            float fricX = c.mass * c.fric * (dir[0] == 0 || (dir[0]>0) != (c.speed[0]>0));
            float fricY = c.mass * c.fric * (dir[1] == 0 || (dir[1]>0) != (c.speed[1]>0));

            //calculate drag+friction
            c.speed[0] = stepTowards(c.speed[0], 0.0f, (dragX + fricX) * t_delta);
            c.speed[1] = stepTowards(c.speed[1], 0.0f, (dragY + fricY) * t_delta);

            //calculate rotation deg
            if (c.speed != ns::vec2{0.0f, 0.0f}) {
                c.deg = ns::normalized(c.speed) * ns::Complex(0,-1);
            }

            //calculate position
            c.pos += c.speed;

            float scale = 32.0f / 512.0f;
            ns::Sprite spr;
            spr.rect = c.sprRct;
            spr.mat
                .scale({scale, scale})
                .translate({-16,-16})
                .rotate(c.deg)
                .translate(c.pos)
            ;
            sb.append(spr);
        }

        glClear(GL_COLOR_BUFFER_BIT);

        sb.draw(indices);
        sb.clear();

        glBindTexture(GL_TEXTURE_2D, fontTI.handle.id);
        sb.texture_size[0] = fontTI.width;
        sb.texture_size[1] = fontTI.height;

        ns::drawString(0, 0, "abcdab", sb, fontInfo, ns::TextAlignment::center, ns::white);
        sb.draw(indices);
        sb.clear();

		glfwSwapBuffers(window);
    }

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

	glfwTerminate();

	return 0;
}
