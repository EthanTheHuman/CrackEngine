#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_impl_glfw.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"

class ImGuiUI
{
	GLFWwindow* _window;
public:
	ImGuiUI();
	~ImGuiUI();
	void init(GLFWwindow* window, unsigned int& _tex);
	void render();

	// Our state
	bool isPressed = false;
	bool show_demo_window = false;
	bool show_fps = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	const char* glsl_version = "#version 130";
	unsigned int tex;

	float deltaTime = 0.f;
	float fps = 0.f;
	
private:
};

