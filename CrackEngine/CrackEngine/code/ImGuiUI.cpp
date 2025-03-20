#include "ImGuiUI.h"
#include "Graphics/AnimManager.h"

ImGuiUI::ImGuiUI()
{
}

ImGuiUI::~ImGuiUI()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiUI::init(GLFWwindow* window, unsigned int& _tex)
{
    tex = _tex;
    _window = window;
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    // ----------------------
    initStyle();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
}

void ImGuiUI::initStyle()
{
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    
    // Window styling
    style.WindowRounding = 5.3f;
    style.FrameRounding = 2.3f;
    style.ScrollbarRounding = 0;

    // Colors
    style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(26.f/255.f, 26.f/255.f, 26.f/255.f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
    style.Colors[ImGuiCol_Border] = ImVec4(227.f/255.f, 171.f/255.f, 114.f/255.f, 1.00f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(26.f/255.f, 26.f/255.f, 26.f/255.f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(26.f/255.f, 26.f/255.f, 26.f/255.f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(240.f/255.f, 136.f/255.f, 40.f/255.f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

void ImGuiUI::render()
{
    ProcessToggle();
    ToggleMouseCursor();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    if (show_fps)
    {
        ShowFPSWindow();
        ShowGameView();
        ShowConsoleWindow();
		
        for (auto* character : characters)
        {
            ShowCharacterView(character);
        }
        animManagerEditor.render(characters);
    }

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiUI::addCharacter(AnimManager* _character)
{
    characters.push_back(_character);
}

// Render functions
void ImGuiUI::ProcessToggle()
{
    if (glfwGetKey(_window, GLFW_KEY_F1) == GLFW_PRESS && !isPressed)
    {
        show_fps = !show_fps;
        isPressed = true;
    }
    else if (glfwGetKey(_window, GLFW_KEY_F1) == GLFW_RELEASE)
    {
        isPressed = false;
    }
}

void ImGuiUI::ToggleMouseCursor()
{
    static double lastMouseMovement = 0;
    if (ImGui::GetIO().MouseDelta.x != 0.f || ImGui::GetIO().MouseDelta.y != 0.f)
    {
        lastMouseMovement = glfwGetTime();
    }

    bool shouldShowCursor = show_fps || (glfwGetTime() - lastMouseMovement <= 3.0f);
    int currentMode = glfwGetInputMode(_window, GLFW_CURSOR);
    
    if (shouldShowCursor && currentMode == GLFW_CURSOR_DISABLED)
    {
        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else if (!shouldShowCursor && currentMode == GLFW_CURSOR_NORMAL)
    {
        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void ImGuiUI::ShowFPSWindow()
{
    ImGui::Begin("Performance tracking");                          // Create a window called "Hello, world!" and append into it.
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", deltaTime, fps);
    //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

void ImGuiUI::ShowGameView()
{
    ImGui::Begin("Render Window");                          // Render game window
    ImGui::BeginChild("GameRender");
    ImVec2 wsize = ImGui::GetWindowSize();
    ImGui::Image((ImTextureID)tex, wsize, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
}

void ImGuiUI::ShowCharacterView(AnimManager* _character)
{
    std::string WindowName = "Character: " + _character->characterName;
    ImGui::Begin(WindowName.c_str());
    ImGui::Text("Character current animation: %i", _character->currentAnim->index);
    ImGui::Text("Character current frame: %i", _character->currentFrame->index);
    ImGui::Text("Character current step: %i", _character->frameCount);
    ImGui::Text("Current palette: %s", _character->mainPalette.paletteFileName.c_str());
    static int paletteCount = _character->paletteList.size() - 1;
    if (_character->paletteList.size() > 0)
    {
        if (ImGui::CollapsingHeader("Palettes"))
        {
            ImGui::Text(_character->mainPalette.paletteName.c_str());
            if (ImGui::SliderInt("Palette:", &_character->paletteIndex, 0, paletteCount))
            {
                _character->setPalette(_character->paletteIndex);
            }
            ImGui::BeginTable("Colormap1", 16, ImGuiTableFlags_Borders);
            for (int j = 0; j < _character->mainPalette.baseColors.size(); j++)
            {
                if (j % 16 == 0)
                {
                    ImGui::TableNextRow();
                }
                ImGui::TableNextColumn();
                auto colorReference = _character->mainPalette.baseColors[j];
                ImGui::ColorButton("Color Template", ImVec4(colorReference.x, colorReference.y, colorReference.z, 1), ImGuiColorEditFlags_NoBorder, ImVec2(10, 10));
                colorReference = _character->mainPalette.convertedColors[j];
                ImGui::ColorButton(_character->mainPalette.paletteName.c_str(), ImVec4(colorReference.x, colorReference.y, colorReference.z, 1), ImGuiColorEditFlags_NoBorder, ImVec2(10, 10));
            }
            ImGui::EndTable();
        }
    }
    ImGui::End();
}

void ImGuiUI::ShowConsoleWindow()
{
    ImGui::Begin("Console");
    for (const auto& message : consoleMessages)
    {
        ImGui::Text("%s", message.c_str());
    }
    ImGui::End();
}