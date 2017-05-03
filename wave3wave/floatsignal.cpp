// stl includes
#include <iostream>
#include <stdexcept>
#include <algorithm>
// other includes
#include <opencv2/opencv.hpp>
#include <opencv2/plot.hpp>
// own dependencies

// own header
#include "floatsignal.h"
// namespaces
using namespace std;



////////////////////////////////////////////////////////////////////////////////
/// constructor&destructor
////////////////////////////////////////////////////////////////////////////////
FloatSignal::FloatSignal(const string wavPath, const bool norm){
  size = -1;
  content = nullptr;
  sfInfo = new SF_INFO;
  sfInfo->format = 0;
  SNDFILE* infile = sf_open(wavPath.c_str(), SFM_READ, sfInfo);
  if(infile == nullptr){
    throw invalid_argument("FloatSignal: Unable to open input file: "+wavPath);
  } else { // if file opens...
    size = sfInfo->frames;
    content = new float[size]();
    sf_read_float(infile, content, size);
    if(norm){normalize();}
    sf_close(infile);
  }
}

FloatSignal::~FloatSignal(){
  delete sfInfo;
  if(content){delete[] content;}
}


////////////////////////////////////////////////////////////////////////////////
/// getters
////////////////////////////////////////////////////////////////////////////////
SF_INFO* FloatSignal::getSFInfo() const {return sfInfo;}
int FloatSignal::getSize() const {return size;}
float* FloatSignal::getContent(){return content;}

////////////////////////////////////////////////////////////////////////////////
/// further functionality
////////////////////////////////////////////////////////////////////////////////
float* FloatSignal::getDownsampledCopy(const int ratio){
    float* dwn = new float[size/ratio]();
    for (int i=0; i<(size/ratio); ++i){
      dwn[i] = content[i*ratio];
    }
    return dwn;
}



void FloatSignal::toWav(const string pathOut, const bool norm){
  // check that SF_INFO was configured
  if(sfInfo->frames == -1){
    throw invalid_argument("toWav: sfInfo not configured");
  }
  // declare and try to open outfile
  SNDFILE* outFile = sf_open(pathOut.c_str(), SFM_WRITE, sfInfo);
  if(outFile == nullptr){ // if file doesn't open...
    throw invalid_argument("toWav: unable to open output file "+pathOut);
  } else{// if file opens...
    if(norm){normalize();} // side effects!!
    sf_write_float(outFile, content, size);
    sf_close(outFile);
    cout  << "toWav: succesfully saved to "<< pathOut << endl;
  }
}


void FloatSignal::plot(const string title, const bool wait,
                       const int width, const int height){
  const float* fArr = getContent();
  const int size = getSize();
  // copy the float array to a double array (required by cv plot) and put it into a Mat
  double* dArr = new double[size]();
  copy(fArr, fArr+size, dArr);
  cv::Mat data(size, 1, CV_64F, dArr);
  data *= -1; // invert because y axis
  // instantiate and configure plot object based on the data Mat
  cv::Ptr<cv::plot::Plot2d> plot = cv::plot::createPlot2d(data);
  plot->setPlotBackgroundColor(cv::Scalar(10, 10, 10));
  plot->setPlotGridColor(cv::Scalar(10, 10, 10));
  plot->setPlotTextColor(cv::Scalar(10, 10, 10));
  plot->setPlotAxisColor(cv::Scalar(10, 10, 90));
  plot->setPlotLineColor(cv::Scalar(120, 120, 120));
  plot->setPlotSize(width, height);
  plot->setMinY(-1.1);
  plot->setMaxY(1.1);
  // render into result, plot, wait for any key and clean exit
  cv::Mat plot_result;
  plot->render(plot_result);
  cv::namedWindow(title, 1);
  cv::imshow(title, plot_result);
  if(wait){cv::waitKey();}
  delete[] dArr;
}

// private
void FloatSignal::normalize(){
  //cout << "normalizing"<< endl;
  int i=0;
  double sum = 0;
  double max = 0;
  double min = 0;
  // calculate mean into accum and get absolute maximum
  for(i=0; i<size; i++){
    sum += content[i];
    if (content[i] > max){
      max = content[i];
    } else if (content[i] < min){
      min = content[i];
    }
  }
  double mean = sum/size;
  if (max==mean){
    cout << "normalizeArray: normalization not possible when max==mean" << endl;
  } else {
    max -= mean;
    min -= mean;
    double deltanorm = (abs(max)>abs(min))? abs(max) : abs(min);
    // substract accum to each sample, and renormalize
    for(i=0; i<size; i++){
      content[i] = (content[i]-mean)/deltanorm;
    }
  }
}
