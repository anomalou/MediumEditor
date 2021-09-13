#ifndef MAP_H
#define MAP_H

#include <QObject>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsSceneWheelEvent>
#include <QDebug>
#include <structs.h>
#include <QList>
#include <tilesetup.h>

#define TILE_WIDTH 30

typedef enum{
    Navigate,
    Edit,
    Delete
}Mode;

typedef enum{
    Top,
    Bottom
}Position;

class Map : public QGraphicsScene
{
    Q_OBJECT

private:
    int width;
    int height;

    double scale;
    Tile* map;

public:
    Mode mode;
    Position position;
    short currentTexture;
    QList<Sprite> sprites;

private:
    void UpdateRect();
    void UpdateMap(int lwidth, int lheight);
    void SetTile(int x, int y);

public:
    Map();
    void SetWidth(int width);
    void SetHeight(int height);
    void SetScale(double scale);
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void drawForeground(QPainter *painter, const QRectF &rect) override;
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // MAP_H
