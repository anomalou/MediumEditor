#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->preview->setScene(new QGraphicsScene());

    textures = QList<Texture>();
    order = QDataStream::LittleEndian;

    editor = new Editor(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_load_clicked()
{
    QString path = "";
    path = QFileDialog::getOpenFileName(this, "Select texture", ".", "PNG (*.png)");
    if(path != ""){
        atlasImg.load(path);
        atlasImg = atlasImg.convertToFormat(QImage::Format_ARGB32);
        QPixmap atlasPixmap = QPixmap::fromImage(atlasImg);
        ui->preview->scene()->setSceneRect(atlasImg.rect());
        ui->preview->scene()->clear();
        ui->preview->scene()->addPixmap(atlasPixmap);
        ui->preview->scene()->invalidate(atlasImg.rect());
        ui->path->setText(path);
        ui->atlas_width->setText(QString::number(atlasImg.rect().width()));
        ui->atlas_height->setText(QString::number(atlasImg.rect().height()));
    }
}

void MainWindow::on_cut_clicked()
{
    QString name = ui->texture_name->text();
    uint16_t x = ui->x_coord->text().toUInt();
    uint16_t y = ui->y_coord->text().toUInt();
    uint16_t width = ui->width->text().toUInt();
    uint16_t height = ui->height->text().toUInt();

    Texture tex = {
        name,
        x,
        y,
        width,
        height
    };

    textures.push_back(tex);

    int rows = ui->textures->rowCount();
    ui->textures->insertRow(rows);

    ui->textures->setItem(rows, 0, new QTableWidgetItem(name));
    ui->textures->setItem(rows, 1, new QTableWidgetItem(QString::number(x)));
    ui->textures->setItem(rows, 2, new QTableWidgetItem(QString::number(y)));
    ui->textures->setItem(rows, 3, new QTableWidgetItem(QString::number(width)));
    ui->textures->setItem(rows, 4, new QTableWidgetItem(QString::number(height)));
}

void MainWindow::on_textures_cellDoubleClicked(int row, int column)
{
    ui->textures->removeRow(row);
    textures.removeAt(row);
}

void MainWindow::on_create_clicked()
{
    QString path = "";
    path = QFileDialog::getSaveFileName(this, "Save atlas", ".", "ATA (*.ata)");

    if(path != "" && !atlasImg.isNull()){
        QFile file(path);
        if(file.open(QIODevice::WriteOnly)){
            QDataStream stream(&file);
            stream.setByteOrder(order);

            QString atlasName = ui->atlas_name->text();

            if(atlasName == ""){
                file.close();
                return;
            }

            short length = atlasName.length();
            stream << length;

            for(short i = 0; i < length; i++){
                uchar c = atlasName.at(i).toLatin1();
                stream << c;
            }

            stream << textures.size();
            for(int i = 0; i < textures.size(); i++){
                int length = textures[i].name.length();
                stream << length;
                for(int t = 0; t < length; t++){
                    QChar c = textures[i].name.at(t);
                    uchar ch = c.toLatin1();
                    stream << ch;
                }
                stream << textures[i].x;
                stream << textures[i].y;
                stream << textures[i].width;
                stream << textures[i].height;
            }

            stream << atlasImg.width();
            stream << atlasImg.height();

            for(int y = 0; y < atlasImg.height(); y++){
                for(int x = 0; x < atlasImg.width(); x++){
                    QColor pixel = QColor::fromRgba(atlasImg.pixel(x, y));
                    qDebug() << pixel;
                    uchar red = pixel.red();
                    uchar green = pixel.green();
                    uchar blue = pixel.blue();
                    uchar alpha = pixel.alpha();
                    stream << red;
                    stream << green;
                    stream << blue;
                    stream << alpha;
                }
            }

            file.close();

            QMessageBox alarm;
            alarm.setWindowTitle("Done");
            alarm.setText("Atlas creation done!");
            alarm.exec();
        }
    }
}

void MainWindow::on_load_atlas_clicked()
{
    QString path = "";
    path = QFileDialog::getOpenFileName(this, "Open atlas", ".", "ATA (*.ata)");

    if(path != ""){
        QFile file(path);
        if(file.open(QIODevice::ReadOnly)){
            QDataStream stream(&file);
            stream.setByteOrder(order);

            QString atlasName;

            short length;
            stream >> length;

            char* str = (char*)malloc(sizeof(char) * length);
            stream.readRawData(str, length);
            ui->atlas_name->setText(str);

            int size;
            textures.clear();
            ui->textures->setRowCount(0);
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
                    h
                };

                textures.push_back(tex);
                int rows = ui->textures->rowCount();
                ui->textures->insertRow(rows);
                ui->textures->setItem(rows, 0, new QTableWidgetItem(name));
                ui->textures->setItem(rows, 1, new QTableWidgetItem(QString::number(x)));
                ui->textures->setItem(rows, 2, new QTableWidgetItem(QString::number(y)));
                ui->textures->setItem(rows, 3, new QTableWidgetItem(QString::number(w)));
                ui->textures->setItem(rows, 4, new QTableWidgetItem(QString::number(h)));
            }

            int width;
            int height;

            stream >> width >> height;

            ui->atlas_width->setText(QString::number(width));
            ui->atlas_height->setText(QString::number(height));

            int byteCount = width * height * 4;

            uchar *bytes = (uchar*)malloc(sizeof(uchar) * byteCount);

            stream.readRawData((char*)bytes, byteCount);

            atlasImg = QImage(bytes, width, height, QImage::Format_RGBA8888);

            atlasImg = atlasImg.convertToFormat(QImage::Format_ARGB32);
            QPixmap atlasPixmap = QPixmap::fromImage(atlasImg);
            ui->preview->scene()->setSceneRect(atlasImg.rect());
            ui->preview->scene()->clear();
            ui->preview->scene()->addPixmap(atlasPixmap);
            ui->preview->scene()->invalidate(atlasImg.rect());
            ui->path->setText(path);
        }
    }
}

void MainWindow::on_actionLittle_eridian_triggered()
{
    order = QDataStream::LittleEndian;
    ui->byte_order->setText("Little endian");
}

void MainWindow::on_actionBig_eridian_triggered()
{
    order = QDataStream::BigEndian;
    ui->byte_order->setText("Big endian");
}

void MainWindow::on_actionMap_editor_triggered()
{
    editor->show();
//    this->hide();
}
