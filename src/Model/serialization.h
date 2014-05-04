#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include<QIODevice>
#include<QXmlStreamWriter>

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
    GameState(QIODevice& f) { this->Deserialize(f); }
    virtual void Serialize(QIODevice&);
    virtual void Deserialize(QIODevice&);
/*
    void setBoard(QString& _board) { board = _board; }
    void setCoordinates(int x, int y) { X = x; Y = y; }
    void setSize(uint _width, uint _height) { width = _width; height = _height; }
    void setScore(uint _score, uint _level, uint _lines, uint _count, uint _iter)
        { score = _score; level = _level; lines = _lines; tetrominosCount = _count; freeFallIterations = _iter; }
    void setNextTetromino(Tetromino*);
    void setCurrentTetromino(Tetromino*);

    QString getBoard() { return board; }
    QVector<int> getCoordinates() { return QVector<int>({ X, Y }); }
    QVector<uint> getSize() { return QVector<uint>({ width, height }); }
    QVector<uint> getScore() { return QVector<uint>({ score, level, lines, tetrominosCount, freeFallIterations }); }
    Tetromino* getNextTetromino();
    Tetromino* getCurrentTetromino();
*/
private:
    /*QString board;
    int X, Y;
    uint width, height;
    uint score, level, lines, tetrominosCount, freeFallIterations;

    int nextShape, nextColor, curShape, curColor;
    QString nextBlocks, curBlocks;*/
};

class IOriginator
{
public:
    virtual GameState SaveState() = 0;
    virtual void RestoreState(GameState&) = 0;
};

#endif // SERIALIZATION_H
