#include <iostream>

#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

void check_confidence(Mat & frame, CascadeClassifier & cascade)
{
  Mat gray;
  std::vector<Rect> faces;
  std::vector<int> reject_levels;
  std::vector<double> weights;

  cvtColor(frame, gray, COLOR_BGR2GRAY);

  cascade.detectMultiScale(frame, faces, reject_levels, weights, 1.1, 5, 0, Size(30,30), Size(), true);

  for (size_t i = 0; i < faces.size(); i++) {
    std::printf("Face %lu : %f\n", i+1, weights[i]); 
  }
}

int main(int argc, const char ** argv)
{
  VideoCapture capture;
  Mat frame;

  if (!capture.open(0, CAP_V4L2)) {
    std::cerr << "Cannot open camera 0 !" << std::endl;
    return 1;
  }

  if (capture.isOpened()) {
    std::cout << "Video capturing has been started ..." << std::endl;

    CascadeClassifier cascade("/usr/share/opencv4/haarcascades/haarcascade_frontalface_default.xml");

    if (cascade.empty()) {
      std::cerr << "failed to open CascadeClassifier !" << std::endl;
      return 1;
    }

    while(true) {
      capture >> frame;

      if (frame.empty()) {
        std::cerr << "No frame is read !" << std::endl;
        break;
      }

      Mat frame1 = frame.clone();

      check_confidence(frame1, cascade);

      char c = (char)waitKey(10);
      if(c == 'q' || c == 'Q' ) {
        std::cout << "Quit" << std::endl;
        break;
      }
    }

    capture.release();
    return 0;
  }


}