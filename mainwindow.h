#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QVBoxLayout>
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

 private slots:
    void on_Train_clicked();                        //on click event of Train button
    void on_ProcessImage_clicked();                 //on click event of Process Image button
private:
   Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
