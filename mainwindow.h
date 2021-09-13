#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDialog>
#include <QMessageBox>
#include <QImage>
#include <QFile>
#include <QDebug>
#include <QDataStream>
#include <editor.h>
#include <structs.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_load_clicked();

    void on_cut_clicked();

    void on_textures_cellDoubleClicked(int row, int column);

    void on_create_clicked();

    void on_load_atlas_clicked();

    void on_actionLittle_eridian_triggered();

    void on_actionBig_eridian_triggered();

    void on_actionMap_editor_triggered();

private:
    Ui::MainWindow *ui;

private:
    QDataStream::ByteOrder order;
    QList<Texture> textures;
    QImage atlasImg;
    Editor* editor;
};
#endif // MAINWINDOW_H
