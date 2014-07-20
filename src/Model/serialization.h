#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include<QIODevice>
#include<QXmlStreamWriter>
#include<vector>
using std::vector;

class ISerializible
{
public:
    virtual void Serialize(QIODevice&) = 0;
    virtual void Deserialize(QIODevice&) = 0;
};

class GameState : public ISerializible
{
public:
    GameState() { }
    GameState(vector<vector<short>>, const unsigned);
    GameState(QIODevice& f) { this->Deserialize(f); }
    virtual void Serialize(QIODevice&);
    virtual void Deserialize(QIODevice&);

    //void setBoard(const short _board[6][6]) : board {_board} { }
    void setStepNum(const unsigned num) { stepNum = num; }

    const vector<short>& operator[](unsigned i) const { return board[i]; }
    const vector<vector<short>>& getBoard() const { return board; }
    unsigned getStepNum() const { return stepNum; }

    unsigned getRowCount() { return height; }
    unsigned getColCount() { return width; }

private:
    unsigned width = 6, height = 6;
    vector<vector<short>> board;
    unsigned stepNum;
};

class IOriginator
{
public:
    virtual GameState SaveGame() = 0;
    virtual void RestoreGame(GameState&) = 0;
};

#endif // SERIALIZATION_H
