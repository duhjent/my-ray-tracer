#include <opencv2/opencv.hpp>

#define W_WIDTH 1024
#define W_HEIGHT 768

int main() {
  float *screenVec = new float[W_WIDTH * W_HEIGHT * 3];

  for (int i = 0; i < W_HEIGHT; i++) {
    for (int j = 0; j < W_WIDTH; j++) {
      screenVec[(3 * W_WIDTH * i) + (3 * j) + 2] = (float)i / W_HEIGHT; // R
      screenVec[(3 * W_WIDTH * i) + (3 * j) + 1] = (float)j / W_WIDTH;  // G
      screenVec[(3 * W_WIDTH * i) + (3 * j)] = 0;                       // B
    }
  }

  cv::Mat screen(W_HEIGHT, W_WIDTH, CV_32FC3);
  memcpy(screen.data, screenVec, W_WIDTH * W_HEIGHT * 3 * sizeof(float));

  delete[] screenVec;

  cv::imshow("image", screen);
  cv::waitKey(0);
  return 0;
}
