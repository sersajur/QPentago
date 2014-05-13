#include "serialization.h"
#include <QStringList>

GameState::GameState(short (*_board)[6][6], const unsigned num) : stepNum{num} {
    board.clear();
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            board.push_back((*_board)[i][j]);
}

void GameState::Serialize(QIODevice& f) {
    QString serialization;
    QXmlStreamWriter stream(&serialization);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("SaveGame");

    stream.writeStartElement("Board");
    stream.writeAttribute("Width", QString::number(this->width));
    stream.writeAttribute("Height", QString::number(this->height));
    stream.writeAttribute("StepsPerformed", QString::number((this->stepNum)));
    for (uint i = 0; i < height; i++)
    {
        stream.writeCharacters("\n        ");
        for (uint j = 0; j < width; j++)
        {
            stream.writeCharacters(QString::number(this->board[i*width+j]));
        }
    }
    stream.writeCharacters("\n    ");
    stream.writeEndElement();

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
            if (stream.name() == "Board") {
                this->height = stream.attributes().value("Height").toInt();
                this->width = stream.attributes().value("Width").toInt();
                this->stepNum = stream.attributes().value("StepsPerformed").toInt();
                QString tmp = stream.readElementText().simplified().split(" ").join("");
                board.clear();
                for (int i = 0; i < height*width; i++)
                    this->board.push_back(QString(tmp[i]).toShort());
            }
        }
    }

    f.close();
}
