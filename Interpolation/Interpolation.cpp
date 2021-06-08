#include "Interpolation.h";

namespace Interpolation {

    double** Array;

    cubic_spline* sp;

    bool write = true;

    std::stringstream sOut;

    LPTSTR BuildPolinom(int n, int h, int x0, int number)
    {

        Array = new double* [2];
        for (int i = 0; i < 2; i++) {
            Array[i] = new double[n];
            memset(Array[i], 0, n * sizeof(double));
        }
        write = true;
        genPoints(x0, h, n);
        sp = new cubic_spline();
        switch (number)
        {
        case 1:
            getPolin(n, 1);
            break;
        case 2:
             GetPolin1(n, 1);
             break;
        case 3:
            sp->build_spline(Array[0], Array[1], n);
            break;
        default:
            break;
        }

        sOut.seekg(0, std::ios::end);
        int size = sOut.tellg();
        char* temp = (char*)malloc((size + 1)*sizeof(char));
        LPTSTR strOut = (wchar_t*)malloc((size + 1) * sizeof(wchar_t)); 
        size_t* PtNumOfCharConverted = NULL;
        auto str = sOut.str();
        std::copy(str.begin(), str.end(), temp);
        mbstowcs_s(PtNumOfCharConverted, strOut, size + 1, temp, size);
        free(temp);
        sOut.str("");
        return strOut;

        for (int i = 0; i < 2; i++) {
            delete[] Array[i];
        }
        delete[] Array;
    }

    float GetValue(int n, int h, int x0, double Xval, int number) {
        Array = new double* [2];
        for (int i = 0; i < 2; i++) {
            Array[i] = new double[n];
            memset(Array[i], 0, n * sizeof(double));
        }

        write = false;
        genPoints(x0, h, n);

        float res;
        if (Xval > 0) {
            switch (number)
            {
            case 1:
                res = getPolin(n, Xval);
                break;
            case 2:
                res = GetPolin1(n, Xval);
            case 3:
                res = sp->f(Xval);
                break;
            default:
                break;
            }
        }
        else return 0;

        for (int i = 0; i < 2; i++) {
            delete[] Array[i];
        }
        delete[] Array;
        return res;
    }

    double sine(double x) {
        __asm {
            fld x
            fsin
            fstp x
        }
        return x;
    }

    double func(double x) {
        double a = (x - 1) * (x - 1);
        double b = sine(2 * x);
        double c = a - b;
        return c;
    }

    void genPoints(int x0, int h, int n) {
        double a{ 0 };

        for (int i = 0; i < n; i++)
            Array[0][i] = x0 + ((double)i * h);
        for (int i = 0; i < n; i++)
            Array[1][i] = a = func(Array[0][i]);
    }

    double getPolin(int n, double x) {
        double val{ 0 };
        char buf[100];

        for (int k = 0; k < n; k++) {
            if (Array[1][k] > 0 && k != 0 && write) {
                sprintf_s(buf, "+ %f*", Array[1][k]);
                sOut << buf;
            }
            else if (Array[1][k] < 0 && k != 0 && write) {
                sprintf_s(buf, "- %f*", Array[1][k]);
                sOut << buf;
            }
            else if (write) {
                cutZeros(Array[1][k]); std::cout << "*";
            }
            val += Array[1][k] * Fk(x, k, n);
        }
        write = false;
        return val;
    }

    double Fk(double x, int k, int n) {
        double res{ 1 };
        char buf[100];

        for (int i = 0; i < n; i++) {
            if (i != k) {
                res *= x - Array[0][i];
                if (write) {
                    sOut << "(x - "; cutZeros(Array[0][i]); sOut << ")";
                }
            }
        }
        double denominator{ 1 };
        for (int i = 0; i < n; i++) {
            if (i != k)
                denominator *= Array[0][k] - Array[0][i];
        }
        if (write) {
            sOut << "/";
            cutZeros(denominator);
            sOut << " ";
        }
        return res / denominator;
    }

    void cutZeros(double val) {
        std::stringstream ss;
        ss << val;
        std::string sDenom = ss.str();
        sDenom = sDenom.substr(0, sDenom.find_last_not_of('0') + 1);
        if (sDenom == "") 
            sDenom = "0";
        sOut << sDenom;
    }

    //-------------------------------------------Niuton's Polinom--------------------------

    double GetPolin1(int n, double x) {
        double val = func(x);
        if (write)
            sOut << val << " + ";

        for (int i = 1; i < n; i++) {
            double temp = 1;

            temp *= subs(0, i);
            if (write) {
                cutZeros(temp);
                sOut << "*";
            }

            for (int j = 0; j < i; j++) {
                temp *= x - Array[0][j];
                if (write) {
                    sOut << "(x - "; 
                    cutZeros(Array[0][j]);
                    sOut << ")";
                }
            }

            val += temp;
        }
        return val;
    }

    double subs(int i, int k) {
        if (k - i == 1)
            return func(Array[0][k]) - func(Array[0][i]) 
            / ((Array[0][k] - Array[0][i]) * 1.0);
        else return (subs(i + 1, k) - subs(i, k - 1))
            /((Array[0][k] - Array[0][i]) * 1.0);
    }



//-----------------------------This code was taken from Wikipedia-------------------------------------------
//https://en.wikipedia.org/wiki/Spline_interpolation
//-------------------------and changed to generate polinom to stream----------------------------------------
    cubic_spline::cubic_spline() : splines(NULL)
    {

    }

    cubic_spline::~cubic_spline()
    {
        free_mem();
    }

    void cubic_spline::build_spline(const double* x, const double* y, std::size_t n)
    {
        free_mem();

        this->n = n;

        splines = new spline_tuple[n];
        for (std::size_t i = 0; i < n; ++i)
        {
            splines[i].x = x[i];
            splines[i].a = y[i];
        }
        splines[0].c = 0.;


        double* alpha = new double[n - 1];
        double* beta = new double[n - 1];
        double h_i1 = 0;
        double A, B, C, F, h_i, z;
        alpha[0] = beta[0] = 0.;
        for (std::size_t i = 1; i < n - 1; ++i)
        {
            h_i = x[i] - x[i - 1], h_i1 = x[i + 1] - x[i];
            A = h_i;
            C = 2. * (h_i + h_i1);
            B = h_i1;
            F = 6. * ((y[i + 1] - y[i]) / h_i1 - (y[i] - y[i - 1]) / h_i);
            z = (A * alpha[i - 1] + C);
            alpha[i] = -B / z;
            beta[i] = (F - A * beta[i - 1]) / z;
        }

        splines[n - 1].c = (F - A * beta[n - 2]) / (C + A * alpha[n - 2]);


        for (std::size_t i = n - 2; i > 0; --i)
            splines[i].c = alpha[i] * splines[i + 1].c + beta[i];


        delete[] beta;
        delete[] alpha;

        // По известным коэффициентам c[i] находим значения b[i] и d[i]
        for (std::size_t i = n - 1; i > 0; --i)
        {
            double h_i = x[i] - x[i - 1];
            splines[i].d = (splines[i].c - splines[i - 1].c) / h_i;
            splines[i].b = h_i * (2. * splines[i].c + splines[i - 1].c) / 6. + (y[i] - y[i - 1]) / h_i;
        }

        for (std::size_t i = 1; i < n-1; ++i)
        {
            sOut << splines[i].d / 6 << "(x - " << splines[i].x << ")^3 + "
                << splines[i].c / 2 << "(x - " << splines[i].x << ")^2 + "
                << splines[i].d << "(x - " << splines[i].x << ") + "
                << splines[i].a << " "
                << splines[i - 1].x << " <= x <= " << splines[i + 1].x << "\n";
        }
    }

    double cubic_spline::f(double x) const
    {
        if (!splines)
            return std::numeric_limits<double>::quiet_NaN(); 

        spline_tuple* s;
        if (x <= splines[0].x)
            s = splines + 1;
        else if (x >= splines[n - 1].x) 
            s = splines + n - 1;
        else 
        {
            std::size_t i = 0, j = n - 1;
            while (i + 1 < j)
            {
                std::size_t k = i + (j - i) / 2;
                if (x <= splines[k].x)
                    j = k;
                else
                    i = k;
            }
            s = splines + j;
        }

        double dx = (x - s->x);
        return s->a + (s->b + (s->c / 2. + s->d * dx / 6.) * dx) * dx; 
    }

    void cubic_spline::free_mem()
    {
        delete[] splines;
        splines = NULL;
    }
}
//-----------------------------------------------------------------------------