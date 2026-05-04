#ifndef PLAYER_H
#define PLAYER_H

#include <QtQuick/QQuickPaintedItem>

class Player : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_DISABLE_COPY(Player)
public:
    explicit Player(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;
    ~Player() override;
};

#endif // PLAYER_H
