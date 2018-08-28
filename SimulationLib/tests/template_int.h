#include "RNG.h"
#include <cmath>
#include <random>

template<typename distype> 
class TempTest {
  public:
    TempTest () {};
    virtual long double pdf(distype value) const = 0;
    virtual long double cdf(distype value) const = 0;
    virtual distype Inverse(long double value) const = 0;
    long double Sample(RNG &rng) const {
      return(this->operator()(rng.mt_));
    }
    virtual distype operator()(std::mt19937_64 &g) const = 0;
};

class TOne : public TempTest<long> {
  public:
    // μ is the expected value here.
    inline TOne(long double mu = 1) {
      init(mu);
    }
    void init(long double = 1);
    virtual long double pdf(long value) const;
    virtual long double cdf(long value) const;
    virtual long Inverse(long double value) const;
    virtual long operator()(std::mt19937_64 &g) const;
};

class Ttwo : public TempTest<long double> {
  public:
    virtual long double pdf(long double value) const;
    virtual long double cdf(long double value) const;
    virtual long double Inverse(long double value) const;
    virtual long double operator()(std::mt19937_64 &g) const;
};

class Poisson_1: public Ttwo {
  public:
    // μ is the expected value here.
    inline Poisson_1(long double mu = 1) {
      init(mu);
    }
    void init(long double = 1);
  private:
    boost::math::poisson_distribution<long double, POLROUNDDOWN> pcdist;
    mutable std::poisson_distribution<long> dist;
};

// class Gamma_1 : public TOne {
// public:
//   // Proportional to x^(α-1)e^-(x/β).
//   inline Gamma(long double alpha = 1, long double beta = 1,
//    long double shift = 0) : pcdist(alpha, beta) {
//     init(alpha, beta, shift);
//   }
//   void init(long double = 1, long double = 1, long double = 0);
//   static Gamma mean_sdev(long double mu = 1, long double sigma = 1);
//   virtual long double pdf(long double value) const;
//   virtual long double cdf(long double value) const;
//   virtual long double Inverse(long double value) const;
//   virtual long double operator()(std::mt19937_64 &g) const;
//   ACCESSORC(shift, pshift);
//   ACCESSOR(cdist, pcdist);
// private:
//   long double pshift;
//   boost::math::gamma_distribution<long double> pcdist;
//   mutable std::gamma_distribution<long double> dist;
// };

void testPoisson1(mt19937_64 &g) {
  cout << "Testing Poisson1:" << endl;
  array<long, 1000> sample;
  long double mu;
  cout << "Enter μ: ";
  cin >> mu;
  Poisson1 pois(mu);
  for(int i = 0; i < 1000; i++)
    sample[i] = pois(g);
  output_xbar_s2(sample);
  cout << "(Expected: σ² = " << mu
       << ")" << endl;
}

// int main (){
//   long seed;
//   cout << "Enter seed: ";
//   cin >> seed;
//   RNG myRNG(seed);
//   mt19937_64 &g = myRNG.mt_;
//   testPoisson1()
// }