#include "CrackEngine.h"
namespace Crack {

    CrackEngine* CrackEngine::instance;
    Config config;
    // per-frame time logic
    // --------------------
    double current = glfwGetTime();
    double previous = current;
    double lag = 0.0;
    double deltaTime = 0.0f;
    double FrameStep = (1000 / config.targetFramestep);

    auto start = std::chrono::steady_clock::now();
    int frames = 0;
    int fps = 0;

	CrackEngine* CrackEngine::Instance()
	{
		if (!instance) {
			instance = new CrackEngine();
		}
		return instance;
	}
    CrackEngine::CrackEngine()
    {
        
    }
    CrackEngine::~CrackEngine()
	{
		delete instance;
	}
	int CrackEngine::Init()
	{
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
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
        player1Manager = new AnimManager((std::string)"data/characters/Gohan.xml", glm::vec3(60.f, 20.f, 0.f));
        playerPos = player1Manager->getPosition();
        player2Manager = new AnimManager((std::string)"data/characters/Android18.xml", glm::vec3((Sprite::pixelsPerUnit - 120), 20.f, 0.f));
        player2Manager->setScale(glm::vec3(-1.f, 1.f, 1.f));
        //Player2.setPosition(glm::vec3((Sprite::pixelsPerUnit - 20), 20.f, 0.f));
        Shadow = new Sprite("data/images/Shadow.png");
        Shadow->setPosition(glm::vec3(-50, 8, 0));

        Sprite SkyBG("data/images/DokkanBGs/Uranai Baba/battle_bg_00012_01.png");
        SkyBG.setPosition(glm::vec3(0, 0.f, 0.f));
        SkyBG.setScale(glm::vec3(0.5f, 1, 1));
        stageElements.push_back(SkyBG);
        Sprite PlainsBG("data/images/DokkanBGs/Uranai Baba/battle_bg_00012_02.png");
        PlainsBG.setPosition(glm::vec3(0, 0.f, 0.f));
        PlainsBG.setScale(glm::vec3(0.5, 1, 1));
        stageElements.push_back(PlainsBG);
        Sprite GroundBG("data/images/DokkanBGs/Uranai Baba/battle_bg_00012_03.png");
        GroundBG.setPosition(glm::vec3(0, 0.f, 0.f));
        GroundBG.setScale(glm::vec3(0.5, 1, 1));
        stageElements.push_back(GroundBG);

        // Input manager
        Player1Inputs = InputManager(window);

        return 0;
	}

    void CrackEngine::Step()
    {
        glfwPollEvents();
        processInput(window);    // Temporary, inputs should be done every frame

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
        lag += deltaTime;
        // --------------------

        // Every actual frame
        while (lag >= FrameStep)
        {
            for (std::vector<Sprite>::iterator it = stageElements.begin(); it != stageElements.end(); ++it)
            {
                it->update();
            }
            Player1Inputs.update();
            player1Manager->update();
            player2Manager->update();
            lag -= FrameStep;
        }

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

        for (std::vector<Sprite>::iterator it = stageElements.begin(); it != stageElements.end(); ++it)
        {
            it->render();
        }
        Shadow->framePos = glm::vec3(player1Manager->getPosition().x, 0, 0);
        Shadow->render();
        Shadow->framePos = glm::vec3(player2Manager->getPosition().x, 0, 0);
        Shadow->render();
        player1Manager->render();
        player2Manager->render();

        // Render UI
        imgui.render();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
    }

    bool CrackEngine::GetWindowShouldClose()
    {
        return glfwWindowShouldClose(window);
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

        player1Manager->processInputs(window, Player1Inputs);

        if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
        {
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

            glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
        }
        /*if (Player1Inputs.getButton(InputManager::eInputs::X))
        {
            camera.ProcessKeyboard(LEFT);
        }
        if (Player1Inputs.getButton(InputManager::eInputs::Y))
        {
            camera.ProcessKeyboard(BACKWARD);
        }
        if (Player1Inputs.getButton(InputManager::eInputs::Z))
        {
            camera.ProcessKeyboard(RIGHT);
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            camera.ProcessKeyboard(FORWARD);
        }*/
    }
}