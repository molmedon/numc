#include <Random.hpp>

#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/poisson_distribution.hpp>

boost::mt19937 gen;

double uniform(double min, double max) {

  // NOTE: boost::uniform_real is a closed range i.e. dist(0, 2) == [0, 2]
  // unlike open range of normal C/C++ RNG's
  boost::uniform_real<double> dist(min, max);

  // we use a global generator to avoid initialize a Mersenne Twister each invocation
  boost::variate_generator<boost::mt19937&, boost::uniform_real<double>> uni(gen, dist);

  return uni();
}

int uniformInt(int min, int max) {

    // NOTE: boost::uniform_int is a closed range i.e. dist(0, 2) == [0, 1, 2]
    // unlike open range of normal C/C++ RNG's
    boost::uniform_int<int> dist(min, max);

    // we use a global generator to avoid initialize a Mersenne Twister each invocation
    boost::variate_generator<boost::mt19937&, boost::uniform_int<int>> uni(gen, dist);

    return uni();
}

int poisson(double mean) {

  // poisson distribution generator
  boost::poisson_distribution<int> dist(mean);

  // we use a global generator to avoid initialize a Mersenne Twister each invocation
  boost::variate_generator<boost::mt19937&,
                           boost::poisson_distribution<int>> pois(gen, dist);

  // return a poisson sampled variable
  return pois();
}

double gaussian(double mean, double stdev) {

  // normal distribution - mean and stdev
  boost::normal_distribution<double> dist(mean, stdev);

  // we use a global generator to avoid initialize a Mersenne Twister each invocation
  boost::variate_generator<boost::mt19937&,
                           boost::normal_distribution<double>> norm(gen, dist);

  return norm();
}

double sampleFromFunction(std::function<double(double)> f,
                          double xmin, double xmax,
                          double fmin, double fmax) {

    // get a uniform sample on [0, 1] and transform it to [xmin, xmax]
    double u = (xmax - xmin)*uniform() + xmin;

    // and another sample scaled to the domain
    double w = (fmax - fmin)*uniform() + fmin;

    // and iterate until we meet the requirement
    while (f(u) > w) {
        // get a new value of u and w
        u = (xmax - xmin)*uniform() + xmin;
        w = (fmax - fmin)*uniform() + fmin;
    }

    return u;
}
