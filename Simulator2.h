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
    
    std::vector<std::vector<double>> simulation(std::vector<double> dates);
    std::vector<double> indexSimulation(const BlackScholesModel& bsm, std::vector<double> dates, std::vector<std::vector<double>> randN) const;
    
};

void testBSMsimulator();

//För en viss uppsättning av bsm, unik T, volatilitet och r, så simulerar vi fram till T, detta kan återanvändas i alla //portföljer som använder samma typ av option??? i nuläget behövde put-option och call-option att köra om samma beräkning// //men om dessa har samma underliggande, så kan man använda exakt samma objekt för simuleringen?

//Till att börja med så simulerar vi för ett visst T, så att en put och call med samma underliggande kan återanvända resultatet.
