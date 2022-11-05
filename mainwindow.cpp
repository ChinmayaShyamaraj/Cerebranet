#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cerebranet.h>
#include <QInputDialog>
#include <FileHandler.h>
#include <QPixmap>
#include <QProgressDialog>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_Train_clicked()    
{   QProgressDialog progress("Training...", "Cancel", 1, 674, this);    //progress bar
    progress.setWindowModality(Qt::WindowModal);
    Net NeuralNetwork;
    NeuralNetwork.get_weights();
    stringstream ss;
                                                                        //go through all files to train
        fmat expected(10,1);
            for(long i=1; i<=674; i++)
            {   progress.setValue(i);
                if (progress.wasCanceled())
                            break;
                for(int j=0; j<10; j++)
                 {
                     ss.str("");
                     string path="";
                     ss<<"D:\\Cerebranet\\ui\\Cerebranet\\trainingSamplepgm\\"<<j<<"\\img_"<<i<<"_result.pgm";
                     path=ss.str();
                     if(!isfile(path)) continue;
                     expected.zeros();
                     expected(j,0)=1;
                     NeuralNetwork.train(path,expected);               //calling train function
                }
            }
       NeuralNetwork.write_weights();                                  //saving weights
       progress.setValue(674);                                         //setting progressbar to maximum
}//on_train_clicked

void MainWindow::on_ProcessImage_clicked()
{   Net NeuralNetwork;
    NeuralNetwork.get_weights();
    QInputDialog p;
    int n;
    stringstream ss;
    string path;
    n = p.getInt(nullptr,"Process Image", "Enter Image Number:",QLineEdit::Normal);//user enters image number
    if(n<1)
        break;
    ss<<"D:\\Cerebranet\\ui\\Cerebranet\\testSamplepgm\\img_"<<n<<"_result.pgm";
    path=ss.str();
    cout<<"\n\nprocess image clicked.\n";
    float confidence;
    int guess;
    NeuralNetwork.predict(path,confidence,guess);                       //calling predict function
    QString temp=QString::fromStdString(path);
    QPixmap pixmap(temp);
    int w=ui->ImageDisplayLabel->width();
    int h=ui->ImageDisplayLabel->height();
    ui->ImagePathLabel->setText(temp);                                  //dispaying Path
    ui->ImageDisplayLabel->setPixmap(pixmap.scaled(w,h,Qt::KeepAspectRatio));//scaling the image up
    ui->ImageDisplayLabel->show();
    ui->ConfidenceLabel->setNum(confidence);                            //displaying net confidence
    QString t=ui->ConfidenceLabel->text();
    t.append("%");
    ui->ConfidenceLabel->setText(t);
    ui->GuessLabel->setNum(guess);
    cout<<"\nGuess\t:"<<guess<<"\nConfidence:"<<confidence<<"\nImage\t:"<<path;



}
