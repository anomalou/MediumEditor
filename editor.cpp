#include "editor.h"
#include "ui_editor.h"

Editor::Editor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Editor)
{
    ui->setupUi(this);

    map = new Map();

    ui->map->setScene(map);
    ui->map->verticalScrollBar()->installEventFilter(this);
    ui->map->horizontalScrollBar()->installEventFilter(this);
}

bool Editor::eventFilter(QObject *obj, QEvent *ev){
    if(ev->type() == QEvent::Wheel){
        if(obj == ui->map->verticalScrollBar() || obj == ui->map->horizontalScrollBar())
            return true;
    }
    return false;
}

void Editor::closeEvent(QCloseEvent* event){

//    QMainWindow* parent = dynamic_cast<QMainWindow*>(this->parent());
//    parent->show();

//    QMainWindow::closeEvent(event);
}

Editor::~Editor()
{
    delete ui;
}

void Editor::UpdateSprites(){
    ui->sprites->setRowCount(0);
    ui->sprites->setRowCount(map->sprites.size());
    for(int i = 0; i < map->sprites.size(); i++){
        QLabel *item = new QLabel();
        item->setPixmap(map->sprites[i].data);
        ui->sprites->setCellWidget(i, 0, item);
        ui->sprites->setItem(i, 1, new QTableWidgetItem(map->sprites[i].name));
        ui->sprites->setItem(i, 2, new QTableWidgetItem(QString::number(map->sprites[i].width) + "x" + QString::number(map->sprites[i].height)));
    }
}

void Editor::on_map_width_valueChanged(int arg1)
{
    map->SetWidth(arg1);
    map->update();
}

void Editor::on_map_height_valueChanged(int arg1)
{
    map->SetHeight(arg1);
    map->update();
}

void Editor::on_actionNavigate_triggered()
{
    map->mode = Navigate;
    ui->map->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->mode->setText("Navigate");
}

void Editor::on_actionEdit_triggered()
{
    map->mode = Edit;
    ui->map->setDragMode(QGraphicsView::NoDrag);
    ui->mode->setText("Edit");
}

void Editor::on_actionDelete_triggered()
{
    map->mode = Delete;
    ui->map->setDragMode(QGraphicsView::NoDrag);
    ui->mode->setText("Delete");
}

void Editor::on_sprites_cellClicked(int row, int column)
{
    map->currentTexture = row;
    ui->preview->setPixmap(map->sprites[row].data);
}

void Editor::on_loadAtlas_clicked()
{
    QString path = "";
    path = QFileDialog::getOpenFileName(this, "Select atlas", ".", "ATA (*.ata)");
    if(path != ""){
        QFile file(path);
        if(file.open(QIODevice::ReadOnly)){
            QDataStream stream(&file);
            stream.setByteOrder(QDataStream::LittleEndian);
            QList<Texture> textures = QList<Texture>();
            map->sprites.clear();
            int size;
            stream >> size;
            for(int i = 0; i < size; i++){
                int length;
                stream >> length;
                char* str = (char*)malloc(sizeof(char) * length);
                stream.readRawData(str, length);
                uint16_t x;
                uint16_t y;
                uint16_t w;
                uint16_t h;
                stream >> x >> y >> w >> h;
                QString name = str;
                name.truncate(length);
                Texture tex = {
                    name,
                    x,
                    y,
                    w,
                    h,
                };
                textures.push_back(tex);
            }

            int width;
            int height;

            stream >> width >> height;

            int byteCount = width * height * 4;

            uchar* data = (uchar*)malloc(sizeof(uchar) * byteCount);
            stream.readRawData((char*)data, byteCount);

            QImage atlas = QImage(data, width, height, QImage::Format_RGBA8888);

            for(int i = 0; i < size; i++){
                int x_offset = textures[i].x;
                int y_offset = textures[i].y;
                int width = textures[i].width;
                int height = textures[i].height;;
                QImage image = QImage(width, height, QImage::Format_ARGB32);
                for(int x = 0; x < width; x++){
                    for(int y = 0; y < height; y++){
                        QColor pixel = QColor::fromRgba(atlas.pixel(x + x_offset, atlas.height() - y_offset - height + y));
                        image.setPixelColor(x, y, pixel);
                    }
                }
                QPixmap pixmap = QPixmap::fromImage(image);
                Sprite sprite = {
                    (short)i,
                    textures[i].name,
                    width,
                    height,
                    pixmap
                };
                map->sprites.push_back(sprite);
            }
            UpdateSprites();
        }
    }
}

void Editor::on_actionTop_triggered()
{
    map->position = Top;
    ui->position->setText("Top");
}

void Editor::on_actionBottom_triggered()
{
    map->position = Bottom;
    ui->position->setText("Bottom");
}
