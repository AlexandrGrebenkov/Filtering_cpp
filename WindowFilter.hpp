#ifndef __WINDOWFILTER_HPP
#define __WINDOWFILTER_HPP

#include "WindowType.hpp"

namespace Filtering
{
class WindowFilter
{
private:
  float *H;   // ���������� ��������������
  int H_Size; // ������ ����

  float *Xn; // ������� ����� ����������
public:
  /**
   * @brief ����� ���������� ������������� ���������� ��������������.
   * ���������� ��������� ����� �������������� �������!!!
   * 
   * @param Fd - ������� �������������
   * @param Fs - ������� �����
   * @param M - ������ ����
   * @param window - ��� ���� (������� ��� �������)
   */
  void PrepareRates(float Fd, float Fs, int M, WindowType window);

  /**
     * @brief ����� ���������� ������������� ����������
     * ���������� ��������������.
     * ���������� ��������� ����� �������������� �������!!!
     * 
     * @param M - ������ ���� (�������� �����)
     */
  void PrepareRates(int M);

  /**
     * @brief ����� ����������
     * 
     * @param data - ��������������� ������
     * @return float - ������������� ������
     */
  float FilterNext(float data);

  /**
   * @brief ������� ������ �������
   * 
   */
  void Clear();
  void Clear(float value);

  WindowFilter();
  ~WindowFilter();
};
} // namespace Filtering

#endif