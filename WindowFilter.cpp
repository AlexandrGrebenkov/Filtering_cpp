/**
 * @brief ����� �������� ������� ��
 * 
 * @file Filter.cpp
 * @author Grebenkov Alexandr
 * @date 2018-08-27
 */

#include <stdint.h>
#include <math.h>
#include "WindowType.hpp"
#include "WindowFilter.hpp"

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

/**
   * @brief ����� ���������� ������������� ���������� ��������������.
   * ���������� ��������� ����� �������������� �������!!!
   * 
   * @param Fd - ������� �������������
   * @param Fs - ������� �����
   * @param M - ������ ����
   * @param window - ��� ���� (������� ��� �������)
   */
void WindowFilter::PrepareRates(float Fd, float Fs, int M, WindowType window)
{
    H_Size = M;
    delete[] H;
    H = new float[M];

    //���������� ������� (� ��������� �� 0 �� 0,5):
    float Fc = Fs / Fd;

    //���������� ������� ������������ ��������������
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

    //���������� ���������� ��������������
    float SUM = 0;
    for (int i = 0; i < M; i++)
        SUM += H[i];
    for (int i = 0; i < M; i++)
        H[i] /= SUM; //����� ������������� ������ ��������� 1
}

/**
     * @brief ����� ���������� ������������� ����������
     * ���������� ��������������.
     * ���������� ��������� ����� �������������� �������!!!
     * 
     * @param M - ������ ���� (�������� �����)
     */
void WindowFilter::PrepareRates(int M)
{
    if (M % 2 == 0)
        M--; //��� ������ �����
    H_Size = M;
    delete[] H;
    H = new float[M];

    for (int i = 0; i <= M / 2; i++)
        H[i] = H[M - i - 1] = i + 1;

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

void WindowFilter::Clear()
{
    Clear(0);
}

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