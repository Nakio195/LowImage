#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Color.h"

#define DITH_FS 0
#define CANNY 1

using namespace std;


uint8_t saturated_add(uint8_t val1, int8_t val2);

int main( int argc, char** argv )
{
    cv::String keys = "{c color||Main color output}{i image||Image to be processed}{t type|0|Select the reducing method}";
    cv::CommandLineParser parser(argc, argv, keys);

    if(!parser.has("image"))
    {
        cout << "Woopsy ! No image file given, so nothing to do !" << endl;
        cout << "-color | -c\tMain color output R,G,B" << endl;
        cout << "-image | -i\tPath to image to process" << endl;
        cout << "-type | -t\tReducing method" << endl;
        cout << "\t\t\t 0 - Floyd-Steinberg dithering" << endl;
        cout << "\t\t\t 1 - Canny contours detection and coloration" << endl << endl;
        cout << "LowImage -c=\"124,124,124\" -i=\"input.jpg\" -t=1" << endl;
        cout << "Press ENTER to exit...";
        getchar();
        return -1;
    }

    cv::String file = parser.get<cv::String>("image");

    cv::Mat Blk;
    cv::Mat Raw = imread(file, cv::IMREAD_COLOR);

    cv::cvtColor(Raw, Blk, cv::COLOR_BGR2GRAY);

    if(Raw.empty())
    {
        std::cout << "Could not read the image: " << file << std::endl;
        return 1;
    }
    else
        std::cout << "Image loaded: " << file << std::endl;

    int Width = Raw.cols;
    int Height = Raw.rows;

    Color mainColor;

    if(parser.has("color"))
    {
        mainColor = Color(parser.get<cv::String>("color"));

        cout << "Main color is " << mainColor.toString() << std::endl;
    }

    else
        cout << "No main color defined assuming '0,0,0'" << std::endl;


    int Type = parser.get<int>("type");
    cout << "Reducing type: ";

    switch(Type)
    {
        case DITH_FS:
            cout << "Floyd-Steinberg dithering";
        break;
        case CANNY:
            cout << "Canny contours detection and coloration";
        break;

        default:
            cout << "Unknown type... Exiting.";
            return -1;
        break;

    }

    cout << std::endl;

    cv::Mat Out = cv::Mat::zeros(static_cast<int>(Height), static_cast<int>(Width), CV_8UC3);

    if(Type == DITH_FS)
    {
        int err;
        int8_t a,b,c,d;

        for(int i = 0; i < Height; i++)
        {
            for(int j = 0; j < Width; j++)
            {
                if(Blk.at<uint8_t>(i,j) > 127)
                {
                    err = Blk.at<uint8_t>(i,j) - 255;
                    Blk.at<uint8_t>(i,j) = 255;
                }

                else
                {
                    err = Blk.at<uint8_t>(i,j) - 0;
                    Blk.at<uint8_t>(i,j) = 0;
                }

                a = (err * 7) / 16;
                b = (err * 1) / 16;
                c = (err * 5) / 16;
                d = (err * 3) / 16;

                if((i != (Height - 1)) && (j != 0) && (j != (Width - 1)))
                {
                    Blk.at<uint8_t>(i+0,j+1) = saturated_add(Blk.at<uint8_t>(i+0,j+1),a);
                    Blk.at<uint8_t>(i+1,j+1) = saturated_add(Blk.at<uint8_t>(i+1,j+1),b);
                    Blk.at<uint8_t>(i+1,j+0) = saturated_add(Blk.at<uint8_t>(i+1,j+0),c);
                    Blk.at<uint8_t>(i+1,j-1) = saturated_add(Blk.at<uint8_t>(i+1,j-1),d);
                }
            }
        }

        for(int r = 0; r < Blk.rows; r++)
        {
            for(int c = 0; c < Blk.cols; c++)
            {
                float lum = static_cast<float>(Blk.at<uint8_t>(r, c))/255.0;

                Out.at<cv::Vec3b>(r, c).val[2] = lum*mainColor.mR;
                Out.at<cv::Vec3b>(r, c).val[1] = lum*mainColor.mG;
                Out.at<cv::Vec3b>(r, c).val[0] = lum*mainColor.mB;
            }
        }
    }

    else if(Type == CANNY)
    {
        cv::Mat canny_output;
        Canny(Blk, canny_output, 120, 210);

        vector<vector<cv::Point>> contours;
        vector<cv::Vec4i> hierarchy;

        findContours(canny_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

        cv::threshold(Blk, Blk, 127, 250, cv::THRESH_BINARY_INV);

        for(int r = 0; r < Out.rows; r++)
        {
            for(int c = 0; c < Out.cols; c++)
            {
                float lum = static_cast<float>(Blk.at<uint8_t>(r, c))/255.0;
                Out.at<cv::Vec3b>(r, c).val[2] = lum*mainColor.mR;
                Out.at<cv::Vec3b>(r, c).val[1] = lum*mainColor.mG;
                Out.at<cv::Vec3b>(r, c).val[0] = lum*mainColor.mB;
            }
        }

        for( size_t i = 0; i< contours.size(); i++ )
        {
            cv::Scalar color = cv::Scalar(mainColor.mB, mainColor.mR, mainColor.mR);
            drawContours(Out, contours, (int)i, color, 1, cv::LINE_AA, hierarchy, 0);
        }

        for(int r = 0; r < Out.rows; r++)
        {
            for(int c = 0; c < Out.cols; c++)
            {
                if(!Out.at<cv::Vec3b>(r, c).val[0] && !Out.at<cv::Vec3b>(r, c).val[1] && !Out.at<cv::Vec3b>(r, c).val[2])
                {
                    Out.at<cv::Vec3b>(r, c).val[1] = mainColor.mR;
                    Out.at<cv::Vec3b>(r, c).val[0] = mainColor.mG;
                    Out.at<cv::Vec3b>(r, c).val[2] = mainColor.mB;
                }
            }
        }

    }

    vector<int> compression_params;

    compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
    compression_params.push_back(50);
    compression_params.push_back(cv::IMWRITE_JPEG_PROGRESSIVE);
    compression_params.push_back(1);
    compression_params.push_back(cv::IMWRITE_JPEG_OPTIMIZE);
    compression_params.push_back(1);
    compression_params.push_back(cv::IMWRITE_JPEG_LUMA_QUALITY);
    compression_params.push_back(50);
    cv::imwrite("out.jpg", Out, compression_params);

    compression_params.clear();

    compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);
    cv::imwrite("out.png", Out, compression_params);

    cv::imshow("Blk", Blk);
    cv::imshow("Out", Out);
    cv::waitKey(10000);

    return 0;
}

uint8_t saturated_add(uint8_t val1, int8_t val2)
{
  int16_t val1_int = val1;
  int16_t val2_int = val2;
  int16_t tmp = val1_int + val2_int;

  if(tmp > 255)
  {
    return 255;
  }
  else if(tmp < 0)
  {
    return 0;
  }
  else
  {
    return tmp;
  }
}
