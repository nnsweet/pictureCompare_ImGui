#pragma once
#include "MyApp/App.h"
#include <direct.h> 
#include <string>
#include "ImGuiFileDialog.h"
#include <iostream>
#include <thread>
#include <cstring>
#include <opencv2/opencv.hpp>
#define STB_IMAGE_IMPLEMENTATION
#define USE_THUMBNAILS


class picCompare_app : public App
{
public:
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiFileDialog fileDialog;

public:
    picCompare_app() = default;
    ~picCompare_app() = default;
    virtual void StartUp() final;
    virtual void Update() final;
    std::string fileDialogFunc();
    void showPic();
    bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
    void SetImage1(const std::string& imagePath);
    void SetImage2(const std::string& imagePath);
    double caculate_between_twoPics(const cv::Mat im1, const cv::Mat im2);


private:
    bool a = true;
    bool no_close = false;
    bool show_demo_window = true;
    bool show_picCompare_window = true;
    bool show_fileDialog1_window = false;
    bool show_fileDialog2_window = false;

    std::string path1 = "./images/goose1.png";
    std::string path2 = "./images/goose2.png";
    int my_image_width = 960;
    int my_image_height = 480;
    GLuint my_image_texture = 0;
    int my_image_width2 = 960;
    int my_image_height2 = 480;
    GLuint my_image_texture2 = 0;

    cv::Mat image1;
    cv::Mat image2;
    cv::MatND hist_image1;
    cv::MatND hist_image2;
    double result_between_TwoPics=0;
    //const char* filters = u8"源文件 (*.cpp *.h *.hpp){.cpp,.h,.hpp},图像文件(*.png *.gif *.jpg *.jpeg){.png,.gif,.jpg,.jpeg},.md";
    const char* filters = u8"图像文件(*.png *.jpg *.jpeg){.png,.jpg,.jpeg}";
};

