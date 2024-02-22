#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_decimation_clicked();

    void on_vigenere_clicked();

    void on_playfer_clicked();

    void on_UIinput_clicked();

    void on_fileinput_clicked();

    void on_decrypt_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
