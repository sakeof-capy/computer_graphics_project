#include <iostream>
#include "model.hpp"

void line(int x0, int y0, int x1, int y1, cv::Mat &image, cv::Vec3b color) {
    bool steep = false;
    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    for (int x=x0; x<=x1; x++) {
        float t = (x-x0)/(float)(x1-x0);
        int y = y0*(1.-t) + y1*t;
         if (x > 0 && y >0)
         {


        if (steep) {
            image.at<cv::Vec3b>(y, x) = color;

        } else {
           image.at<cv::Vec3b>(x, y) = color;
        }
         }
    }
}

int main()
{
    std::string imageName ("D:/Images/07/1.tif");

    const int width  = 800;
    const int height = 800;

	cv::Mat image_readed = cv::imread(imageName.c_str(), cv::IMREAD_COLOR);


	cv::Mat image(width, height, CV_8UC3, cv::Scalar(0, 0, 0));
	cv::imshow( "Show empty image", image);

    Model *model = NULL;
    model = new Model("./models/african_head.obj");
    cv::Vec3b white (255, 255, 255);

     for (int i=0; i<model->nfaces(); i++) {
            std::vector<int> face = model->face(i);
            for (int j=0; j<3; j++) {
                cv::Vec3f v0 = model->vert(face[j]);
                cv::Vec3f v1 = model->vert(face[(j+1)%3]);
                int x0 = (v0[0]+1.)*width/2.;
                int y0 = (v0[1]+1.)*height/2.;
                int x1 = (v1[0]+1.)*width/2.;
                int y1 = (v1[1]+1.)*height/2.;
                line(x0, y0, x1, y1, image, white);
            }
        }
   cv::imshow( "Display window", image);  // Show our image inside it.

   cv::waitKey(0); // Wait for a keystroke in the window

   delete model;
   system("Pause");
   return 0;
}
