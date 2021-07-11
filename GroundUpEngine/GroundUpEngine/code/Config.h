#pragma once
class Config
{
public:
	static void Init();

	static unsigned int SCR_WIDTH;
	static unsigned int SCR_HEIGHT;
	static bool vsync;
	static bool fullscreen;
	static bool resizableWindow;
	static unsigned int targetFPS;
};

