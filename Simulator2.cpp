#include <stdio.h>

#include "matlib.h"
#include "Simulator2.h"

#include <iterator>
#include <algorithm>
#include <functional>
#include <chrono>

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
        for (int j = 0; j < nDates; j++)//There should be a better way to add two vectors, something like transform. 
        {
            res[j] += temp_res[j]/nScenarios;
        }
    }
     
    return res;
}

vector<vector<double>> Simulator2::simulation(vector<double> dates, bool antithetic, bool standardisation)
{
    rng("default");
    
    vector<vector<double>> randNumbers; //randNumbers(nScenarios,dates); man kanske kan göra nåt i den stilen och skippa push_back, vilket blir lite snabbare.
    
    int nDates = dates.size();
    
    int anti = 1; //dåligt namn.
    if(antithetic)
    {
        anti = 2;
    }
    
    for(int i = 0; i < nScenarios / anti; i++)
    {
        vector<double> unnormalized_randn = randn(nDates);
        
        //Normalization so that we really get the N(0,1)
        if(standardisation)
        {
            vector<double> normalized_randn(nDates,0.0);
            double normalized_mean = mean(unnormalized_randn);
            double normalized_std = standardDeviation(unnormalized_randn, true);
            vector<double> myconstants = {normalized_mean,normalized_std};
            
            transform(unnormalized_randn.begin(), unnormalized_randn.end(), normalized_randn.begin(), [&myconstants](double& c){return (c-myconstants[0]) / myconstants[1] ;});
            randNumbers.push_back(normalized_randn);
        }
        else
        {
            randNumbers.push_back(unnormalized_randn);
        }
       
    }
    
    if(antithetic)
    {
        double myconstant{-1.0};
        for(int i = 0; i < nScenarios / anti; i++) //notera att randn är dyrt och vi slipper det här.
        {
            vector<double> anti_path(nDates,0.0);
            vector<double> temp_rand = randNumbers[i];
            transform(temp_rand.begin(), temp_rand.end(), anti_path.begin(), [&myconstant](double& c){return c*myconstant;});
            randNumbers.push_back(anti_path);
            
        }
    }
    return randNumbers;
}


void testBSMsimulator()
{
    
    //vector<vector<double>> randNumbers;
    

    Simulator2 simulator;
    simulator.nScenarios = 1000000;
    simulator.dates = {3/12, 6/12, 1, 2, 3, 5, 7, 10, 15, 20};
    //randNumbers = simulator.simulation(simulator.dates,true,true);
    
    BlackScholesModel m;
    m.volatility = 0.1;
    m.riskFreeRate = 0.05;
    m.stockPrice = 100.0;
    m.drift = 0.1;
    m.date = 0;
    
    vector<vector<double>> randNumbers;
    vector<double> res;
    
    vector<vector<double>> randNumbers2;
    vector<double> res2;
    
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    randNumbers = simulator.simulation(simulator.dates,true,true);
    res = simulator.indexSimulation(m, simulator.dates, randNumbers);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    
    chrono::steady_clock::time_point begin2 = chrono::steady_clock::now();
    randNumbers2 = simulator.simulation(simulator.dates,false,true);
    res2 = simulator.indexSimulation(m, simulator.dates, randNumbers2);
    chrono::steady_clock::time_point end2 = chrono::steady_clock::now();
    

    double time1 = (end - begin).count();
    double time2 = (end2 - begin2).count();
    //double time_diff = time2-time1;
    cout<< time1<<endl;
    cout << time2 <<endl;
    //cout << time_diff << endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
    
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end2 - begin2).count() << "[µs]" << std::endl;
    //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
    
    cout<<"testBSMsimulator is done."<<endl;

    
}
 
