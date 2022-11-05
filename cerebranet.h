#ifndef CEREBRANET_H
#define CEREBRANET_H
#include<fstream>
#include<iostream>
#include<armadillo>
#include<cstring>
#include<sstream>
#include<FileHandler.h>
using namespace std;
using namespace arma;

fmat activation(fmat z)                 //activation function
{
    return 1/(1+exp(-z));
}

fmat activationgradient(fmat z)         //derivative of activaton function
{
    return (activation(z))%(1-activation(z));
}

int isfile(string path)                 //function to check if file is created
{
            fstream file;
            char *p=new char[path.size()+1];
            strcpy(p,path.c_str());
            file.open(p, ios::in | ios::binary);
            if(file)return 1;
            else return 0;
            file.close();
            delete[] p;
}


class Net                               //Neural Net
{
fmat InLayer,HiddenLayer,OutLayer;
float InLayerBias,HiddenLayerBias;

fmat theta1;
fmat theta2;
fmat InLayerBiasWeights;
fmat HiddenLayerBiasWeights;
fmat error2,error3;
fmat delta1,delta2;
fmat D1,D2;                             //partial derivative terms;
fmat InLayerBiasD1, HiddenLayerBiasD2;
double lambda;                           //regularisation term
double alpha;                            //learning rate

public:
    Net()
    {  InLayer=fmat(784,1);
       HiddenLayer=fmat(784,1);
       OutLayer=fmat(10,1);
       error2=fmat(784,1);
       error3=fmat(10,1);
       delta1=fmat(784,784);
       delta2=fmat(10,784);
       D1=fmat(784,784);
       D2=fmat(10,784);
       InLayerBiasD1=fmat(784,1);
       HiddenLayerBiasD2=fmat(10,1);
       theta1=randu<fmat>(784,784);     //randu assigns random float values
       theta2=randu<fmat>(10,784);
       InLayerBiasWeights=randu<fmat>(784,1);
       HiddenLayerBiasWeights=randu<fmat>(10,1);
       lambda=0.8;                     //regularisation term
       alpha=0.27;                       //learning rate
       InLayerBias=1;
       HiddenLayerBias=1;
    }
    void write_weights();
    void get_weights();
    void predict(string path,float &confidence,int &guess);
    void activation_gradient();
    void forward_prop();
    void back_prop(fmat expected);
    int train(string path, fmat expected);

};
int Net::train(string path, fmat expected)      //Function to train the neural net
{
                                                //open file
    cout<<"opening file"<<endl;
    int numcols,numrows,maxval;
    fstream file;
    char *p=new char[path.size()+1];
    strcpy(p,path.c_str());
    file.open(p, ios::in | ios::binary);
    if(!file) {cout<<"\n[TR] File not found\n"<<endl; return 0;}

    stringstream ss;                            //stringstream is used to perform string operations
    string junk;
    getline(file,junk);
    ss<<file.rdbuf();
    ss>>numcols>>numrows;
    ss>>maxval;
    unsigned char pixelval;
                                                //read file
    for(int i=0; i<numrows;i++)
    {
         for(int j=0; j<numcols; j++)
         {
                  ss>>pixelval;
                  InLayer(i*numcols+j,0)=pixelval;
         }
    }

    cout<<"Image read"<<endl;
    cout<<"Image directory: "<<path<<endl;
                                                //forward and backprop calls
    forward_prop();
    cout<<"Forward prop"<<endl;
    back_prop(expected);
    cout<<"Backprop"<<endl;
    file.close();
    return 1;
}//train

void Net::write_weights()                       //function to write weights onto a file after training
{
    theta1.save("D:\\Cerebranet\\ui\\Cerebranet\\Theta1");
    theta2.save("D:\\Cerebranet\\ui\\Cerebranet\\Theta2");
    InLayerBiasWeights.save("D:\\Cerebranet\\ui\\Cerebranet\\InLayerBiasWeights");
    HiddenLayerBiasWeights.save("D:\\Cerebranet\\ui\\Cerebranet\\HiddenBiasWeights");
}

void Net::get_weights()                        //function to read weights off the files
{
    fmat temp;
    if(temp.load("D:\\Cerebranet\\ui\\Cerebranet\\Theta1"))
        theta1.load("D:\\Cerebranet\\ui\\Cerebranet\\Theta1");
    if(temp.load("D:\\Cerebranet\\ui\\Cerebranet\\Theta2"))
        theta2.load("D:\\Cerebranet\\ui\\Cerebranet\\Theta2");
    if(temp.load("D:\\Cerebranet\\ui\\Cerebranet\\InLayerBiasWeights"))
        InLayerBiasWeights.load("D:\\Cerebranet\\ui\\Cerebranet\\InLayerBiasWeights");
    if(temp.load("D:\\Cerebranet\\ui\\Cerebranet\\HiddenBiasWeights"))
        HiddenLayerBiasWeights.load("D:\\Cerebranet\\ui\\Cerebranet\\HiddenBiasWeights");
}


void Net::forward_prop()                    //forward propogation
{
    fmat z=theta1*InLayer + InLayerBiasWeights*InLayerBias;
  //  cout<<z<<endl<<getch();
    HiddenLayer=activation(z);
  //  cout<<HiddenLayer<<endl<<getch();
    z=theta2*HiddenLayer + HiddenLayerBiasWeights*HiddenLayerBias;
    OutLayer=activation(z);
  //  cout<<OutLayer<<endl<<getch();
}


void Net::back_prop(fmat expected)
{
                                                    //error of each node in all layers
                                                    //no error for input layer
    error3=OutLayer-expected;
    error2=(theta2.t() *error3) % activationgradient((theta1*HiddenLayer));

    delta1.zeros();
    delta2.zeros();

                                                     //computing partial derivatives
    delta2+= error3*HiddenLayer.t();
    delta1+= error2*InLayer.t();

    D1+= delta1/784 + lambda*theta1;                 //lambda
    D2+= delta2/10 + lambda*theta2;

    InLayerBiasD1= error2/784;
    HiddenLayerBiasD2= error3/10;


                                                     //gradient descent
    theta1 -= alpha*D1;
    theta2 -= alpha*D2;
    cout<<theta1;
    InLayerBiasWeights -= alpha*InLayerBiasD1;
    HiddenLayerBiasWeights -= alpha*HiddenLayerBiasD2;

}
void Net::predict(string path,float &confidence,int &guess) //function to pass an image through the net
{
                                                       //open file
        cout<<"opening file"<<endl;
        int numcols,numrows,maxval;
        fstream file;
        file.open(path, ios::in | ios::binary);
        if(!file) {cout<<"\n[PD] File not found\n "<<endl;}
        stringstream ss;
        string junk;
        getline(file,junk);
        ss<<file.rdbuf();
        ss>>numcols>>numrows;
        ss>>maxval;
        unsigned char pixelval;
                                                       //read file
        for(int i=0; i<numrows;i++)
        {
            for(int j=0; j<numcols; j++)
            {
              ss>>pixelval;
              InLayer((i*numcols+j),0)=pixelval;
            }
        }
        cout<<"Image read."<<endl;
        cout<<"Image directory: "<<path<<endl;
                                                       //forward and backprop calls
        forward_prop();
        cout<<"Forward prop"<<endl;
        guess=OutLayer.index_max();
        confidence = OutLayer.max()/accu(OutLayer) *100;
        cout<<OutLayer;
}
#endif
