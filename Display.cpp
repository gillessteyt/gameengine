#include "Display.h"
#include <iostream>
#include <glew/glew.h>

Display::Display(int width, int height, const std::string& title)
{
	std::cout << "Display Constructor fired!" << std::endl;
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); //2^8 = 256 shades of red
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8); //2^8 = 256 shades of green
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8); //2^8 = 256 shades of blue
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8); //2^8 = 256 shades of alpha
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	m_Window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL); //Window centered in screen
	m_GLContext = SDL_GL_CreateContext(m_Window);

	m_IsOpen = true;

	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLenum status = glewInit();

	if (status != GLEW_OK)
	{
		std::cerr << "Error initializing glew..." << std::endl;
	}
	SetClearColor(.4f, .4f, .4f, 1.0f);
}

Display::~Display()
{
	std::cout << "Display Destructor fired!" << std::endl;
	SDL_GL_DeleteContext(m_GLContext);
	SDL_DestroyWindow(m_Window);
}

void Display::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::SetClearColor(float r, float g, float b, float a)
{
	glClearColor(r,g,b,a);
}

void Display::Update()
{
	SDL_GL_SwapWindow(m_Window);


}
