//   GAMBIT: Global and Modular BSM Inference Tool
//   *********************************************
///  \file
///
///  Frontend header for the MultiModeCode backend.
///
///  Compile-time registration of available
///  functions and variables from this backend.
///
///  *********************************************
///
///  Authors (add name and date if you modify):
///
///  \author Selim C. Hotinli
///          (selim.hotinli14@pimperial.ac.uk)
///  \date 2017 Jul
///
///  *********************************************

#define BACKENDNAME MultiModeCode
#define BACKENDLANG FORTRAN
#define VERSION 2.0.0
#define SAFE_VERSION 2_0_0

// Load it
LOAD_LIBRARY

BE_FUNCTION(multimodecode_gambit_driver, gambit_inflation_observables,
						(int&  ,int& ,  int& , int& , double* , double* ,double* ,
						 double&, double&, double& , int& , double& ,
						 double& , int& , int& ),
						"__multimodecode_gambit_MOD_multimodecode_gambit_driver", "multimodecode_gambit_driver")

BE_INI_FUNCTION{}
END_BE_INI_FUNCTION

// Undefine macros to avoid conflict with other backends
#include "gambit/Backends/backend_undefs.hpp"
