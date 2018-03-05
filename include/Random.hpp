#pragma once

#include <functional>
#include <boost/random/mersenne_twister.hpp>

// define global RNG
extern boost::mt19937 gen;

// Returns a uniform random variable between min and max, inclusive
double uniform(double min=0, double max=1);

// Returns a uniform random integer between min and max, inclusive
int uniformInt(int min, int max);

// Returns a poisson-distributed random variable with 'mean'
int poisson(double mean);

// Returns a gaussian with mean 'mean' and standard deviation 'stdev'
double gaussian(double mean=0, double stdev=1);

// use Von Neumann acceptance-reject sampling to draw samples
// from a function f(). f must have domain [xmin, xmax] and range [fmin, fmax]
double sampleFromFunction(std::function<double(double)> f,
                          double xmin, double xmax,
                          double fmin, double fmax);
