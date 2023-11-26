#pragma once
#include <vector>
class ColumnsBoard;  // forward declaration
enum GridValue;

class ActivePiece {
public:
    ActivePiece(int x, int y);  // Starting position

    // Basic piece movements
    void moveLeft();
    void moveRight();
    void moveDown();
    void rotateColors();  // This can be used to change the order of the blocks' colors

    // Collision checking with the board
    bool canMoveLeft(const ColumnsBoard& board) const;
    bool canMoveRight(const ColumnsBoard& board) const;
    bool canMoveDown(const ColumnsBoard& board) const;

    // Getters for the current position and colors
    std::pair<int, int> getPosition() const;
    std::vector<GridValue> getColors() const;

private:
    int x, y;  // Current position
    std::vector<GridValue> colors;  // Colors of the blocks in the active piece
};
