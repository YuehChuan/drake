#include <mex.h>
#include <iostream>
#include <memory>
#include "drakeMexUtil.h"
#include "RigidBodyManipulator.h"
#include "drakeGeometryUtil.h"
#include "math.h"

using namespace Eigen;
using namespace std;


void mexFunction(int nlhs, mxArray *plhs[],int nrhs, const mxArray *prhs[]) {

  std::string usage = "Usage [phi, n, x, body_x, body_idx] = signedDistancesmex(model_ptr, cache_ptr, points, use_margins)";
  if (nrhs != 4) {
    mexErrMsgIdAndTxt("Drake:signedDistancesmex:WrongNumberOfInputs", usage.c_str());
    printf("Had %d inputs\n", nrhs);
  }

  if (nlhs != 5) {
    mexErrMsgIdAndTxt("Drake:signedDistancesmex:WrongNumberOfOutputs", usage.c_str());
    printf("Had %d outputs\n", nlhs);
  }

  int arg_num = 0;
  RigidBodyManipulator *model = static_cast<RigidBodyManipulator*>(getDrakeMexPointer(prhs[arg_num++]));
  KinematicsCache<double>* cache = static_cast<KinematicsCache<double>*>(getDrakeMexPointer(prhs[arg_num++]));

  auto points = matlabToEigen<SPACE_DIMENSION, Eigen::Dynamic>(prhs[arg_num++]);
  bool use_margins = (bool) (mxGetLogicals(prhs[arg_num++]))[0];

  VectorXd phi; 
  Matrix3Xd normal;
  Matrix3Xd x;
  Matrix3Xd body_x;
  vector<int> body_idx;

  model->signedDistances(*cache, points, phi, normal, x, body_x, body_idx, use_margins);
  
  plhs[0] = eigenToMatlab(phi);
  plhs[1] = eigenToMatlab(normal);
  plhs[2] = eigenToMatlab(x);
  plhs[3] = eigenToMatlab(body_x);
  plhs[4] = stdVectorToMatlab(body_idx);
}
