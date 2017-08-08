#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <MidiManager.h>
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

    void on_PauseButton_clicked();

    void on_StartButton_clicked();

    void playSong();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
