#include<iostream>
#include <map>
#include <string>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<stb/stb_image.h>
#include<stb/stb_easy_font.h>
#include<vector>
#include"shaderClass.h"
#include"Particle.h"

//-------- INPUTS --------//
const int screenSize = 512;
const int pixelSize = 8;
const int radius = 4;
Color3 bgColor = Color3( 0.1f, 0.1f, 0.1f ); 
//-----------------------//



float pixelSizeNormalized = 1.0f;
const int normalizedBufSize = screenSize / pixelSize;
int colorBuffer[(normalizedBufSize) + 1][(normalizedBufSize) + 1] = {0};
Particle particleBuffer[(normalizedBufSize)+1][(normalizedBufSize)+1];

Particle Air = Particle(0, bgColor, false);
Particle Sand = Particle(1, Color3(1.0f, 1.0f, 0.0f), false);
Particle Water = Particle(2, Color3(0.0f, 0.0f, 1.0f), true);
Particle Lava = Particle(3, Color3(0.8f, 0.1f, 0.1f), true);
Particle Obsidian = Particle(4, Color3(0.3f, 0.0f, 0.5f), false);
Particle Stone = Particle(5, Color3(0.5f, 0.5f, 0.5f), false);
Particle slot = Sand;
GLfloat vertices[] =
{
	-pixelSizeNormalized, -pixelSizeNormalized,
	 pixelSizeNormalized, -pixelSizeNormalized,
	-pixelSizeNormalized,  pixelSizeNormalized, 
	 pixelSizeNormalized,  pixelSizeNormalized 
};

void setParticle(int x, int y, Particle particle)
{
	if ((x * pixelSize < screenSize && y * pixelSize <= screenSize) && (x >= 0 && y >= 0))
	{
		if (particleBuffer[y][x].getType() == 0 || particle.getType() == 0)
		{
			particleBuffer[y][x] = particle;
		}
	}
	//std::cout << "set " << color << " x: " << x << " y: " << y;
}

void SquareBrush(int x, int y, int brushRadius, Particle particle)
{
	for (int w = (-brushRadius / 2); w < brushRadius; w++)
	{
		for (int h = (-brushRadius / 2); h < brushRadius; h++)
		{
			double rnd = round(static_cast<double>(std::rand()) / RAND_MAX);
			if (rnd == 1)
			{
				int rnd1 = (round(static_cast<double>(std::rand()) / RAND_MAX) * 2) - 1;
				setParticle(x + h + rnd1, y + w, particle);
			}
		}
	}
}

void drawLine(int x1, int y1, int x2, int y2, int brushRadius, Particle particle)
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	int sign_x = (dx > 0) - (dx < 0);
	int sign_y = (dy > 0) - (dy < 0);

	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;

	int pdx, pdy, es, el;

	if (dx > dy) {
		pdx = sign_x;
		pdy = 0;
		es = dy;
		el = dx;
	}
	else {
		pdx = 0;
		pdy = sign_y;
		es = dx;
		el = dy;
	}

	int x = x1;
	int y = y1;

	int error = el / 2;
	int t = 0;

	if (brushRadius == 1) { setParticle(x, y, particle); }
	else { SquareBrush(x, y, brushRadius, particle); }
	
	while (t < el) {
		error -= es;
		if (error < 0) {
			error += el;
			x += sign_x;
			y += sign_y;
		}
		else {
			x += pdx;
			y += pdy;
		}
		t++;
		if (brushRadius == 1) { setParticle(x, y, particle); }
		else { SquareBrush(x, y, brushRadius, particle); }

	}
}

int limitNum(int n, int limiter) 
{
	if (n > limiter) { return limiter; }
	else{return n;}
}
void SwapParticle()
{

}

void updatePixelBuffer()
{
	int p = 0;
	for (int x = normalizedBufSize; x >= 0; x--) //for (int i = 0; i <= normalizedBufSize; i++)
	{
		for (int y = normalizedBufSize; y >= 0; y--)
		{
			if (x < normalizedBufSize && y < normalizedBufSize)
			{
				Particle thisParticle = particleBuffer[y][x]; 
				Particle p = Air;
				if (thisParticle.getType() == 1) 
				{
					
					if (particleBuffer[y + 1][x].getType() == 0 || particleBuffer[y + 1][x].isLiquid) 
					{ 
						p = particleBuffer[y + 1][x]; 
						particleBuffer[y][x] = p; 
						particleBuffer[y + 1][x] = thisParticle; 
					} 
					else if (particleBuffer[y + 1][x + 1].getType() == 0 || particleBuffer[y + 1][x + 1].isLiquid) 
					{
						p = particleBuffer[y + 1][x + 1]; 
						particleBuffer[y][x] = p; 
						particleBuffer[y + 1][x + 1] = thisParticle; 
					} 
					else if (particleBuffer[y + 1][x - 1].getType() == 0 || particleBuffer[y + 1][x - 1].isLiquid) 
					{
						p = particleBuffer[y + 1][x - 1]; 
						particleBuffer[y][x] = p; 
						particleBuffer[y + 1][x - 1] = thisParticle; 
					}
				}
				else if (thisParticle.getType() == 2)
				{
					double rnd = round(static_cast<double>(std::rand()) / RAND_MAX);
					if (particleBuffer[y + 3][x].getType() == 0)
					{
						particleBuffer[y][x] = Air; 
						particleBuffer[y + 3][x] = thisParticle; 
					}
					else if (particleBuffer[y + 1][x].getType() == 0)
					{
						particleBuffer[y][x] = Air;
						particleBuffer[y + 1][x] = thisParticle;
					}
					else if (particleBuffer[y + 1][x + 1].getType() == 0 && rnd == 0)
					{
						particleBuffer[y][x] = Air;
						particleBuffer[y + 1][x + 1] = thisParticle;
					}
					else if (particleBuffer[y + 1][x - 1].getType() == 0 && rnd == 1)
					{
						particleBuffer[y][x] = Air;
						particleBuffer[y + 1][x - 1] = thisParticle;
					}
					else if (particleBuffer[y][x + 1].getType() == 0 && rnd == 0)
					{
						particleBuffer[y][x] = Air;
						particleBuffer[y][x + 1] = thisParticle;
					}
					else if (particleBuffer[y][x - 1].getType() == 0 && rnd == 1)
					{
						particleBuffer[y][x] = Air;
						particleBuffer[y][x - 1] = thisParticle;
					}
				}
				else if (thisParticle.getType() == 3)
				{
					double rnd = round(static_cast<double>(std::rand()) / RAND_MAX);
					if (particleBuffer[y + 1][x].getType() == 0)
					{
						particleBuffer[y][x] = Air;
						particleBuffer[y + 1][x] = thisParticle;
					}
					else if (particleBuffer[y + 1][x + 1].getType() == 0 && rnd == 0)
					{
						particleBuffer[y][x] = Air;
						particleBuffer[y + 1][x + 1] = thisParticle;
					}
					else if (particleBuffer[y + 1][x - 1].getType() == 0 && rnd == 1)
					{
						particleBuffer[y][x] = Air;
						particleBuffer[y + 1][x - 1] = thisParticle;
					}
					else if (particleBuffer[y][x + 1].getType() == 0 && rnd == 0)
					{
						particleBuffer[y][x] = Air;
						particleBuffer[y][x + 1] = thisParticle;
					}
					else if (particleBuffer[y][x - 1].getType() == 0 && rnd == 1)
					{
						particleBuffer[y][x] = Air;
						particleBuffer[y][x - 1] = thisParticle;
					}
					if (particleBuffer[y - 1][x].getType() == 2)
					{
						particleBuffer[y][x] = Obsidian;
					}
					if (particleBuffer[y][x + 1].getType() == 2) 
					{ 
						particleBuffer[y][x] = Obsidian; 
					} 
					if (particleBuffer[y][x - 1].getType() == 2)
					{
						particleBuffer[y][x] = Obsidian;
					}
					if (particleBuffer[y + 1][x].getType() == 2)
					{
						particleBuffer[y][x] = Stone;
					}
				}
			}
		}
	}
}
void ClearBuffer()
{
	for (int i = 0; i <= normalizedBufSize; i++)
	{
		for (int j = 0; j <= normalizedBufSize; j++)
		{
			setParticle(i, j, Air);
		}
	}
}
int main() 
{
	ClearBuffer();
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(screenSize, screenSize, "SandBox", NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, screenSize, screenSize);

	Shader shaderProgram("default.vert", "default.frag");

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	



	
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;
	double xpos{}, ypos{}, xpos0, ypos0; 
	bool LMB, RMB = false;
	glfwSwapInterval(0);
	//int itCounter = 0;
	while (!glfwWindowShouldClose(window))
	{
		GLint transformLoc = glGetUniformLocation(shaderProgram.ID, "inColor");
		glEnable(GL_SCISSOR_TEST);
		glClear(GL_COLOR_BUFFER_BIT);
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;
		if (timeDiff >= 1.0 / 30.0)
		{
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			glfwGetCursorPos(window, &xpos, &ypos);
			std::string xposS = std::to_string(std::ceil(xpos / pixelSize));
			std::string yposS = std::to_string(std::ceil(ypos / pixelSize));
			std::string newTitle = "SandBox | " + FPS + " FPS | " + ms + " ms | x: " + xposS + " y : " + yposS
				+ (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ? " PRESSED" : "");

			glfwSetWindowTitle(window, newTitle.c_str());
			prevTime = crntTime;
			counter = 0;
		}
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {slot = Sand;}
		else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){slot = Water;}
		else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) { slot = Lava; }
		else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) { slot = Obsidian; }

		LMB = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
		RMB = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
		if (LMB || RMB)
		{
			drawLine(	std::ceil(xpos / pixelSize), std::ceil(ypos / pixelSize),
						std::ceil(xpos0 / pixelSize), std::ceil(ypos0 / pixelSize), radius, LMB == 1 ? slot : Air);
		}



		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) 
		{
			ClearBuffer();
		}
		float rnd = 0;
		for (int i = 0; i <= normalizedBufSize; i++)
		{
			for (int j = 0; j <= normalizedBufSize; j++)
			{
				Color3 particleColor = particleBuffer[j][i].getColor();
				rnd = 0;
				if (particleColor.R + particleColor.G + particleColor.B == 0.0f || (particleColor.R == Air.getColor().R && particleColor.G == Air.getColor().G && particleColor.B == Air.getColor().B))
				{
					particleColor = Air.getColor();
				}
				else
				{
					//rnd = (static_cast<double>(std::rand()) / RAND_MAX)/2 - 0.25f;
					particleColor = Color3(particleColor.R, particleColor.G, particleColor.B);
				}
				glUniform4f(transformLoc, particleColor.R + rnd, particleColor.G + rnd, particleColor.B + rnd, 1.0f);

				glScissor(i * pixelSize, (screenSize - j * pixelSize), pixelSize, pixelSize);
				shaderProgram.Activate();
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				glBindVertexArray(VAO);
			}
		}



		updatePixelBuffer();		
		xpos0 = xpos;
		ypos0 = ypos;
		glDisable(GL_SCISSOR_TEST); 
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}