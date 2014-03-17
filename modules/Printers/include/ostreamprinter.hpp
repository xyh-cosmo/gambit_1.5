
//   GAMBIT: Global and Modular BSM Inference Tool
//   *********************************************
///  \file
///
///  ostream printer class declaration.
///
///  *********************************************
///
///  Authors (add name and date if you modify):
///   
///  \author Ben Farmer
///          (benjamin.farmer@monash.edu.au)
///  \date 2013 Jul, Sep, 2014 Jan
///
///  *********************************************


#ifndef __ostream_printer_hpp__
#define __ostream_printer_hpp__

// Standard libraries
#include <vector>
#include <sstream>
#include <fstream>
#include <iomanip>


// Gambit
#include "baseprinter.hpp"

// Code!
namespace Gambit
{

  namespace Printers 
  {

    // Printer to ostream
    class ostreamPrinter : public BasePrinter
    {
      public:
        // Default constructor: sets my_ostream to std::cout and verbose to True
        ostreamPrinter();
  
        // Constructor
        ostreamPrinter(std::ostream&, bool);
   
        // default destructor should be fine?
 
        // Initialisation function
        // Run by dependency resolver, which supplies the functors with a vector of VertexIDs whose requiresPrinting flags are set to true.
        // (currently does nothing for ostream printer)
        void initialise(const std::vector<int>&);
 
        // Tell printer that we have moved to a new model point
        void endline();
         
        // PRINT FUNCTIONS
        //----------------------------
        // Need to define one of these for every type we want to print!
        // Could use macros again to generate identical print functions 
        // for all types that have a << operator already defined.
        void print(double const&,              const functor*);
        void print(std::vector<double> const&, const functor*);
        void print(int const&,                 const functor*);
        void print(std::vector<int> const&,    const functor*);
        void print(ModelParameters const&,     const functor*);
      
      private:
        std::ostream& my_ostream;
        bool verbose;
    };
    
  } // end namespace printers
} // end namespace Gambit

#endif //ifndef __ostream_printer_hpp__
