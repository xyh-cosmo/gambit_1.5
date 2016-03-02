//   GAMBIT: Global and Modular BSM Inference Tool
//   *********************************************
///  \file
///
///  Various simple likelihood functions.
///
///  *********************************************
///
///  Authors (add name and date if you modify):
///   
///  \author Christoph Weniger
///          (c.weniger@uva.nl)
///  \date 2013 Jul - 2015 May
///
///  *********************************************

#include "gambit/Elements/gambit_module_headers.hpp"
#include "gambit/Utils/statistics.hpp"
#include "gambit/DarkBit/DarkBit_rollcall.hpp"

namespace Gambit {
  namespace DarkBit {

    //////////////////////////////////////////////////////////////////////////
    //
    //                    Simple likelihood functions
    //
    //////////////////////////////////////////////////////////////////////////

    /*! \brief Fermi LAT dwarf likelihoods, based on arXiv:1108.2914.
    */
    void lnL_FermiLATdwarfsSimple(double &result)
    {
      using namespace Pipes::lnL_FermiLATdwarfsSimple;
      // Koushiappas' limits [arXiv:1108.2914]
      //
      // This is the tabulated Phi-Likelihood function from Koushiappas et al.
      // Above L = 36, we use linear extrapolation up to L = 360000
      //
      // phi (defined as phi = sigmav/mDM**2*Ntot/8/pi * 1e26)
      double xgridArray [101] = { 0. , 6.74308086122e-05 , 0.000123192463137 , 
        0.000171713798503 , 0.000215245918518 , 0.000255093268618 , 0.00029207805123 ,
        0.000326751732695 , 0.000359503469472 , 0.000390620122006 , 0.000420321264006,
        0.00044878042576 , 0.000476138421008 , 0.000502511975672 , 0.000527999496499,
        0.000552685056887 , 0.000576641243501 , 0.000599931255273 ,
        0.000622610497068 , 0.000644727821172 , 0.000666326515638 , 0.000687445105269,
        0.000708118010141 , 0.000728376093388 , 0.000748247120993 , 0.00076775615078,
        0.000786925863514 , 0.000805776846231 , 0.000824327835809 , 0.00084259592922,
        0.000860596765645 , 0.000878344684789 , 0.000895852864914 ,
        0.000913133443547 , 0.000930197623331 , 0.0009470557651 , 0.000963717469925 ,
        0.00098019165163 , 0.000996486601006 , 0.00101261004288 , 0.00102856918685 ,
        0.00104437077256 , 0.00106002111016 , 0.00107552611658 , 0.00109089134805 ,
        0.00110612202935 , 0.00112122308019 , 0.00113619913897 , 0.00115105458439 ,
        0.00116579355487 , 0.00118041996631 , 0.00119493752815 , 0.00120934975806 ,
        0.00122365999528 , 0.00123787141289 , 0.00125198702892 , 0.00126600971667 ,
        0.00127994221404 , 0.00129378713223 , 0.00130754696367 , 0.00132122408935 ,
        0.00133482078559 , 0.00134833923028 , 0.00136178150869 , 0.0013751496188 ,
        0.00138844547626 , 0.00140167091906 , 0.00141482771173 , 0.00142791754942 ,
        0.00144094206154 , 0.0014539028153 , 0.00146680131887 , 0.00147963902447 ,
        0.00149241733116 , 0.00150513758749 , 0.00151780109399 , 0.00153040910553 ,
        0.00154296283341 , 0.00155546344754 , 0.00156791207827 , 0.00158030981824 ,
        0.00159265772411 , 0.00160495681814 , 0.00161720808976 , 0.00162941249692 ,
        0.00164157096757 , 0.00165368440081 , 0.00166575366823 , 0.00167777961494 ,
        0.00168976306076 , 0.00170170480119 , 0.00171360560841 , 0.00172546623219 ,
        0.00173728740083 , 0.00174906982191 , 0.00176081418314 , 0.00177252115315 ,
        0.00178419138212 , 0.00179582550256 , 0.00180742412988 , 18.0 };
      //
      // Normalization w.r.t. p-value of phi=0
      //
      // chi^2
      double ygridArray [101] = { 0.0, 
        0.0513551, 0.177438, 0.35228, 0.561353, 0.795726, 1.04953, 1.3187, 1.60032, 
        1.89222, 2.19274, 2.50059, 2.81476, 3.13441, 3.45887, 3.78757, 4.12006,
        4.45594, 4.79486, 5.13653, 5.48072, 5.82719, 6.17576, 6.52625, 6.87853,
        7.23244, 7.58789, 7.94475, 8.30294, 8.66236, 9.02294, 9.38462, 9.74731,
        10.111, 10.4755, 10.841, 11.2072, 11.5742, 11.9419, 12.3104, 12.6795, 13.0492,
        13.4195, 13.7904, 14.1619, 14.5339, 14.9063, 15.2793, 15.6527, 16.0266,
        16.4008, 16.7755, 17.1506, 17.5261, 17.9019, 18.2781, 18.6546, 19.0315,
        19.4087, 19.7861, 20.1639, 20.542, 20.9203, 21.2989, 21.6778, 22.0569,
        22.4362, 22.8158, 23.1957, 23.5757, 23.956, 24.3365, 24.7171, 25.098, 25.4791,
        25.8604, 26.2418, 26.6235, 27.0053, 27.3872, 27.7694, 28.1517, 28.5342,
        28.9168, 29.2996, 29.6825, 30.0655, 30.4487, 30.8321, 31.2155, 31.5992,
        31.9829, 32.3667, 32.7507, 33.1348, 33.519, 33.9034, 34.2878, 34.6724,
        35.0571, 350000.0 };
      // Convert arrays to vectors.
      std::vector<double> xgrid(xgridArray,
          xgridArray + sizeof xgridArray / sizeof xgridArray[0]);
      std::vector<double> ygrid(ygridArray,
          ygridArray + sizeof ygridArray / sizeof ygridArray[0]);
      // Construct interpolated function, using GAMBIT base functions.
      auto dwarf_likelihood = Funk::interp("phi", xgrid, ygrid);

      double fraction = *Dep::RD_fraction;

      // Integate spectrum 
      // (the zero velocity limit of the differential annihilation
      // cross-section as function of individual final state photons)
      // FIXME: Clean this up?
      //std::ofstream os;
      //os.open("test.dat");
      //(*Dep::GA_AnnYield)->writeToFile(Funk::logspace(-1., 5., 10000), os);
      //os.close();
      // TODO: Make this take ->set_epsrel(1e-3)
      double AnnYieldint = (*Dep::GA_AnnYield)->
        set("v", 0.)->gsl_integration("E", 1, 100)->bind()->eval();
      logger() << "AnnYieldInt (1-100 GeV): " << AnnYieldint << std::endl;

      // Calculate phi-value
      double phi = AnnYieldint / 8. / M_PI * 1e26 * fraction * fraction;

      // And return final likelihood
      result = 0.5*dwarf_likelihood->bind("phi")->eval(phi);
      logger() << "dwarf_likelihood: " << result << std::endl;
      logger() << "phi: " << phi << std::endl;
    }

    /*! \brief Fermi LAT dwarf likelihoods, using gamLike backend.
    */
    void lnL_FermiLATdwarfs_gamLike(double &result)
    {
      using namespace Pipes::lnL_FermiLATdwarfs_gamLike;

      double fraction = *Dep::RD_fraction;
      result = 0;

      // from 0.1 to 500 GeV
      std::vector<double> x = Funk::logspace(-1, 2.698, 100);
      std::vector<double> y = ((*Dep::GA_AnnYield)/8./M_PI*fraction*fraction)->
        set("v", 0)->bind("E")->vect(x);

      result += BEreq::lnL(1, x, y);

      logger() << "GamLike likelihood is lnL = " << result << std::endl;
    }

    /*! \brief Fermi LAT galactic center likelihoods, using gamLike backend.
    */
    void lnL_FermiGC_gamLike(double &result)
    {
      using namespace Pipes::lnL_FermiGC_gamLike;

      double fraction = *Dep::RD_fraction;
      result = 0;

      // from 0.1 to 500 GeV
      std::vector<double> x = Funk::logspace(-1, 2.698, 100);
      std::vector<double> y = ((*Dep::GA_AnnYield)/8./M_PI*fraction*fraction)->
        set("v", 0)->bind("E")->vect(x);

      result += BEreq::lnL(2, x, y);

      logger() << "GamLike likelihood is lnL = " << result << std::endl;
    }

    /// \brief Likelihood for cosmological relic density constraints.
    /// Default data: 
    ///   Omega_c h^2 = 0.1188 +/- 0.0010 (1 sigma), Gaussian.  Planck TT,TE,EE+lowP+lensing+ext 2015, arxiv:1502.01589v2
    ///   theory error: 5% 
    void lnL_oh2_Simple(double &result)
    {
      using namespace Pipes::lnL_oh2_Simple;
      double oh2_theory = *Dep::RD_oh2;
      double oh2_theoryerr = oh2_theory*runOptions->getValueOrDef<double>(0.05, "oh2_fractional_theory_err");
      double oh2_obs = runOptions->getValueOrDef<double>(0.1188, "oh2_obs");
      double oh2_obserr  = runOptions->getValueOrDef<double>(0.001, "oh2_obserr");
      result = Stats::gaussian_loglikelihood(oh2_theory, oh2_obs, oh2_theoryerr, oh2_obserr);
      logger() << "lnL_oh2_Simple yields " << result << std::endl;
    }

    /// \brief Likelihood for cosmological relic density constraints, implemented as an upper limit only
    /// Default data: 
    ///   Omega_c h^2 = 0.1188 +/- 0.0010 (1 sigma), Gaussian.  Planck TT,TE,EE+lowP+lensing+ext 2015, arxiv:1502.01589v2
    ///   theory error: 5% 
    void lnL_oh2_upperlimit(double &result)
    {
      using namespace Pipes::lnL_oh2_upperlimit;
      double oh2_theory = *Dep::RD_oh2;
      double oh2_theoryerr = oh2_theory*runOptions->getValueOrDef<double>(0.05, "oh2_fractional_theory_err");
      double oh2_obs = runOptions->getValueOrDef<double>(0.1188, "oh2_obs");
      double oh2_obserr  = runOptions->getValueOrDef<double>(0.001, "oh2_obserr");
      result = Stats::detection_as_upper_limit(oh2_theory, oh2_obs, oh2_theoryerr, oh2_obserr,
                                               runOptions->getValueOrDef<str>("simple", "limit_method"));
      logger() << "lnL_oh2_upperlimit yields " << result << std::endl;
    }


    /// \brief Likelihoods for spin independent nuclear parameters. Follows treatment
    /// of Cline, et. al. Phys. Rev. D. 88, 055025 (2013)
    /// Default data:
    ///  sigma_s = 43 +/- 8 MeV arXiv:1112.2435v1
    ///  sigma_s < 70 MeV arXiv:1301.1765 -- Top-hat function not yet implemented
    ///  sigma_l = 58 +/- 9 MeV

    void lnL_sigmas_sigmal(double &result)
    {
        using namespace Pipes::lnL_sigmas_sigmal;
        double sigmas = *Param["sigmas"];
        double sigmal = *Param["sigmal"];
        double sigmas_central = runOptions->getValueOrDef<double>(43., "sigmas_central");
        double sigmas_error = runOptions->getValueOrDef<double>(8., "sigmas_error");
        double sigmal_central = runOptions->getValueOrDef<double>(58., "sigmal_central");
        double sigmal_error = runOptions->getValueOrDef<double>(9., "sigmal_error");

        result = Stats::gaussian_loglikelihood(sigmas, sigmas_central, 0, sigmas_error)
            + Stats::gaussian_loglikelihood(sigmal, sigmal_central, 0, sigmal_error);
        logger() << "lnL for SI nuclear parameters is " << result << EOM;
    }

    /// \brief Likelihoods for halo parameters. The likelihood for the local DM density follows a
    /// log normal distribution while for the velocities the distribution is Gaussian.
    /// For discussion of the default values for measured halo paramters and their errors,
    /// see JCAP04(2011)012.

    void lnL_rho0_lognormal(double &result)
    {
        using namespace Pipes::lnL_rho0_lognormal;
        double rho0 = *Param["rho0"];
        double rho0_obs = runOptions->getValueOrDef<double>(.4, "rho0_obs");
        double rho0_obserror = runOptions->getValueOrDef<double>(.15, "rho0_obserr");

        result = Stats::lognormal_loglikelihood(rho0, rho0_obs, 0.,
                rho0_obserror);
        logger() << "lnL_rho0 yields " << result << EOM;
    }

    void lnL_vrot_gaussian(double &result)
    {
      using namespace Pipes::lnL_vrot_gaussian;
      double vrot = *Param["vrot"];
      double vrot_obs = runOptions->getValueOrDef<double>(235, "vrot_obs");
      double vrot_obserr  = runOptions->getValueOrDef<double>(20, "vrot_obserr");
      result = Stats::gaussian_loglikelihood(vrot, vrot_obs, 0., vrot_obserr);
      logger() << "lnL_vrot yields " << result << EOM;
    }

    void lnL_v0_gaussian(double &result)
    {
      using namespace Pipes::lnL_v0_gaussian;
      double v0 = *Param["v0"];
      double v0_obs = runOptions->getValueOrDef<double>(235, "v0_obs");
      double v0_obserr  = runOptions->getValueOrDef<double>(20, "v0_obserr");
      result = Stats::gaussian_loglikelihood(v0, v0_obs, 0., v0_obserr);
      logger() << "lnL_v0 yields " << result << EOM;
    }

    void lnL_vesc_gaussian(double &result)
    {
      using namespace Pipes::lnL_vesc_gaussian;
      double vesc = *Param["vesc"];
      double vesc_obs = runOptions->getValueOrDef<double>(550, "vesc_obs");
      double vesc_obserr  = runOptions->getValueOrDef<double>(35, "vesc_obserr");
      result = Stats::gaussian_loglikelihood(vesc, vesc_obs, 0., vesc_obserr);
      logger() << "lnL_vesc yields " << result << EOM;
    }

    /*! \brief Helper function to dump gamma-ray spectra.
     *
     * NOTE: DEPRECATED!! (replaced by UnitTest)
     * TODO: Delete
     */
    void dump_GammaSpectrum(double &result)
    {
      using namespace Pipes::dump_GammaSpectrum;
      // Construct interpolated function, using GAMBIT base functions.
      Funk::Funk spectrum = (*Dep::GA_AnnYield)->set("v", 0.);
      std::string filename = runOptions->getValueOrDef<std::string>(
          "dNdE.dat", "filename");
      logger() << "FILENAME for gamma dump: " << filename << std::endl;
      std::ofstream myfile (filename);
      if (myfile.is_open())
      {
        for (int i = 0; i<=1000; i++)
        {
          double energy = pow(10., i/200. - 2.);

          myfile << energy << " " << spectrum->bind("E")->eval(energy) << "\n";
        }
        myfile.close();
      }
      result = 0.;
    }
  }
}
