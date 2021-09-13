#ifndef STRUCTS_H
#define STRUCTS_H

#include <QString>
#include <QPointF>
#include <QPixmap>

typedef struct{
    short bottomTexture;
    short topTexture;
    QPointF bottomOffset;
    QPointF topOffset;
    bool isWall;
    QMargins margins;
}Tile;

typedef struct{
    short id;
    QString name;
    int width;
    int height;
    QPixmap data;
}Sprite;

typedef struct{
    QString name;
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
}Texture;

#endif // STRUCTS_H
