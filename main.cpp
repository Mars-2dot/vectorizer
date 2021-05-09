#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

Mat src_gray;
Mat srcShow;
int minLineLength = 50;
int maxLineGap = 23;
int rho = 1;
int lineCount = 0;
int thresHoldVar = 26;
RNG rng( 12345 );
string nameWindowResult = "Result", fileToTest;
vector<Vec4i> linesP;
vector<Vec4i> test;

vector<string>& loadData();
vector<Vec4i>& findHoughLines( string& files );
void makeObjFile( vector<Vec4i>& lines, int count );
void threshHold( int, void* );
void minLineLen( int, void* );
void maxLineLen( int, void* );
void rhoLine( int, void* );

int main(  )
{
    vector<string>  dataSource = loadData();
    cout << "Start convert. Waiting..." << endl;

    for ( size_t i = 0; i < dataSource.size() ; i++ ) {
        makeObjFile( findHoughLines( dataSource[( int )i] ), i );
    }

    cout << "Convert finish" << endl;

    fileToTest = dataSource[5];
    srcShow = imread( fileToTest, IMREAD_GRAYSCALE  );
    namedWindow( nameWindowResult );

    createTrackbar( "Thresh:", nameWindowResult, &thresHoldVar, 500, threshHold );
    createTrackbar( "Min line:", nameWindowResult, &minLineLength, 500, minLineLen );
    createTrackbar( "Max line:", nameWindowResult, &maxLineGap, 500, maxLineLen );
    createTrackbar( "Rho: ", nameWindowResult, &rho, 500, rhoLine );

    imshow( nameWindowResult, srcShow );

    waitKey();
    return 0;
}

vector<string>& loadData()
{
    vector<string>* data = new vector<string>;
    string fileName = "/resources/input", fileType = ".png", path = filesystem::current_path().generic_string();
    int count = 0;

    while ( filesystem::exists( path + fileName + to_string( count ) + fileType ) ) {
        data->push_back( string( path + fileName + to_string( count ) + fileType ) );
        count++;
    }

    return *data;
}

vector<Vec4i>& findHoughLines( string& files )
{
    Mat dst, cdst, cdstP;
    Mat src = imread( files, IMREAD_GRAYSCALE );
    test.erase( test.begin(), test.end() );
    cout << "Image number: " << lineCount << " ";

    if ( src.empty() ) {
        cout << endl << "Error opening image" << endl;
    }

    Canny( src, dst, 50, 200, 3 );

    HoughLinesP( dst, linesP, rho, CV_PI / 180, thresHoldVar, minLineLength, maxLineGap );

    int range = 10;

    for ( size_t i = 1; i < linesP.size(); i++ ) {
        for ( size_t j = i - 1; j < linesP.size() - 1; j++ ) {
            long long nextX = ( linesP[( int )i][0] + linesP[( int )i][1]  );
            long long prevX = ( linesP[( int )j][0] + linesP[( int )j][1]  );
            long long nextY = ( linesP[( int )i][2] + linesP[( int )i][3]  );
            long long prevY = ( linesP[( int )j][2] + linesP[( int )j][3]  );

            if ( ( ( nextX - prevX < range ) && ( nextX - prevX > -range ) )
                    && ( ( nextY - prevY < range ) && ( nextY - prevY > -range ) ) ) {
                test.push_back( linesP[j] );
            }
        }
    }

    cout << "\tLines found: " << test.size() << endl;
    lineCount++;
    return test;
}

void makeObjFile( vector<Vec4i>& lines, int count )
{
    string fileName = filesystem::current_path().generic_string() + "/output/output", fileType = ".obj";
    fstream objFile( fileName + to_string( count ) + fileType, ios::out );

    objFile << "o Find lines" << count << endl;

    for ( size_t i = 0; i < lines.size(); i++ ) {
        if ( objFile.is_open() ) {
            objFile << "v " << lines[( int )i][0] << " 0 " <<  lines[( int )i][1] << endl;
            objFile << "v " << lines[( int )i][2] << " 0 " << lines[( int )i][3] << endl;
        }
    }

    objFile << "vt " << 1 << " 1 " << 1 << endl;
    objFile << "vn " << 1 << " 1 " << 1 << endl;
    objFile << "usemtl Material" << endl << "s off" << endl;

    int line = 1;

    for ( size_t i = 0; i < lines.size(); i = i + 2 ) {

        if ( objFile.is_open() ) {
            objFile << "f " << line << "/" << 1 << "/" << 1 << " ";
            objFile << ++line << "/" << 1 << "/" << 1 << endl;
        }

        line = i;
        line++;
    }

    objFile.close();
}


void threshHold( int, void* )
{
    srcShow = imread( fileToTest, IMREAD_GRAYSCALE  );
    Canny( srcShow, srcShow, 50, 200, 3 );
    HoughLinesP( srcShow, linesP, rho, CV_PI / 180, thresHoldVar, minLineLength, maxLineGap );
    cvtColor( srcShow, srcShow, COLOR_GRAY2BGR );

    for ( size_t i = 0; i < linesP.size(); i++ ) {
        Vec4i l = linesP[i];
        line( srcShow, Point( l[0], l[1] ), Point( l[2], l[3] ), Scalar( 0, 0, 255 ), 3, LINE_AA );
    }

    imshow( nameWindowResult, srcShow );
    cout << "Lines found: " << linesP.size() << endl;
}


void minLineLen( int, void* )
{
    srcShow = imread( fileToTest, IMREAD_GRAYSCALE  );
    Canny( srcShow, srcShow, 50, 200, 3 );
    HoughLinesP( srcShow, linesP, rho, CV_PI / 180, thresHoldVar, minLineLength, maxLineGap );
    cvtColor( srcShow, srcShow, COLOR_GRAY2BGR );

    for ( size_t i = 0; i < linesP.size(); i++ ) {
        Vec4i l = linesP[i];
        line( srcShow, Point( l[0], l[1] ), Point( l[2], l[3] ), Scalar( 0, 0, 255 ), 3, LINE_AA );
    }

    imshow( nameWindowResult, srcShow );
    cout << "Lines found: " << linesP.size() << endl;
}


void maxLineLen( int, void* )
{
    srcShow = imread( fileToTest, IMREAD_GRAYSCALE  );
    Canny( srcShow, srcShow, 50, 200, 3 );
    HoughLinesP( srcShow, linesP, rho, CV_PI / 180, thresHoldVar, minLineLength, maxLineGap );
    cvtColor( srcShow, srcShow, COLOR_GRAY2BGR );

    for ( size_t i = 0; i < linesP.size(); i++ ) {
        Vec4i l = linesP[i];
        line( srcShow, Point( l[0], l[1] ), Point( l[2], l[3] ), Scalar( 0, 0, 255 ), 3, LINE_AA );
    }

    imshow( nameWindowResult, srcShow );
    cout << "Lines found: " << linesP.size() << endl;
}


void rhoLine( int, void* )
{
    srcShow = imread( fileToTest, IMREAD_GRAYSCALE  );
    Canny( srcShow, srcShow, 50, 200, 3 );
    HoughLinesP( srcShow, linesP, rho, CV_PI / 180, thresHoldVar, minLineLength, maxLineGap );
    cvtColor( srcShow, srcShow, COLOR_GRAY2BGR );

    for ( size_t i = 0; i < linesP.size(); i++ ) {
        Vec4i l = linesP[i];
        line( srcShow, Point( l[0], l[1] ), Point( l[2], l[3] ), Scalar( 0, 0, 255 ), 3, LINE_AA );
    }

    imshow( nameWindowResult, srcShow );
    cout << "Lines found: " << linesP.size() << endl;
}
