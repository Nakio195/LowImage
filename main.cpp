#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Color.h"

using namespace std;

int main( int argc, char** argv )
{
    cv::String keys = "{c color||Main color output}{i image||Image to be processed}";
    cv::CommandLineParser parser(argc, argv, keys);

    if(!parser.has("image"))
    {
        cout << "Woopsy ! No image file given, so nothing to do !";
        return -1;
    }

    cv::String file = parser.get<cv::String>("image");

    cv::Mat Raw = imread(file, cv::IMREAD_COLOR);

    if(Raw.empty())
    {
        std::cout << "Could not read the image: " << file << std::endl;
        return 1;
    }
    else
        std::cout << "Image loaded: " << file << std::endl;

    Color mainColor;

    if(parser.has("color"))
    {
        mainColor = Color(parser.get<cv::String>("color"));

        cout << "Main color is " << mainColor.toString();
    }

    else
        cout << "No main color defined assuming '0,0,0'" << std::endl;


    for(int r = 0; r < Raw.rows; r++)
    {
        for(int c = 0; c < Raw.cols; c++)
        {
            if(Raw.channels() == 3)
            {
                Color old(Raw.at<cv::Vec3b>(r, c).val[2], Raw.at<cv::Vec3b>(r, c).val[1], Raw.at<cv::Vec3b>(r, c).val[0]);
                float lum = static_cast<float>(old.luminosity())/256.0;

                if(c%4)
                {
                    if(!r%4)
                    {
                        Raw.at<cv::Vec3b>(r, c).val[2] = 255;
                        Raw.at<cv::Vec3b>(r, c).val[1] = 255;
                        Raw.at<cv::Vec3b>(r, c).val[0] = 255;
                    }

                    else
                    {
                        Raw.at<cv::Vec3b>(r, c).val[2] = mainColor.mR*lum;
                        Raw.at<cv::Vec3b>(r, c).val[1] = mainColor.mG*lum;
                        Raw.at<cv::Vec3b>(r, c).val[0] = mainColor.mB*lum;
                    }
                }

                else
                {
                    if(r%4)
                    {
                        Raw.at<cv::Vec3b>(r, c).val[2] = mainColor.mR*lum;
                        Raw.at<cv::Vec3b>(r, c).val[1] = mainColor.mG*lum;
                        Raw.at<cv::Vec3b>(r, c).val[0] = mainColor.mB*lum;
                    }

                    else
                    {
                        Raw.at<cv::Vec3b>(r, c).val[2] = 255;
                        Raw.at<cv::Vec3b>(r, c).val[1] = 255;
                        Raw.at<cv::Vec3b>(r, c).val[0] = 255;
                    }
                }

            }
        }
    }

    vector<int> compression_params;
    compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
    compression_params.push_back(30);
    compression_params.push_back(cv::IMWRITE_JPEG_PROGRESSIVE);
    compression_params.push_back(1);
    compression_params.push_back(cv::IMWRITE_JPEG_OPTIMIZE);
    compression_params.push_back(1);
    compression_params.push_back(cv::IMWRITE_JPEG_LUMA_QUALITY);
    compression_params.push_back(30);

    cv::imwrite("out.jpg", Raw, compression_params);
    cv::imshow("Out", Raw);
    cv::waitKey();

    return 0;
}
