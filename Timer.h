#pragma once
#include <SDL/SDL.h>

class Timer
{
public:
	Timer()
	{
		m_StartTime = SDL_GetTicks();
	}
	float totalTime() { return m_TotalTime; }
	float deltaTime() { return m_DeltaTime; };
	void Update()
	{
		m_CurrentTime = SDL_GetTicks();

		m_DeltaTime = (m_CurrentTime - m_PreviousTime)*100;

		m_PreviousTime = m_CurrentTime;
	}
private:
	float m_StartTime = 0;
	float m_TotalTime = 0;
	float m_CurrentTime = 0;
	float m_PreviousTime = 0;
	float m_DeltaTime = 0;
};