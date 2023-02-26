//
//  Simulator2.h
//  FMLib
//
//  Created by Wojciech Ratusznik on 2023-02-21.
//  Copyright © 2023 C++ for financial mathematics. All rights reserved.
//

//
#pragma once

#include "stdafx.h"
#include "BlackScholesModel.h"
#include "matlib.h"


//BSM simulator
class Simulator2
{
public:
    
    Simulator2();
    
    int nScenarios;
    std::vector<double> dates;
    
    std::vector<std::vector<double>> simulation(std::vector<double> dates, bool antithetic, bool standardisation);
    std::vector<double> indexSimulation(const BlackScholesModel& bsm, std::vector<double> dates, std::vector<std::vector<double>> randN) const;
    
};

void testBSMsimulator();
