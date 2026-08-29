#include <opencv2/opencv.hpp>
#include <iostream>

#define MAIN_PATH "img/main_sh.png"
#define FOREIGN_PATH "img/foreign_7_sh.png"
#define OWN_PATH "img/own_7_sh.png"

using namespace std;

void printProgressBar(int current, int total, const string& prefix = "", int barWidth = 50);
void saveImage(const cv::Mat& image, const string& name, const string& suffix = "");

int main()
{
    // Прдготовка
    cv::Mat image_main, image_foreign, image_own, image_out1, image_out2;
    int main_width, main_height, foreign_width, foreign_height, own_width, own_height;
    double main_avg_brigh = 0;
    double sum_brigh = 0;

    image_main = cv::imread(MAIN_PATH, cv::IMREAD_UNCHANGED);
    main_width = image_main.cols;
    main_height = image_main.rows;
    image_out1 = image_main.clone(); 
    image_out2 = image_main.clone(); 

    image_foreign = cv::imread(FOREIGN_PATH, cv::IMREAD_UNCHANGED);
    foreign_width = image_foreign.cols;
    foreign_height = image_foreign.rows;

    image_own = cv::imread(OWN_PATH, cv::IMREAD_UNCHANGED);
    own_width = image_own.cols;
    own_height = image_own.rows;

    if (image_main.empty())
    {
        return -1;
    }

    if (image_foreign.empty())
    {
        return -2;
    }

    if (image_own.empty())
    {
        return -3;
    }

    if (image_out1.empty())
    {
        return -4;
    }

    for (int i = 0; i < main_width; i++)
    {
        for (int j = 0; j < main_height; j++)
        {
            sum_brigh += image_main.at<uchar>(i, j);
        }
    }

    main_avg_brigh = sum_brigh / (main_width*main_height);
    cout << main_avg_brigh << endl;

    cv::imshow("Test OpenCV0", image_main);

    for (int y = 0; y < main_height; y++)
    {
        for (int x = 0; x < main_width; x++)
        {
            uchar& pixel = image_main.at<uchar>(y, x);
            pixel = cv::saturate_cast<uchar>(pixel - main_avg_brigh);
        }
    }

    for (int y = 0; y < foreign_height; y++)
    {
        for (int x = 0; x < foreign_width; x++)
        {
            uchar& pixel = image_foreign.at<uchar>(y, x);
            pixel = cv::saturate_cast<uchar>(pixel - main_avg_brigh);
        }
    }

    for (int y = 0; y < own_height; y++)
    {
        for (int x = 0; x < own_width; x++)
        {
            uchar& pixel = image_own.at<uchar>(y, x);
            pixel = cv::saturate_cast<uchar>(pixel - main_avg_brigh);
        }
    }

   // Сама лаба

    image_out1 = cv::Mat::zeros(main_height, main_width, CV_8UC1);
    image_out2 = cv::Mat::zeros(main_height, main_width, CV_8UC1);

    double max_C= -1;
    int max_C_x = 0, max_C_y = 0;

    cout << "Вычисление корреляции с foreign..." << endl;
    for (int y = 0; y < main_height- foreign_height; y++)
        {
            for (int x = 0; x < main_width - foreign_width; x++)
            {
                double C = 0;
                for (int k = 0; k < foreign_height; k++)
                {
                    for (int l = 0; l < foreign_width; l++)
                    {
                        uchar pixel_main = image_main.at<uchar>(y + k, x + l);
                        uchar pixel_foreign = image_foreign.at<uchar>(k, l);
                        C += pixel_main * pixel_foreign;
                    }
                }
                image_out1.at<uchar>(y, x) = cv::saturate_cast<uchar>(C/ (foreign_width * foreign_height));
                    if (C > max_C)
                    {
                        max_C = C;
                        max_C_x = x;
                        max_C_y = y;
                    }
            }
            printProgressBar(y, main_width - foreign_width);
        }


    cout << "\nМаксимальная корреляция c foreign найдена в позиции: (" << max_C_x << ";" << max_C_y << ")" << endl;
    cout << "Значение корреляции: " << max_C << endl;

    cout << "Вычисление корреляции с own..." << endl;
    for (int y = 0; y < main_height- own_height; y++)
    {
        for (int x = 0; x < main_width - own_width; x++)
        {
            double C = 0;
            for (int k = 0; k < own_height; k++)
            {
                for (int l = 0; l < own_width; l++)
                {
                    uchar pixel_main = image_main.at<uchar>(y + k, x + l);
                    uchar pixel_own = image_own.at<uchar>(k, l);
                    C += pixel_main * pixel_own;
                }
            }
             image_out2.at<uchar>(y, x) = cv::saturate_cast<uchar>(C/ (own_width * own_height));
                if (C > max_C)
                {
                    max_C = C;
                    max_C_x = x;
                    max_C_y = y;
                 }
        }
        printProgressBar(y, main_width - foreign_width);
    }

    cout << "Максимальная корреляция c ownнайдена в позиции: (" << max_C_x << ";" << max_C_y << ")" << endl;
    cout << "Значение корреляции: " << max_C << endl;

    cv::imshow("Test I1", image_out1); 
    cv::imshow("Test I2", image_out2); 

    saveImage(image_out1, "correlation_foreign");
    saveImage(image_out2, "correlation_own");

    //cv::imshow("Test OpenCV1", image_main);
   // cv::imshow("Test OpenCV2", image_foreign);
  //  cv::imshow("Test OpenCV3", image_own);
  //  cv::waitKey(0);
    return 0;
}

// Progressbar
void printProgressBar(int current, int total, const string& prefix, int barWidth) {
    float progress = (float)current / total;
    
    cout << "\r" << prefix << " [";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) cout << "=";
        else if (i == pos) cout << ">";
        else cout << " ";
    }
    cout << "] " << int(progress * 100.0) << "%";
    
    if (current == total - 1 || current == total) {
        cout << " | Готово!" << endl;
    }
    cout.flush();
}

// Сохранение изображения
void saveImage(const cv::Mat& image, const string& name, const string& suffix) {
    string filename = "result_" + name + suffix + ".png";
    bool success = cv::imwrite(filename, image);
    if (success) {
        cout << "Изображение сохранено: " << filename << endl;
    } else {
        cout << "Ошибка сохранения: " << filename << endl;
    }
}