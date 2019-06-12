//   GAMBIT: Global and Modular BSM Inference Tool
//   *********************************************
///  \file
///
///  Source code for types for module CosmoBit.
///  For instructions on adding new types, see
///  the corresponding header.
///
///  *********************************************
///
///  Authors (add name and date if you modify):
///
///  \author Patrick Stoecker
///          (stoecker@physik.rwth-aachen.de)
///  \date 2017 Nov
///
///  \author Selim Hotinli
///	 \date 2018 Jan
///
///  \author Janina Renk
///          (janina.renk@fysik.su.se)
///  \date 2018 Oct
///  *********************************************

#include <string>
#include <iostream>
#include <stdlib.h>     /* malloc, free, rand */
#include <valarray>

#include "gambit/CosmoBit/CosmoBit_types.hpp"
#include "gambit/Utils/numerical_constants.hpp"

namespace Gambit
{
  namespace CosmoBit
  {

    BBN_container::BBN_container()
    { 
      // maps elements to their position in 'ratioH' array in AlterBBN holding 
      // primordial element abundances relative to H abundance
      abund_map["H2"] = 3;
      abund_map["D"] = 3;
      abund_map["H3"] = 4;
      abund_map["He3"] = 5;
      abund_map["He4"] = 6;   
      abund_map["Yp"] = 6;
      abund_map["Li6"] = 7;
      abund_map["Li7"] = 8;
      abund_map["Be7"] = 9;
      abund_map["Li8"] = 10;
    }

    void BBN_container::init_arr(int nnuc)
    {
      NNUC = nnuc;
      BBN_abund.resize(NNUC+1, 0.);
      BBN_covmat.resize(NNUC+1, std::vector<double>(NNUC+1,0.));
    }
    
    MPLike_data_container::MPLike_data_container(){}
    MPLike_data_container::MPLike_data_container(pybind11::object &data_in, map_str_pyobj likelihoods_in): data(data_in), likelihoods(likelihoods_in){}

    void Classy_cosmo_container::set_input_dict(pybind11::dict input_dict) {cosmo_input_dict = input_dict;}

    SM_time_evo::SM_time_evo(double t0, double tf, double N_t) : grid_size(N_t), t_grid(N_t), T_evo(N_t), Tnu_evo(N_t), H_evo(N_t), H_int(N_t)
    {
      
      // check if implemented routines are valid for given initial time
      if(t0 < 1e3) 
      {
        std::ostringstream err;
        err << "Requested initial time for evolution of Temperature & Hubble rate for SM for Temperatures t_initial was "<< t0<<". Implemented routines are only valid after e+/- annihilation (t > 10^3). Aborting now.";
        CosmoBit_error().raise(LOCAL_INFO, err.str());
      }

      // initialize time grid in log space
      double Delta_logt = (log(tf) - log(t0))/(grid_size-1);      
      for (int jj = 0; jj<grid_size; ++jj) 
      {
           t_grid[jj] = exp(log(t0) + jj*Delta_logt);
      }
      double Neff_SM = 3.046;
      double g_star_SM = 2.+2.*7./8.*Neff_SM*pow(4./11.,4./3.); // contribution from photons & neutrinos with Neff = 3.046
      
      // factor needed to calculate temperature evolution. For details see definition of functions set_T_evo(),.. in CosmoBit_types.hpp header
      factor_T_evo = 1./sqrt(2.*sqrt(8.*pi*pi*pi*_GN_SI_ *pow(_kB_SI_,4.)*g_star_SM/90./_c_SI_/_c_SI_/pow(_hP_SI_/2./pi*_c_SI_,3.)))*_kB_eV_over_K_/1e3;
      // TODO: different from T_nu/T_gamma from nu oscillation results?
      factor_Tnu_evo = pow(Neff_SM/3.,1./4.)* pow(4./11.,1./3.)*factor_T_evo; // = T_nu/T_gamma * factor_T_evo
      factor_HT_evo = sqrt(8.*pi*_GN_SI_/3.*pi*pi/30.*g_star_SM/pow(_hP_SI_/2./pi*_c_SI_,3.))*(1e6*_eV_to_J_*_eV_to_J_)/_c_SI_;

      set_T_evo();
      set_Tnu_evo();
      set_Ht_evo();
    }


    void SM_time_evo::calc_H_int()
    {
      /*
      This calculates int(y(x') dx', {x', x0, x}) for each x in x_grid (with x0 = x_grid[0]), using a simple trapezoidal integration.
      This function explicitly assumes that the logarithms of the values in x_grid are equally spaced.
      This is very simplified and designed to work fast in case of the Hubble rate. Can go wrong with other functions, so it should
      really only be used in this context (or if you exactly know what you are doing..).
      */
      std::valarray<double> g_grid(grid_size);
      double Delta_z = (log(t_grid[grid_size-1]) - log(t_grid[0]))/(grid_size - 1);
      
      g_grid = t_grid*H_evo;
      
      H_int[0] = 0.0;
      H_int[1] = 0.5*Delta_z*(g_grid[0] + g_grid[1]);
      
      double cumsum = g_grid[1];
      for (int i = 2; i<grid_size; ++i) 
      {
          H_int[i] = 0.5*Delta_z*(g_grid[0] + g_grid[i] + 2.0*cumsum);
          cumsum = cumsum + g_grid[i];
      }
    }


    Class_container::Class_container() : lmax(2508)
    {
      input.clear();
      Cl_TT.resize(lmax+1, 0.);
      Cl_TE.resize(lmax+1, 0.);
      Cl_EE.resize(lmax+1, 0.);
      Cl_BB.resize(lmax+1, 0.);
      Cl_PhiPhi.resize(lmax+1, 0.);
      //std::cout << "Hello it's me. I am a Class_container" << std::endl;

      Pk_S.resize(lmax+1, 0.); // Primordial Scalar Power Spectrum
      Pk_T.resize(lmax+1, 0.); // Primordial Tensor Power Spectrum
      k_ar.resize(lmax+1, 0.); // Corresponding wavenumbers.
    }
/*
    Class_container::~Class_container()
    {
      //std::cout << "And I am out" << std::endl;
    }
*/
    void ClassInput::addEntry(std::string key, std::string val)
    {
      input_list[key] = val;
    }

    void ClassInput::addEntry(std::string key, double val)
    {
      std::ostringstream stringified_val;
      stringified_val << val;
      addEntry(key,stringified_val.str());
    }

    void ClassInput::addEntry(std::string key, int val)
    {
      std::ostringstream stringified_val;
      stringified_val << val;
      addEntry(key,stringified_val.str());
    }

    void ClassInput::addEntry(std::string key, std::vector<double> val)
    {
      std::ostringstream stringified_val;
      bool first = true;
      for (auto it = val.begin(); it != val.end(); it++)
      {
	if (not first)
	  stringified_val << ", ";
	stringified_val << *it;
	first = false;
      }
      addEntry(key,stringified_val.str());
    }

    void ClassInput::addEntry(std::string key, std::vector<int> val)
    {
      std::ostringstream stringified_val;
      bool first = true;
      for (auto it = val.begin(); it != val.end(); it++)
      {
	if (not first)
	  stringified_val << ", ";
	stringified_val << *it;
	first = false;
      }
      addEntry(key,stringified_val.str());
    }

    std::string ClassInput::print_entries_to_logger()
    {
      using namespace LogTags;
      std::ostringstream log_msg;
      log_msg << "Parameters passed to class: \n \n";
      std::map<std::string,std::string> in_map = get_map();
      int len_of_input = in_map.size();
      for(auto iter=in_map.begin(); iter != in_map.end(); iter++)
      {
        log_msg << iter->first.c_str() << " = " << iter->second.c_str() << " \n"; // uncomment if you want class input to be printed in terminal before class call 
      }
      return log_msg.str();
    }

    void ClassInput::clear()
    {
      input_list.clear();
    }

    std::map<std::string,std::string> ClassInput::get_map()
    {
      return input_list;
    }

  }
}
