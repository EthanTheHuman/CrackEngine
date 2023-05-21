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
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.3f;
    style.FrameRounding = 2.3f;
    style.ScrollbarRounding = 0;

    style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1., 1.f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4((26.f / 255.f), (26.f / 255.f), (26.f / 255.f), 1.00f);
    //style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
    style.Colors[ImGuiCol_Border] = ImVec4((227.f / 255.f), (171.f / 255.f), (114.f / 255.f), 1.00f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4((26.f / 255.f), (26.f / 255.f), (26.f / 255.f), 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4((26.f / 255.f), (26.f / 255.f), (26.f / 255.f), 1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4((240.f / 255.f), (136.f / 255.f), (40.f / 255.f), 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
    //style.Colors[ImGuiCol_ComboBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.99f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
    //style.Colors[ImGuiCol_Column] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    //style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
    //style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
    //style.Colors[ImGuiCol_CloseButton] = ImVec4(0.50f, 0.50f, 0.90f, 0.50f);
    //style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.70f, 0.70f, 0.90f, 0.60f);
    //style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
    //style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

    //ImGui::StyleColorsClassic();

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

void ImGuiUI::render()
{
    if (glfwGetKey(_window, GLFW_KEY_F1) == GLFW_PRESS)
    {
        if (isPressed == false)
        {
            show_fps = !show_fps;
            isPressed = true;
            /*if (show_fps)
                glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            else
                glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);*/
        }
    }
    else
    {
        if (isPressed == true)
        {
            isPressed = false;
        }
    }

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
        ImGui::Begin("Performance tracking");                          // Create a window called "Hello, world!" and append into it.
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", deltaTime, fps);
        //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Begin("Render Window");                          // Render game window
        ImGui::BeginChild("GameRender");
		ImVec2 wsize = ImGui::GetWindowSize();
        ImGui::Image((ImTextureID)tex, wsize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();

        for (int i = 0; i < characters.size(); i++)
        {
            std::string WindowName = "Character: " + characters[i]->characterName;
			ImGui::Begin(WindowName.c_str());
            ImGui::Text("Character current animation: %i", characters[i]->currentAnim->index);
            ImGui::Text("Character current frame: %i", characters[i]->currentFrame->index);
            ImGui::Text("Character current step: %i", characters[i]->frameCount);
            ImGui::Text("Current palette: %s", characters[i]->mainPalette.paletteFileName.c_str());
            if (characters[i]->paletteList.size() > 0)
            {
                if (ImGui::CollapsingHeader("Palettes"))
                {
                    if (ImGui::SliderInt(characters[i]->mainPalette.paletteName.c_str(), &characters[i]->paletteIndex, 0, characters[i]->paletteList.size() - 1))
                    {
                        characters[i]->setPalette(characters[i]->paletteIndex);
                    }
                    ImGui::BeginTable("Colormap1", 16, ImGuiTableFlags_Borders);
                    for (int j = 0; j < characters[i]->mainPalette.baseColors.size(); j++)
                    {
                        if (j % 16 == 0)
                        {
                            ImGui::TableNextRow();
                        }
                        ImGui::TableNextColumn();
                        auto colorReference = characters[i]->mainPalette.baseColors[j];
                        ImGui::ColorButton("Color Template", ImVec4(colorReference.x, colorReference.y, colorReference.z, 1), ImGuiColorEditFlags_NoBorder, ImVec2(10, 10));
                        colorReference = characters[i]->mainPalette.convertedColors[j];
                        ImGui::ColorButton(characters[i]->mainPalette.paletteName.c_str(), ImVec4(colorReference.x, colorReference.y, colorReference.z, 1), ImGuiColorEditFlags_NoBorder, ImVec2(10, 10));
                    }
                    ImGui::EndTable();
                }
            }
            ImGui::End();
			
        }
        ImGui::SetNextWindowSize(ImVec2(350, 560), ImGuiCond_FirstUseEver);
		bool p_open = true;
        if (!ImGui::Begin("Example: Custom rendering", &p_open))
        {
            ImGui::End();
            return;
        }

        // Tip: If you do a lot of custom rendering, you probably want to use your own geometrical types and benefit of overloaded operators, etc. 
        // Define IM_VEC2_CLASS_EXTRA in imconfig.h to create implicit conversions between your types and ImVec2/ImVec4. 
        // ImGui defines overloaded operators but they are internal to imgui.cpp and not exposed outside (to avoid messing with your types) 
        // In this example we are not using the maths operators! 
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        // Primitives 
        ImGui::Text("Primitives");
        static int characterNo = 0;
        static int animNo = 1;
        static int frameNo = 1;
        static float sz = 10.0f;
        static float thickness = 2.0f;
        static float boxTransparency = 0.2f;
        static ImVec4 hurtboxCol = ImVec4(0.2f, 0.2f, 1.0f, 1.0f);
        static ImVec4 hitboxCol = ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
        static ImVec4 pushboxCol = ImVec4(0.2f, 1.0f, 0.2f, 1.0f);
        if (ImGui::DragInt("Character Number", &characterNo, 0.1f, 0, characters.size() - 1, "%.0f"))
        {
			// reset stuff
            animNo = 0;
            frameNo = 0;
        }
		
        static int animSliderIndex = 1;
        static int frameSliderIndex = 1;
		
        std::vector<int> animKeys;
        for (const auto& pair : characters[characterNo]->animList) {
            animKeys.push_back(pair.first);
        }
		
        std::vector<int> frameKeys;
        for (const auto& pair : characters[characterNo]->animList[animNo].frameList) {
            frameKeys.push_back(pair.first);
        }
		
        std::string animLabel = "Animation: " + std::to_string(animNo);
        if (ImGui::SliderInt(animLabel.c_str(), &animSliderIndex, 0, animKeys.size() - 1)) {
            // Slider moved, get new animation id
            frameSliderIndex = 1;
			animNo = animKeys[animSliderIndex];
            frameNo = characters[characterNo]->animList[animNo].frameList[0].index;
            // Now you can access the selected animation with animations[animationId]
        }
		
		std::string frameLabel = "Frame: " + std::to_string(frameNo);
        if (ImGui::SliderInt(frameLabel.c_str(), &frameSliderIndex, 0, frameKeys.size() - 1)) {
            frameNo = frameKeys[frameSliderIndex];
			// do stuff
        }
		
        ImGui::DragFloat("Cursor Size", &sz, 0.2f, 2.0f, 72.0f, "%.0f");
        ImGui::DragFloat("Outline Thickness", &thickness, 0.05f, 1.0f, 8.0f, "%.02f");
        ImGui::ColorEdit4("Hurtbox Color", &hurtboxCol.x);
        ImGui::ColorEdit4("Hitbox Color", &hitboxCol.x);
        ImGui::ColorEdit4("Pushbox Color", &pushboxCol.x);
        ImGui::DragFloat("Box transparency", &boxTransparency, 0.01f, .0f, 1.0f, "%.02f");
        ImGui::Separator();
        static float canvasWidth = 640.0f;
        static float canvasHeight = 480.0f;
		static ImVec2 originPos = ImVec2(320, 240);
        static float canvasScale = 1.f;
        //ImGui::DragFloat("Canvas Width", &canvasWidth, 0.2f, 2.0f, 2500.0f, "%.0f");
        //ImGui::DragFloat("Canvas Height", &canvasHeight, 0.2f, 2.0f, 2500.0f, "%.0f");
        {
			// create colors
            const ImVec2 p = ImGui::GetCursorScreenPos();
            const ImU32 hbcol = ImColor(hurtboxCol);
            ImVec4 hurtboxColTrans = hurtboxCol;
            hurtboxColTrans.w *= boxTransparency;
            const ImU32 hbcoltrans = ImColor(hurtboxColTrans);
            const ImU32 htbcol = ImColor(hitboxCol);
            ImVec4 hitboxColTrans = hitboxCol;
            hitboxColTrans.w *= boxTransparency;
            const ImU32 htbcoltrans = ImColor(hitboxColTrans);
            const ImU32 pbcol = ImColor(pushboxCol);
            ImVec4 pushboxColTrans = pushboxCol;
            pushboxColTrans.w *= boxTransparency;
            const ImU32 pbcoltrans = ImColor(pushboxColTrans);
			
			// If the user pans around with the right mouse button
			if (ImGui::IsMouseDragging(1))
			{
				originPos.x += ImGui::GetIO().MouseDelta.x;
				originPos.y += ImGui::GetIO().MouseDelta.y;
			}
			// If the user zooms with the mouse wheel
			if (ImGui::GetIO().MouseWheel != 0.0f)
			{
				// Zoom in or out  
				canvasScale += ImGui::GetIO().MouseWheel * 0.1f;
				// Clamp the zoom
				if (canvasScale > 20.0f)
				{
                    canvasScale = 20.0f;
				}
                if (canvasScale < 0.1f)
                {
					canvasScale = 0.1f;
                }
			}
			
            static ImVector<ImVec2> points;
            static bool adding_line = false;
            ImGui::Text("Hitbox Wizard");
            if (ImGui::Button("Clear")) points.clear();
            if (points.Size >= 2) { ImGui::SameLine(); if (ImGui::Button("Undo")) { points.pop_back(); points.pop_back(); } }
            ImGui::Text("Left-click and drag to add lines,\nRight-click to undo");

            // Here we are using InvisibleButton() as a convenience to 1) advance the cursor and 2) allows us to use IsItemHovered() 
            // But you can also draw directly and poll mouse/keyboard by yourself. You can manipulate the cursor using GetCursorPos() and SetCursorPos(). 
            // If you only use the ImDrawList API, you can notify the owner window of its extends by using SetCursorPos(max). 
            ImVec2 canvas_pos = ImGui::GetCursorScreenPos();            // ImDrawList API uses screen coordinates! 
            ImVec2 canvas_size = ImGui::GetContentRegionAvail();       // Resize canvas to what's available 
            //ImVec2 canvas_size = ImVec2(canvasWidth, canvasHeight);        // Resize canvas based on the variables
            if (canvas_size.x < 50.0f) canvas_size.x = 50.0f;
            if (canvas_size.y < 50.0f) canvas_size.y = 50.0f;
			// Canvas background
            draw_list->AddRectFilledMultiColor(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), IM_COL32(50, 50, 50, 255), IM_COL32(50, 50, 60, 255), IM_COL32(60, 60, 70, 255), IM_COL32(50, 50, 60, 255));
            draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), IM_COL32(255, 255, 255, 255));

            bool adding_preview = false;
            ImGui::InvisibleButton("canvas", canvas_size);
			// Get the mouse pos relative to the OriginPos
			ImVec2 origin_pos = ImVec2(canvas_pos.x + originPos.x, canvas_pos.y + originPos.y);
			ImVec2 mouse_pos_from_origin = ImVec2((int)((ImGui::GetIO().MousePos.x - origin_pos.x) / canvasScale), (int)((ImGui::GetIO().MousePos.y - origin_pos.y) / canvasScale));
            ImVec2 mouse_pos_in_canvas = ImVec2((int)((ImGui::GetIO().MousePos.x - canvas_pos.x) / canvasScale), (int)((ImGui::GetIO().MousePos.y - canvas_pos.y) / canvasScale));
            if (adding_line)
            {
                adding_preview = true;
                points.push_back(mouse_pos_from_origin);
                if (!ImGui::IsMouseDown(0))
                    adding_line = adding_preview = false;
            }
            if (ImGui::IsItemHovered())
            {
                if (!adding_line && ImGui::IsMouseClicked(0))
                {
                    points.push_back(mouse_pos_from_origin);
                    adding_line = true;
                }
				// If the user presses ctrl+z
				if (ImGui::IsKeyPressed(90) && ImGui::GetIO().KeyCtrl && !points.empty())
                {
                    adding_line = adding_preview = false;
                    points.pop_back();
                    points.pop_back();
                }
            }
            draw_list->PushClipRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), true);      // clip lines within the canvas (if we resize it, etc.) 
			// TODO: Character image origin is based on top left in imgui, ours is based on bottom left, do some conversion

            ImGui::Text("Left-click and drag to add lines,\nRight-click to undo");
            Frame* currentFrame = &characters[characterNo]->animList[animNo].frameList[frameNo];
			// Get the sprite in a position relative to the origin
            ImVec2 characterMin = ImVec2(
                origin_pos.x + ((currentFrame->xPos * currentFrame->xScale) * canvasScale),
                origin_pos.y - ((currentFrame->yPos * currentFrame->yScale) * canvasScale) - ((currentFrame->spriteHeight * currentFrame->yScale) * canvasScale)
            );
            ImVec2 characterMax = characterMin;
            characterMax.x += ((currentFrame->spriteWidth * currentFrame->xScale) * canvasScale);
            characterMax.y += ((currentFrame->spriteHeight * currentFrame->yScale) * canvasScale);
			// The origin is scaled and compared to the origin
            draw_list->AddImage((void*)currentFrame->spriteImage, characterMin, characterMax, ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 255));
			// draw origin
            {
				float x = origin_pos.x;
				float y = origin_pos.y;
                float cursorSize = sz * canvasScale;
                float H = cursorSize * 0.5f;
                float cross_extent = (cursorSize - thickness) * 0.5f - 0.5f;
                draw_list->AddLine(ImVec2(x, y - cursorSize), ImVec2(x, y + cursorSize), IM_COL32(255, 255, 255, 255), thickness);  // vertical outer
                draw_list->AddLine(ImVec2(x - cursorSize, y), ImVec2(x + cursorSize, y), IM_COL32(255, 255, 255, 255), thickness);  // horizontal outer
                draw_list->AddLine(ImVec2(x, y - H - cross_extent), ImVec2(x, y + H + cross_extent), IM_COL32(0, 0, 0, 255), thickness / 2); // vertical inner
                draw_list->AddLine(ImVec2(x - H - cross_extent, y), ImVec2(x + H + cross_extent, y), IM_COL32(0, 0, 0, 255), thickness / 2); // horizontal inner
            }
            for (int i = 0; i < points.Size - 1; i += 2)
            {
                draw_list->AddRectFilled(ImVec2(origin_pos.x + (points[i].x * canvasScale), origin_pos.y + (points[i].y * canvasScale)), ImVec2(origin_pos.x + (points[i + 1].x * canvasScale), origin_pos.y + (points[i + 1].y * canvasScale)), hbcoltrans, 0.0f, ImDrawCornerFlags_All);
                draw_list->AddRect(ImVec2(origin_pos.x + (points[i].x * canvasScale), origin_pos.y + (points[i].y * canvasScale)), ImVec2(origin_pos.x + (points[i + 1].x * canvasScale), origin_pos.y + (points[i + 1].y * canvasScale)), hbcol, 0.0f, ImDrawCornerFlags_All, thickness);
            }
            draw_list->PopClipRect();
            if (adding_preview)
                points.pop_back();
        }
        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiUI::addCharacter(AnimManager* _character)
{
    characters.push_back(_character);
}

