//Printer.cpp
#include "Printer.h"

void Printer :: init(GridStag* sGrid)
{
	this->sGrid = sGrid;
}

void Printer :: matrices(matrix<double> mat)
{
	std::cout<<std::endl;	
	for( int i = (int)(mat.size1()-1);i >= 0; i--,std::cout<<std::endl) //value returned by size1 & size2 is of type unsignedint..
		for( int j=0;j < (int)mat.size2();j++,std::cout<<" ")
			/*if(mat(i,j)>0 && mat(i,j)<1)
				cout<<"9";	
			 */
			std::cout<<mat(i,j);
	std::cout<<std::endl;
}

void Printer :: msg(char* str)
{
std::	cout<<std::endl<<str<<std::endl;
}
void Printer :: toFile( char *msg=(char*)"Dummy1",  char fname[]=(char*)"defaultPrint.log",char ch='a')
{
	std::ofstream out;
	if(ch=='a')
		out.open(fname,std::ios::app) ;
	else
		out.open(fname) ;
	out<<msg<<std::endl ;
	out.close();
}

void Printer :: toFile( matrix<double> mat,  char fname[]=(char*)"defaultPrint.log",char ch='a',char ch1='z')//zero allowed
{
	std::ofstream out;
	if(ch=='a')
		out.open(fname,std::ios::app) ;
	else
		out.open(fname) ;
	out<<std::endl;
	for( int i = (int)(mat.size1()-1);i >= 0; i--) //value returned by size1 & size2 is of type unsigned int..
	{
		bool flagEndl = false;
		for( int j=0;j < (int)mat.size2();j++)

			if((ch1=='n' && mat(i,j)!=0) || ch1 !='n'){
				out<<mat(i,j)<<" ";
				flagEndl = true;
			}
		if(flagEndl)
			out<<std::endl;
	}

	out<<std::endl;

	out.close();
}
