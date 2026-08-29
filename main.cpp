#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>

#define MAIN_PATH "img/main_sh.png"
#define FOREIGN_PATH "img/foreign_7_sh.png"
#define OWN_PATH "img/own_7_sh.png"

using namespace std;

void printProgressBar(int current, int total, const string& prefix = "", int barWidth = 50);
void saveImage(const cv::Mat& image, const string& name, const string& suffix = "");
int find_correlation(string main_name, string quest_name, string result_name);

int main()
{
    int e;
    e = find_correlation(MAIN_PATH, FOREIGN_PATH, "result_foreign");
    if(e != 0)
    {
        cout << "Ошибка нахождения корреляции с foreign: " <<  e << endl;
        return -1;
    }

    e = find_correlation(MAIN_PATH, OWN_PATH, "result_own");
    if(e != 0)
    {
        cout << "Ошибка нахождения корреляции с own: " <<  e << endl;
        return -1;
    }

    return 0;
}

// Сохранение изображения
void saveImage(const cv::Mat& image, const string& name, const string& suffix) {
    string filename = name + ".png";
    bool success = cv::imwrite(filename, image);
    if (success) {
        cout << "Изображение сохранено: " << filename << endl;
    } else {
        cout << "Ошибка сохранения: " << filename << endl;
    }
}

int find_correlation(string main_name, string quest_name, string result_name)
{
    // Подготовка

    cv::Mat image_main, image_quest, image_out;
    int main_width, main_height, quest_width, quest_height;
    double main_avg_brigh = 0;
    double sum_brigh = 0;

    image_main = cv::imread(main_name, cv::IMREAD_UNCHANGED);
    main_width = image_main.cols;
    main_height = image_main.rows;
    image_out = image_main.clone(); 

    image_quest = cv::imread(quest_name, cv::IMREAD_UNCHANGED);
    quest_width = image_quest.cols;
    quest_height = image_quest.rows;


    if (image_main.empty())
    {
        return -1;
    }

    if (image_quest.empty())
    {
        return -2;
    }

    if (image_out.empty())
    {
        return -3;
    }

    // Нахождение средней яркости 

    for (int i = 0; i < main_width; i++)
    {
        for (int j = 0; j < main_height; j++)
        {
            sum_brigh += image_main.at<uchar>(i, j);
        }
    }

    main_avg_brigh = sum_brigh / (main_width*main_height);
    cout << main_avg_brigh << endl;

    // Центрирование по яркости

    for (int y = 0; y < main_height; y++)
    {
        for (int x = 0; x < main_width; x++)
        {
            uchar& pixel = image_main.at<uchar>(y, x);
            pixel = cv::saturate_cast<uchar>(pixel - main_avg_brigh);
        }
    }

    for (int y = 0; y < quest_height; y++)
    {
        for (int x = 0; x < quest_width; x++)
        {
            uchar& pixel = image_quest.at<uchar>(y, x);
            pixel = cv::saturate_cast<uchar>(pixel - main_avg_brigh);
        }
    }

   // Поиск корреляции

    image_out = cv::Mat::zeros(main_height, main_width, CV_8UC1);

    double max_C= -1;
    int max_C_x = 0, max_C_y = 0;

    cout << "Вычисление корреляции..." << endl;

    auto start = std::chrono::high_resolution_clock::now();

    for (int y = 0; y < main_height- quest_height; y++)
    {
        for (int x = 0; x < main_width - quest_width; x++)
        {
            double C = 0;
            for (int k = 0; k < quest_height; k++)
            {
                 for (int l = 0; l < quest_width; l++)
                 {
                    uchar pixel_main = image_main.at<uchar>(y + k, x + l);
                    uchar pixel_quest = image_quest.at<uchar>(k, l);
                    C += pixel_main * pixel_quest;
                }
            }
            image_out.at<uchar>(y, x) = cv::saturate_cast<uchar>(C/ (quest_width * quest_height));
            if (C > max_C)
            {
                max_C = C;
                max_C_x = x;
                max_C_y = y;
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    cout << "Максимальная корреляция найдена в позиции: (" << max_C_x << ";" << max_C_y << ")" << endl;
    cout << "Значение корреляции: " << max_C << endl;

    std::cout << "Время нахождения корреляции: " << duration.count() / 1000.0 << " мс" << std::endl;
    std::cout << "Размер основного изображения: " << main_width << " x " << main_height << std::endl;
    std::cout << "Размер искомого изображения: " << quest_width << " x " << quest_height << std::endl;

    saveImage(image_out, result_name);
    return 0;
    //cv::imshow("Test OpenCV1", image_main);
    // cv::waitKey(0);
}