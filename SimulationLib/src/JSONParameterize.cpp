#include "../include/SimulationLib/JSONParameterize.h"
#include <fstream>
#include <cmath>
namespace SimulationLib {

  GeneralStatDist mapDistribution(json jobj){

    if (jobj.find("distribution") == jobj.end() && jobj.find("Distribution") == jobj.end()){
        // no distibution found
        long double p1 = 0;
        for (auto it = jobj.begin(); it != jobj.end(); ++it)
        {
            if (it.key() != "Year" && it.key() != "Sex" && it.key() != "Age"){
                p1 = it.value();
            }
        }
        Constant d(p1);
        return GeneralStatDist(d);
    }

    // Check and assign correct Distribution based on string
    if (jobj["distribution"] == "Bernoulli"){
        long double p1 = jobj["parameter-1"];
        Bernoulli d(p1);
        return GeneralStatDist(d);
    } else if (jobj["distribution"] == "Beta"){
        long double p1 = jobj["parameter-1"], p2 = jobj["parameter-2"], p3 = jobj["parameter-3"], p4 = jobj["parameter-4"];
        Beta d(p1, p2, p3, p4);
        return GeneralStatDist(d);
    } else if (jobj["distribution"] == "Beta Binomial"){
        long p1 = jobj["parameter-1"];
        long double p2 = jobj["parameter-2"], p3 = jobj["parameter-3"];
        BetaBinomial d(p1, p2, p3);
        return GeneralStatDist(d);
    } else if (jobj["distribution"] == "Binomial"){
        long p1 = jobj["parameter-1"];
        long double p2 = jobj["parameter-2"];
        Binomial d(p1, p2);
        return GeneralStatDist(d);
    } else if (jobj["distribution"] == "Constant"){
        long double p1 = jobj["parameter-1"];
        Constant d(p1);
        return GeneralStatDist(d);
    } else if (jobj["distribution"] == "Johnson Su"){
        long double p1 = jobj["parameter-1"], p2 = jobj["parameter-2"], p3 = jobj["parameter-3"], p4 = jobj["parameter-4"];
        JohnsonSu d(p1, p2, p3, p4);
        return GeneralStatDist(d);
    } else if (jobj["distribution"] == "Uniform"){
        long double p1 = jobj["parameter-1"], p2 = jobj["parameter-2"];
        Uniform d(p1, p2);
        return GeneralStatDist(d);
    } else if (jobj["distribution"] == "Weibull"){
      long double p1 = jobj["parameter-1"], p2 = jobj["parameter-2"];
        Weibull d(p1, p2);
        return GeneralStatDist(d);
    } else if (jobj["distribution"] == "Exponential") {
      long double p1 = jobj["parameter-1"], p2 = jobj["parameter-2"];
      Exponential d(p1, p2);
      return GeneralStatDist(d);
    } else {
        printf("Warning: No matching distribution");
    }

    Constant d(0);
    // Default is to return constant distribution containing 0.
    return GeneralStatDist(d);
  }
  // Takes JSON Object

  DataFrame createDataFrame(json jobj){
    double year = jobj["year"];
    int sex = 0;
    if (jobj["sex"] == "M"){
        sex = 0;
    } else if (jobj["sex"] == "F"){
        sex = 1;
    } else{
        // error invalid sex
    }
    
    double age = jobj["age"];
    DataFrame d {year, sex, age, GeneralStatDist(mapDistribution(jobj))};
    return d;
}
    

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

    string fname = "";

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


  void mapShortNames(json j, map<string, Param>& m){
    json::iterator it;

    for (it = j.begin(); it != j.end(); ++it){
      m[(*it)["short-name"]] = parameterize(*it);
    }
  }

 }