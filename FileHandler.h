#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <fstream>
#include <iostream>

using namespace std;
class FileSource
{
   public:
    string ItemName;
    string ItemPath;
};

string GetPath(string t)
{
    fstream file;
    FileSource a;
    file.open("Paths.txt", ios::in);
    file.read((char*)&a, sizeof(a));
    while(!file.eof())
    {
        if((t=="dataset") && (a.ItemName=="Dataset"))               //for dataset
        {
             return a.ItemPath;
        }
        else if(t=="image" && a.ItemName=="Images")                 //for images to be processed
        {
             return a.ItemPath;
        }
        else if(t=="Theta1" && a.ItemName=="Theta1")                //for Theta 1
        {
             return a.ItemPath;
        }
        else if(t=="Theta2" && a.ItemName=="Theta2")                //for Theta 2
        {
             return a.ItemPath;
        }
        else if(t=="ILDW" && a.ItemName=="InLayerBiasWeights")      //for in layer Weights
        {
             return a.ItemPath;
        }
        else if(t=="HBW" && a.ItemName=="HiddenBiasWeights")        //for hidden layer weights
        {
             return a.ItemPath;
        }
        file.read((char*)&a, sizeof(a));
      }
    file.close();
    }

#endif // FILEHANDLER_H
