#pragma once
#include <iostream>
#include <sstream>
#include <iosfwd>
#include <Windows.h>
#include <string.h>
#include <tchar.h>
#include <stdlib.h>
#include <math.h>

namespace Interpolation {

	LPTSTR BuildPolinom(int n, int h, int x0,int number);		//Allocates mem with malloc 

	double sine(double x);

	double func(double x);

	void genPoints(int x0, int h, int n);

	double getPolin(int n, double x);

	double Fk(double x, int k, int n);

	void cutZeros(double val);

	float GetValue(int n, int h, int x0, double Xval, int number);

	double subs(int i, int k);

	double GetPolin1(int n, double x);

    class cubic_spline
    {
    private:
        // Структура, описывающая сплайн на каждом сегменте сетки
        struct spline_tuple
        {
            double a, b, c, d, x;
        };

        spline_tuple* splines; // Сплайн
        std::size_t n; // Количество узлов сетки

        void free_mem(); // Освобождение памяти

    public:
        cubic_spline(); //конструктор
        ~cubic_spline(); //деструктор

        // Построение сплайна
        // x - узлы сетки, должны быть упорядочены по возрастанию, кратные узлы запрещены
        // y - значения функции в узлах сетки
        // n - количество узлов сетки
        void build_spline(const double* x, const double* y, std::size_t n);

        // Вычисление значения интерполированной функции в произвольной точке
        double f(double x) const;
    };

}