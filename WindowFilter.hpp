#ifndef __WINDOWFILTER_HPP
#define __WINDOWFILTER_HPP

#include "WindowType.hpp"

namespace Filtering
{
class WindowFilter
{
private:
  float *H;   // Импульсная характеристика
  int H_Size; // Размер окна

  float *Xn; // Входной буфер фильтрации
public:
  /**
   * @brief Метод подготовки коэффициентов импульсной характеристики.
   * Необходимо выполнить перед использованием фильтра!!!
   * 
   * @param Fd - Частота дискретизации
   * @param Fs - Частота среза
   * @param M - Размер окна
   * @param window - Тип окна (Блекмен или Хэмминг)
   */
  void PrepareRates(float Fd, float Fs, int M, WindowType window);

  /**
     * @brief Метод подготовки коэффициентов упрощённой
     * импульсной характеристики.
     * Необходимо выполнить перед использованием фильтра!!!
     * 
     * @param M - Размер окна (нечётное число)
     */
  void PrepareRates(int M);

  /**
     * @brief Метод фильтрации
     * 
     * @param data - нефильтрованные данные
     * @return float - фильтрованные данные
     */
  float FilterNext(float data);

  /**
   * @brief Очистка буфера фильтра
   * 
   */
  void Clear();
  void Clear(float value);

  WindowFilter();
  ~WindowFilter();
};
} // namespace Filtering

#endif