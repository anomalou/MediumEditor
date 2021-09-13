#ifndef EDITOR_H
#define EDITOR_H

#include <QMainWindow>
#include <map.h>
#include <QScrollBar>
#include <QFileDialog>
#include <QLabel>

namespace Ui {
class Editor;
}

class Editor : public QMainWindow
{
    Q_OBJECT

private:
    Map *map;

public:
    explicit Editor(QWidget *parent = nullptr);
    ~Editor();

private:
    void UpdateSprites();

protected:
    bool eventFilter(QObject *obj, QEvent *ev);
    void closeEvent(QCloseEvent* event);

private slots:
    void on_map_width_valueChanged(int arg1);
    void on_map_height_valueChanged(int arg1);
    void on_actionNavigate_triggered();
    void on_actionEdit_triggered();
    void on_actionDelete_triggered();
    void on_sprites_cellClicked(int row, int column);
    void on_loadAtlas_clicked();
    void on_actionTop_triggered();
    void on_actionBottom_triggered();

private:
    Ui::Editor *ui;
};

#endif // EDITOR_H
