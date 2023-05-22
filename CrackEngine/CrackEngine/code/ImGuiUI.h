#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_impl_glfw.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"

class AnimManager;
class Frame;

class ImGuiUI
{
	GLFWwindow* _window;
public:
	ImGuiUI();
	~ImGuiUI();
	void init(GLFWwindow* window, unsigned int& _tex);
	void render();
	void addCharacter(AnimManager* _character);

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
	std::vector<AnimManager*> characters;

	// Render functions
	void ProcessToggle();
	void ToggleMouseCursor();
	void ShowFPSWindow();
	void ShowGameView();
	void ShowCharacterView(AnimManager* _character);
	void ShowAnimManagerEditorWindow();

	// Animation Manager Editor functions
	void ProcessAnimManagerInputs(ImVec2& _originPos, float& _spriteXPos, float& _spriteYPos, float& _canvasScale);
	Frame* RefreshAnimManagerCharacterInfo(AnimManager* _character, int _animIndex, int _frameIndex, float& spriteXPos, float& spriteYPos);
	void RefreshAnimManagerKeysLists(std::vector<int>& _animKeys, std::vector<int>& _frameKeys, AnimManager* _character, int _animIndex);
	std::vector<int> RefreshAnimManagerAnimKeysList(AnimManager* _character);
	std::vector<int> RefreshAnimManagerFrameKeysList(AnimManager* _character, int _animIndex);
};

