#pragma once
#include "../../CrackEngine/code/Graphics/AnimManager.h"
#include "ColumnsConfig.h"
#include <vector>

class ColumnsActivePiece;
class InputManager;

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
	};
	/*
		Start at falling,
		After every fall->clear->postclear, check for damage
		If damage, do the fall->clear->postclear again
		If no damage, can go to fall
	*/
	AnimManager* playBoardGraphics[13][6];
	AnimManager* background;
	AnimManager* foreground;
	GridValue playBoardValues[13][6];
	bool playBoardActiveValues[13][6];

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
	void updateGraphics();
	void transitionTo(GameState newState);
	bool isFallingFinished();
	bool canMoveLeft();
	bool canMoveRight();
	bool isClearingFinished();
	bool isClearFallFinished();
	bool isPostClearFinished();
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

	void resetPanelSprite(int x, int y);
	void spawnNewBlocks();
};

