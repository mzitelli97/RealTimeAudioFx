#pragma once
#include <cmath>
#include <vector>
#include <complex>
#include <valarray>
#include <map>
#include <algorithm>
#include <windows.h>

#include <intrin.h>
using namespace std;
void fft(complex<float>* out, complex<float>* in,size_t n);
void ifft(complex<float>* out, complex<float>* in, size_t n);
