#include "serialization.h"
#include <QStringList>

GameState::GameState(const vector<vector<short>>& _board, const unsigned num) : width{_board[0].size()}, height{_board.size()}, board{_board}, stepNum{num} {
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
            stream.writeCharacters(QString::number(this->board[i][j]) + " ");
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
                QStringList tmp = stream.readElementText().simplified().split(" ");
                board = std::move(vector<vector<short>>(height, vector<short>(width, 0)));
                for (uint i = 0; i < height; i++)
                    for (uint j = 0; j < width; j++)
                        this->board[i][j] = QString(tmp[i*height+j]).toShort();
            }
        }
    }

    f.close();
}
