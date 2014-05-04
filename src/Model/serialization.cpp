#include "serialization.h"
#include <QStringList>

void GameState::Serialize(QIODevice& f) {
    QString serialization;
    QXmlStreamWriter stream(&serialization);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("SaveGame");
/*
    stream.writeStartElement("Board");
    stream.writeAttribute("Width", QString::number(this->width));
    stream.writeAttribute("Height", QString::number(this->height));
    for (uint i = 0; i < height; i++)
    {
        stream.writeCharacters("\n        " + this->board.mid(i * this->width, this->width));
    }
    stream.writeCharacters("\n    ");
    stream.writeEndElement();

    stream.writeStartElement("CurrentPiece");
    stream.writeAttribute("Shape", QString::number(this->curShape));
    stream.writeAttribute("Color", QString::number(this->curColor));
    stream.writeStartElement("Coordinates");
    stream.writeAttribute("X", QString::number(this->X));
    stream.writeAttribute("Y", QString::number(this->Y));
    stream.writeEndElement();
    stream.writeTextElement("Blocks", this->curBlocks);
    stream.writeEndElement();

    stream.writeStartElement("NextPiece");
    stream.writeAttribute("Shape", QString::number(this->nextShape));
    stream.writeAttribute("Color", QString::number(this->nextColor));
    stream.writeTextElement("Blocks", this->nextBlocks);
    stream.writeEndElement();

    stream.writeStartElement("GameState");
    stream.writeAttribute("Score", QString::number(this->score));
    stream.writeAttribute("Level", QString::number(this->level));
    stream.writeAttribute("Lines", QString::number(this->lines));
    stream.writeAttribute("TotalTetrominosFalled", QString::number(this->tetrominosCount));
    stream.writeAttribute("FreeFallIterations", QString::number(this->freeFallIterations));
    stream.writeEndElement();
*/
    stream.writeEndElement();
    stream.writeEndDocument();
    f.open(QIODevice::WriteOnly);
    f.write(serialization.toUtf8());
    f.close();
}

void GameState::Deserialize(QIODevice& f) {
    f.open(QIODevice::ReadOnly);
    QXmlStreamReader stream(&f);

    while (!stream.atEnd()) {
        stream.readNext();
        if (stream.isStartElement()) {
            /*if (stream.name() == "Board") {
                this->height = stream.attributes().value("Height").toInt();
                this->width = stream.attributes().value("Width").toInt();
                this->board = stream.readElementText().simplified().split(" ").join("");
            }
            if (stream.name() == "CurrentPiece") {
                this->curShape = stream.attributes().value("Shape").toInt();
                this->curColor = stream.attributes().value("Color").toInt();
                stream.readNext();
                stream.readNext();
                this->X = stream.attributes().value("X").toInt();
                this->Y = stream.attributes().value("Y").toInt();
                stream.readNext();
                stream.readNext();
                stream.readNext();
                this->curBlocks = stream.readElementText();
            }
            if (stream.name() == "NextPiece") {
                this->nextShape = stream.attributes().value("Shape").toInt();
                this->nextColor = stream.attributes().value("Color").toInt();
                stream.readNext();
                stream.readNext();
                this->nextBlocks = stream.readElementText();
            }
            if (stream.name() == "GameState") {
                this->score = stream.attributes().value("Score").toInt();
                this->level = stream.attributes().value("Level").toInt();
                this->lines = stream.attributes().value("Lines").toInt();
                this->tetrominosCount = stream.attributes().value("TotalTetrominosFalled").toInt();
                this->freeFallIterations = stream.attributes().value("FreeFallIterations").toInt();
            }*/
        }
    }

    f.close();
}

void GameState::setNextTetromino(Tetromino* next) {
    nextShape = (int)next->getShape();
    nextColor = next->getColor();

    QString tmp;
    for (int i = 0; i < 4; i++)
    {
        tmp.append(QString::number(next->x(i)) + " ");
        tmp.append(QString::number(next->y(i)) + " ");
    }
    nextBlocks = tmp;
}

void GameState::setCurrentTetromino(Tetromino* current) {
    curShape = (int)current->getShape();
    curColor = current->getColor();

    QString tmp;
    for (int i = 0; i < 4; i++)
    {
        tmp.append(QString::number(current->x(i)) + " ");
        tmp.append(QString::number(current->y(i)) + " ");
    }
    curBlocks = tmp;
}

Tetromino* GameState::getNextTetromino() {
    int blocks[4][2];
    QStringList _blocks = nextBlocks.split(" ", QString::SplitBehavior::SkipEmptyParts);
    for (int i = 0; i < 4; i++)
    {
        blocks[i][0] = QString(_blocks[i * 2]).toInt();
        blocks[i][1] = QString(_blocks[i * 2 + 1]).toInt();
    }

    return new Tetromino(Tetromino::TetrominoShape(nextShape), nextColor, blocks);
}

Tetromino* GameState::getCurrentTetromino() {
    int blocks[4][2];
    QStringList _blocks = curBlocks.split(" ", QString::SplitBehavior::SkipEmptyParts);
    for (int i = 0; i < 4; i++)
    {
        blocks[i][0] = QString(_blocks[i * 2]).toInt();
        blocks[i][1] = QString(_blocks[i * 2 + 1]).toInt();
    }

    return new Tetromino(Tetromino::TetrominoShape(curShape), curColor, blocks);
}
