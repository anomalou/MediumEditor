#ifndef TILESETUP_H
#define TILESETUP_H

#include <QDialog>
#include <structs.h>

namespace Ui {
class TileSetup;
}

class TileSetup : public QDialog
{
    Q_OBJECT

public:
    QPoint pos;
    Tile tile;
    QList<Sprite> sprites;

public:
    explicit TileSetup(QWidget *parent = nullptr);
    ~TileSetup();
    void Init();

private slots:
    void on_buttonBox_accepted();

    void on_wall_toggled(bool checked);

private:
    Ui::TileSetup *ui;
};

#endif // TILESETUP_H
