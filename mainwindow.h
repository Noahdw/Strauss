#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <MidiManager.h>
#include <QGraphicsScene>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(MidiManager *mngr,QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_quitButton_clicked();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_PauseButton_clicked(int type);

    void on_StartButton_clicked();

    void playSong();

    void updatePROLL(int x,int y,int width,int start, int length);

    void deleteFromPROLL(QGraphicsItem *item);

    void on_actionPlay_triggered();

    void updateSceneRect(QRectF newRect,const QRectF *oldRect,QRectF visibleRect);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
};

#endif // MAINWINDOW_H
