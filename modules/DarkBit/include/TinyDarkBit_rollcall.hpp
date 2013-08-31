//  GAMBIT: Global and Modular BSM Inference Tool
//  *********************************************
//
//  Rollcall header for module TinyDarkBit
//
//  Compile-time registration of available obser-
//  vables and likelihoods, as well as their
//  dependencies.
//
//  Add to this if you want to add an observable
//  or likelihood to this module.
//
//  *********************************************
//
//  Authors
//  =======
//
//  (add name and date if you modify)
//
//  Pat Scott
//  2013 Jan, Feb 04
//
//  Christoph Weniger (c.weniger@uva.nl)
//  2013 Jan, Jun
//  *********************************************

#ifndef __TinyDarkBit_rollcall_hpp__
#define __TinyDarkBit_rollcall_hpp__

#include <types_DarkBit.hpp>
#include <MSSM_classes.hpp> // Ben: your "DS_MSSMPAR" struct is now in this file (under ModelBit/include)

#define MODULE TinyDarkBit
START_MODULE

  #define CAPABILITY initialise
  START_CAPABILITY
    #define FUNCTION initDS
    START_FUNCTION(int)
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY CMSSM_definition
  START_CAPABILITY 
    #define FUNCTION CMSSM_definition 
    START_FUNCTION(double)
    DEPENDENCY(m0, double)
    DEPENDENCY(m1, double)
    #undef FUNCTION
  #undef CAPABILITY
  
  #define CAPABILITY SLHA
  START_CAPABILITY 
    #define FUNCTION SLHA
    START_FUNCTION(double)
    DEPENDENCY(CMSSM_definition, double)
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY Wstruct
  START_CAPABILITY 
    #define FUNCTION Wstruct
    START_FUNCTION(GAMBIT::types::Wstruct)
    DEPENDENCY(SLHA, double) 
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY Weff
  START_CAPABILITY 
    #define FUNCTION Weff
    START_FUNCTION(double)
    DEPENDENCY(SLHA, double) 
      #define BACKEND_REQ LibFirst_returnResult_capability
      START_BACKEND_REQ(double)
      BACKEND_OPTION(LibFirst, 1.0)
      #undef BACKEND_REQ
    #undef FUNCTION
    #define FUNCTION Weff_alt1
    START_FUNCTION(double)
    DEPENDENCY(SLHA, double) 
    DEPENDENCY(m0, double) 
    DEPENDENCY(m1, double) 
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY omega_DM
  START_CAPABILITY 
    #define FUNCTION omega_DM
    START_FUNCTION(double)
    DEPENDENCY(Weff, double)
    DEPENDENCY(Wstruct, GAMBIT::types::Wstruct)
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY dssusy
  START_CAPABILITY
    #define FUNCTION dssusy
    START_FUNCTION(int)
    DEPENDENCY(m1, double)
    DEPENDENCY(m2, double)
    DEPENDENCY(m3, double)
      #define BACKEND_REQ dsinit
      START_BACKEND_REQ(void)
      BACKEND_OPTION(DarkSUSY, 0.1)
      #undef BACKEND_REQ
      #define BACKEND_REQ dssusy
      START_BACKEND_REQ(void)
      BACKEND_OPTION(DarkSUSY, 0.1)
      #undef BACKEND_REQ
      #define BACKEND_REQ DarkSUSY_setmssmpar_capability
      START_BACKEND_REQ(void)
      BACKEND_OPTION(DarkSUSY, 0.1)
      #undef BACKEND_REQ
      #define BACKEND_REQ DarkSUSY_getmssmpar_capability
      START_BACKEND_REQ(DS_MSSMPAR)
      BACKEND_OPTION(DarkSUSY, 0.1)
      #undef BACKEND_REQ
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY dsrdomega
  START_CAPABILITY
    #define FUNCTION dsrdomega
    START_FUNCTION(double)
    DEPENDENCY(dssusy, int)
      #define BACKEND_REQ dsrdomega
      START_BACKEND_REQ(double)
      BACKEND_OPTION(DarkSUSY, 0.1)
      #undef BACKEND_REQ
    #undef FUNCTION
  #undef CAPABILITY

#undef MODULE

#endif /* defined(__TinyDarkBit_rollcall_hpp__) */

