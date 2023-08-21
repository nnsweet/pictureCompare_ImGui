#include "picCompare_app.h"
#include "stb_image.h"

void picCompare_app::StartUp()
{
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImFont* font = io.Fonts->AddFontFromFileTTF("./font/minecrosoft.ttc",
        20.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

    bool ret = LoadTextureFromFile(path1.c_str(), &my_image_texture, &my_image_width, &my_image_height);
    bool ret2 = LoadTextureFromFile(path2.c_str(), &my_image_texture2, &my_image_width2, &my_image_height2);
    IM_ASSERT(ret);
    IM_ASSERT(ret2);

    style.FrameRounding = 3.0f;
}

void picCompare_app::Update()
{
    /**********1.demo_window**************/
    //if (show_demo_window)
    //{
    //    ImGui::ShowDemoWindow(&show_demo_window);
    //}
    //// //2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    //{
    //    static float f = 0.0f;
    //    static int counter = 0;
    //    ImGui::Begin(u8"Hello, world! 你好世界！");                          // Create a window called "Hello, world!" and append into it.
    //    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    //    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    //    ImGui::Checkbox("Another Window", &show_picCompare_window);
    //    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    //    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
    //    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    //        counter++;
    //    ImGui::SameLine();
    //    ImGui::Text("counter = %d", counter);
    //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    //    ImGui::End();
    //}
    // GPU Rendering Zone // To call for Create/ Destroy Textures

    /**********绘制选择图片的菜单栏**************/
    {
        ImGui::Begin(u8"选择图片菜单栏",&a,NULL| ImGuiWindowFlags_NoMove);

        ImGui::Text(u8"选择第一张图片:");
        ImGui::SameLine();
        if (ImGui::Button(u8"选择一图"))
        { 
            show_fileDialog1_window = true;
        }

        ImGui::Text(u8"选择第二张图片:");
        ImGui::SameLine();
        if (ImGui::Button(u8"选择二图"))
        {
            show_fileDialog2_window = true;
        }
        
        ImGui::Text(u8"展示图片");
        ImGui::SameLine();
        ImGui::Checkbox(u8"开启/关闭", &show_picCompare_window);

        ImGui::Text(u8"计算两张图片的相似度:");
        ImGui::SameLine();
        if (ImGui::Button(u8"计算"))
        {
            if (path1 != "" || path2 != "")
            {
                show_picCompare_window = false;
                SetImage1(path1);
                SetImage2(path2);
                result_between_TwoPics = caculate_between_twoPics(image1,image2);
            }
            
        }
        ImGui::TextColored(ImVec4(0.9375f, 0.62890625f, 0.65625f, 1.00f),u8"两张图片相似度为：%0.2lf%%", result_between_TwoPics);

        //ImGui::Text(u8"测试按钮");ImGui::SameLine();
        //if (ImGui::Button(u8"测试"))
        //{
        //    std::cout << "path1" << path1 << std::endl;
        //    std::cout << "path2" << path2 << std::endl;
        //}

        ImGui::TextUnformatted(u8"计算结果参考表：");
        ImGui::TextUnformatted(u8"| <0.10％		        ****完全不同");
        ImGui::TextUnformatted(u8"| 0.10 - 0.39％		****相似度很低");
        ImGui::TextUnformatted(u8"| 0.39 - 0.70％		****正常范围");
        ImGui::TextUnformatted(u8"| 0.70 - 0.89％		****相似度很高");
        ImGui::TextUnformatted(u8"| 0.89 - 0.99％	    *****完全一致");

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End(); 
    }
    
    /**********绘制文件对话框**************/
    if (show_fileDialog1_window)
    {
        show_picCompare_window = false;
        path1 =fileDialogFunc();
        if (path1 != "")
        {
            SetImage1(path1);
            bool ret = LoadTextureFromFile(path1.c_str(), &my_image_texture, &my_image_width, &my_image_height);
        }
    }
    if (show_fileDialog2_window)
    {
        show_picCompare_window = false;
        path2 = fileDialogFunc();
        if (path2 != "")
        {
            SetImage2(path2);
            bool ret2 = LoadTextureFromFile(path2.c_str(), &my_image_texture2, &my_image_width2, &my_image_height2);
        }
    }

    /**********绘制选择的图片**************/
    if (show_picCompare_window)
    {
        showPic();
    }
}

//打开文件对话框|窗口
std::string picCompare_app::fileDialogFunc()
{
    ImGui::Begin(u8"选择文件");
    std::string path;
    // open Dialog Simple
    if (ImGui::Button(u8"打开文件对话框"))
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFey", u8"选择文件", filters, ".");
    // display
    if (ImGuiFileDialog::Instance()->Display("ChooseFey"))
    {
        // action if OK
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            path = ImGuiFileDialog::Instance()->GetFilePathName().c_str();
            //std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
            std::cout << "filePathName:" << path << std::endl << std::endl;
            // action
        }else
        {
            path = "./images/goose1.png";
        }
        // close
        show_fileDialog1_window = false;
        show_fileDialog2_window = false;
        show_picCompare_window = true;
        ImGuiFileDialog::Instance()->Close();
    }
    ImGui::End();
    return path;
}

//图像显示|窗口
void picCompare_app::showPic()
{
    ImGui::Begin(u8"图像对比结果", &show_picCompare_window, 
        ImGuiWindowFlags_AlwaysVerticalScrollbar| ImGuiWindowFlags_AlwaysHorizontalScrollbar);
    ImGui::Text("Pointer_Texture = %p", my_image_texture);
    ImGui::Text(u8"图像一|尺寸 = %d x %d pix", my_image_width, my_image_height);
    ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(my_image_width, my_image_height));
    ImGui::Text(u8"图像二|尺寸 = %d x %d pix", my_image_width2, my_image_height2);
    ImGui::Image((void*)(intptr_t)my_image_texture2, ImVec2(my_image_width2, my_image_height2));
    ImGui::TextColored(ImVec4(0.9375f, 0.62890625f, 0.65625f, 1.00f), u8"两张图片相似度为：%0.2lf%%", result_between_TwoPics);

    ImGui::End();
}

//直方图法对比图像得值，返回一个double型的值
double picCompare_app::caculate_between_twoPics(const cv::Mat im1, const cv::Mat im2)
{
    float h_ranges[] = { 0, 255 };
    const float* ranges[] = { h_ranges };
    constexpr int channels[] = { 0 };
    constexpr int hist_size[] = { 255 };
    calcHist(&im1, 1, channels, cv::Mat(), hist_image1, 1, hist_size, ranges, true, false);
    calcHist(&im2, 1, channels, cv::Mat(), hist_image2, 1, hist_size, ranges, true, false);

    const double diff = compareHist(hist_image1, hist_image2, cv::HISTCMP_HELLINGER);
    double Result = 100*(1.0 - diff);

    return Result;
}

//设置cv::image1的路径
void picCompare_app::SetImage1(const std::string& imagePath) 
{
    image1= imread(imagePath, cv::IMREAD_GRAYSCALE);
}
//设置cv::image2的路径
void picCompare_app::SetImage2(const std::string& imagePath)
{
    image2 = imread(imagePath, cv::IMREAD_GRAYSCALE);
}

// Simple helper function to load an image into a OpenGL texture with common settings
bool picCompare_app::LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}
