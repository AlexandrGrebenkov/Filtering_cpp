#ifndef __WINDOWTYPE_HPP
#define __WINDOWTYPE_HPP

namespace Filtering
{
typedef enum
{
    /// <summary>���� ��������</summary>
    Hamming,
    /// <summary>���� ��������</summary>
    Blackman,
    /// <summary>���������� ����</summary>
    Rectangle,
    /// <summary>����������� ����</summary>
    Triangle
} WindowType;
}

#endif