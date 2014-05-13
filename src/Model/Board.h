#ifndef BOARD_H
#define BOARD_H

#include "Model/serialization.h"

class Board : public IOriginator {
public:
	enum RotateDirection { Left, Right };

	Board();
    virtual ~Board() { }
	bool putStone(short, short, short);
	void Rotate(short, RotateDirection);
	
    //TODO: треба зробити один з них const
	short& operator()(short, short);
	short* operator[](short);
	
	void Clear();

    virtual GameState SaveGame();
    virtual void RestoreGame(GameState&);

private:
    typedef short TheBoard[6][6];
    TheBoard board;
	unsigned stepNum;
};

#endif /* BOARD_H */
