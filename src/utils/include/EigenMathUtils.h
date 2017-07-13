/*

Aalto University Game Tools license

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/


#ifndef EIGENMATHUTILS_H
#define EIGENMATHUTILS_H


#include <Eigen/Eigen> 

//Various math utilities depending on the Eigen library.
//Note that for maximum compatibility, there's also plain float array versions of may of the functions.

namespace AaltoGames
{
	void addEigenMatrixRow( Eigen::MatrixXf &m );

	void calcMeanWeighed(const float *input, const double *inputWeights, int vectorLength, int nVectors, Eigen::VectorXf &out_mean);
	//input vectors as columns (Eigen defaults to column major storage)
	void calcMeanWeighed(const Eigen::MatrixXf &input, const Eigen::VectorXd &inputWeights, Eigen::VectorXf &out_mean);
	void calcCovarWeighed(const float *input, const double *inputWeights, int vectorLength, int nVectors, Eigen::MatrixXf &out_covMat, const Eigen::VectorXf &mean);
	void calcCovarWeighed(const Eigen::MatrixXf &input, const Eigen::VectorXd &inputWeights, Eigen::MatrixXf &out_covMat, const Eigen::VectorXf &out_mean);
	void calcMeanAndCovarWeighed(const float *input, const double *inputWeights, int vectorLength, int nVectors, Eigen::MatrixXf &out_covMat, Eigen::VectorXf &out_mean);
	void calcMeanAndCovarWeighed(const Eigen::MatrixXf &input, const Eigen::VectorXd &inputWeights, Eigen::MatrixXf &out_covMat, Eigen::VectorXf &out_mean);
	//faster but not as accurate, as only single precision accumulator is used
	void calcMeanAndCovarWeighedVectorized(const Eigen::MatrixXf &input, const Eigen::VectorXd &inputWeights, Eigen::MatrixXf &out_covMat, Eigen::VectorXf &out_mean,Eigen::MatrixXf &temp);
	//Tries to find x for which the residuals are zero, regularized towards the xb (with residuals rb)
	//X contains samples of x as rows
	void gaussNewtonFromSamplesWeighed(const Eigen::VectorXf &xb, const Eigen::VectorXf &rb, const Eigen::MatrixXf &X, const Eigen::VectorXf &weights, const Eigen::VectorXf &residuals, float regularization, Eigen::VectorXf &out_result);

}//AaltoGames


#endif
