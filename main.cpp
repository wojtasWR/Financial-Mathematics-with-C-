#include <iostream>
#include <string>
#include "basic.h"
#include "gaussians.h"
#include <vector>
#include <random>
#include <numeric>
#include "testing.h"

using namespace std;

class  BlackScholesModel
{
public:
double stockPrice;
double volatility;
double riskFreeRate;
double date;
};

class CallOption
{
    public:
    double strike;
    double maturity;
    double payoff(double stockAtMaturity) const;
    double price(const BlackScholesModel& bsm) const;
};

double CallOption::payoff (double stockAtMaturity) const
{
if(stockAtMaturity > strike){
    return  stockAtMaturity - strike ;
}else{
    return 0.0;
}
}

double CallOption::price(const BlackScholesModel& bsm) const
{
    double S = bsm.stockPrice;
    double K = strike;
    double sigma = bsm.volatility;
    double r = bsm.riskFreeRate;
    double T = maturity - bsm.date;
    double numerator = log(S/K) + (r + sigma*sigma*0.5)*T;
    double denominator = sigma * sqrt(T);
    double d1 = numerator / denominator;
    double d2 = d1 - denominator;
    return S*normalCdf(d1) - exp(-r*T)*K*normalCdf(d2);
}

static void testCallOptionPrice ()
{
    CallOption callOption;
    callOption.strike = 105.0;
    callOption.maturity = 2.0;
    BlackScholesModel bsm;
    bsm.date = 1.0;
    bsm.volatility = 0.1;
    bsm.riskFreeRate = 0.05;
    bsm.stockPrice = 100.0;
    double price = callOption.price(bsm);
    std::cout << price << std::endl;
    ASSERT_APPROX_EQUAL(price ,4.046 , 0.01);
}


int main(){
    
    testCallOptionPrice();
    double d;
    cout<<"What␣is␣the␣value␣of␣d?\n";
    cout << d <<endl;
    return 0;
}



