#include "../include/SimulationLib/JSONParameterize.h"
#include <fstream>
#include <cmath>
namespace SimulationLib {

  // enum class var_type {
  //   int_type = 1, long_type, long_double_type, arr_type, template_type, vector_type
  // };
  GeneralStatDist mapDistribution(json jobj){
    //map contains all possible distribution names with number of parameters it takes
    //second value in vector corresponds with the type of the parameter
    //If a value is arr_type, following two values describe values taken in 2d array
    //If a value is vector_type, following value describe values taken in 1d vector
    // map <string, vector<int>> const dmap{
    //   {"Bernoulli", {1, long_double_type}},
    //   {"Beta", {4, long_double_type, long_double_type, long_double_type, long_double_type}},
    //   {"Beta Binomial", {3, long_type, long_double_type, long_double_type}},
    //   {"Binomial", {2, long_type, long_double_type}},
    //   {"Chi Squared", {1, long_double_type}},
    //   {"Constant", {1, long_double_type}},
    //   {"Dirac Delta", {1, long_double_type}},
    //   {"Dirichlet", {1, arr_type, long_double_type, template_type}},
    //   {"Dirichlet Multinomial", {2, arr_type, long_double_type, template_type, long_type}},
    //   {"Empirical", {1, vector_type, long_double_type}},
    //   {"Exponential", {2, long_double_type, long_double_type}},
    //   {"Gamma", {3, long_double_type, long_double_type, long_double_type}},
    //   {"Gamma Poisson", {2, long_double_type, long_double_type}},
    //   {"Geometric", {1, long_double_type}},
    //   {"Johnson Sb", {4, long_double_type, long_double_type, long_double_type, long_double_type}},
    //   {"Johnson Sl", {4, long_double_type, long_double_type, long_double_type, long_double_type}},
    //   {"Johnson Su", {4, long_double_type, long_double_type, long_double_type, long_double_type}},
    //   {"Kronecker Delta", {1, long_type}},
    //   {"Lognormal", {3, long_double_type, long_double_type, long_double_type}},
    //   {"Multinomial", {2, arr_type, long_type, template_type}},
    //   {"Neg Binomial", {2, long_type, long_double_type}},
    //   {"Normal", {2, long_double_type, long_double_type}},
    //   {"Poisson", {1, long_double_type}},
    //   {"Triangular", {3, long_double_type, long_double_type, long_double_type}},
    //   {"Uniform", {2, long_double_type, long_double_type}},
    //   {"Uniform Discrete", {2, long_type, long_type}},
    //   {"Weibull", {2, long_double_type, long_double_type}}
    // }

    if (jobj["distribution"] == "Bernoulli"){
        long double p1 = jobj["parameter-1"];
        Bernoulli d(p1);
        return GeneralStatDist(d);
    } else if (jobj["distribution"] == "Beta"){
        long double p1 = jobj["parameter-1"], p2 = jobj["parameter-2"], p3 = jobj["parameter-3"], p4 = jobj["parameter-4"];
        Beta d(p1, p2, p3, p4);
        return GeneralStatDist(d);
    } else if (jobj["distribution"] == "Constant"){
        long double p1 = jobj["parameter-1"];
        Constant d(p1);
        return GeneralStatDist(d);
    } else if (jobj["distribution"] == "Johnson Su"){

        long double p1 = jobj["parameter-1"], p2 = jobj["parameter-2"], p3 = jobj["parameter-3"], p4 = jobj["parameter-4"];
        // cout << "p1: " << p1 << "\n";
        // cout << "p2: " << p2 << "\n";
        // cout << "p3: " << p3 << "\n";
        // cout << "p4: " << p4 << "\n";
        // cout << "fabs(p4)" << fabs(p4)<< "\n";
        // cout << "fabs(p2)" << fabs(p2)<< "\n";
        // cout << normal_distribution<long double>(-pgamma / pdelta, 1 / pdelta);
        JohnsonSu d(p1, p2, p3, p4);
        // exit(1);
        return GeneralStatDist(d);
    } else if (jobj["distribution"] == "Weibull"){
      long double p1 = jobj["parameter-1"], p2 = jobj["parameter-2"];
        Weibull d(p1, p2);
        return GeneralStatDist(d);
    } 
    // string dname = jobj["distribution"];
    // map<string,vector<int>>::iterator it;
    // it = dmap.find(dname);
    // if (it != dmap.end()){ // valid distribution
    //   vector<int> v = it->second;
    //   sd = new 
    // } else{
    //   //return error
    // }
    Constant d(0);
    return GeneralStatDist(d);
  }
  // Takes JSON Object
  Param parameterize(json jobj){
    string d = jobj["description"];
    Type t;
    if (jobj["type"] == "v"){
      t = Type::val_type;
    } else 
    if (jobj["type"] == "f"){
      t = Type::file_type;
    } else
      t = Type::inv_type;
    //   Constant d(0);
    // return GeneralStatDist(d);
    //   Param p1{"d", Type::file_type, NULL, "d", false};
    //   return p1;

    // GeneralStatDist sd;
    string fname = "";
    // if (t == Type::val_type){
    //   sd = mapDistribution(jobj);
    // } else
    if (t == Type::file_type){
      fname = jobj["distribution"];
    }

    bool cal = false;

    if (!jobj["included-in-calibration"].is_null()){
        cal = jobj["included-in-calibration"];
    }

    Param p {d, t, GeneralStatDist(mapDistribution(jobj)), fname, cal};
    return p;
  }

  // Takes string contianing filename
  Param parameterize(string fname){
    return parameterize(JSONImport::fileToJSON(fname));
  }

 }