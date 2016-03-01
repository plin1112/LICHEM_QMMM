/*

###############################################################################
#                                                                             #
#                 LICHEM: Layered Interacting CHEmical Models                 #
#                              By: Eric G. Kratz                              #
#                                                                             #
#                      Symbiotic Computational Chemistry                      #
#                                                                             #
###############################################################################

 Functions for data analysis and trajectory processing.

 Reference for Kabsch algorithm:
 Kabsch, Acta Crystallogr. Sect. A, 32, 922, (1976)
 Kabsch, Acta Crystallogr. Sect. A, 34, 827, (1978)

*/

//Trajectory analysis functions
void Print_traj(vector<QMMMAtom>& Struct, fstream& traj,
                QMMMSettings& QMMMOpts)
{
  //Function to print the trajectory or restart files for all beads
  stringstream call; //Only used to save traj stream settings
  //Print XYZ file
  int Ntot = QMMMOpts.Nbeads*Natoms; //Total number of particles
  traj << Ntot << '\n' << '\n'; //Print number of particles and a blank line
  //Loop over the atoms in the structure
  for (int i=0;i<Natoms;i++)
  {
    //Print all replicas of atom i
    for (int j=0;j<QMMMOpts.Nbeads;j++)
    {
      traj << setw(3) << left << Struct[i].QMTyp << " ";
      traj << LICHEMFormFloat(Struct[i].P[j].x,16) << " ";
      traj << LICHEMFormFloat(Struct[i].P[j].y,16) << " ";
      traj << LICHEMFormFloat(Struct[i].P[j].z,16) << '\n';
    }
  }
  //Write data and return
  traj.flush(); //Force printing
  return;
};

void BurstTraj(vector<QMMMAtom>& Struct, QMMMSettings& QMMMOpts)
{
  //Function to split reaction path and path-integral trajectory frames
  int ct; //Generic counter
  stringstream call; //Stream for system calls and reading/writing files
  fstream burstfile;
  string dummy; //Generic string
  //Open new trajectory file
  call.str("");
  call << "BurstStruct.xyz";
  ct = 0; //Start counting at the second file
  while (CheckFile(call.str()))
  {
    //Avoids overwriting files
    ct += 1; //Increase file counter
    call.str(""); //Change file name
    call << "BurstStruct_";
    call << ct << ".xyz";
  }
  burstfile.open(call.str().c_str(),ios_base::out);
  //Print trajectory
  for (int j=0;j<QMMMOpts.Nbeads;j++)
  {
    //Print all atoms in replica j
    burstfile << Natoms; //Number of atoms
    burstfile << '\n' << '\n'; //Print blank comment line
    for (int i=0;i<Natoms;i++)
    {
      //Print data for atom i
      burstfile << setw(3) << left << Struct[i].QMTyp << " ";
      burstfile << LICHEMFormFloat(Struct[i].P[j].x,16) << " ";
      burstfile << LICHEMFormFloat(Struct[i].P[j].y,16) << " ";
      burstfile << LICHEMFormFloat(Struct[i].P[j].z,16) << '\n';
    }
  }
  //Write data and return
  burstfile.flush(); //Print trajectory
  burstfile.close(); //File is nolonger needed
  return;
};

//Trajectory manipulation functions
void ReorderQMPBBA(int& argc, char**& argv)
{
  //Rewrite the xyzfile to put QM, PB, and BA atoms first
  stringstream call; //Stream for system calls and reading/writing files
  string dummy; //Generic string
  //Analyze structure
  
  //Print structure
  
  //Exit LICHEM
  exit(0);
  return;
};

void KabschRotation(MatrixXd& A, MatrixXd& B, int MatSize)
{
  //Function to translate/rotate two structures for maximum overlap
  MatrixXd CoVar; //Covariance matrix
  MatrixXd RotMat; //Rotation matrix
  //Calculate the center of the structures
  double Ax = 0; //Average x position of matrix A
  double Ay = 0; //Average y position of matrix A
  double Az = 0; //Average z position of matrix A
  double Bx = 0; //Average x position of matrix B
  double By = 0; //Average y position of matrix B
  double Bz = 0; //Average z position of matrix B
  #pragma omp parallel
  {
    //Update sum of the atomic positions
    #pragma omp for nowait schedule(dynamic) reduction(+:Ax)
    for (int i=0;i<MatSize;i++)
    {
      Ax += A(i,0);
    }
    #pragma omp for nowait schedule(dynamic) reduction(+:Ay)
    for (int i=0;i<MatSize;i++)
    {
      Ay += A(i,1);
    }
    #pragma omp for nowait schedule(dynamic) reduction(+:Az)
    for (int i=0;i<MatSize;i++)
    {
      Az += A(i,2);
    }
    #pragma omp for nowait schedule(dynamic) reduction(+:Bx)
    for (int i=0;i<MatSize;i++)
    {
      Bx += B(i,0);
    }
    #pragma omp for nowait schedule(dynamic) reduction(+:By)
    for (int i=0;i<MatSize;i++)
    {
      By += B(i,1);
    }
    #pragma omp for nowait schedule(dynamic) reduction(+:Bz)
    for (int i=0;i<MatSize;i++)
    {
      Bz += B(i,2);
    }
  }
  #pragma omp barrier
  //Take average
  Ax /= MatSize;
  Ay /= MatSize;
  Az /= MatSize;
  Bx /= MatSize;
  By /= MatSize;
  Bz /= MatSize;
  //Translate centroids
  #pragma omp parallel
  {
    //Move A and B to (0,0,0)
    #pragma omp for nowait schedule(dynamic)
    for (int i=0;i<MatSize;i++)
    {
      A(i,0) -= Ax;
    }
    #pragma omp for nowait schedule(dynamic)
    for (int i=0;i<MatSize;i++)
    {
      A(i,1) -= Ay;
    }
    #pragma omp for nowait schedule(dynamic)
    for (int i=0;i<MatSize;i++)
    {
      A(i,2) -= Az;
    }
    #pragma omp for nowait schedule(dynamic)
    for (int i=0;i<MatSize;i++)
    {
      B(i,0) -= Bx;
    }
    #pragma omp for nowait schedule(dynamic)
    for (int i=0;i<MatSize;i++)
    {
      B(i,1) -= By;
    }
    #pragma omp for nowait schedule(dynamic)
    for (int i=0;i<MatSize;i++)
    {
      B(i,2) -= Bz;
    }
  }
  #pragma omp barrier
  //Calculate covariance matrix
  CoVar = (B.transpose())*A;
  //Compute SVD and identity matrix
  JacobiSVD<MatrixXd> SVDMat(CoVar,ComputeFullU|ComputeFullV);
  MatrixXd DetMat = SVDMat.matrixV()*(SVDMat.matrixU().transpose());
  double SignVal = DetMat.determinant();
  if (SignVal < 0)
  {
    SignVal = -1;
  }
  else
  {
    SignVal = 1;
  }
  MatrixXd Ident(3,3);
  Ident.setIdentity();
  Ident(2,2) *= SignVal; //Change sign for rotation
  //Find optimal rotation matrix
  RotMat = SVDMat.matrixV()*Ident*(SVDMat.matrixU().transpose());
  //Rotate matrix A
  B *= RotMat;
  //Return the modified positions
  return;
};

VectorXd KabschDisplacement(MatrixXd& A, MatrixXd& B, int MatSize)
{
  //Returns the distance between two superimposed structures
  VectorXd Dist(3*MatSize);
  //Rotate structures
  KabschRotation(A,B,MatSize);
  //Calculate displacement
  #pragma omp parallel for schedule(dynamic)
  for (int i=0;i<(3*MatSize);i++)
  {
    //Find the correct location in the arrays
    int Direc = i%3; //Find the remainder: x=0,y=1,z=2
    int AtID = (i-Direc)/3; //Find array index
    //Calculate displacement
    Dist(i) = A(AtID,Direc)-B(AtID,Direc);
  }
  //Return array
  return Dist;
};

//Physical property analysis functions
double LICHEMDensity(vector<QMMMAtom>& Struct, QMMMSettings& QMMMOpts)
{
  //Function to calculate the density for periodic calculations
  double rho = 0;
  //Sum the masses
  #pragma omp parallel for schedule(dynamic) reduction(+:rho)
  for (int i=0;i<Natoms;i++)
  {
    rho += Struct[i].m;
  }
  //Divide by the volume (SI)
  rho /= (Lx*Ly*Lz);
  //Change to g/cm^3 and return
  rho *= amu2kg; //amu to kg
  rho *= m2Ang*m2Ang*m2Ang/1000; //A^3 to cm^3
  return rho;
};

VectorXd LICHEMFreq(MatrixXd& QMMMHess, QMMMSettings& QMMMOpts, int Bead)
{
  //Function to perform a frequency analysis
  VectorXd QMMMFreqs(3*(Nqm+Npseudo));
  QMMMFreqs.setZero();
  //Diagonalize Hessian
  EigenSolver<MatrixXd> FreqAnalysis;
  FreqAnalysis.compute(QMMMHess);
  QMMMFreqs = FreqAnalysis.eigenvalues().real();
  //Remove translation and rotation
  
  //Take the square root and keep the sign
  
  //Change units
  
  //Return frequencies
  return QMMMFreqs;
};
