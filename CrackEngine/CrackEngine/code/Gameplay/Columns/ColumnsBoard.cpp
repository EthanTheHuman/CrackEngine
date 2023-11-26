#include "ColumnsBoard.h"
#include "ColumnsActivePiece.h"
#include "../../Input/InputManager.h"

const int BOARD_WIDTH = 6;
const int BOARD_HEIGHT = 13;
const int FRAME_DROP_RATE = 30;
const int FRAME_DROP_RATE_DROP = 5;

ColumnsBoard::ColumnsBoard() {
    state = GameState::FALLING;
    init();
}

ColumnsBoard::ColumnsBoard(InputManager* _controls)
{
	controls = _controls;
    state = GameState::FALLING;
    init();
}

ColumnsBoard::~ColumnsBoard() {
    // Deallocate memory (e.g. playBoardGraphics)
}

void ColumnsBoard::update() {
    updateBlockMovements();
    handleCollisions();
    updateGraphics();
    frameCount++;

    switch (state) {
    case GameState::FALLING:
        {
            if (controls->getButtonDown(controls->WEST) == true)
            {
                if (canMoveLeft())
                {
                    frameCount = 0;
                }
                // move left
            }
            if (controls->getButtonDown(controls->EAST) == true)
            {
                if (canMoveRight())
                {
                    frameCount = 0;
                }
                // move right
            }
            if (controls->getButtonDown(controls->NORTH) == true)
            {
                while (!isFallingFinished()) {
                    // do nothing
                }
                transitionTo(GameState::CLEAR);
                break;
            }
            break;
        }
    }
    
    if (frameCount % FRAME_DROP_RATE == 0) {
        switch (state) {

        case GameState::FALLING:
        {
            // Handle falling logic
            if (controls->getButton(controls->SOUTH) == true)
            {
                frameCount = FRAME_DROP_RATE - FRAME_DROP_RATE_DROP;
            }
            if (isFallingFinished()) {
                transitionTo(GameState::CLEAR);
            }
            break;
        }
        case GameState::CLEAR:
            // Handle clearing logic
            if (isClearingFinished()) {
                transitionTo(GameState::POSTCLEAR);
            }
            break;
        case GameState::POSTCLEAR:
            // Handle post-clearing logic
            if (isPostClearFinished()) {
                if (checkForDamage()) {
                    transitionTo(GameState::DAMAGE);
                }
                else {
                    transitionTo(GameState::PLACE);
                }
            }
            break;
        case GameState::DAMAGE:
            // Handle damage logic
            if (isDamageHandled()) {
                transitionTo(GameState::FALLING);
            }
            break;
        case GameState::PLACE:
            // Handle placement logic where the player guides the active piece
            if (isPiecePlaced()) {
                spawnNewBlocks();
                transitionTo(GameState::FALLING);
            }
            break;
        }
	}
}

void ColumnsBoard::updateBlockMovements() {
    // Logic to move blocks
}

void ColumnsBoard::handleCollisions() {
    // Logic to handle block collisions
}

void ColumnsBoard::updateGraphics() {
    // Update board graphics
    background->update();
    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            playBoardGraphics[i][j]->update();
        }
    }
    foreground->update();
}

void ColumnsBoard::render() {
    // Render playBoard
    background->render();
    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            playBoardGraphics[i][j]->render();
        }
    }
    foreground->render();
}

void ColumnsBoard::transitionTo(GameState newState)
{
    state = newState;
}

bool ColumnsBoard::isFallingFinished()
{
    bool finished = true;
    // From the bottom, scroll up and move blocks down
    for (int i = 1; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            if (playBoardValues[i][j] != GridValue::EMPTY)
            {
                if (playBoardValues[i - 1][j] == GridValue::EMPTY)
                {
                    playBoardValues[i - 1][j] = playBoardValues[i][j];
                    playBoardValues[i][j] = GridValue::EMPTY;
                    resetPanelSprite(i, j);
                    resetPanelSprite(i - 1, j);
                    finished = false;
                }
            }
        }
    }
    return finished;
}

bool ColumnsBoard::canMoveLeft()
{
    bool finished = true;
    // From the bottom, scroll up and move blocks down
    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 1; j < BOARD_WIDTH; j++)
        {
            if (playBoardValues[i][j] != GridValue::EMPTY)
            {
                if (playBoardValues[i][j - 1] == GridValue::EMPTY)
                {
                    playBoardValues[i][j - 1] = playBoardValues[i][j];
                    playBoardValues[i][j] = GridValue::EMPTY;
                    resetPanelSprite(i, j);
                    resetPanelSprite(i, j - 1);
                    finished = false;
                }
            }
        }
    }
    return finished;
}

bool ColumnsBoard::canMoveRight()
{
    bool finished = true;
    // From the bottom, scroll up and move blocks down
    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH - 1; j++)
        {
            if (playBoardValues[i][j] != GridValue::EMPTY)
            {
                if (playBoardValues[i][j + 1] == GridValue::EMPTY)
                {
                    playBoardValues[i][j + 1] = playBoardValues[i][j];
                    playBoardValues[i][j] = GridValue::EMPTY;
                    resetPanelSprite(i, j);
                    resetPanelSprite(i, j + 1);
                    finished = false;
                    j++;
                }
            }
        }
    }
    return finished;
}

bool ColumnsBoard::isClearingFinished()
{
    // look for 3 of the same color in any direction
    bool finished = true;

    // Scan horizontally
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 4; j++) {
            if (playBoardValues[i][j] != GridValue::EMPTY &&
                playBoardValues[i][j] == playBoardValues[i][j + 1] &&
                playBoardValues[i][j] == playBoardValues[i][j + 2])
            {
                playBoardValues[i][j] = GridValue::EMPTY;
                resetPanelSprite(i, j);
                playBoardValues[i][j + 1] = GridValue::EMPTY;
                resetPanelSprite(i, j + 1);
                playBoardValues[i][j + 2] = GridValue::EMPTY;
                resetPanelSprite(i, j + 2);
                finished = false;
            }
        }
    }

    // Scan vertically
    for (int j = 0; j < 6; j++) {
        for (int i = 0; i < 11; i++) {
            if (playBoardValues[i][j] != GridValue::EMPTY &&
                playBoardValues[i][j] == playBoardValues[i + 1][j] &&
                playBoardValues[i][j] == playBoardValues[i + 2][j]) {
                playBoardValues[i][j] = GridValue::EMPTY;
                resetPanelSprite(i, j);
                playBoardValues[i + 1][j] = GridValue::EMPTY;
                resetPanelSprite(i + 1, j);
                playBoardValues[i + 2][j] = GridValue::EMPTY;
                resetPanelSprite(i + 2, j);
                finished = false;
            }
        }
    }

    // Scan diagonally (top-left to bottom-right)
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 4; j++) {
            if (playBoardValues[i][j] != GridValue::EMPTY &&
                playBoardValues[i][j] == playBoardValues[i + 1][j + 1] &&
                playBoardValues[i][j] == playBoardValues[i + 2][j + 2]) {
                playBoardValues[i][j] = GridValue::EMPTY;
                resetPanelSprite(i, j);
                playBoardValues[i + 1][j + 1] = GridValue::EMPTY;
                resetPanelSprite(i + 1, j + 1);
                playBoardValues[i + 2][j + 2] = GridValue::EMPTY;
                resetPanelSprite(i + 2, j + 2);
                finished = false;
            }
        }
    }

    // Scan diagonally (top-right to bottom-left)
    for (int i = 0; i < 11; i++) {
        for (int j = 2; j < 6; j++) {
            if (playBoardValues[i][j] != GridValue::EMPTY && 
                playBoardValues[i][j] == playBoardValues[i + 1][j - 1] &&
                playBoardValues[i][j] == playBoardValues[i + 2][j - 2]) {
                playBoardValues[i][j] = GridValue::EMPTY;
                resetPanelSprite(i, j);
                playBoardValues[i + 1][j - 1] = GridValue::EMPTY;
                resetPanelSprite(i + 1, j - 1);
                playBoardValues[i + 2][j - 2] = GridValue::EMPTY;
                resetPanelSprite(i + 2, j - 2);
                finished = false;
            }
        }
    }
    return finished;
}

bool ColumnsBoard::isPostClearFinished()
{
    return true;
}

bool ColumnsBoard::checkForDamage()
{
    return false;
}

bool ColumnsBoard::isDamageHandled()
{
    return true;
}

bool ColumnsBoard::isPiecePlaced()
{
    return true;
}

void ColumnsBoard::init() {
    // Initial setup
    initializeBoard();
    initializeGraphics();
}

void ColumnsBoard::initializeBoard() {
    // Setup initial board state
    // fill playBoard with empty blocks
	for (int i = 0; i < BOARD_HEIGHT; i++)
	{
		for (int j = 0; j < BOARD_WIDTH; j++)
		{
            playBoardGraphics[i][j] = new AnimManager((std::string)"data/characters/RedPiece.xml", glm::vec3(left + (spriteWidth * j), bottom + (spriteWidth * i), 0.f));
            playBoardValues[i][j] = GridValue::EMPTY;
		}
	}
    spawnNewBlocks();
}

void ColumnsBoard::initializeGraphics() {
    background = new AnimManager((std::string)"data/characters/RedPiece.xml", glm::vec3(left, bottom, 0.f));
    background->changeAnimation(101);
    foreground = new AnimManager((std::string)"data/characters/RedPiece.xml", glm::vec3(left, bottom, 0.f));
    foreground->changeAnimation(102);
    // Setup initial graphics state
}

void ColumnsBoard::resetPanelSprite(int x, int y) {
    // Logic
	if (playBoardGraphics[x][y] != nullptr)
	{
		if (playBoardValues[x][y] == GridValue::EMPTY && playBoardGraphics[x][y]->currentAnim->index != 1)
		{
			playBoardGraphics[x][y]->changeAnimation(1);
		}
		else if (playBoardValues[x][y] == GridValue::GREEN && playBoardGraphics[x][y]->currentAnim->index != 2)
		{
			playBoardGraphics[x][y]->changeAnimation(2);
		}
		else if (playBoardValues[x][y] == GridValue::ORANGE && playBoardGraphics[x][y]->currentAnim->index != 3)
		{
			playBoardGraphics[x][y]->changeAnimation(3);
		}
        else if (playBoardValues[x][y] == GridValue::RED && playBoardGraphics[x][y]->currentAnim->index != 4)
        {
            playBoardGraphics[x][y]->changeAnimation(4);
        }
        else if (playBoardValues[x][y] == GridValue::BLUE && playBoardGraphics[x][y]->currentAnim->index != 5)
        {
            playBoardGraphics[x][y]->changeAnimation(5);
        }
        else if (playBoardValues[x][y] == GridValue::PURPLE && playBoardGraphics[x][y]->currentAnim->index != 6)
        {
            playBoardGraphics[x][y]->changeAnimation(6);
        }
        else if (playBoardValues[x][y] == GridValue::TEAL && playBoardGraphics[x][y]->currentAnim->index != 7)
        {
            playBoardGraphics[x][y]->changeAnimation(7);
        }
	}
}

void ColumnsBoard::spawnNewBlocks() {
    // Logic
	// Spawn new blocks
	playBoardValues[12][3] = GridValue::GREEN;
	resetPanelSprite(12, 3);
	playBoardValues[11][3] = GridValue::GREEN;
	resetPanelSprite(11, 3);
	playBoardValues[10][3] = GridValue::GREEN;
	resetPanelSprite(10, 3);
}
