#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_impl_glfw.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"
#include "Graphics/AnimManager.h"
#include "Graphics/Frame.h"
#include "Engine/CrkBox.h"
#include "AnimManagerEditorUI.h"

class AnimManager;
class Frame;
class CrkBox;

class ImGuiUI
{
	GLFWwindow* _window;
public:
	ImGuiUI();
	~ImGuiUI();
	void init(GLFWwindow* window, unsigned int& _tex);
	void render();
	void addCharacter(AnimManager* _character);

	// Performance metrics
	float deltaTime = 0.0f;
	float fps = 0.0f;

	// Our state
	bool isPressed = false;
	bool show_demo_window = true;
	bool show_fps = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	const char* glsl_version = "#version 130";
	unsigned int tex;

	std::vector<std::string> consoleMessages;
	
private:
	void initStyle();
	void ProcessToggle();
	void ToggleMouseCursor();
	void ShowFPSWindow();
	void ShowGameView();
	void ShowCharacterView(AnimManager* _character);
	void ShowConsoleWindow();

	std::vector<AnimManager*> characters;
	AnimManagerEditorUI animManagerEditor;
};

