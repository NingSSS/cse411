#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <chrono>
#include <string>
#include <stdio.h>
#include <typeinfo>
#include <vector>
#include <string>
#include <stack>
#include <mutex>
#include <list>
#include <future>
#include <iostream>
#include <algorithm>
#include <random>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <tbb/tbb.h>
#include "tbb/task_scheduler_init.h"
#include "tbb/tick_count.h"
#include "unistd.h"

using namespace std;
using namespace boost;
using namespace cv;
using namespace tbb;

int row;
int column;
int siz;
string text;


/** Store the arguments for our program */
struct arg_t {
    /** The input image */
    std::string in_img = "";

    /** The output image */
    std::string out_img = "";

    /** The output video */
    std::string out_vid = "";

    int fps ;

    int cell_length;

    int round;

    std::string textname = "";

    /** Should we show the usage */
    bool usage = false;
};

/**
 * Parse the command-line arguments, and use them to populate the provided args
 * object.
 *
 * @param argc The number of command-line arguments passed to the program
 * @param argv The list of command-line arguments
 * @param args The struct into which the parsed args should go
 */
void parse_args(int argc, char **argv, arg_t &args) {
    using namespace std;
    long opt;
    while ((opt = getopt(argc, argv, "i:o:f:p:r:w:s:")) != -1) {
        switch (opt) {
            case 'i':
                args.in_img = string(optarg);
                break;
            case 'o':
                args.out_vid = string(optarg);
                break;
            case 'f':
                args.fps = stoi(optarg);
                break;
            case 'p':
                args.cell_length = stoi(optarg);
                break;
            case 'r':
                args.round = stoi(optarg);
                break;
            case 'w':
                args.textname = string(optarg);
                break;
            case 's':
                args.out_img = string(optarg);
                break;
        }
    }
}

cv::Mat conway(vector<vector<int>>& array){

    vector<vector<int> > bigarray(row+2,vector<int>(column+2));



    for(int i=0;i<row;i++){
        for(int j=0;j<column;j++){
            bigarray[i+1][j+1] = array[i][j];
        }
    }


    int xk[8] = {-1,0,1,-1,1,-1,0,1};
    int yk[8] = {-1,-1,-1,0,0,1,1,1};

    for(int i=1; i<row+1; i++){
        for(int j=1;j<column+1;j++){
            int neiboor = 0;

            //if(i>0&&j>0&&i<row-1&&j<column-1){
            for(int k=0;k<8;k++){
                if(bigarray[i+xk[k]][j+yk[k]] == 1)
                    neiboor++;
            }

            if(neiboor<2&&bigarray[i][j]==1){
                array[i-1][j-1] =0;
            }

            if((neiboor==2 || neiboor==3)&& (bigarray[i][j]==1)){
                array[i-1][j-1] =1;
            }

            if(neiboor>3&&bigarray[i][j]==1){
                array[i-1][j-1] =0;
            }

            if(neiboor==3&&bigarray[i][j]==0){
                array[i-1][j-1] =1;
            }

            //}
        }
    }

    Mat img = Mat::zeros(row*siz,column*siz,CV_8UC1);
    //putText(img, "Ning", Point(5,25), FONT_HERSHEY_DUPLEX, 0.8, cvScalar(100, 0, 0), 1, CV_AA);
    parallel_for(blocked_range<int>(0,row,1) , [&](blocked_range<int> &r) {
        for (int i = r.begin(); i != r.end(); ++i) {
            //for(int i=0;i<row;i++){
            for (int j = 0; j < column; j++) {
                if (array[i][j] == 1) {
                    for (int k = i * siz; k < (i + 1) * siz; k++) {
                        for (int l = j * siz; l < (j + 1) * siz; l++) {
                            img.at<uchar>(k, l) = siz;
                        }
                    }
                }

            }
            putText(img, text, Point(50, 250), FONT_HERSHEY_DUPLEX, 2, cvScalar(100, 0, 0), 1, CV_AA);
        }
    });

    //cv::putText(img, "Ning Song", Point(5, 25), FONT_HERSHEY_DUPLEX, 0.8, cvScalar(0, 0, 255), 1);

    return img;
}


int main(int argc, char **argv) {
    arg_t args;
    parse_args(argc, argv, args);
    if (args.usage) {
        cout << "-i: in-image, -j: out-image, -v: out-video" << endl;
        exit(0);
    }
    ifstream infile;
    infile.open(args.in_img);//打开文档
    string str;
    int m;
    int n;
    while (getline(infile,str,'\n')) {
        m++;
        vector<string> strs;
        boost::split(strs,str,is_any_of(" "));
        n = strs.size();
    }
    row = m;
    column = n;
    //cout << row << " " << column <<endl;

    infile.close();




    infile.close();
    text = args.textname;
    siz = args.cell_length;
    ifstream inFile;
    inFile.open(args.in_img,ios::in);

    vector<vector<int> > array(row,vector<int>(column));

    for(int i=0; i<row; i++){
        for(int j=0;j<column;j++){
            inFile>>array[i][j];
        }
    }

    vector<vector<int> > array1(row,vector<int>(column));

    for(int i=0; i<row; i++){
        for(int j=0;j<column;j++){
            array1[i][j] = array[i][j];
        }
    }

    VideoWriter out;
    //args.out_vid = "out.avi";
    bool label = out.open(args.out_vid, CV_FOURCC('M', 'J', 'P', 'G'), args.fps, Size(column*siz, row*siz), false);

    Mat imginit = Mat::zeros(row*siz,column*siz,CV_8UC1);

    for(int i=0;i<row;i++){
        for(int j=0;j<column;j++){
            if(array1[i][j]==1){
                for(int k=i* siz;k<(i+1)*siz;k++){
                    for(int l=j*siz;l<(j+1)*siz;l++){
                        imginit.at<uchar>(k,l)=siz;
                    }
                }
            }

        }
        putText(imginit, text, Point(50, 250), FONT_HERSHEY_DUPLEX, 2, cvScalar(100, 0, 0), 1, CV_AA);
    }

    out << imginit;

    string s;
    s = args.out_img;
    vector<string> wordstrs;
    if (s.empty()){
        cout <<  "Please input the picture that you want"<<endl;
    }else {
        //vector<int> wordints;
        boost::split(wordstrs, s, is_any_of(","));
    }

//    for(int i=0;i<wordstrs.size();i++){
//        cout << wordstrs[i] << endl;
//    }

    for(int i = 0; i <  args.round ; i++){
        Mat result = conway(array);
        if(!s.empty()) {
            for (int j = 0; j < wordstrs.size(); j++) {
                if (i + 1 == stoi(wordstrs[j])) {
                    string s1, s2, s3;
                    s1 = "frame";
                    s2 = ".png";
                    //s3=s1 + wordstrs[j] +s2;
                    imwrite(s1 + wordstrs[j] + s2, result);
                }
            }
        }
        out<< result;
    }
    out.release();

//    for(int i=0; i<row; i++,cout<<endl){
//        for(int j=0;j<column;j++){
//            cout << array1[i][j] << " ";
//        }
//    }
//
//    for(int i=0; i<row; i++,cout<<endl){
//        for(int j=0;j<column;j++){
//            cout << array[i][j] << " ";
//        }
//    }




    return 0;
}