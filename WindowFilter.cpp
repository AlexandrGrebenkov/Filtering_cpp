/**
 * @brief Класс оконного фильтра НЧ
 * 
 * @file Filter.cpp
 * @author Grebenkov Alexandr
 * @date 2018-08-27
 */

#include <stdint.h>
#include <math.h>
#include "WindowType.hpp"
#include "WindowFilter.hpp"

#define PI 3.14159265358979323846 // Число Пи

namespace Filtering
{
float GetBlackmanWindow(int i, int M)
{
    return 0.42f - 0.5f * cos((2 * PI * i) / M) + 0.08 * cos((4 * PI * i) / M);
}

float GetHammingWindow(int i, int M)
{
    return 0.54f - 0.46f * cos((2 * PI * i) / M);
}

/**
   * @brief Метод подготовки коэффициентов импульсной характеристики.
   * Необходимо выполнить перед использованием фильтра!!!
   * 
   * @param Fd - Частота дискретизации
   * @param Fs - Частота среза
   * @param M - Размер окна
   * @param window - Тип окна (Блекмен или Хэмминг)
   */
void WindowFilter::PrepareRates(float Fd, float Fs, int M, WindowType window)
{
    H_Size = M;
    delete[] H;
    H = new float[M];

    //Приведённая частота (в диапазоне от 0 до 0,5):
    float Fc = Fs / Fd;

    //Определяем весовые коэффициенты характеристики
    float h;
    for (int i = 0; i < M; i++)
    {
        if (i - M / 2 == 0)
            h = 2 * PI * Fc;
        else
            h = sin(2 * PI * Fc * (i - M / 2)) / (i - M / 2);

        if (window == WindowType::Hamming)
            H[i] = h * GetHammingWindow(i, M);
        else
            H[i] = h * GetBlackmanWindow(i, M);
    }

    //Нормировка импульсной характеристики
    float SUM = 0;
    for (int i = 0; i < M; i++)
        SUM += H[i];
    for (int i = 0; i < M; i++)
        H[i] /= SUM; //сумма коэффициентов должна равняться 1
}

/**
     * @brief Метод подготовки коэффициентов упрощённой
     * импульсной характеристики.
     * Необходимо выполнить перед использованием фильтра!!!
     * 
     * @param M - Размер окна (нечётное число)
     */
void WindowFilter::PrepareRates(int M)
{
    if (M % 2 == 0)
        M--; //Для чётных чисел
    H_Size = M;
    delete[] H;
    H = new float[M];

    for (int i = 0; i <= M / 2; i++)
        H[i] = H[M - i - 1] = i + 1;

    //Нормировка импульсной характеристики
    float SUM = 0;
    for (int i = 0; i < M; i++)
        SUM += H[i];
    for (int i = 0; i < M; i++)
        H[i] /= SUM; //сумма коэффициентов должна равняться 1
}

/**
     * @brief Метод фильтрации
     * 
     * @param data - нефильтрованные данные
     * @return float - фильтрованные данные
     */
float WindowFilter::FilterNext(float data)
{
    if (H == nullptr) // Если нет импульсной характеристики
        return data;
    if (Xn == nullptr)
    { // Если массива не было, то создадим его
        Xn = new float[H_Size];
        for (int i = 0; i < H_Size; i++)
            Xn[i] = data; // и заполним первым числом
    }
    else
    {
        for (int i = 0; i < H_Size - 1; i++)
            Xn[i] = Xn[i + 1]; // Сдвигаем весь массив
        Xn[H_Size - 1] = data; // И дописываем новое число
    }

    //Фильтрация входных данных
    float result = 0;
    for (int j = 0; j < H_Size; j++) // свёртка
        result += H[j] * Xn[j];

    return result;
}

void WindowFilter::Clear()
{
    Clear(0);
}

void WindowFilter::Clear(float value)
{
    if (Xn == nullptr)
        Xn = new float[H_Size]; // Если массива не было, то создадим его

    for (int i = 0; i < H_Size; i++)
        Xn[i] = value;
}

WindowFilter::WindowFilter()
{
    H = nullptr;
    Xn = nullptr;
}

WindowFilter::~WindowFilter()
{
    delete[] H;
    delete[] Xn;
}

} // namespace Filtering