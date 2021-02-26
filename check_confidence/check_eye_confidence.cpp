#include <iostream>

#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

void detect_eye(Mat & frame, CascadeClassifier & cascade)
{
  Mat gray;
  std::vector<Rect> eyes;
  std::vector<int> reject_levels;
  std::vector<double> weights;
  Point center;
  int radius;

  cvtColor(frame, gray, COLOR_BGR2GRAY);

  cascade.detectMultiScale(gray, eyes, reject_levels, weights, 1.1, 5, 0, Size(30,30), Size(), true);

  for (size_t i = 0; i < eyes.size(); i++) {
    std::printf("eye %lu : %f\n", i+1, weights[i]);
    if (weights[i] > 1.7) {
      Rect nr = eyes[i];
      center.x = cvRound(nr.x + nr.width * 0.5);
      center.y = cvRound(nr.y + nr.height * 0.5);
      radius = cvRound((nr.width + nr.height) * 0.25);
      circle(frame, center, radius,  Scalar(255,0,0), 3, 8, 0);
    }
  }
}

int main(int argc, const char ** argv)
{
  VideoCapture capture;
  Mat frame;
  uint64_t count = 0;

  if (!capture.open(0, CAP_V4L2)) {
    std::cerr << "Cannot open camera 0 !" << std::endl;
    return 1;
  }

  if (capture.isOpened()) {
    std::cout << "Video capturing has been started ..." << std::endl;

    if (!capture.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'))) {
      std::cerr << "Failed to set CV_CAP_PROP_FOURCC !" << std::endl;
      return 1;
    }

    if (!capture.set(CAP_PROP_FRAME_WIDTH, 640)) {
      std::cerr << "Failed to set CAP_PROP_FRAME_WIDTH !" << std::endl;
      return 1;
    }

    if (!capture.set(CAP_PROP_FRAME_HEIGHT, 480)) {
      std::cerr << "Failed to set CAP_PROP_FRAME_HEIGHT !" << std::endl;
      return 1;
    }

    if (!capture.set(CAP_PROP_FPS, 30)) {
      std::cerr << "Failed to set CAP_PROP_FPS !" << std::endl;
      return 1;
    }

    int width = (int) capture.get(CAP_PROP_FRAME_WIDTH);
    int height = (int) capture.get(CAP_PROP_FRAME_HEIGHT);
    double fps = capture.get(CAP_PROP_FPS);

    std::printf("Vide size: %d x %d (fps: %f)\n", width, height, fps);

    CascadeClassifier eye_cascade("/usr/share/opencv4/haarcascades/haarcascade_eye.xml");

    if (eye_cascade.empty()) {
      std::cerr << "failed to open CascadeClassifier !" << std::endl;
      return 1;
    }

    while(true) {
      capture >> frame;

      if (frame.empty()) {
        std::cerr << "No frame is read !" << std::endl;
        break;
      }

      //Mat frame1 = frame.clone();

      //if (++count % 3 == 0 )
        detect_eye(frame, eye_cascade);

      imshow("Webcam", frame);

      char c = (char)waitKey(33);
      if(c == 'q' || c == 'Q' ) {
        std::cout << "Quit" << std::endl;
        break;
      }
    }

    capture.release();
    return 0;
  }
}

