//
//  Simulator2.cpp
//  FMLib
//
//  Created by Wojciech Ratusznik on 2023-02-21.
//  Copyright © 2023 C++ for financial mathematics. All rights reserved.
//

#include <stdio.h>

#include "matlib.h"
#include "Simulator2.h"

#include <iterator>
#include <algorithm>
#include <functional>

using namespace std;

Simulator2::Simulator2():
    nScenarios(10000),
    dates({3/12,6/12,1,2,3,5,7,10,15,20}){
}

//GBM simultion, we make an assumption about the index that call and put options use.
vector<double> Simulator2::indexSimulation(const BlackScholesModel& bsm, vector<double> dates, vector<vector<double>> randN) const
{
    int nDates = dates.size();
    
    vector<double> res(nDates, 0.0);
    vector<double> temp_res(nDates, 0.0);
    
    for (int i = 0; i < nScenarios; i++)
    {
        temp_res = bsm.generateRiskNeutralPricePath(dates,
                                                    randN[i]);
        for (int j = 0; j < nDates; j++)
        {
            res[j] += temp_res[j]/nScenarios;
        }
    }
     
    return res;
}

vector<vector<double>> Simulator2::simulation(vector<double> dates)
{
    rng("default");
    
    vector<vector<double>> randNumbers; //randNumbers(nScenarios,dates); man kanske kan göra nåt i den stilen och skippa push_back, vilket blir lite snabbare.
    
    int nDates = dates.size();
    
    for(int i = 0; i < nScenarios / 2; i++) //lägger på antithetic + normalisering snart. ska få det att fungera först,
    {
        vector<double> unnormalized_randn = randn(nDates);
        vector<double> normalized_randn(nDates,0.0);
        
         //Normalization so that we really get the N(0,1)
        double normalized_mean = mean(unnormalized_randn);
        double normalized_std = standardDeviation(unnormalized_randn, true);
        vector<double> myconstants = {normalized_mean,normalized_std};
        
        transform(unnormalized_randn.begin(), unnormalized_randn.end(), normalized_randn.begin(), [&myconstants](double& c){return (c-myconstants[0]) / myconstants[1] ;});
        
        randNumbers.push_back(normalized_randn);
    }
    
    double myconstant{-1.0};
    for(int i = 0; i < nScenarios / 2; i++) //notera att randn är dyrt och vi slipper det här.
    {
        vector<double> anti_path(nDates,0.0);
        vector<double> temp_rand = randNumbers[i];
        transform(temp_rand.begin(), temp_rand.end(), anti_path.begin(), [&myconstant](double& c){return c*myconstant;});
        randNumbers.push_back(anti_path);
        
    }
    
    return randNumbers;
}


void testBSMsimulator()
{
    rng("default");
    
    vector<vector<double>> randNumbers;
    

    Simulator2 simulator;
    simulator.nScenarios = 10000;
    simulator.dates = {3/12, 6/12, 1, 2, 3, 5, 7, 10, 15, 20};
    randNumbers = simulator.simulation(simulator.dates);
    
    BlackScholesModel m;
    m.volatility = 0.1;
    m.riskFreeRate = 0.05;
    m.stockPrice = 100.0;
    m.drift = 0.1;
    m.date = 0;
    
    vector<double> res = simulator.indexSimulation(m, simulator.dates, randNumbers);
    double a = 5.0;
    cout<<a<<endl;
    

   
    
  
    //105.1615 vid 1k steg vs 105.011 vid 10 steg, båda fallen 10k scenarios.
    //Antithetic 104.6027, 104.6027, fick exakt samma nu.
    //Första metoden var att for loopa och köra n_inv på 1-U, men n_inv kostar nog mycket? Så vi vill istället
    //Köra n/2 stycken U, och ta fram n_inv på dessa och sen ersätta dessa med negativa värden, verkar bli samma sak
    //Det som återstår att testa är ifall jag kommer få exakt samma sak om jag inte kör atithetic men kör fler scenarios,
    //Den borde konvergera till samma värde!
    //Ska man lägga till antithetic sampling direkt och normaliserings optimering + vektoriseringen???
    //Sen kommer alla derivat använda samma index-vektor?
    
    
}
 
