#include "ColumnsBoard.h"
#include "ColumnsActivePiece.h"
#include "../../Input/InputManager.h"
#include <algorithm>
#include <random>

// Game constants
const int BOARD_WIDTH = 6;
const int BOARD_HEIGHT = 13;
const int FRAME_DROP_RATE = 76;
const int FRAME_DROP_RATE_DROP = 52;
const int LOCK_DELAY = 25;
int lockDelayCount = 0;
const int SPAWN_DELAY = 15;
int spawnDelayCount = 0;
int blocksToChain = 3;
bool verticalChaining = true;
bool horizontalChaining = true;
bool diagonalChaining = true;
const int AUTO_REPEAT_DELAY = 10;
const int AUTO_REPEAT_RATE = 5;
int westHoldFrames = 0;
int eastHoldFrames = 0;

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
            bool input = false;
            if (controls->getButtonDown(controls->X) == true)
            {
                rotateDown();
                // move left
            }
            if (controls->getButtonDown(controls->Y) == true)
            {
                rotateUp();
                // move left
            }
            // Handle left movement
            if (controls->getButton(controls->WEST)) {
                westHoldFrames++;
                if (controls->getButtonDown(controls->WEST) || 
                    (westHoldFrames > ColumnsConfig::getInstance().getAutoRepeatDelay() && 
                     (westHoldFrames - ColumnsConfig::getInstance().getAutoRepeatDelay()) % ColumnsConfig::getInstance().getAutoRepeatRate() == 0)) {
                    if (canMoveLeft()) {
                        input = true;
                        frameCount = 0;
                    }
                }
            } else {
                westHoldFrames = 0;
            }
            // Handle right movement
            if (controls->getButton(controls->EAST)) {
                eastHoldFrames++;
                if (controls->getButtonDown(controls->EAST) || 
                    (eastHoldFrames > ColumnsConfig::getInstance().getAutoRepeatDelay() && 
                     (eastHoldFrames - ColumnsConfig::getInstance().getAutoRepeatDelay()) % ColumnsConfig::getInstance().getAutoRepeatRate() == 0)) {
                    if (canMoveRight()) {
                        input = true;
                        frameCount = 0;
                        lockDelayCount = ColumnsConfig::getInstance().getLockDelay();
                    }
                }
            } else {
                eastHoldFrames = 0;
            }
            if (controls->getButtonDown(controls->NORTH) == true)
            {
                lockDelayCount = ColumnsConfig::getInstance().getLockDelay();
                input = true;
                while (!isFallingFinished()) {
                    frameCount = ColumnsConfig::getInstance().getFrameDropRate();
                    // do nothing
                }
                transitionTo(GameState::CLEAR);
                break;
            }
            if (controls->getButton(controls->SOUTH) == true)
            {
                frameCount += ColumnsConfig::getInstance().getFrameDropRateDrop();
            }
            if (input == false)
            {
                // Handle falling logic
                if (isFallingFinished()) {
                    transitionTo(GameState::CLEAR);
                }
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
                else
                {
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
                transitionTo(GameState::FALLING);
                spawnNewBlocks();
            }
            break;
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

    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            playBoardActiveValues[i][j] = false;
        }
    }
}

bool ColumnsBoard::isFallingFinished()
{
    bool finished = true;
    // From the bottom, scroll up and move blocks down
    for (int i = 1; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            if (playBoardActiveValues[i][j] == true)
            {
                if (playBoardValues[i - 1][j] == GridValue::EMPTY)
                {
                    if (frameCount >= ColumnsConfig::getInstance().getFrameDropRate())
                    {
                        if (playBoardActiveValues[i][j] == true)
                        {
                            playBoardActiveValues[i][j] = false;
                            playBoardActiveValues[i - 1][j] = true;
                        }
                        playBoardValues[i - 1][j] = playBoardValues[i][j];
                        playBoardValues[i][j] = GridValue::EMPTY;
                        resetPanelSprite(i, j);
                        resetPanelSprite(i - 1, j);
                        lockDelayCount = 0;
                    }
                    finished = false;
                }
            }
        }
    }
    if (frameCount >= ColumnsConfig::getInstance().getFrameDropRate())
    {
        frameCount = 0;
    }
    if (finished == true)
    {
        if (lockDelayCount >= ColumnsConfig::getInstance().getLockDelay())
        {
            finished = true;
            lockDelayCount = 0;
        }
        else {
            finished = false;
            lockDelayCount++;
        }
    }
    return finished;
}

bool ColumnsBoard::canMoveLeft()
{
    bool finished = false;
    // Find if the active blocks are on the left edge

    // Find if the active blocks have anything to the left
    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        if (playBoardActiveValues[i][0] == true)
        {
            return false;
        }
        for (int j = 1; j < BOARD_WIDTH; j++)
        {
            if (playBoardActiveValues[i][j] == true)
            {
                if (playBoardValues[i][j - 1] != GridValue::EMPTY)
                {
                    return false;
                }
            }
        }
    }

    // move everything left
    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 1; j < BOARD_WIDTH; j++)
        {
            if (playBoardActiveValues[i][j] == true)
            {
                if (playBoardValues[i][j - 1] == GridValue::EMPTY)
                {
                    playBoardValues[i][j - 1] = playBoardValues[i][j];
                    playBoardActiveValues[i][j - 1] = true;
                    playBoardValues[i][j] = GridValue::EMPTY;
                    playBoardActiveValues[i][j] = false;
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
    bool finished = false;
    // Find if the active blocks are on the right edge
    // Find if the active blocks have anything to the right
    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        if (playBoardActiveValues[i][BOARD_WIDTH - 1] == true)
        {
            return false;
        }
        for (int j = 0; j < BOARD_WIDTH - 1; j++)
        {
            if (playBoardActiveValues[i][j] == true)
            {
                if (playBoardValues[i][j + 1] != GridValue::EMPTY)
                {
                    return false;
                }
            }
        }
    }
    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH - 1; j++)
        {
            if (playBoardActiveValues[i][j] == true)
            {
                if (playBoardValues[i][j + 1] == GridValue::EMPTY)
                {
                    playBoardValues[i][j + 1] = playBoardValues[i][j];
                    playBoardActiveValues[i][j + 1] = true;
                    playBoardValues[i][j] = GridValue::EMPTY;
                    playBoardActiveValues[i][j] = false;
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
    // drop pieces down
    if (isClearFallFinished() == false)
    {
        return false;
    }

    // look for 3 of the same color in any direction
    bool finished = true;
    std::vector<glm::vec2> cellsToClear;
    // Scan horizontally
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (playBoardValues[i][j] != GridValue::EMPTY) {
                if (ColumnsConfig::getInstance().getVerticalChaining()) { // Vertical chaining
                    int matchedPieces = 1;
                    int currentY = i;
                    int currentX = j;
                    bool matching = true;
                    auto colourToMatch = playBoardValues[i][j];
                    std::vector<glm::vec2> cells;
                    cells.push_back(glm::vec2(currentX, currentY));
                    currentY++;
                    while (matching == true) {
                        // check the value above if it is the same
                        if (currentY > BOARD_HEIGHT) {
                            matching = false;
                        }
                        else if (playBoardValues[currentY][currentX] == colourToMatch) {
                            matching = true;
                            cells.push_back(glm::vec2(currentX, currentY));
                            matchedPieces++;
                            currentY++;
                        }
                        else {
                            matching = false;
                        }
                    }
                    if (matchedPieces >= ColumnsConfig::getInstance().getBlocksToChain())
                    {
                        // Chain successful! Mark it as ready to delete
                        cellsToClear.insert(cellsToClear.end(), cells.begin(), cells.end());
                        cells.clear();
                    }
                    matchedPieces = 1;
                }
                if (ColumnsConfig::getInstance().getHorizontalChaining()) { // Horizontal chaining
                    int matchedPieces = 1;
                    int currentY = i;
                    int currentX = j;
                    bool matching = true;
                    auto colourToMatch = playBoardValues[i][j];
                    std::vector<glm::vec2> cells;
                    cells.push_back(glm::vec2(currentX, currentY));
                    currentX++;
                    while (matching == true) {
                        // check the value above if it is the same
                        if (currentX > BOARD_WIDTH) {
                            matching = false;
                        }
                        else if (playBoardValues[currentY][currentX] == colourToMatch) {
                            matching = true;
                            cells.push_back(glm::vec2(currentX, currentY));
                            matchedPieces++;
                            currentX++;
                        }
                        else {
                            matching = false;
                        }
                    }
                    if (matchedPieces >= ColumnsConfig::getInstance().getBlocksToChain())
                    {
                        // Chain successful! Mark it as ready to delete
                        cellsToClear.insert(cellsToClear.end(), cells.begin(), cells.end());
                        cells.clear();
                    }
                    matchedPieces = 1;
                }
                if (ColumnsConfig::getInstance().getDiagonalChaining()) { // Forward slash chaining
                    int matchedPieces = 1;
                    int currentY = i;
                    int currentX = j;
                    bool matching = true;
                    auto colourToMatch = playBoardValues[i][j];
                    std::vector<glm::vec2> cells;
                    cells.push_back(glm::vec2(currentX, currentY));
                    currentX++;
                    currentY++;
                    while (matching == true) {
                        // check the value above if it is the same
                        if (currentX > BOARD_WIDTH || currentY > BOARD_HEIGHT) {
                            matching = false;
                        }
                        else if (playBoardValues[currentY][currentX] == colourToMatch) {
                            matching = true;
                            cells.push_back(glm::vec2(currentX, currentY));
                            matchedPieces++;
                            currentX++;
                            currentY++;
                        }
                        else {
                            matching = false;
                        }
                    }
                    if (matchedPieces >= ColumnsConfig::getInstance().getBlocksToChain())
                    {
                        // Chain successful! Mark it as ready to delete
                        cellsToClear.insert(cellsToClear.end(), cells.begin(), cells.end());
                        cells.clear();
                    }
                    matchedPieces = 1;
                }
                if (ColumnsConfig::getInstance().getDiagonalChaining()) { // Backslash chaining
                    int matchedPieces = 1;
                    int currentY = i;
                    int currentX = j;
                    bool matching = true;
                    auto colourToMatch = playBoardValues[i][j];
                    std::vector<glm::vec2> cells;
                    cells.push_back(glm::vec2(currentX, currentY));
                    currentX--;
                    currentY++;
                    while (matching == true) {
                        // check the value above if it is the same
                        if (currentX < 0 || currentY > BOARD_HEIGHT) {
                            matching = false;
                        }
                        else if (playBoardValues[currentY][currentX] == colourToMatch) {
                            matching = true;
                            cells.push_back(glm::vec2(currentX, currentY));
                            matchedPieces++;
                            currentX--;
                            currentY++;
                        }
                        else {
                            matching = false;
                        }
                    }
                    if (matchedPieces >= ColumnsConfig::getInstance().getBlocksToChain())
                    {
                        // Chain successful! Mark it as ready to delete
                        cellsToClear.insert(cellsToClear.end(), cells.begin(), cells.end());
                        cells.clear();
                    }
                    matchedPieces = 1;
                }
            }
        }
    }

    if (cellsToClear.size() > 0) {
        for (int i = 0; i < cellsToClear.size(); i++) {
            glm::vec2 newCell = cellsToClear[i];
            playBoardValues[(int)newCell.y][(int)newCell.x] = GridValue::EMPTY;
            resetPanelSprite((int)newCell.y, newCell.x);
        }
        cellsToClear.clear();
        return false;
    }
    else {
        return true;
    }
}

bool ColumnsBoard::isClearFallFinished()
{
    bool finished = true;
    // From the bottom, scroll up and move blocks down
    for (int i = 1; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            if (playBoardValues[i][j] != GridValue::EMPTY)
            {
                lockDelayCount = 0;
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

bool ColumnsBoard::isPostClearFinished()
{
    if (spawnDelayCount < ColumnsConfig::getInstance().getSpawnDelay())
    {
        spawnDelayCount++;
        return false;
    }
    else
    {
        spawnDelayCount = 0;
        return true;
    }
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

bool ColumnsBoard::rotateDown()
{
    glm::vec3 top = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 middle = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 bottom = glm::vec3(0.f, 0.f, 0.f);

    for (int y = 0; y < BOARD_HEIGHT; y++)
    {
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            if (playBoardActiveValues[y][x] == true)    // If the piece is true
            {
                if (bottom.z == 0.f)
				{
					bottom = glm::vec3(x, y, 1);
				}
                else if (middle.z == 0.f)
                {
                    middle = glm::vec3(x, y, 1);
                }
                else
                {
                    top = glm::vec3(x, y, 1);
                }
            }
        }
    }
    auto temp = playBoardValues[(int)bottom.y][(int)bottom.x];
    playBoardValues[(int)bottom.y][(int)bottom.x] = playBoardValues[(int)middle.y][(int)middle.x];
    playBoardValues[(int)middle.y][(int)middle.x] = playBoardValues[(int)top.y][(int)top.x];
    playBoardValues[(int)top.y][(int)top.x] = temp;
    resetPanelSprite((int)bottom.y, (int)bottom.x);
    resetPanelSprite((int)middle.y, (int)middle.x);
    resetPanelSprite((int)top.y, (int)top.x);
    return false;
}

bool ColumnsBoard::rotateUp()
{
    glm::vec3 top = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 middle = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 bottom = glm::vec3(0.f, 0.f, 0.f);

    for (int y = 0; y < BOARD_HEIGHT; y++)
    {
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            if (playBoardActiveValues[y][x] == true)    // If the piece is true
            {
                if (bottom.z == 0.f)
                {
                    bottom = glm::vec3(x, y, 1);
                }
                else if (middle.z == 0.f)
                {
                    middle = glm::vec3(x, y, 1);
                }
                else
                {
                    top = glm::vec3(x, y, 1);
                }
            }
        }
    }
    auto temp = playBoardValues[(int)top.y][(int)top.x];
    playBoardValues[(int)top.y][(int)top.x] = playBoardValues[(int)middle.y][(int)middle.x];
    playBoardValues[(int)middle.y][(int)middle.x] = playBoardValues[(int)bottom.y][(int)bottom.x];
    playBoardValues[(int)bottom.y][(int)bottom.x] = temp;
    resetPanelSprite((int)bottom.y, (int)bottom.x);
    resetPanelSprite((int)middle.y, (int)middle.x);
    resetPanelSprite((int)top.y, (int)top.x);
    return false;
}

void ColumnsBoard::init() {
    // Initial setup
    ColumnsConfig::getInstance().setConfigFile("data/config/columns_config.json");
    initializeBag();
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
            playBoardActiveValues[i][j] = false;
		}
	}
    spawnNewBlocks();
}

void ColumnsBoard::initializeGraphics() {
    background = new AnimManager((std::string)"data/characters/RedPiece.xml", glm::vec3(left, bottom, -1.f));
    background->changeAnimation(101);
    foreground = new AnimManager((std::string)"data/characters/RedPiece.xml", glm::vec3(left, bottom, 1.f));
    foreground->changeAnimation(102);
    // Setup initial graphics state
}

void ColumnsBoard::initializeBag()
{
    // Setup initial bag state
    randomBag.clear();
    for (int i = 0; i < 64; i++)
    {
        randomBag.push_back(GridValue::GREEN);
        randomBag.push_back(GridValue::ORANGE);
        randomBag.push_back(GridValue::RED);
        randomBag.push_back(GridValue::BLUE);
        randomBag.push_back(GridValue::PURPLE);
        randomBag.push_back(GridValue::TEAL);
	}
    // randomise the bag
    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);
    std::shuffle(std::begin(randomBag), std::end(randomBag), rng);
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
    for (int i = 0; i < 3; i++) {
        if (bagIndex >= randomBag.size())
        {
            bagIndex = 0;
        }
        playBoardValues[12 - i][3] = randomBag[bagIndex];
        playBoardActiveValues[12 - i][3] = true;
        resetPanelSprite(12 - i, 3);
        bagIndex++;
    }
}
