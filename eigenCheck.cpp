#include <iostream>

#include "eigen/Eigen/IterativeLinearSolvers"

using namespace Eigen;

int main()
{


int n = 10000;
 VectorXd x(n), b(n);
 SparseMatrix<double> A(n,n);
 // fill A and b
 ConjugateGradient<SparseMatrix<double> > cg;
 cg.compute(A);
 x = cg.solve(b);
 std::cout << "#iterations:     " << cg.iterations() << std::endl;
 std::cout << "estimated error: " << cg.error()      << std::endl;
 // update b, and solve again
 x = cg.solve(b);

}
