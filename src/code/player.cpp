#include "player.h"

#include <QPainter>
#include <qmmp/soundcore.h>
#include <qmmp/abstractengine.h>
#include <qmmp/output.h>

Player::Player(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    // By default, QQuickItem does not draw anything. If you subclass
    // QQuickItem to create a visual item, you will need to uncomment the
    // following line and re-implement updatePaintNode()

    // setFlag(ItemHasContents, true);
}

void Player::paint(QPainter *painter)
{
    QPen pen(QColorConstants::Red, 2);
    QBrush brush(QColorConstants::Red);

    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(0, 0, 100, 100);
}

Player::~Player() {}
