#pragma once
#include "../../CrackEngine/code/Graphics/AnimManager.h"
#include "ColumnsConfig.h"
#include <vector>

class ColumnsActivePiece;
class InputManager;

// Board dimensions
constexpr int BOARD_WIDTH = 6;
constexpr int BOARD_HEIGHT = 16; // adjusted height

class ColumnsBoard
{
public:
	// playBoard grid enum
	enum class GridValue
	{
		EMPTY,
		GREEN,
		ORANGE,
		RED,
		BLUE,
		YELLOW,
		PURPLE,
		TEAL,
		GRAY,
		BLACK,
		WHITE
	};
	enum class GameState
	{
		PLACE,
		CLEAR,
		POSTCLEAR,
		FALLING,
		DAMAGE,
		GAMEOVER
	};
	/*
		Start at falling,
		After every fall->clear->postclear, check for damage
		If damage, do the fall->clear->postclear again
		If no damage, can go to fall
	*/
	AnimManager* playBoardGraphics[BOARD_HEIGHT][BOARD_WIDTH];
	AnimManager* background;
	AnimManager* foreground;
	GridValue playBoardValues[BOARD_HEIGHT][BOARD_WIDTH];
	bool playBoardActiveValues[BOARD_HEIGHT][BOARD_WIDTH];

	// Preview (2 sets of 3 blocks)
	AnimManager* previewGraphics[2][3];
	GridValue previewValues[2][3];

	// Game state
	GameState state = GameState::FALLING;

	// Board state
	std::vector<std::vector<int>> boardValues;
	std::vector<AnimManager*> staticBlocks;
	ColumnsActivePiece* activePiece = nullptr;

	// Input
	InputManager* controls;
	int eastHoldFrames = 0;  // Track how long east/right is held
	int westHoldFrames = 0;  // Track how long west/left is held

	ColumnsBoard();
	ColumnsBoard(InputManager* _controls);
	~ColumnsBoard();
	void update();
	void init();
	void initializeBoard();
	void initializeGraphics();
	void initializeBag();
	void render();
	void updateBlockMovements();
	void handleCollisions();
	void updatePreviewGraphics();
	void updateGraphics();
	void transitionTo(GameState newState);
	bool isFallingFinished();
	bool canMoveLeft();
	bool canMoveRight();
	bool isClearingFinished();
	bool isClearFallFinished();
	bool isPostClearFinished();
	bool checkLostGame();
	bool checkForDamage();
	bool isDamageHandled();
	bool isPiecePlaced();
	bool rotateDown();
	bool rotateUp();
private:
	float spriteWidth = 40.f;
	float left = -200.f;
	float bottom = 100.f;
	int frameCount = 0;
	int bagIndex = 0;
	std::vector<GridValue> randomBag;

	// Preview layout origin (to the side of the board)
	float previewLeft = left + (spriteWidth * BOARD_WIDTH) + 20.f; // right side with padding
	float previewBottom = bottom + (spriteWidth * (BOARD_HEIGHT - 3)); // align vertically with spawn top

	void resetPanelSprite(int x, int y);
	void resetPreviewSprite(int setIndex, int slotIndex);
	void spawnNewBlocks();
	bool isActivePieceOnGround();
};

