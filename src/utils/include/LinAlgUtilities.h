/*

Aalto University Game Tools license

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef LINALGUTILITIES_H
#define LINALGUTILITIES_H
#include <Eigen/Eigen>
#include <vector>
#include "NormalDistribution.h"

namespace AaltoGames
{

class Tensor3{

	std::vector<Eigen::MatrixXd> elements;
	unsigned int dim1;
	unsigned int dim2;
	unsigned int dim3;


public:

	Tensor3();


	//Constructor that initializes to zero
	Tensor3(unsigned int i, unsigned int j, unsigned int k);


	void set(unsigned int i, unsigned int j, unsigned int k, double value);

	double get(unsigned int i, unsigned int j, unsigned int k);

	void initZero(void);

	Eigen::MatrixXd vectorTensor3ProductLeft(Eigen::VectorXd vector);

	Eigen::MatrixXd singletonDim1ToMatrix(void);

	Eigen::VectorXi size(void);

	void print(void);

};

class QuadraticForm{

private:

	std::vector<int> freeIndeces;
	std::vector<int> clampedIndeces;
	Eigen::MatrixXd hessian_ff;
	Eigen::MatrixXd hessian_cf;
	Eigen::MatrixXd hessian_cc;
	Eigen::VectorXd gradient_f;
	Eigen::VectorXd gradient_c;
	Eigen::VectorXd state_f;
	Eigen::VectorXd state_c;
	Eigen::VectorXd searchDirection;

	//The indeces to assemble the system back from the decomposition.
	std::vector<int> assemblingIndecesForFree;
	std::vector<int> assemblingIndecesForClamped;

	void decomposeIndecesToFreeAndClamped(void);

	void decomposeSystem(void);

	Eigen::VectorXd composeVector(Eigen::VectorXd input);

	void getSearchDirectionInFreeSubspace(void);

	double evaluate(Eigen::MatrixXd point);

	Eigen::VectorXd backTrackingLineSearch(double reductionFactor, double treshold,double tolerance);

	Eigen::VectorXd findMaxPropagationInDirection(void);

	Eigen::VectorXd gradientOfQuadraticForm(Eigen::VectorXd point);

public:
	Eigen::MatrixXd hessian;
	Eigen::VectorXd gradient;
	Eigen::VectorXd state;

	Eigen::VectorXd upperLimits;
	Eigen::VectorXd lowerLimits;

	Eigen::VectorXd splitSearch(double tolerance);

};

class GMM{
public:

	std::vector<NormalDistribution> distributions;
	std::vector<double> weights;

	void addComponent(NormalDistribution component,double weight);

	void normalizeWeights(void);

	std::vector<Eigen::VectorXd> generateSamples(int amountOfSamples);

};

double euclidDistance(const Eigen::VectorXd& pt1,const Eigen::VectorXd& pt2);

std::vector<int> findKNearestNeighbors(const std::vector<Eigen::VectorXd>& dataSet, int centerIndex, int amountOfNeighbors);

Eigen::MatrixXd evaluateCovarianceMatrixFromKNearestNeighbors(const std::vector<Eigen::VectorXd>& dataSet, int centerIndex,const int& amountOfNeighbors,const std::vector<double>& weights);

Eigen::MatrixXd pseudoInverseWithRegularization(const Eigen::MatrixXd& matIn,double deltaForTikhonov=1e-6);

double conditionNumber(const Eigen::MatrixXd& A);

//Gives the Hessian of function fun in point point using the central finite difference of size delta.
Tensor3 finDiffHessian(Eigen::VectorXd (*fun)(const Eigen::VectorXd&),Eigen::VectorXd point,double delta);

//Gives the Hessian of function fun in point point using the central finite difference of size delta.
Eigen::MatrixXd finDiffHessian(double (*fun)(const Eigen::VectorXd&),const Eigen::VectorXd& point,double delta);

//Gives the gradient of function fun in point point using the central finite difference of size delta.
Eigen::MatrixXd finDiffJacobian(Eigen::VectorXd (*fun)(const Eigen::VectorXd&),const Eigen::VectorXd& point,double delta);

Tensor3 finDiffCrossJacobian(Eigen::VectorXd (*fun)(const Eigen::VectorXd&,const Eigen::VectorXd&),const Eigen::VectorXd& coord1point,const Eigen::VectorXd& coord2point,double delta);

Tensor3 finDiffCrossJacobian_invOrder(Eigen::VectorXd (*fun)(const Eigen::VectorXd&,const Eigen::VectorXd&),const Eigen::VectorXd& coord1point,const Eigen::VectorXd& coord2point,double delta);

double smoothMax(double input, double beta = 1.0/8.0);

double clamp(const double input,const double upperLimit, const double lowerLimit);

Eigen::VectorXd clamp(const Eigen::VectorXd& input,const Eigen::VectorXd& upperLimits,const Eigen::VectorXd& lowerLimits);

std::vector<int> findClampedIndeces(const Eigen::VectorXd& input,const Eigen::VectorXd& upperLimits,const Eigen::VectorXd& lowerLimits,const Eigen::VectorXd& gradient);

std::vector<int> complementaryIndeces(const std::vector<int>& setOfIndeces,int indexAmount);

bool hasMember(const std::vector<int>& set,int member);

} //AaltoGames
#endif //LINALGUTILITIES_H