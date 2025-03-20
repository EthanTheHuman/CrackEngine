#pragma once
#include "../vendor/imgui/imgui.h"
#include <vector>
#include <string>

// Forward declarations
class GameObject;
class AnimManager;
class Frame;
class CrkBox;
class InputManager;
class AnimAction;

// Include full definitions
#include "GameObject.h"
#include "Input/InputManager.h"
#include "Graphics/AnimManager.h"
#include "Graphics/Frame.h"
#include "Engine/CrkBox.h"

class AnimManagerEditorUI {
public:
    AnimManagerEditorUI();
    ~AnimManagerEditorUI();

    void render(std::vector<AnimManager*>& characters);
    void addConsoleMessage(const std::string& message);

private:
    // Panel rendering methods
    void renderLeftPanel(AnimManager* character, int& characterNo, int characterCount);
    void renderCanvas(AnimManager* character, Frame* currentFrame);
    void renderPaletteEditor(AnimManager* character);
    void renderAnimationActions(Frame* currentFrame);
    void renderInputActions(Frame* currentFrame);
    
    // Input and canvas handling
    void processCanvasInputs(ImVec2& originPos, float& spriteXPos, float& spriteYPos, float& canvasScale);
    void drawCharacterSprite(ImDrawList* drawList, Frame* currentFrame, const ImVec2& originPos, float canvasScale);
    void drawHitboxes(ImDrawList* drawList, Frame* currentFrame, const ImVec2& originPos, float canvasScale);
    void handleHitboxCreation(Frame* currentFrame, const ImVec2& mousePos);
    
    // Animation action handling
    void addAnimationAction(Frame* currentFrame);
    void editAnimationAction(AnimAction* action);
    void removeAnimationAction(Frame* currentFrame, int index);
    
    // Input action handling
    void addInputAction(Frame* currentFrame);
    void editInputAction(Frame::InputAction& action);
    void removeInputAction(Frame* currentFrame, int index);
    
    // Palette handling
    void addPalette(AnimManager* character);
    void editPalette(int paletteIndex);
    void removePalette(AnimManager* character, int index);
    
    // Save/Load
    void saveCharacter(AnimManager* character);
    
    // Utility methods
    Frame* refreshCharacterInfo(AnimManager* character, int animIndex, int frameIndex, 
                              float& spriteXPos, float& spriteYPos, ImVector<ImVec2>* points, CrkBox* rect);
    std::vector<int> getAnimKeysList(AnimManager* character);
    std::vector<int> getFrameKeysList(AnimManager* character, int animIndex);

    // UI state
    float sz = 10.0f;
    float thickness = 2.0f;
    float boxTransparency = 0.2f;
    ImVec4 hurtboxCol = ImVec4(0.2f, 0.2f, 1.0f, 1.0f);
    ImVec4 hitboxCol = ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
    ImVec4 pushboxCol = ImVec4(0.2f, 1.0f, 0.2f, 1.0f);
    float canvasScale = 1.0f;
    bool adding_line = false;
    ImVector<ImVec2> points;
    std::vector<std::string> consoleMessages;
    float spriteXPos = 0.0f;
    float spriteYPos = 0.0f;

    // Animation state
    int animNo = 0;
    int frameNo = 0;
    char saveFilename[256] = "character.xml";  // Buffer for filename input
    
    // Animation action state
    struct AnimActionState {
        std::string type;
        float xVel = 0.0f;
        float yVel = 0.0f;
        float xAccel = 0.0f;
        float yAccel = 0.0f;
        float xDelta = 0.0f;
        float yDelta = 0.0f;
        std::string frequency = "always";
        int stepCount = 0;
        std::string soundFilename;
        float soundVolume = 1.0f;
    } currentAnimAction;
    
    // Input action state
    struct InputActionState {
        int animChangeIndex = 0;
        Frame::InputCommand inputCommand = Frame::InputCommand::NONE;
        Frame::InputButton inputButton = Frame::InputButton::ANY;
    } currentInputAction;
    
    // Palette state
    struct PaletteState {
        char templateFile[256] = "";
        char paletteFile[256] = "";
        std::vector<ImVec4> colors;
    } currentPalette;
    
    // Animation properties
    bool keepVelocity = false;
    bool keepAcceleration = false;
    int fallbackIndex = 0;
}; 