/**
 * @brief ����� �������� ������� ��
 *
 * @file Filter.cpp
 * @author Grebenkov Alexandr
 * @date 2018-08-27
 */

#include "WindowFilter.hpp"
#include "WindowType.hpp"
#include <math.h>
#include <stdint.h>

#define PI 3.14159265358979323846 // ����� ��

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

float GetTriangleWindow(int i, int M)
{
    if (i <= M / 2)
        return i;
    else
        return M / 2 - (i - M / 2);
}

float GetRectangleWindow(int i, int M) { return 1; }

/**
 * @brief ����� ���������� ������������� ���������� ��������������.
 * ���������� ��������� ����� �������������� �������!!!
 *
 * @param Fd - ������� �������������
 * @param Fs - ������� �����
 * @param M - ������ ����
 * @param window - ��� ����
 */
void WindowFilter::PrepareRates(float Fd, float Fs, int M, WindowType window)
{
    if (M % 2 == 0)
        M--;
    H_Size = M;
    delete[] H;
    H = new float[M];

    //���������� ������� (� ��������� �� 0 �� 0,5):
    float Fc = Fs / Fd;

    //���������� ������� ������������ ��������������
    float h;
    for (int i = 0; i < M; i++)
    {
        switch (window)
        {
        case WindowType::Hamming:
        {
            if (i - M / 2 == 0)
                h = 2 * PI * Fc;
            else
                h = sin(2 * PI * Fc * (i - M / 2)) / (i - M / 2);
            H[i] = h * GetHammingWindow(i, M);
            break;
        }
        case WindowType::Blackman:
        {
            if (i - M / 2 == 0)
                h = 2 * PI * Fc;
            else
                h = sin(2 * PI * Fc * (i - M / 2)) / (i - M / 2);
            H[i] = h * GetBlackmanWindow(i, M);
            break;
        }
        case WindowType::Triangle:
        {
            H[i] = GetTriangleWindow(i, M);
            break;
        }
        case WindowType::Rectangle:
        {
            H[i] = GetRectangleWindow(i, M);
            break;
        }
        }
    }

    //���������� ���������� ��������������
    float SUM = 0;
    for (int i = 0; i < M; i++)
        SUM += H[i];
    for (int i = 0; i < M; i++)
        H[i] /= SUM; //����� ������������� ������ ��������� 1
}

/**
 * @brief ����� ����������
 *
 * @param data - ��������������� ������
 * @return float - ������������� ������
 */
float WindowFilter::FilterNext(float data)
{
    if (H == nullptr) // ���� ��� ���������� ��������������
        return data;
    if (Xn == nullptr)
    { // ���� ������� �� ����, �� �������� ���
        Xn = new float[H_Size];
        for (int i = 0; i < H_Size; i++)
            Xn[i] = data; // � �������� ������ ������
    }
    else
    {
        for (int i = 0; i < H_Size - 1; i++)
            Xn[i] = Xn[i + 1]; // �������� ���� ������
        Xn[H_Size - 1] = data; // � ���������� ����� �����
    }

    //���������� ������� ������
    float result = 0;
    for (int j = 0; j < H_Size; j++) // ������
        result += H[j] * Xn[j];

    return result;
}

void WindowFilter::Clear() { Clear(0); }

void WindowFilter::Clear(float value)
{
    if (Xn == nullptr)
        Xn = new float[H_Size]; // ���� ������� �� ����, �� �������� ���

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