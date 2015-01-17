//Printer.h

#ifndef _PRINTER_H
#define _PRINTER_H

#include "GridStag.h"
#include <fstream>
#include <iostream>
#include <boost/numeric/ublas/matrix.hpp>
using namespace boost::numeric::ublas;

class Printer{
public:
	GridStag* sGrid;
	void init(GridStag* );
	void matrices(boost::numeric::ublas::matrix<double> );
	void toFile( boost::numeric::ublas::matrix<double>,  char* fname,char,char);
	void toFile( char* msg,  char* fname,char);
	void msg(char* );
};


#endif

