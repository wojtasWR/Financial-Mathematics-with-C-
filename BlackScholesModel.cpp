#include "BlackScholesModel.h"

using namespace std;

#include "matlib.h"

BlackScholesModel::BlackScholesModel() :
    drift(0.0),
    stockPrice(0.0),
    volatility(0.0),
    riskFreeRate(0.0),
    date(0.0){
}

/**
 *  Creates a price path according to the model parameters
 */
vector<double> BlackScholesModel::
        generateRiskNeutralPricePath(
        vector<double> dates,
        vector<double> randN) const {
            
            int nDates = dates.size();
            vector<double> path(nDates,0.0);
            path[0] = stockPrice;
            for (int i = 1; i < nDates; i++)
            {
                path[i] = path[0]*exp((riskFreeRate-volatility*volatility/2)*dates[i] + volatility * randN[i]);
            }
            
    return path;
}


////////////////////////////////
//
//   TESTS
//
////////////////////////////////

            
            
/*
 
void testRiskNeutralPricePath() {
    rng("default");

    BlackScholesModel bsm;
    bsm.riskFreeRate = 0.05;
    bsm.volatility = 0.1;
    bsm.stockPrice = 100.0;
    bsm.date = 2.0;

    int nPaths = 10000;
    int nsteps = 5;
    double maturity = 4.0;
    vector<double> finalPrices(nPaths,0.0);
    for (int i=0; i<nPaths; i++) {
        vector<double> path =
            bsm.generateRiskNeutralPricePath( maturity,
                                              nsteps );
        finalPrices[i] = path.back();
    }
    ASSERT_APPROX_EQUAL( mean( finalPrices ),
        exp( bsm.riskFreeRate*2.0)*bsm.stockPrice, 0.5);
}

void testVisually() {
    BlackScholesModel bsm;
    bsm.riskFreeRate = 0.05;
    bsm.volatility = 0.1;
    bsm.stockPrice = 100.0;
    bsm.date = 2.0;

    int nSteps = 1000;
    double maturity = 4.0;

    vector<double> path = bsm.generatePricePath( maturity,
                                                 nSteps );
    double dt = (maturity-bsm.date)/nSteps;
    vector<double> times = linspace(dt, maturity, nSteps );
    plot("examplePricePath.html",
         times,
         path );
}


            
void testBlackScholesModel() {
    TEST( testRiskNeutralPricePath );
    TEST( testVisually );
}
 
*/
