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
    double FrameStep = (1000.f / (double)config.targetFramestep);

    auto start = std::chrono::steady_clock::now();
    int frames = 0;
    int fps = 0;


    SF_INFO CrackEngine::sfinfo;
    int CrackEngine::numFrames;
    float* CrackEngine::soundBuffer;
	

	CrackEngine* CrackEngine::Instance()
	{
		if (!instance) {
			instance = new CrackEngine();
		}
		return instance;
	}
    CrackEngine::CrackEngine() : music(nullptr)
    {
    }
    CrackEngine::~CrackEngine()
	{
        delete frameBufferShader;
        delete music;
		delete instance;
	}
	int CrackEngine::Init()
	{
        config.Init();
        SCR_WIDTH = config.SCR_WIDTH;
        SCR_HEIGHT = config.SCR_HEIGHT;
        GAME_WIDTH = config.pixelsPerUnit;
        GAME_HEIGHT = config.internalHeight;
        FrameStep = (1000.f / (double)config.targetFramestep);

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
		// get window icon
        GLFWimage images[1];
        images[0].pixels = stbi_load("data/icon.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 

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

        glfwSetWindowIcon(window, 1, images);
        stbi_image_free(images[0].pixels);

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
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

        // Create framebuffer for game view
		glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		// create a color attachment texture
		glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, config.pixelsPerUnit, config.internalHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// attach it to currently bound framebuffer object
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, config.pixelsPerUnit, config.internalHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

        frameBufferShader = new Shader("data/shaders/shad_renderbuffer.vs", "data/shaders/shad_renderbuffer.fs");
        backgroundShader = new Shader("data/shaders/shad_renderbackground.vs", "data/shaders/shad_renderbackground.fs");

        // define frame buffer quad to display
		// -----------------------------------
		float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			1.0f, -1.0f,  1.0f, 0.0f,
			
			-1.0f,  1.0f,  0.0f, 1.0f,
			1.0f, -1.0f,  1.0f, 0.0f,
			1.0f,  1.0f,  1.0f, 1.0f
		};
		// screen quad VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		
        // set up ImGui
        // ------------
        imgui.init(window, textureColorbuffer);

        //Sprite Player1("data/images/ghn/ghn00_06.png");
        player1Manager = new AnimManager((std::string)"data/characters/Gohan.xml", glm::vec3(((1024.f / 2.f) / 2.f) - 120.f, 20.f, 0.f));
        playerPos = player1Manager->getPosition();
        imgui.addCharacter(player1Manager);
        player2Manager = new AnimManager((std::string)"data/characters/Goku.xml", glm::vec3(((1024.f / 2.f) / 2.f) + 120.f, 20.f, 0.f));
        player2Manager->setScale(glm::vec3(-1.f, 1.f, 1.f));
        imgui.addCharacter(player2Manager);
		// camera.Position = glm::vec3((1024.f / 2.f) - ((float)config.pixelsPerUnit / 2.f), 0.f, 0.f);
        camera.Position = glm::vec3((1024.f / 2.f) / 2.f, 0.f, 0.f);
        //Player2.setPosition(glm::vec3((Sprite::pixelsPerUnit - 20), 20.f, 0.f));
        Shadow = new Sprite("data/images/Shadow.png");
        Shadow->setPosition(glm::vec3(-50, 8, 0));

        Sprite SkyBG("data/images/Maps/RedRibbonBase/001.png");
        SkyBG.setPosition(glm::vec3(0, 0.f, 0.f));
        SkyBG.setScale(glm::vec3(0.5, 0.5, 1));
        stageElements.push_back(SkyBG);
        Sprite PlainsBG("data/images/Maps/RedRibbonBase/002.png");
        PlainsBG.setPosition(glm::vec3(0, 0.f, 0.f));
        PlainsBG.setScale(glm::vec3(0.5, 0.5, 1));
        stageElements.push_back(PlainsBG);
        Sprite GroundBG("data/images/Maps/RedRibbonBase/003.png");
        GroundBG.setPosition(glm::vec3(0, 0.f, 0.f));
        GroundBG.setScale(glm::vec3(0.5, 0.5, 1));
        stageElements.push_back(GroundBG);
        /*Sprite ScreenBG("data/images/Maps/DirtDesert/003.png");
        ScreenBG.setPosition(glm::vec3(214, 121, 0.f));
        ScreenBG.setScale(glm::vec3(0.1, -0.1, 1));
        ScreenBG.setImage(textureColorbuffer);
        stageElements.push_back(ScreenBG);*/
        Sprite GroundBG2("data/images/Maps/DirtDesert/003.png");
        GroundBG2.setPosition(glm::vec3(0, 0.f, 0.f));
        GroundBG2.setScale(glm::vec3(0.5, 0.5, 1));
        stageElements.push_back(GroundBG2);

		// load the file music.mp3 with sndfile
        delete music;
        std::string fileName = AudioPlayer::randomFileFromDirectory("data/sounds/");
		music = new AudioPlayer(fileName, true, 1.f);
        music->play();
		
        // Input manager
        Player1Inputs = InputManager(window);

        return 0;
	}
	
    int CrackEngine::soundCallback(const void* inputBuffer, void* outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void* userData)
    {
		float* out = (float*)outputBuffer;
		static float* in = (float*)userData;
		unsigned long i;
		(void)timeInfo; // Prevent unused variable warnings.
		(void)statusFlags;
		(void)inputBuffer;
		
	    // check for end of file
		if (in >= soundBuffer + numFrames * sfinfo.channels)
		{
			for (i = 0; i < framesPerBuffer; i++)
			{
				*out++ = 0;  // left
				*out++ = 0;  // right
			}
			return paComplete;
		}
		
        for (i = 0; i < framesPerBuffer; i++)
        {
            *out++ = *in++;  // left
            *out++ = *in++;  // right
        }
		
		return paContinue;
	}

    void CrackEngine::Step()
    {
        glfwPollEvents();

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
            player1Manager->processInputs(window, Player1Inputs);
            processInput(window);    // Temporary, inputs should be done every frame
            player2Manager->update();
            lag -= FrameStep;
        }

    }
    void CrackEngine::Render()
    {
		
		// render to framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, GAME_WIDTH, GAME_HEIGHT);

        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Model::cameraPos = camera.Position;

        // view/projection transformations
        glm::mat4 projection = glm::ortho(0.f, 1.f, 0.f, ((float)GAME_HEIGHT / (float)GAME_WIDTH), -100.f, 100.f);
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

		// get window size
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        SCR_WIDTH = width;
        SCR_HEIGHT = height;
		
        // swap to main window
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// draw background behind the framebuffer
		// Resize the background to cover the screen but keep the aspect ratio, clipping the sides where necessary
        backgroundShader->use();
        {
            float ratio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
            float bgRatio = (float)GAME_WIDTH / (float)GAME_HEIGHT;
            float bgWidth = 1;
            float bgHeight = 1;
            if (ratio > bgRatio)
            {
				// Get the increase of width compared to the image aspect ratio
                bgHeight = 1 * (ratio / bgRatio);
                if (bgHeight < 1)
                {
                    bgHeight = 1;
                }
            }
            else
            {
                bgWidth = 1 * (bgRatio / ratio);
                if (bgWidth < 1)
                {
                    bgWidth = 1;
                }
            }
            backgroundShader->setFloat("gameWidth", bgWidth);
            backgroundShader->setFloat("gameHeight", bgHeight);
        }
        backgroundShader->setFloat("xs", GAME_WIDTH);
        backgroundShader->setFloat("ys", GAME_HEIGHT);
        backgroundShader->setFloat("r", 20);
		backgroundShader->setVec4("tint", glm::vec4(0.6f, 0.6f, 0.6f, 1.f));
        glBindVertexArray(quadVAO);
        glDisable(GL_DEPTH_TEST);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);
		
		// draw framebuffer
        frameBufferShader->use();
        if (SCR_WIDTH > 0 && SCR_HEIGHT > 0 && GAME_WIDTH > 0 && GAME_HEIGHT > 0) // scale quad to always match frame buffer aspect ratio
        {
            // compare aspect ratio
            float screenAspect = (float)SCR_WIDTH / (float)SCR_HEIGHT;    // If this is > 1 it is horizontal
            float gameAspect = (float)GAME_WIDTH / (float)GAME_HEIGHT;    // If this is >1 it is horizontal
            float quadWidth = 1;
            float quadHeight = 1;

            // If the window is wider than the framebuffer
            if (screenAspect > gameAspect)
            {
                quadWidth = 1 / (screenAspect / gameAspect);
            }

            // If the window is taller than the framebuffer
            if (screenAspect < gameAspect)
            {
                quadHeight = 1 / (gameAspect / screenAspect);
            }
            frameBufferShader->setFloat("gameWidth", quadWidth);
            frameBufferShader->setFloat("gameHeight", quadHeight);
        }
		glBindVertexArray(quadVAO);
        glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
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
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_REPEAT)
        {
            glfwSetWindowShouldClose(window, true);
            Pa_Terminate();
        }

        //player1Manager->processInputs(window, Player1Inputs);

        static bool bF2Pressed = false;
        if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
        {
            if (bF2Pressed == false)
            {
                //// set to windowed mode
                //const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
                //glfwSetWindowMonitor(window, NULL, (mode->width - config.SCR_WIDTH) / 2, (mode->height - config.SCR_HEIGHT) / 2, config.SCR_WIDTH, config.SCR_HEIGHT, GLFW_DONT_CARE);
                //isFullscreen = false;

                // set to windowed mode
                glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
                glfwSetWindowSize(window, config.SCR_WIDTH, config.SCR_HEIGHT);
				// set window position to center of screen
				const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
				glfwSetWindowPos(window, (mode->width - config.SCR_WIDTH) / 2, (mode->height - config.SCR_HEIGHT) / 2);
                isFullscreen = false;
            }
            else
            {
                // set to fullscreen
                /*const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
                glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
                isFullscreen = true;*/

                // set to undecorated window
				glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
				// set window to size of the screen
				const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
				glfwSetWindowSize(window, mode->width, mode->height);
				// set window to top left corner
                glfwSetWindowPos(window, 0, 0);
				isFullscreen = true;
            }
            bF2Pressed = true;
        }
        else
        {
			bF2Pressed = false;
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