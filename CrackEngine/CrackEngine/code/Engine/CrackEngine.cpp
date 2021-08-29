#include "CrackEngine.h"
namespace Crack {

    CrackEngine* CrackEngine::instance;

	CrackEngine* CrackEngine::Instance()
	{
		if (!instance) {
			instance = new CrackEngine();
		}
		return instance;
	}
    CrackEngine::CrackEngine() { }
    CrackEngine::~CrackEngine()
	{
		delete instance;
        delete window;
	}
	int CrackEngine::Init()
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
        player1Manager = new AnimManager((std::string)"data/characters/Gohan.JSON");
        player1Manager->setPalette((std::string)"data/images/ghn/Pal_SSJ.pal", (std::string)"data/images/ghn/Pal_Template.pal");
        player1Manager->setPosition(glm::vec3(60.f, 20.f, 0.f));
        playerPos = player1Manager->getPosition();
        player2Manager = new AnimManager((std::string)"data/characters/GohanHurt.JSON");
        player2Manager->setPosition(glm::vec3((Sprite::pixelsPerUnit - 120), 20.f, 0.f));
        player2Manager->setPalette((std::string)"data/images/ghn/Pal_Demon.pal", (std::string)"data/images/ghn/Pal_Template.pal");
        player2Manager->setScale(glm::vec3(-1.f, 1.f, 1.f));
        //Player2.setPosition(glm::vec3((Sprite::pixelsPerUnit - 20), 20.f, 0.f));
        Shadow1 = new Sprite("data/images/Shadow.png");
        Shadow1->setPosition(glm::vec3(-50, 8, 0));
        Shadow2 = new Sprite("data/images/Shadow.png");
        Shadow2->setPosition(glm::vec3(-50, 8, 0));
        AddToGameObject(Shadow1);
        AddToGameObject(Shadow2);
        AddToGameObject(player2Manager);
        AddToGameObject(player1Manager);

        Sprite* SkyBG = new Sprite("data/images/DokkanBGs/Muscle Tower/battle_bg_00030_01.png");
        SkyBG->setPosition(glm::vec3(0, 0.f, 0.f));
        AddToBackground(SkyBG);
        Sprite* PlainsBG = new Sprite("data/images/DokkanBGs/Muscle Tower/battle_bg_00030_02.png");
        PlainsBG->setPosition(glm::vec3(0, 0.f, 0.f));
        AddToBackground(PlainsBG);
        Sprite* GroundBG = new Sprite("data/images/DokkanBGs/Muscle Tower/battle_bg_00030_03.png");
        GroundBG->setPosition(glm::vec3(0, 0.f, 0.f));
        AddToBackground(GroundBG);
        return 0;
	}
    void CrackEngine::Step()
    {
        glfwPollEvents();
        processInput(window);    // Temporary, inputs should be done every frame

        player1Manager->setPosition(playerPos);
        player1Manager->update();
        player2Manager->update();
    }
    void CrackEngine::Render()
    {

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Model::cameraPos = camera.Position;

        // view/projection transformations
        glm::mat4 projection = glm::ortho(0.f, 1.f, 0.f, ((float)SCR_HEIGHT / (float)SCR_WIDTH), -100.f, 100.f);
        //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        Model::projection = projection;
        Model::view = view;

        Shadow1->framePos = glm::vec3(player1Manager->getPosition().x, 0, 0);
        Shadow2->framePos = glm::vec3(player2Manager->getPosition().x, 0, 0);
        RenderLists();

        // Render UI
        imgui.render();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
    }

    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
    void CrackEngine::framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }

    // glfw: whenever the mouse moves, this callback is called
    // -------------------------------------------------------
    void CrackEngine::mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        if (CrackEngine::Instance()->firstMouse)
        {
            CrackEngine::Instance()->lastX = xpos;
            CrackEngine::Instance()->lastY = ypos;
            CrackEngine::Instance()->firstMouse = false;
        }

        float xoffset = xpos - CrackEngine::Instance()->lastX;
        float yoffset = CrackEngine::Instance()->lastY - ypos; // reversed since y-coordinates go from bottom to top

        CrackEngine::Instance()->lastX = xpos;
        CrackEngine::Instance()->lastY = ypos;

        CrackEngine::Instance()->camera.ProcessMouseMovement(xoffset, yoffset);
    }

    // glfw: whenever the mouse scroll wheel scrolls, this callback is called
    // ----------------------------------------------------------------------
    void CrackEngine::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        CrackEngine::Instance()->camera.ProcessMouseScroll(yoffset);
    }

    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    void CrackEngine::processInput(GLFWwindow* window)
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
}