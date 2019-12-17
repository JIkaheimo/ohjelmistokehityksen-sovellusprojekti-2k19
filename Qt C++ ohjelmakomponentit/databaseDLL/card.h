#ifndef CARD_H
#define CARD_H

#include <QString>

class Card
{

public:
    QString validate(const QString& cardNumber, const int cardPin) const;
};

#endif // CARD_H


