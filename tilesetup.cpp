#include "tilesetup.h"
#include "ui_tilesetup.h"

TileSetup::TileSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TileSetup)
{
    ui->setupUi(this);
}

TileSetup::~TileSetup()
{
    delete ui;
}

void TileSetup::on_buttonBox_accepted()
{
    float bx = ui->bx->text().toFloat();
    float by = ui->by->text().toFloat();
    float tx = ui->tx->text().toFloat();
    float ty = ui->ty->text().toFloat();

    int up = ui->up->text().toInt();
    int down = ui->down->text().toInt();
    int left = ui->left->text().toInt();
    int right = ui->right->text().toInt();

    tile.bottomOffset = {bx, by};
    tile.topOffset = {tx, ty};
    tile.margins = QMargins(left, up, right, down);
}

void TileSetup::Init(){
    if(tile.topTexture != -1)
        ui->top->setPixmap(sprites[tile.topTexture].data);
    if(tile.bottomTexture != -1)
        ui->bottom->setPixmap(sprites[tile.bottomTexture].data);

    ui->x->setText("x: " + QString::number(pos.x()));
    ui->y->setText("y: " + QString::number(pos.y()));

    ui->bx->setText(QString::number(tile.bottomOffset.x()));
    ui->by->setText(QString::number(tile.bottomOffset.y()));
    ui->tx->setText(QString::number(tile.topOffset.x()));
    ui->ty->setText(QString::number(tile.topOffset.y()));

    ui->up->setText(QString::number(tile.margins.top()));
    ui->down->setText(QString::number(tile.margins.bottom()));
    ui->left->setText(QString::number(tile.margins.left()));
    ui->right->setText(QString::number(tile.margins.right()));

    ui->wall->setChecked(tile.isWall);
}

void TileSetup::on_wall_toggled(bool checked)
{
    tile.isWall = checked;
}
