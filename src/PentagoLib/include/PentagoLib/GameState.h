#ifndef GAMESTATE_H
#define GAMESTATE_H

#include<vector>

#include<QIODevice>
#include<QXmlStreamWriter>

using std::vector;

class Serializible
{
public:
    virtual void serialize(QIODevice&) = 0;
    virtual void deserialize(QIODevice&) = 0;
};

class GameState : public Serializible
{
public:
    GameState() { }
    GameState(const vector<vector<short>>&, const unsigned);
    GameState(QIODevice& f) { this->deserialize(f); }
    virtual void serialize(QIODevice&);
    virtual void deserialize(QIODevice&);

    //void setBoard(const short _board[6][6]) : board {_board} { }
    void setStepNum(const unsigned num) { step_num = num; }

    const vector<short>& operator[](unsigned i) const { return board[i]; }
    const vector<vector<short>>& getBoard() const { return board; }
    unsigned getStepNum() const { return step_num; }

    unsigned getRowCount() { return height; }
    unsigned getColCount() { return width; }

private:
    unsigned width = 6, height = 6;
    vector<vector<short>> board;
    unsigned step_num;
};

class Originator
{
public:
    virtual GameState saveGame() = 0;
    virtual void restoreGame(GameState&) = 0;
};

#endif // GAMESTATE_H
