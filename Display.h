#pragma once
#include <string>
#include <sdl/SDL.h>
class Display
{
public:
	Display(int width, int height, const std::string& title);

	virtual ~Display();

	void Clear();
	void SetClearColor(float r, float g, float b, float a);
	void Update();
	void Close() { m_IsOpen = false; }
	SDL_Window* GetWindow() { return m_Window; }
	const bool &IsOpen() const { return m_IsOpen; }

private:
	Display(const Display& other) {}
	Display& operator=(const Display& other) {}

	SDL_Window* m_Window;
	SDL_GLContext m_GLContext;
	bool m_IsOpen = false;
};

