#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"
#include "camera.h"
#include "code/Graphics/Sprite.h"
#include "code/Graphics/Model.h"

#include <iostream>
#include "code/Includes.h"
#include "code/Log.h"

#include "code/ImGuiUI.h"

#include <chrono>
#include <thread>
#include "includes/json.hpp"
#include "code/Config.h"
#include <vector>
#include "code/Graphics/AnimManager.h"

// for convenience
using json = nlohmann::json;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

// settings
unsigned int SCR_WIDTH = 1536;
unsigned int SCR_HEIGHT = 896;

// ImGui
ImGuiUI imgui;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

glm::vec3 playerPos = glm::vec3(20.0f, 20.0f, 0.0f);

// timing
//float deltaTime = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    Config config;
    config.Init();
    SCR_WIDTH = config.SCR_WIDTH;
    SCR_HEIGHT = config.SCR_HEIGHT;

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if (config.resizableWindow)
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    else
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window;
    if (config.fullscreen)
    {
        config.SCR_WIDTH = mode->width;
        config.SCR_HEIGHT = mode->height;
        SCR_WIDTH = mode->width;
        SCR_HEIGHT = mode->height;
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, config.windowName.c_str(), glfwGetPrimaryMonitor(), NULL);
    }
    else
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, config.windowName.c_str(), NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // set up ImGui
    // ------------
    imgui.init(window);

    //Sprite Player1("data/images/ghn/ghn00_06.png");
    AnimManager player1Manager((std::string)"data/characters/Gohan.JSON");
    player1Manager.setPosition(glm::vec3(60.f, 20.f, 0.f));
    playerPos = player1Manager.getPosition();
    Sprite Player2("data/images/ghn/ghn00_01.png");
    Player2.setPosition(glm::vec3((Sprite::pixelsPerUnit - 120), 20.f, 0.f));
    AnimManager player2Manager(&Player2, (std::string)"data/characters/GohanHurt.JSON");
    Player2.setScale(glm::vec3(-1.f, 1.f, 1.f));
    //Player2.setPosition(glm::vec3((Sprite::pixelsPerUnit - 20), 20.f, 0.f));
    Sprite Shadow("data/images/Shadow.png");
    Shadow.setPosition(glm::vec3(-50, 8, 0));

    std::vector<Sprite> stageElements;
    Sprite SkyBG("data/images/DokkanBGs/Muscle Tower/battle_bg_00030_01.png");
    SkyBG.setPosition(glm::vec3(0, 0.f, 0.f));
    stageElements.push_back(SkyBG);
    Sprite PlainsBG("data/images/DokkanBGs/Muscle Tower/battle_bg_00030_02.png");
    PlainsBG.setPosition(glm::vec3(0, 0.f, 0.f));
    stageElements.push_back(PlainsBG);
    Sprite GroundBG("data/images/DokkanBGs/Muscle Tower/battle_bg_00030_03.png");
    GroundBG.setPosition(glm::vec3(0, 0.f, 0.f));
    stageElements.push_back(GroundBG);

    // per-frame time logic
    // --------------------
    double current = glfwGetTime();
    double previous = current;
    //std::cout << "glfwGetTime: " << previous << std::endl;
    double lag = 0.0;
    double deltaTime = 0.0f;
    double FrameStep = (1000 / config.targetFramestep);

    auto start = std::chrono::steady_clock::now();
    int frames = 0;
    int fps = 0;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------

        auto now = std::chrono::steady_clock::now();
        auto diff = now - start;
        auto end = now + std::chrono::microseconds((int)((1000.f / config.targetFPS) * 1000));
        frames++;
        if (diff >= std::chrono::seconds(1))
        {
            fps = frames;
            imgui.fps = fps;
            start = now;
            frames = 0;
        }


        current = glfwGetTime();
        deltaTime = (current - previous) * 1000.f;
        imgui.deltaTime = deltaTime;
        previous = current;
        //std::cout << "glfwGetTime: " << deltaTime << std::endl;
        lag += deltaTime;

        // input
        // -----
        glfwPollEvents();

        // update
        // ------
        while (lag >= FrameStep)
        {
            processInput(window);    // Temporary, inputs should be done every frame

            for (std::vector<Sprite>::iterator it = stageElements.begin(); it != stageElements.end(); ++it)
            {
                it->update();
            }
            player1Manager.setPosition(playerPos);
            player1Manager.update();
            player2Manager.update();
            Player2.update();
            lag -= FrameStep;
        }

        // render
        // ------

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Model::cameraPos = camera.Position;

        // view/projection transformations
        glm::mat4 projection = glm::ortho(0.f, 1.f, 0.f, ((float)SCR_HEIGHT / (float)SCR_WIDTH), -100.f, 100.f);
        //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        Model::projection = projection;
        Model::view = view;

        for (std::vector<Sprite>::iterator it = stageElements.begin(); it != stageElements.end(); ++it)
        {
            it->render();
        }
        Shadow.framePos = glm::vec3(player1Manager.getPosition().x, 0, 0);
        Shadow.render();
        Shadow.framePos = glm::vec3(Player2.getPosition().x, 0, 0);
        Shadow.render();
        player1Manager.render();
        player2Manager.render();

        // Render UI
        imgui.render();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);

        if (config.targetFPS != 0) // If target FPS is provided, sleep the program until the expected time for next frame
        {
            std::this_thread::sleep_until(end);
        }
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    //glDeleteVertexArrays(1, &cubeVAO);
    //glDeleteVertexArrays(1, &lightCubeVAO);
    //glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
    {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        //playerPos.x -= (deltaTime * 60.0f);
        playerPos.x -= 1;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        //playerPos.x += (deltaTime * 60.0f);
        playerPos.x += 1;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = GL_RED;;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}