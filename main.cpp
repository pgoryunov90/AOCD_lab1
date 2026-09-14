#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>

#define MAIN_PATH "img/main_sh.png"
#define FOREIGN_PATH "img/foreign_7_sh.png"
#define OWN_PATH "img/own_7_sh.png"
#define OWN_P2_PATH "img/own_7_sh_p2.png"
#define OWN_P4_PATH "img/own_7_sh_p4.png"
#define OWN_P6_PATH "img/own_7_sh_p6.png"
#define OWN_P8_PATH "img/own_7_sh_p8.png"
#define OWN_P10_PATH "img/own_7_sh_p10.png"
#define OWN_M2_PATH "img/own_7_sh_m2.png"
#define OWN_M4_PATH "img/own_7_sh_m4.png"
#define OWN_M6_PATH "img/own_7_sh_m6.png"
#define OWN_M8_PATH "img/own_7_sh_m8.png"
#define OWN_M10_PATH "img/own_7_sh_m10.png"
#define OWN_B025_PATH "img/own_7_sh_b025.png"
#define OWN_B050_PATH "img/own_7_sh_b050.png"
#define OWN_B075_PATH "img/own_7_sh_b075.png"
#define OWN_B100_PATH "img/own_7_sh_b100.png"
#define OWN_S025_PATH "img/own_7_sh_s025.png"
#define OWN_S050_PATH "img/own_7_sh_s050.png"
#define OWN_S075_PATH "img/own_7_sh_s075.png"
#define OWN_S100_PATH "img/own_7_sh_s100.png"

using namespace std;

void saveImage(const cv::Mat& image, const string& name, const string& suffix = "");
int find_correlation(string main_name, string quest_name, string result_name);

// Функция центрирования яркости (из второго файла)
void bright(cv::Mat& img)
{
    if (img.channels() != 1) 
        cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
    img.convertTo(img, CV_32F);
    img -= cv::mean(img)[0];
}

int main()
{
    int e;
    
   /* cout << "Нахождение корреляции с foreign" << endl;
    e = find_correlation(MAIN_PATH, FOREIGN_PATH, "2_result_foreign");
    if(e != 0)
    {
        cout << "Ошибка нахождения корреляции с foreign: " <<  e << endl;
        return -1;
    }

    cout << "Нахождение корреляции с own" << endl;
    e = find_correlation(MAIN_PATH, OWN_PATH, "2_result_own");
    if(e != 0)
    {
        cout << "Ошибка нахождения корреляции с own: " <<  e << endl;
        return -1;
    }
    
    cout << "Нахождение корреляции с own (поворот -2)" << endl;
    e = find_correlation(MAIN_PATH, OWN_M2_PATH, "2_result_own_rot_m2");
    if(e != 0)
    {
        cout << "Ошибка нахождения корреляции: " <<  e << endl;
        return -1;
    }

    cout << "Нахождение корреляции с own (поворот -4)" << endl;
    e = find_correlation(MAIN_PATH, OWN_M4_PATH, "2_result_own_rot_m4");
    if(e != 0)
    {
        cout << "Ошибка нахождения корреляции: " <<  e << endl;
        return -1;
    }

    cout << "Нахождение корреляции с own (поворот -6)" << endl;
    e = find_correlation(MAIN_PATH, OWN_M6_PATH, "2_result_own_rot_m6");
    if(e != 0)
    {
        cout << "Ошибка нахождения корреляции: " <<  e << endl;
        return -1;
    }

    cout << "Нахождение корреляции с own (поворот -8)" << endl;
    e = find_correlation(MAIN_PATH, OWN_M8_PATH, "2_result_own_rot_m8");
    if(e != 0)
    {
        cout << "Ошибка нахождения корреляции: " <<  e << endl;
        return -1;
    }

    cout << "Нахождение корреляции с own (поворот -10)" << endl;
    e = find_correlation(MAIN_PATH, OWN_M10_PATH, "2_result_own_rot_m10");
    if(e != 0)
    {
        cout << "Ошибка нахождения корреляции: " <<  e << endl;
        return -1;
    }

    cout << "Нахождение корреляции с own (поворот 2)" << endl;
    e = find_correlation(MAIN_PATH, OWN_P2_PATH, "2_result_own_rot_p2");
    if(e != 0)
    {
        cout << "Ошибка нахождения корреляции: " <<  e << endl;
        return -1;
    }

    cout << "Нахождение корреляции с own (поворот 4)" << endl;
    e = find_correlation(MAIN_PATH, OWN_P4_PATH, "2_result_own_rot_p4");
    if(e != 0)
    {
        cout << "Ошибка нахождения корреляции: " <<  e << endl;
        return -1;
    }

    cout << "Нахождение корреляции с own (поворот 6)" << endl;
    e = find_correlation(MAIN_PATH, OWN_P6_PATH, "2_result_own_rot_p6");
    if(e != 0)
    {
        cout << "Ошибка нахождения корреляции: " <<  e << endl;
        return -1;
    }

    cout << "Нахождение корреляции с own (поворот 8)" << endl;
    e = find_correlation(MAIN_PATH, OWN_P8_PATH, "2_result_own_rot_p8");
    if(e != 0)
    {
        cout << "Ошибка нахождения корреляции: " <<  e << endl;
        return -1;
    }

    cout << "Нахождение корреляции с own (поворот 10)" << endl;
    e = find_correlation(MAIN_PATH, OWN_P10_PATH, "2_result_own_rot_p10");
    if(e != 0)
    {
        cout << "Ошибка нахождения корреляции: " <<  e << endl;
        return -1;
    }

    cout << "Нахождение корреляции с own (увеличение в 1,025 раза)" << endl;
    e = find_correlation(MAIN_PATH, OWN_B025_PATH, "2_result_own_size_p025");
    if(e != 0)
    {
        cout << "Ошибка нахождения корреляции: " <<  e << endl;
        return -1;
    }
*/
    cout << "Нахождение корреляции с own (увеличение в 1,050 раза)" << endl;
    e = find_correlation(MAIN_PATH, OWN_B050_PATH, "2_result_own_size_p050");
    if(e != 0)
    {
        cout << "Ошибка нахождения корреляции: " <<  e << endl;
        return -1;
    }

    cout << "Нахождение корреляции с own (увеличение в 1,075 раза)" << endl;
    e = find_correlation(MAIN_PATH, OWN_B075_PATH, "2_result_own_size_p075");
    if(e != 0)
    {
        cout << "Ошибка нахождения корреляции: " <<  e << endl;
        return -1;
    }

    cout << "Нахождение корреляции с own (увеличение в 1,1 раза)" << endl;
    e = find_correlation(MAIN_PATH, OWN_B100_PATH, "2_result_own_size_p100");
    if(e != 0)
    {
        cout << "Ошибка нахождения корреляции: " <<  e << endl;
        return -1;
    }

    cout << "Нахождение корреляции с own (уменьшение в 1,025 раза)" << endl;
    e = find_correlation(MAIN_PATH, OWN_S025_PATH, "2_result_own_size_m025");
    if(e != 0)
    {
        cout << "Ошибка нахождения корреляции: " <<  e << endl;
        return -1;
    }

    cout << "Нахождение корреляции с own (уменьшение в 1,050 раза)" << endl;
    e = find_correlation(MAIN_PATH, OWN_S050_PATH, "2_result_own_size_m050");
    if(e != 0)
    {
        cout << "Ошибка нахождения корреляции: " <<  e << endl;
        return -1;
    }

    cout << "Нахождение корреляции с own (уменьшение в 1,075 раза)" << endl;
    e = find_correlation(MAIN_PATH, OWN_S075_PATH, "2_result_own_size_m075");
    if(e != 0)
    {
        cout << "Ошибка нахождения корреляции: " <<  e << endl;
        return -1;
    }

    cout << "Нахождение корреляции с own (уменьшение в 1,1 раза)" << endl;
    e = find_correlation(MAIN_PATH, OWN_S100_PATH, "2_result_own_size_m100");
    if(e != 0)
    {
        cout << "Ошибка нахождения корреляции: " <<  e << endl;
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
    // Загрузка изображений
    cv::Mat image_main = cv::imread(main_name, cv::IMREAD_UNCHANGED);
    cv::Mat image_quest = cv::imread(quest_name, cv::IMREAD_UNCHANGED);
    
    if (image_main.empty())
        return -1;
    if (image_quest.empty())
        return -2;

    // Центрирование яркости (алгоритм из второго файла)
    bright(image_main);
    bright(image_quest);

    int main_h = image_main.rows;
    int main_w = image_main.cols;
    int quest_h = image_quest.rows;
    int quest_w = image_quest.cols;

    cout << "Размер основного изображения: " << main_w << " x " << main_h << endl;
    cout << "Размер искомого изображения: " << quest_w << " x " << quest_h << endl;

    // Матрица корреляции (результат)
    cv::Mat cor(main_h - quest_h + 1, main_w - quest_w + 1, CV_64F, cv::Scalar(0));
    
    cout << "Вычисление корреляции..." << endl;
    auto start = std::chrono::high_resolution_clock::now();

    // Вычисление корреляции (алгоритм из второго файла)
    for (int y = 0; y <= main_h - quest_h; y++)
    {
        for (int x = 0; x <= main_w - quest_w; x++)
        {
            double c = 0;
            for (int l = 0; l < quest_h; l++)
            {
                for (int k = 0; k < quest_w; k++)
                {
                    c += image_main.at<float>(y + l, x + k) * image_quest.at<float>(l, k);
                }
            }
            cor.at<double>(y, x) = c;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // Поиск максимума
    double maxVal;
    cv::Point maxLoc;
    cv::minMaxLoc(cor, nullptr, &maxVal, nullptr, &maxLoc);

    cout << "Максимальная корреляция найдена в позиции: (" << maxLoc.x << ";" << maxLoc.y << ")" << endl;
    cout << "Значение корреляции: " << maxVal << endl;
    cout << "Время нахождения корреляции: " << duration.count() / 1000.0 << " мс" << endl;

    // Нормализация и сохранение результата
    cv::Mat cor_normalized;
    cv::normalize(cor, cor_normalized, 0, 255, cv::NORM_MINMAX, CV_8U);
    saveImage(cor_normalized, result_name);
    
    return 0;
}