#ifndef __WINDOWTYPE_HPP
#define __WINDOWTYPE_HPP

namespace Filtering
{
typedef enum
{
    /// <summary>Окно Хэмминга</summary>
    Hamming,
    /// <summary>Окно Блекмена</summary>
    Blackman,
    /// <summary>Квадратное окно</summary>
    Rectangle,
    /// <summary>Треугольное окно</summary>
    Triangle
} WindowType;
}

#endif