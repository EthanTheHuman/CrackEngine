#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../Shader_m.h"
#include "../../Camera.h"
#include "../Graphics/Sprite.h"
#include "../Graphics/Model.h"

#include "../Log.h"

#include "../ImGuiUI.h"
#include "../../includes/json.hpp"
#include "../Config.h"
#include "../Graphics/AnimManager.h"

namespace Crack {

	class CrackEngine
	{
    public:
		static CrackEngine* Instance();
		int Init();
        void Step();
        void Render();

	private:
		static CrackEngine *instance;
		CrackEngine();
		~CrackEngine();

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
        void processInput(GLFWwindow* window);

        ImGuiUI imgui;
        AnimManager* player1Manager;
        AnimManager* player2Manager;
        Sprite* Shadow;
        std::vector<Sprite> stageElements;
        GLFWwindow* window;

        unsigned int SCR_WIDTH = 1536;
        unsigned int SCR_HEIGHT = 896;

        bool firstMouse = true;
        float lastX = SCR_WIDTH / 2.0f;
        float lastY = SCR_HEIGHT / 2.0f;

        Camera camera = (glm::vec3(0.0f, 0.0f, 3.0f));
        glm::vec3 playerPos = glm::vec3(20.0f, 20.0f, 0.0f);
	};
}