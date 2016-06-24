/*

###############################################################################
#                                                                             #
#                 LICHEM: Layered Interacting CHEmical Models                 #
#                              By: Eric G. Kratz                              #
#                                                                             #
#                      Symbiotic Computational Chemistry                      #
#                                                                             #
###############################################################################

 LICHEM wrapper functions for AMBER.

 Reference for AMBER:
 Case et al., AMBER 14, (2015)

*/

//MM utility functions


//MM wrapper functions
double AMBEREnergy(vector<QMMMAtom>& QMMMData, QMMMSettings& QMMMOpts,
                   int bead)
{
  //Runs AMBER energy calculations
  double E = 0.0;
  
  //Change units
  E *= kcal2eV;
  return E;
};

double AMBERForces(vector<QMMMAtom>& QMMMData, VectorXd& Forces,
                   QMMMSettings& QMMMOpts, int bead)
{
  //Function to calculate MM forces on a set of QM atoms
  double E = 0.0;
  
  //Change units
  E *= kcal2eV;
  return E;
};

MatrixXd AMBERHessian(vector<QMMMAtom>& QMMMData, QMMMSettings& QMMMOpts,
                      int bead)
{
  //Function to calculate the MM Hessian for the QM atoms
  MatrixXd MMHess((3*(Nqm+Npseudo)),(3*(Nqm+Npseudo)));
  MMHess.setZero();
  
  return MMHess;
};

double AMBEROpt(vector<QMMMAtom>& QMMMData, QMMMSettings& QMMMOpts, int bead)
{
  //Runs AMBER optimizations
  double E = 0.0;
  
  //Change units
  E *= kcal2eV;
  return E;
};

