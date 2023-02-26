#pragma once

#include "stdafx.h"

class BlackScholesModel {
public:
    BlackScholesModel();
    double drift;
    double stockPrice;
    double volatility;
    double riskFreeRate;
    double date;

    std::vector<double> generateRiskNeutralPricePath(
                            std::vector<double> dates,
                            std::vector<double> randN_) const;

};

void testBlackScholesModel();









