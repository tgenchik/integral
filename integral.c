#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

extern double f1(double x);
extern double f2(double x);
extern double f3(double x);

double e = exp(1.0);

double test_1(double x);
double test_2(double x);
double test_3(double x);

double test_1(double x) {
    return (double)(5) / x / x;
}

double test_2(double x) {
    return 3 * pow (e, x);
}
double test_3(double x) {
    return 4 * x + 3 * e - (double)(1);
}


double root(double (*g1)(double), double (*g2)(double), double a, double b, double eps);
double integral(double (*f)(double), double a, double b, double eps);

int use_iteration_count = 0; // считаем ли мы итерации в функции root

double root(double (*g1)(double), double (*g2)(double), double a, double b, double eps) { // метод тарпеций

    double h_a = g1(a) - g2(a), h_b = g1(b) - g2(b);
    double left = a, right = b;
    double f_left = h_a, f_right = h_b;
    double x_now = a, x_last;
    int iter = 0, max_iter = 1337;

    while (iter < max_iter) {
        x_last = x_now;

        if (fabs(f_right - f_left) < 1e-15) //  что бы не возникло zero division error
            break;
        x_now = (left * f_right - right * f_left) / (f_right - f_left);
        double f_now = g1(x_now) - g2(x_now);

        if (fabs(f_now) < eps)
            break;

        if (f_left * f_now < 0) {
            right = x_now;
            f_right = f_now;
        } else {
            left = x_now;
            f_left = f_now;
        }
        if (iter >= 1 && fabs(x_now - x_last) < eps) {
            break;
        }
        iter++;
    }
    if (use_iteration_count) {
        printf("itterations %d \n", iter);
    }
    return x_now;
}


double integral(double (*f)(double), double a, double b, double eps) { // метод хорд

    int reverse = 0;
    if (a > b) {
        double temp = a;
        a = b;
        b = temp;
        reverse = 1;
    }

    int n = 1;
    double h = b - a;
    double T_last = 0.5 * h * (f(a) + f(b));

    const int max_iter = 22; // до четырех миллионов точек
    double T_new = T_last;

    for (int iter = 0; iter < max_iter; iter++) {
        n *= 2;
        h /= 2.0;
        double sum_new = 0.0;
        for (int i = 1; i < n; i += 2) { // только НОВЫЕ точки
            double x = a + i * h;
            sum_new += f(x);
        }

        T_new = 0.5 * T_last + h * sum_new;
        if (iter > 0 && fabs(T_new - T_last) < eps) // точность достигнута?
            break;
        T_last = T_new;
    }

    return reverse ? -T_new : T_new;
}


int main(int argc, char* argv[]) {
    if (argc == 1) {
        double A = root(f1, f2, 0.2, 5.0, 1e-8); // точки пересечения
        double B = root(f2, f3, 0.2, 5.0, 1e-8);
        double C = root(f1, f3, 0.2, 5.0, 1e-8);
        //printf("%lf %lf %lf ", A, B, C);

        double s1 = integral(f1, C, A, 1e-8);
        double s2 = 0.5 * (A - 1) * f2(A);
        double s3 = 0.5 * (B - 1) * f2(B);
        double s4 = integral(f3, C, B, 1e-8);
        //printf("%lf %lf %lf ", s1, s2, s3);

        double answer = s1 - s2 + s3 - s4;
        printf ("%lf\n", answer);
    }
    for (int i = 0; i < argc; i++) {
        char* param = argv[i];
        if ((strcmp(param, "--help") == 0) || (strcmp(param, "-h") == 0)) {
            printf("\t-r, --root\t\tCalc root\n");
            printf("\t-h, --help\t\tCall help\n");
            printf("\t-i, --iterations\tPrint number of iterations\n");
            printf("\t-R, --test-root\t\tTest function root\n");
            printf("\t-I, --test-integral\tTest functoion integral\n");
        }
        else if ((strcmp(param, "--root") == 0) || (strcmp(param, "-r") == 0)) {
            printf("f1^f2 : %lf\n", root(f1, f2, 0.2, 5.0, 1e-8));
            printf("f2^f3 : %lf\n", root(f2, f3, 0.2, 5.0, 1e-8));
            printf("f1^f3 : %lf\n", root(f1, f3, 0.2, 5.0, 1e-8));
        }
        else if ((strcmp(param, "--test-root") == 0) || (strcmp(param, "-R") == 0)) {
            int f, g;
            double a, b, eps, r, ans;
            i++;
            double (*functions[3])(double) = {test_1, test_2, test_3};
            sscanf(argv[i], "%d:%d:%lf:%lf:%lf:%lf", &f, &g, &a, &b, &eps, &r);
            ans = root(functions[f - 1], functions[g - 1], a, b, eps);
            printf("%lf %lf %lf\n", ans, fabs(ans - r), fabs((ans - r) / r));
        }
        else if ((strcmp(param, "--iterations") == 0) || (strcmp(param, "-i") == 0)) {
            use_iteration_count = 1; // считаем операции
            printf("f1^f2:\n");
            root(f1, f2, 0.2, 5.0, 1e-8);
            printf("f2^f3:\n");
            root(f2, f3, 0.2, 5.0, 1e-8);
            printf("f1^f3:\n");
            root(f1, f3, 0.2, 5.0, 1e-8);
            use_iteration_count = 0;
        }
        else if ((strcmp(param, "--test-integral") == 0) || (strcmp(param, "-I") == 0)) {
            ++i;
            int f;
            double a, b, eps, r, ans;
            sscanf(argv[i], "%d:%lf:%lf:%lf:%lf", &f, &a, &b, &eps, &r);
            if (f == 1) {
                ans = integral(test_1, a, b, eps);
            } else if (f == 2) {
                ans = integral(test_2, a, b, eps);
            } else {
                ans = integral(test_3, a, b, eps);
            }
            printf("%lf\n%lf\n%lf\n", ans, fabs(ans - r), fabs((ans - r) / r));
        }
    }
    return 0;
}
