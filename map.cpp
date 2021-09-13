#include "map.h"

Map::Map()
{
    width = 0;
    height = 0;
    scale = 1;

    mode = Navigate;
    position = Bottom;

    currentTexture = -1;

    map = (Tile*)malloc(0);
    sprites = QList<Sprite>();
}

void Map::UpdateRect(){
    setSceneRect(0, 0, round((double)width * (double)TILE_WIDTH * scale), round((double)height * (double)TILE_WIDTH * scale));
}

void Map::UpdateMap(int lwidth, int lheight){
    map = (Tile*)realloc(map, sizeof(Tile) * width * height);
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            (map + x * height + y)->bottomOffset = {0, 0};
            (map + x * height + y)->bottomTexture = -1;
            (map + x * height + y)->topOffset = {0, 0};
            (map + x * height + y)->topTexture = -1;
            (map + x * height + y)->isWall = false;
            (map + x * height + y)->margins.setTop(0);
            (map + x * height + y)->margins.setBottom(0);
            (map + x * height + y)->margins.setLeft(0);
            (map + x * height + y)->margins.setRight(0);
        }
    }
}

void Map::SetTile(int x, int y){
    if(mode == Edit){
        if(position == Bottom)
            (map + x * height + y)->bottomTexture = currentTexture;
        else
            (map + x * height + y)->topTexture = currentTexture;
    }else if(mode == Delete){
        if(position == Bottom)
            (map + x * height + y)->bottomTexture = -1;
        else
            (map + x * height + y)->topTexture = -1;
    }
}

void Map::SetWidth(int width){
    int lwidth = this->width;
    this->width = width;
    UpdateRect();
    UpdateMap(lwidth, height);
}

void Map::SetHeight(int height){
    int lheight = this->height;
    this->height = height;
    UpdateRect();
    UpdateMap(width, lheight);
}

void Map::SetScale(double scale){
    this->scale = scale;
    UpdateRect();
}

void Map::drawBackground(QPainter *painter, const QRectF &rect){
    QGraphicsScene::drawBackground(painter, rect);
    painter->setPen(Qt::black);
    for(int x = 0; x <= width; x++){
        int pos = round((double)x * (double)TILE_WIDTH * scale);
        painter->drawLine(pos, 0, pos, round((double)height * (double)TILE_WIDTH * scale));
    }
    for(int y = 0; y <= height; y++){
        int pos = round((double)y * (double)TILE_WIDTH * scale);
        painter->drawLine(0, pos, round((double)width * (double)TILE_WIDTH * scale), pos);
    }
}

void Map::drawForeground(QPainter *painter, const QRectF &rect){
    QGraphicsScene::drawForeground(painter, rect);
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            Tile tile = *(map + x * height + y);
            if(tile.bottomTexture != -1)
                painter->drawPixmap(round((double)x * (double)TILE_WIDTH * scale + tile.bottomOffset.x() * scale) + 1,
                                  round((double)y * (double)TILE_WIDTH * scale + tile.bottomOffset.y() * scale) + 1,
                                  round((double)TILE_WIDTH * scale) - 1,
                                  round((double)TILE_WIDTH * scale) - 1,
                                  sprites[tile.bottomTexture].data);
            if(tile.topTexture != -1)
                painter->drawPixmap(round((double)x * (double)TILE_WIDTH * scale + tile.topOffset.x() * scale) + 1,
                                  round((double)y * (double)TILE_WIDTH * scale + tile.topOffset.y() * scale) + 1,
                                  round((double)TILE_WIDTH * scale) - 1,
                                  round((double)TILE_WIDTH * scale) - 1,
                                  sprites[tile.topTexture].data);
            if(tile.isWall){
                painter->setPen(Qt::red);
                painter->drawRect(round((double)x * (double)TILE_WIDTH * scale + (double)tile.margins.left() * scale),
                                  round((double)y * (double)TILE_WIDTH * scale + (double)tile.margins.top() * scale),
                                  round((double)TILE_WIDTH * scale - (double)tile.margins.right() * scale - (double)tile.margins.left() * scale),
                                  round((double)TILE_WIDTH * scale - (double)tile.margins.bottom() * scale - (double)tile.margins.top() * scale));
            }
        }
    }
}

void Map::wheelEvent(QGraphicsSceneWheelEvent *event){
    QGraphicsScene::wheelEvent(event);
    if(event->modifiers() == Qt::ControlModifier){
        if(event->delta() < 0){
            if(scale > 0)
                scale -= 0.1;
        }else
            scale += 0.1;
        UpdateRect();
        update();
    }else{
        if(event->delta() < 0){
            position = Bottom;
        }else{
            position = Top;
        }
    }
}

void Map::mousePressEvent(QGraphicsSceneMouseEvent *event){
    QPointF point = event->scenePos();
    if(point.x() >= 0 && point.y() >= 0){
        int x = floor((double)point.x() / ((double)TILE_WIDTH * scale));
        int y = floor((double)point.y() / ((double)TILE_WIDTH * scale));
        if(x < width && y < height){
            if(event->button() == Qt::LeftButton){
                SetTile(x, y);
            }else if(event->button() == Qt::RightButton && mode == Edit){
                Tile tile = *(map + x * height + y);
                TileSetup* tileSetup = new TileSetup();
                tileSetup->pos = {x, y};
                tileSetup->tile = tile;
                tileSetup->sprites = sprites;
                tileSetup->Init();
                if(tileSetup->exec() == QDialog::Accepted){
                    (map + x * height + y)->bottomOffset = tileSetup->tile.bottomOffset;
                    (map + x * height + y)->topOffset = tileSetup->tile.topOffset;
                    (map + x * height + y)->isWall = tileSetup->tile.isWall;
                    (map + x * height + y)->margins = tileSetup->tile.margins;
                }
            }
        }

        update();
    }
}

void Map::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    QPointF point = event->scenePos();
    if(point.x() >= 0 && point.y() >= 0){
        int x = floor((double)point.x() / ((double)TILE_WIDTH * scale));
        int y = floor((double)point.y() / ((double)TILE_WIDTH * scale));
        if(x < width && y < height){
            if(event->buttons() == Qt::LeftButton){
                SetTile(x, y);
                update();
            }
        }
    }
}
