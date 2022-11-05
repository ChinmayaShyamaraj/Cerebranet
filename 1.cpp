//#include "H:\\XII C\\20\\Project\\TestNet\\cerebranet.h"
#include "/home/chinmaya/PROJECT/v1.21/cerebranet.h"
using namespace std;
int train(string path, fmat expected)
{
        //open file
        cout<<"opening file"<<endl;
        int numcols,numrows,maxval;
        fstream file;
        char p[path.size()+1];
        strcpy(p,path.c_str());
        file.open(p, ios::in | ios::binary);
        if(!file) {cout<<"file term"<<endl; return 0;}

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
              InLayer(i*numcols+j,0)=pixelval;
            }
        }

        cout<<"Image read"<<endl;
        cout<<"Image directory: "<<path<<endl;
        //forward and backprop calls
        fwdprop();
        cout<<"Forward prop"<<endl;
        backprop(expected);
        cout<<"Backprop"<<endl;

        file.close();
        return 1;

}


int main()
{
 //   get_weights();

    stringstream ss;


    //go through all files to train
    fmat expected(10,1);

    for(long i=1; i<=647; i++)
    {
        for(int j=0; j<10; j++)
        {
            ss.str("");
            string path="";

            ss<<"\\home\\chinmaya\\PROJECT\\trainingSamplepgm\\"<<j<<"\\img_"<<i<<"_result.pgm";
            path=ss.str();

            if(!isfile(path)) continue;

            expected.zeros();
            expected(j,0)=1;

            train(path,expected);
        }
    }
   write_weights();

    return 0;
}
