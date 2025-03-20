#include "AnimManagerEditorUI.h"
#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_impl_glfw.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"
#include "Engine/CrkBox.h"
#include "Gameplay/AnimAction.h"
#include "Gameplay/Anim Actions/Act_SetXVel.h"
#include "Gameplay/Anim Actions/Act_SetYVel.h"
#include "Gameplay/Anim Actions/Act_AddXVel.h"
#include "Gameplay/Anim Actions/Act_AddYVel.h"
#include "Gameplay/Anim Actions/Act_SetXAccel.h"
#include "Gameplay/Anim Actions/Act_SetYAccel.h"
#include "Gameplay/Anim Actions/Act_AddXAccel.h"
#include "Gameplay/Anim Actions/Act_AddYAccel.h"
#include "Gameplay/Anim Actions/Act_MoveDelta.h"
#include "Gameplay/Anim Actions/Act_PlaySound.h"
#include <algorithm>

AnimManagerEditorUI::AnimManagerEditorUI() {}

AnimManagerEditorUI::~AnimManagerEditorUI() {}

void AnimManagerEditorUI::render(std::vector<AnimManager*>& characters) {
    if (characters.empty()) return;

    ImGui::Begin("Character tool");
    ImGui::Columns(2, "mycolumns"); // 2-Columns setup
    
    static int characterNo = 0;
    static float spriteXPos = 0;
    static float spriteYPos = 0;
    static CrkBox* currentRectangle = nullptr;
    
    // Get current character and frame info
    AnimManager* character = characters[characterNo];
    
    // Initialize animation and frame numbers if needed
    if (animNo == 0) {
        std::vector<int> animKeys = getAnimKeysList(character);
        if (!animKeys.empty()) {
            animNo = animKeys[0];
            std::vector<int> frameKeys = getFrameKeysList(character, animNo);
            if (!frameKeys.empty()) {
                frameNo = frameKeys[0];
            }
        }
    }

    Frame* currentFrame = refreshCharacterInfo(character, animNo, frameNo, spriteXPos, spriteYPos, &points, currentRectangle);

    // Render left panel with controls
    renderLeftPanel(character, characterNo, characters.size());

    // Render canvas with character sprite and hitboxes
    renderCanvas(character, currentFrame);

    ImGui::End();
}

void AnimManagerEditorUI::renderLeftPanel(AnimManager* character, int& characterNo, int characterCount) {
    ImGui::SetNextWindowContentSize(ImVec2(0.0f, 0.0f));
    ImGui::BeginChild("Left Side", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

    // Save controls
    if (ImGui::Button("Save Character")) {
        saveCharacter(character);
    }
    ImGui::SameLine();
    ImGui::Text("File: %s", character->getDataFilePath().c_str());
    ImGui::Separator();

    // Character selection
    if (ImGui::SliderInt("Character Number", &characterNo, 0, characterCount - 1)) {
        // Reset animation and frame indices when character changes
        std::vector<int> newAnimKeys = getAnimKeysList(character);
        if (!newAnimKeys.empty()) {
            animNo = newAnimKeys[0];
            std::vector<int> newFrameKeys = getFrameKeysList(character, animNo);
            if (!newFrameKeys.empty()) {
                frameNo = newFrameKeys[0];
            }
        }
    }

    // Animation selection
    std::vector<int> animKeys = getAnimKeysList(character);
    if (!animKeys.empty()) {
        int animIndex = 0;
        for (int i = 0; i < animKeys.size(); i++) {
            if (animKeys[i] == animNo) {
                animIndex = i;
                break;
            }
        }

        if (ImGui::SliderInt("Animation Number", &animIndex, 0, animKeys.size() - 1)) {
            animNo = animKeys[animIndex];
            // Reset frame index when animation changes
            std::vector<int> newFrameKeys = getFrameKeysList(character, animNo);
            if (!newFrameKeys.empty()) {
                frameNo = newFrameKeys[0];
            }
        }

        // Frame selection
        std::vector<int> frameKeys = getFrameKeysList(character, animNo);
        if (!frameKeys.empty()) {
            int frameIndex = 0;
            for (int i = 0; i < frameKeys.size(); i++) {
                if (frameKeys[i] == frameNo) {
                    frameIndex = i;
                    break;
                }
            }

            if (ImGui::SliderInt("Frame Number", &frameIndex, 0, frameKeys.size() - 1)) {
                frameNo = frameKeys[frameIndex];
            }

            // Get current frame for editing
            Frame& currentFrame = character->animList[animNo].frameList[frameNo];

            // Sprite Position and Scale controls
            ImGui::Separator();
            ImGui::Text("Sprite Transform");
            
            // Position controls
            float xPos = static_cast<float>(currentFrame.xPos);
            float yPos = static_cast<float>(currentFrame.yPos);
            if (ImGui::DragFloat("X Position", &xPos, 0.5f)) {
                currentFrame.xPos = static_cast<int>(xPos);
            }
            if (ImGui::DragFloat("Y Position", &yPos, 0.5f)) {
                currentFrame.yPos = static_cast<int>(yPos);
            }
            
            // Scale controls
            float xScaleTemp = static_cast<float>(currentFrame.xScale);
            float yScaleTemp = static_cast<float>(currentFrame.yScale);
            if (ImGui::DragFloat("X Scale", &xScaleTemp, 0.25f, -100.0f, 100.0f, "%.0f")) {
                currentFrame.xScale = static_cast<int>(xScaleTemp);
            }
            if (ImGui::DragFloat("Y Scale", &yScaleTemp, 0.25f, -100.0f, 100.0f, "%.0f")) {
                currentFrame.yScale = static_cast<int>(yScaleTemp);
            }

            // Flip controls
            if (ImGui::Button("Flip Horizontally")) {
                currentFrame.xScale *= -1;
            }
            ImGui::SameLine();
            if (ImGui::Button("Flip Vertically")) {
                currentFrame.yScale *= -1;
            }
        }
    }

    // Animation controls
    ImGui::Separator();
    ImGui::Text("Box Controls");
    ImGui::DragFloat("Cursor Size", &sz, 0.2f, 2.0f, 72.0f, "%.0f");
    ImGui::DragFloat("Outline Thickness", &thickness, 0.05f, 1.0f, 8.0f, "%.02f");
    ImGui::ColorEdit4("Hurtbox Color", &hurtboxCol.x);
    ImGui::ColorEdit4("Hitbox Color", &hitboxCol.x);
    ImGui::ColorEdit4("Pushbox Color", &pushboxCol.x);
    ImGui::DragFloat("Box transparency", &boxTransparency, 0.01f, .0f, 1.0f, "%.02f");

    // Animation Properties
    if (ImGui::CollapsingHeader("Animation Properties")) {
        auto& currentAnim = character->animList[animNo];

        // Add name input field
        char nameBuffer[256];
        strncpy_s(nameBuffer, currentAnim.name.c_str(), sizeof(nameBuffer) - 1);
        if (ImGui::InputText("Animation Name", nameBuffer, sizeof(nameBuffer))) {
            currentAnim.name = nameBuffer;
        }

        // Update the UI controls and apply changes immediately
        if (ImGui::Checkbox("Keep Velocity", &currentAnim.keepVelocity)) {
            // Value is automatically updated by ImGui
        }
        if (ImGui::Checkbox("Keep Acceleration", &currentAnim.keepAcceleration)) {
            // Value is automatically updated by ImGui
        }
        if (ImGui::InputInt("Fallback Index", &currentAnim.fallbackindex)) {
            // Value is automatically updated by ImGui
        }
    }
    
    // Animation Actions
    if (ImGui::CollapsingHeader("Animation Actions")) {
        if (Frame* currentFrame = refreshCharacterInfo(character, animNo, frameNo, spriteXPos, spriteYPos, &points, nullptr)) {
            renderAnimationActions(currentFrame);
        }
    }
    
    // Input Actions
    if (ImGui::CollapsingHeader("Input Actions")) {
        if (Frame* currentFrame = refreshCharacterInfo(character, animNo, frameNo, spriteXPos, spriteYPos, &points, nullptr)) {
            renderInputActions(currentFrame);
        }
    }
    
    // Palette Editor
    if (ImGui::CollapsingHeader("Palette Editor")) {
        renderPaletteEditor(character);
    }

    ImGui::EndChild();
    ImGui::NextColumn();
}

void AnimManagerEditorUI::renderCanvas(AnimManager* character, Frame* currentFrame) {
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();
    canvas_size.y -= 30;

    static ImVec2 originPos = ImVec2(canvas_size.x * 0.5f, canvas_size.y * 0.75f);
    bool isInCanvas = ImGui::IsMouseHoveringRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y));

    // Process canvas inputs (pan, zoom)
    if (isInCanvas) {
        // Handle zoom with mouse wheel
        if (ImGui::GetIO().MouseWheel != 0.0f) {
            canvasScale = std::clamp(canvasScale + ImGui::GetIO().MouseWheel * 0.1f, 0.1f, 20.0f);
        }

        // Update tempX and tempY with current frame values when mouse interaction starts
        static float tempX;
        static float tempY;
        static bool isDragging = false;

        if (ImGui::IsMouseDragging(1) || ImGui::IsMouseDragging(2)) {
            if (!isDragging) {
                // Initialize temp values when starting to drag
                tempX = currentFrame->xPos;
                tempY = currentFrame->yPos;
                isDragging = true;
            }
            processCanvasInputs(originPos, tempX, tempY, canvasScale);
            currentFrame->xPos = (int)tempX;
            currentFrame->yPos = (int)tempY;
        } else {
            isDragging = false;
        }

        // Handle hitbox creation
        ImVec2 mouse_pos_in_canvas = ImVec2(
            (ImGui::GetMousePos().x - canvas_pos.x - originPos.x) / canvasScale,
            -(ImGui::GetMousePos().y - canvas_pos.y - originPos.y) / canvasScale
        );
        handleHitboxCreation(currentFrame, mouse_pos_in_canvas);
    }

    // Setup canvas
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilledMultiColor(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y),
        IM_COL32(50, 50, 50, 255), IM_COL32(50, 50, 60, 255),
        IM_COL32(60, 60, 70, 255), IM_COL32(50, 50, 60, 255));
    draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y),
        IM_COL32(255, 255, 255, 255));

    // Draw character sprite and hitboxes
    ImVec2 origin_pos = ImVec2(canvas_pos.x + originPos.x, canvas_pos.y + originPos.y);
    draw_list->PushClipRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), true);
    
    drawCharacterSprite(draw_list, currentFrame, origin_pos, canvasScale);
    drawHitboxes(draw_list, currentFrame, origin_pos, canvasScale);

    // Draw current hitbox being created
    if (adding_line && points.size() == 1) {
        ImVec2 p1 = ImVec2(
            origin_pos.x + points[0].x * canvasScale,
            origin_pos.y - points[0].y * canvasScale
        );
        ImVec2 p2 = ImGui::GetMousePos();
        draw_list->AddRect(p1, p2, ImColor(hitboxCol), 0.0f, ImDrawCornerFlags_All, thickness);
    }

    // Draw origin marker (plus symbol at 0,0) on top of everything
    float markerSize = 10.0f;  // Size of the plus symbol
    float gapSize = 2.0f;     // Size of the gap in the center
    ImU32 markerColor = IM_COL32(255, 255, 0, 255);  // Yellow color
    
    // Draw horizontal line with gap in center
    draw_list->AddLine(
        ImVec2(origin_pos.x - markerSize, origin_pos.y),
        ImVec2(origin_pos.x - gapSize, origin_pos.y),
        markerColor,
        2.0f
    );
    draw_list->AddLine(
        ImVec2(origin_pos.x + gapSize, origin_pos.y),
        ImVec2(origin_pos.x + markerSize, origin_pos.y),
        markerColor,
        2.0f
    );

    // Draw vertical line with gap in center
    draw_list->AddLine(
        ImVec2(origin_pos.x, origin_pos.y - markerSize),
        ImVec2(origin_pos.x, origin_pos.y - gapSize),
        markerColor,
        2.0f
    );
    draw_list->AddLine(
        ImVec2(origin_pos.x, origin_pos.y + gapSize),
        ImVec2(origin_pos.x, origin_pos.y + markerSize),
        markerColor,
        2.0f
    );
    
    draw_list->PopClipRect();

    // Add controls for hitbox creation
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + canvas_size.y);
    if (ImGui::Button(adding_line ? "Cancel Hitbox" : "New Hitbox")) {
        if (adding_line) {
            points.clear();
            adding_line = false;
        }
        adding_line = !adding_line;
    }
    ImGui::SameLine();
    ImGui::Text("Right click and drag to pan, Middle click and drag to move sprite, Scroll to zoom");
    ImGui::SameLine();
    ImGui::Text("(Zoom: %.2fx)", canvasScale);
}

void AnimManagerEditorUI::processCanvasInputs(ImVec2& originPos, float& spriteXPos, float& spriteYPos, float& canvasScale) {
    if (ImGui::IsMouseDragging(1)) {
        originPos.x += (int)(ImGui::GetIO().MouseDelta.x);
        originPos.y += (int)(ImGui::GetIO().MouseDelta.y);
    }
    if (ImGui::IsMouseDragging(2)) {
        spriteXPos += (ImGui::GetIO().MouseDelta.x / canvasScale);
        spriteYPos -= (ImGui::GetIO().MouseDelta.y / canvasScale);
    }
}

void AnimManagerEditorUI::drawCharacterSprite(ImDrawList* drawList, Frame* currentFrame, const ImVec2& originPos, float canvasScale) {
    ImVec2 characterMin = ImVec2(
        originPos.x + (currentFrame->xPos * canvasScale),
        originPos.y - ((currentFrame->spriteHeight * abs(currentFrame->yScale)) * canvasScale) - 
                     (currentFrame->yPos * canvasScale)
    );
    ImVec2 characterMax = characterMin;
    
    if (currentFrame->xScale < 0) {
        characterMin.x -= (currentFrame->spriteWidth * abs(currentFrame->xScale)) * canvasScale;
    } else {
        characterMax.x += (currentFrame->spriteWidth * abs(currentFrame->xScale)) * canvasScale;
    }

    if (currentFrame->yScale < 0) {
        characterMax.y += (currentFrame->spriteHeight * abs(currentFrame->yScale)) * canvasScale;
    } else {
        characterMax.y += (currentFrame->spriteHeight * currentFrame->yScale) * canvasScale;
    }
    
    drawList->AddImage((void*)currentFrame->spriteImage, characterMin, characterMax,
        ImVec2(0, currentFrame->yScale < 0 ? 1 : 0), ImVec2(1, currentFrame->yScale < 0 ? 0 : 1), IM_COL32(255, 255, 255, 255));
}

void AnimManagerEditorUI::drawHitboxes(ImDrawList* drawList, Frame* currentFrame, const ImVec2& originPos, float canvasScale) {
    ImVec4 hitboxColTrans = hitboxCol;
    hitboxColTrans.w *= boxTransparency;
    ImU32 hitboxColorFill = ImColor(hitboxColTrans);
    ImU32 hitboxColorOutline = ImColor(hitboxCol);

    // Draw existing hitboxes
    if (currentFrame->hitbox != nullptr) {
        drawList->AddRectFilled(
            ImVec2(originPos.x + (currentFrame->hitbox->left * canvasScale),
                   originPos.y - (currentFrame->hitbox->up * canvasScale)),
            ImVec2(originPos.x + (currentFrame->hitbox->right * canvasScale),
                   originPos.y - (currentFrame->hitbox->down * canvasScale)),
            hitboxColorFill);
        drawList->AddRect(
            ImVec2(originPos.x + (currentFrame->hitbox->left * canvasScale),
                   originPos.y - (currentFrame->hitbox->up * canvasScale)),
            ImVec2(originPos.x + (currentFrame->hitbox->right * canvasScale),
                   originPos.y - (currentFrame->hitbox->down * canvasScale)),
            hitboxColorOutline, 0.0f, ImDrawCornerFlags_All, thickness);
    }

    // Draw additional hitboxes
    for (const auto& box : currentFrame->hitboxes) {
        drawList->AddRectFilled(
            ImVec2(originPos.x + (box->left * canvasScale),
                   originPos.y - (box->up * canvasScale)),
            ImVec2(originPos.x + (box->right * canvasScale),
                   originPos.y - (box->down * canvasScale)),
            hitboxColorFill);
        drawList->AddRect(
            ImVec2(originPos.x + (box->left * canvasScale),
                   originPos.y - (box->up * canvasScale)),
            ImVec2(originPos.x + (box->right * canvasScale),
                   originPos.y - (box->down * canvasScale)),
            hitboxColorOutline, 0.0f, ImDrawCornerFlags_All, thickness);
    }
}

Frame* AnimManagerEditorUI::refreshCharacterInfo(AnimManager* character, int animIndex, int frameIndex,
    float& spriteXPos, float& spriteYPos, ImVector<ImVec2>* points, CrkBox* rect) {
    Frame* frame = &character->animList[animIndex].frameList[frameIndex];
    spriteXPos = frame->xPos;
    spriteYPos = frame->yPos;
    rect = frame->hitbox;
    points->clear();
    return frame;
}

std::vector<int> AnimManagerEditorUI::getAnimKeysList(AnimManager* character) {
    std::vector<int> animKeys;
    for (const auto& pair : character->animList) {
        if (pair.first != 0)
            animKeys.push_back(pair.first);
    }
    return animKeys;
}

std::vector<int> AnimManagerEditorUI::getFrameKeysList(AnimManager* character, int animIndex) {
    std::vector<int> frameKeys;
    for (const auto& pair : character->animList[animIndex].frameList) {
        if (pair.first != 0)
            frameKeys.push_back(pair.first);
    }
    return frameKeys;
}

void AnimManagerEditorUI::addConsoleMessage(const std::string& message) {
    consoleMessages.push_back(message);
}

void AnimManagerEditorUI::handleHitboxCreation(Frame* currentFrame, const ImVec2& mousePos) {
    if (!adding_line) return;  // Only handle input if we're in hitbox creation mode

    if (ImGui::IsMouseClicked(0)) {
        if (points.empty()) {
            // Start new hitbox
            points.push_back(mousePos);
        } else {
            // Complete hitbox
            points.push_back(mousePos);
            
            // Create hitbox from points
            float left = points[0].x;
            float right = points[1].x;
            float up = points[0].y;
            float down = points[1].y;
            
            // Normalize coordinates
            if (left > right) std::swap(left, right);
            if (down > up) std::swap(up, down);  // Note: Y is inverted in our coordinate system
            
            // Create and add the hitbox
            CrkBox* newBox = new CrkBox();
            newBox->left = left;
            newBox->right = right;
            newBox->up = up;
            newBox->down = down;
            
            currentFrame->hitboxes.push_back(newBox);
            
            // Reset state
            points.clear();
            adding_line = false;
        }
    }
    
    // Cancel hitbox creation with right click
    if (ImGui::IsMouseClicked(1) && adding_line) {
        points.clear();
        adding_line = false;
    }
}

void AnimManagerEditorUI::saveCharacter(AnimManager* character) {
    character->saveToXml(character->getDataFilePath().c_str());
    addConsoleMessage("Character saved to " + character->getDataFilePath());
}

void AnimManagerEditorUI::renderAnimationActions(Frame* currentFrame) {
    // List existing animation actions
    for (int i = 0; i < currentFrame->animActions.size(); i++) {
        auto* action = currentFrame->animActions[i];
        ImGui::PushID(i);
        
        if (ImGui::TreeNode(action->type.c_str())) {
            editAnimationAction(action);
            if (ImGui::Button("Remove Action")) {
                removeAnimationAction(currentFrame, i);
            }
            ImGui::TreePop();
        }
        
        ImGui::PopID();
    }
    
    // Add new animation action
    if (ImGui::Button("Add Animation Action")) {
        ImGui::OpenPopup("Add Animation Action");
    }
    
    if (ImGui::BeginPopup("Add Animation Action")) {
        static const char* actionTypes[] = {
            "SetXVel", "SetYVel", "AddXVel", "AddYVel",
            "SetXAccel", "SetYAccel", "AddXAccel", "AddYAccel",
            "MoveDelta", "PlaySound"
        };
        
        for (const char* type : actionTypes) {
            if (ImGui::Selectable(type)) {
                currentAnimAction.type = type;
                addAnimationAction(currentFrame);
            }
        }
        ImGui::EndPopup();
    }
}

void AnimManagerEditorUI::renderInputActions(Frame* currentFrame) {
    // List existing input actions
    for (int i = 0; i < currentFrame->inputActions.size(); i++) {
        auto& action = currentFrame->inputActions[i];
        ImGui::PushID(i);
        
        if (ImGui::TreeNode(("Input Action " + std::to_string(i + 1)).c_str())) {
            editInputAction(action);
            if (ImGui::Button("Remove Action")) {
                removeInputAction(currentFrame, i);
            }
            ImGui::TreePop();
        }
        
        ImGui::PopID();
    }
    
    // Add new input action
    if (ImGui::Button("Add Input Action")) {
        addInputAction(currentFrame);
    }
}

void AnimManagerEditorUI::renderPaletteEditor(AnimManager* character) {
    // Template and palette file selection
    if (ImGui::InputText("Template File", currentPalette.templateFile, sizeof(currentPalette.templateFile))) {
        // Update template file
    }
    if (ImGui::InputText("Palette File", currentPalette.paletteFile, sizeof(currentPalette.paletteFile))) {
        // Update palette file
    }
    
    // List existing palettes
    for (int i = 0; i < character->paletteList.size(); i++) {
        ImGui::PushID(i);
        if (ImGui::TreeNode(("Palette " + std::to_string(i + 1)).c_str())) {
            editPalette(i);
            if (ImGui::Button("Remove Palette")) {
                removePalette(character, i);
            }
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
    
    // Add new palette
    if (ImGui::Button("Add Palette")) {
        addPalette(character);
    }
}

void AnimManagerEditorUI::addAnimationAction(Frame* currentFrame) {
    AnimAction* newAction = nullptr;
    
    // Create the appropriate action type
    if (strcmp(currentAnimAction.type.c_str(), "SetXVel") == 0) {
        auto* action = new Act_SetXVel();
        action->xVel = currentAnimAction.xVel;
        newAction = action;
    }
    else if (strcmp(currentAnimAction.type.c_str(), "SetYVel") == 0) {
        auto* action = new Act_SetYVel();
        action->yVel = currentAnimAction.yVel;
        newAction = action;
    }
    else if (strcmp(currentAnimAction.type.c_str(), "AddXVel") == 0) {
        auto* action = new Act_AddXVel();
        action->xVel = currentAnimAction.xVel;
        newAction = action;
    }
    else if (strcmp(currentAnimAction.type.c_str(), "AddYVel") == 0) {
        auto* action = new Act_AddYVel();
        action->yVel = currentAnimAction.yVel;
        newAction = action;
    }
    else if (strcmp(currentAnimAction.type.c_str(), "SetXAccel") == 0) {
        auto* action = new Act_SetXAccel();
        action->xAccel = currentAnimAction.xAccel;
        newAction = action;
    }
    else if (strcmp(currentAnimAction.type.c_str(), "SetYAccel") == 0) {
        auto* action = new Act_SetYAccel();
        action->yAccel = currentAnimAction.yAccel;
        newAction = action;
    }
    else if (strcmp(currentAnimAction.type.c_str(), "AddXAccel") == 0) {
        auto* action = new Act_AddXAccel();
        action->xAccel = currentAnimAction.xAccel;
        newAction = action;
    }
    else if (strcmp(currentAnimAction.type.c_str(), "AddYAccel") == 0) {
        auto* action = new Act_AddYAccel();
        action->yAccel = currentAnimAction.yAccel;
        newAction = action;
    }
    else if (strcmp(currentAnimAction.type.c_str(), "MoveDelta") == 0) {
        auto* action = new Act_MoveDelta();
        action->xDelta = currentAnimAction.xDelta;
        action->yDelta = currentAnimAction.yDelta;
        newAction = action;
    }
    else if (strcmp(currentAnimAction.type.c_str(), "PlaySound") == 0) {
        auto* action = new Act_PlaySound();
        action->filename = currentAnimAction.soundFilename;
        action->volume = currentAnimAction.soundVolume;
        newAction = action;
    }
    
    if (newAction) {
        newAction->type = currentAnimAction.type;
        currentFrame->animActions.push_back(newAction);
    }
}

void AnimManagerEditorUI::editAnimationAction(AnimAction* action) {
    // Load current values into currentAnimAction first
    currentAnimAction.stepCount = action->stepCount;
    currentAnimAction.frequency = action->frequency;
    
    // Common fields
    if (ImGui::InputText("Frequency", &currentAnimAction.frequency[0], currentAnimAction.frequency.capacity())) {
        action->frequency = currentAnimAction.frequency;
    }
    if (ImGui::InputInt("Step Count", &currentAnimAction.stepCount)) {
        action->stepCount = currentAnimAction.stepCount;
    }
    
    // Type-specific fields
    if (auto* setXVel = dynamic_cast<Act_SetXVel*>(action)) {
        currentAnimAction.xVel = setXVel->xVel;
        if (ImGui::DragFloat("X Velocity", &currentAnimAction.xVel, 0.1f)) {
            setXVel->xVel = currentAnimAction.xVel;
        }
    }
    else if (auto* setYVel = dynamic_cast<Act_SetYVel*>(action)) {
        currentAnimAction.yVel = setYVel->yVel;
        if (ImGui::DragFloat("Y Velocity", &currentAnimAction.yVel, 0.1f)) {
            setYVel->yVel = currentAnimAction.yVel;
        }
    }
    else if (auto* addXVel = dynamic_cast<Act_AddXVel*>(action)) {
        currentAnimAction.xVel = addXVel->xVel;
        if (ImGui::DragFloat("X Velocity", &currentAnimAction.xVel, 0.1f)) {
            addXVel->xVel = currentAnimAction.xVel;
        }
    }
    else if (auto* addYVel = dynamic_cast<Act_AddYVel*>(action)) {
        currentAnimAction.yVel = addYVel->yVel;
        if (ImGui::DragFloat("Y Velocity", &currentAnimAction.yVel, 0.1f)) {
            addYVel->yVel = currentAnimAction.yVel;
        }
    }
    else if (auto* setXAccel = dynamic_cast<Act_SetXAccel*>(action)) {
        currentAnimAction.xAccel = setXAccel->xAccel;
        if (ImGui::DragFloat("X Acceleration", &currentAnimAction.xAccel, 0.1f)) {
            setXAccel->xAccel = currentAnimAction.xAccel;
        }
    }
    else if (auto* setYAccel = dynamic_cast<Act_SetYAccel*>(action)) {
        currentAnimAction.yAccel = setYAccel->yAccel;
        if (ImGui::DragFloat("Y Acceleration", &currentAnimAction.yAccel, 0.1f)) {
            setYAccel->yAccel = currentAnimAction.yAccel;
        }
    }
    else if (auto* addXAccel = dynamic_cast<Act_AddXAccel*>(action)) {
        currentAnimAction.xAccel = addXAccel->xAccel;
        if (ImGui::DragFloat("X Acceleration", &currentAnimAction.xAccel, 0.1f)) {
            addXAccel->xAccel = currentAnimAction.xAccel;
        }
    }
    else if (auto* addYAccel = dynamic_cast<Act_AddYAccel*>(action)) {
        currentAnimAction.yAccel = addYAccel->yAccel;
        if (ImGui::DragFloat("Y Acceleration", &currentAnimAction.yAccel, 0.1f)) {
            addYAccel->yAccel = currentAnimAction.yAccel;
        }
    }
    else if (auto* moveDelta = dynamic_cast<Act_MoveDelta*>(action)) {
        currentAnimAction.xDelta = moveDelta->xDelta;
        currentAnimAction.yDelta = moveDelta->yDelta;
        if (ImGui::DragFloat("X Delta", &currentAnimAction.xDelta, 0.1f)) {
            moveDelta->xDelta = currentAnimAction.xDelta;
        }
        if (ImGui::DragFloat("Y Delta", &currentAnimAction.yDelta, 0.1f)) {
            moveDelta->yDelta = currentAnimAction.yDelta;
        }
    }
    else if (auto* playSound = dynamic_cast<Act_PlaySound*>(action)) {
        currentAnimAction.soundFilename = playSound->filename;
        currentAnimAction.soundVolume = playSound->volume;
        char buffer[256];
        strncpy_s(buffer, currentAnimAction.soundFilename.c_str(), sizeof(buffer) - 1);
        if (ImGui::InputText("Sound File", buffer, sizeof(buffer))) {
            playSound->filename = buffer;
        }
        if (ImGui::DragFloat("Volume", &currentAnimAction.soundVolume, 0.01f, 0.0f, 1.0f)) {
            playSound->volume = currentAnimAction.soundVolume;
        }
    }
}

void AnimManagerEditorUI::removeAnimationAction(Frame* currentFrame, int index) {
    if (index >= 0 && index < currentFrame->animActions.size()) {
        delete currentFrame->animActions[index];
        currentFrame->animActions.erase(currentFrame->animActions.begin() + index);
    }
}

void AnimManagerEditorUI::addInputAction(Frame* currentFrame) {
    Frame::InputAction newAction;
    newAction.animChangeIndex = currentInputAction.animChangeIndex;
    newAction.inputCommand = currentInputAction.inputCommand;
    newAction.inputButton = currentInputAction.inputButton;
    currentFrame->inputActions.push_back(newAction);
}

void AnimManagerEditorUI::editInputAction(Frame::InputAction& action) {
    ImGui::InputInt("Animation Change Index", &action.animChangeIndex);
    
    // Input string field
    char inputStringBuffer[256];
    strncpy_s(inputStringBuffer, action.inputString.c_str(), sizeof(inputStringBuffer) - 1);
    if (ImGui::InputText("Input String", inputStringBuffer, sizeof(inputStringBuffer))) {
        action.inputString = inputStringBuffer;
    }
    
    // Input command combo
    const char* commandItems[] = {
        "FORWARD", "BACK", "UP", "DOWN", "UPFORWARD", "UPBACK",
        "NONE", "ANY", "DOUBLEFORWARD", "DOUBLEBACK"
    };
    int currentCommand = static_cast<int>(action.inputCommand);
    if (ImGui::Combo("Input Command", &currentCommand, commandItems, IM_ARRAYSIZE(commandItems))) {
        action.inputCommand = static_cast<Frame::InputCommand>(currentCommand);
    }
    
    // Input button combo
    const char* buttonItems[] = {
        "A", "B", "C", "X", "Y", "Z", "NONE", "ANY"
    };
    int currentButton = static_cast<int>(action.inputButton);
    if (ImGui::Combo("Input Button", &currentButton, buttonItems, IM_ARRAYSIZE(buttonItems))) {
        action.inputButton = static_cast<Frame::InputButton>(currentButton);
    }
}

void AnimManagerEditorUI::removeInputAction(Frame* currentFrame, int index) {
    if (index >= 0 && index < currentFrame->inputActions.size()) {
        currentFrame->inputActions.erase(currentFrame->inputActions.begin() + index);
    }
}

void AnimManagerEditorUI::addPalette(AnimManager* character) {
    // Create a new palette with the current template and palette files
    Palette newPalette(currentPalette.paletteFile, currentPalette.templateFile);
    character->paletteList.push_back(newPalette);
}

void AnimManagerEditorUI::editPalette(int paletteIndex) {
    // TODO: Implement color editing interface
    ImGui::Text("Color editing coming soon...");
}

void AnimManagerEditorUI::removePalette(AnimManager* character, int index) {
    if (index >= 0 && index < character->paletteList.size()) {
        character->paletteList.erase(character->paletteList.begin() + index);
    }
} 