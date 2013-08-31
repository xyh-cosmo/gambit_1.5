//   GAMBIT: Global and Modular BSM Inference Tool
//   *********************************************
///  \file
///
///  Functions of module ExampleBit_A.
///
///  Put your functions in files like this
///  if you wish to add observables or likelihoods
///  to this module.
///
///  *********************************************
///
///  Authors (add name and date if you modify):
///
///  \author Pat Scott
///    \date 2012 Nov
///    \date 2013 Jan, Feb, May 
///
///  \author Christoph Weniger
///    \date 2013 Jan 
///
///  *********************************************

#include <string>
#include <iostream>
#include <math.h>

#include "gambit_module_headers.hpp"
#include "ExampleBit_A_rollcall.hpp"

namespace GAMBIT {

  namespace ExampleBit_A {

    // Some local module codes and declarations
    double count = 3.5;
    double some_other_function(int &input)
    {
      std::cout << "  This is some_other_function, invoked with argument " << input << std::endl;
      return input * 2.0;
    }


    // Initialization routine
    void initialize ()
    {
      std::cout << std::endl;
      std::cout << "********************************************" << std::endl;
      std::cout << "***       Initializing ExampleBit_A      ***" << std::endl;
      std::cout << "********************************************" << std::endl;
    }


    // Module functions
    void nevents_dbl  (double &result)    { result = count++; }
    void nevents_int  (int    &result)    { result = (int) (*SafePointers::nevents_int::Dep::nevents); }
    void nevents_like (double &result)    { result = 1.5; }
    void identity     (str    &result)    { result = "turkion"; }
    void function_pointer_retriever( double(*&result)(int&) )
    {
      //Two ways to try this: a pointer to a fortran function that has been backended (and takes an int as an input by reference):
      result = GET_BE_POINTER(function_pointer_retriever::externalFunction, int&);
      //or a pointer to a local C++ funtion
      //result = &some_other_function;
    }

    // Example of interacting with models
    void damu (double &result)
    {
      using namespace SafePointers::damu;
      // Put these in a map or some such automatically?
      double p1 = Param::test_parent_I->getValue("p1");
      double p2 = Param::test_parent_I->getValue("p2");
      double p3 = Param::test_parent_I->getValue("p3");
      
      std::cout << "In ExampleBit_A, function damu" << std::endl;
      std::cout << "  test_parent_parameters resolved successfully!" << std::endl;
      std::cout << "  Printing values:" << std::endl;
      Param::test_parent_I->print();
      
    }
    
    // Helper function: not wrapped in rollcall header
    // (un-normalised gaussian log-likelihood)
    double logf (double x, double mu, double sig)
      {
        return pow(x-mu, 2) / (2*pow(sig, 2));
      }
      
    // Likelihood function for fitting the population parameters of a
    // normal distribution (with hard-coded "observations")
    void normaldist_loglike (double &result)
    {
      using namespace SafePointers::normaldist_loglike;
      const ModelParameters &p = *Param::NormalDist_I;
      double mu    = p["mu"];
      double sigma = p["sigma"];
      
      //double muTrue = 20;   // Actually these are irrelevant since I just
      //double sigmaTrue = 3; // made up the data rather than sampling.
      
      // Say we have a sample of 20 drawn from a normal distribution with
      // parameters muTrue and sigmaTrue. Let the sample mean and standard
      // deviation be as follows (this is our data):
      double N = 20;
      double samples [] = { 
        21.32034213,  20.39713359,  19.27957134,  19.81839231,
        20.89474358,  20.11058756,  22.38214557,  21.41479798,
        23.49896999,  17.55991187,  24.9921142 ,  23.90166585,
        20.97913273,  18.59180551,  23.49038072,  19.08201714,
        21.19538797,  16.42544039,  18.93568891,  22.40925288 
        };
    
      double loglTotal;
      
      // The loglikelihood value for the hypothesised parameters is then:
      for (int i=0; i <= N; ++i)
      {
        //std::cout<<samples[i]<<mu<<sigma<<std::endl;
        loglTotal += logf(samples[i], mu, sigma);
      }
      
      result = loglTotal;
    }  

  }

}
