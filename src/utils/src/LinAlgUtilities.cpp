#include "LinAlgUtilities.h"
#include <iostream>
#include<algorithm>
#include<vector>

namespace AaltoGames
{

Tensor3::Tensor3(unsigned int i, unsigned int j, unsigned int k){
	dim1 = i;
	dim2 = j;
	dim3 = k;

	Eigen::MatrixXd aMatrix = Eigen::MatrixXd::Zero(dim2,dim3);
	for (unsigned int i=0;i<dim1;i++){
		elements.push_back(aMatrix);
	}

}

//Tensor3::~Tensor3(){
//
//}

void Tensor3::initZero(void){
	for (unsigned int i=0;i<dim1;i++)
		for(unsigned int j=0;j<dim2;j++)
			for(unsigned int k=0;k<dim3;k++)
				this->set(i,j,k,0.0);
}

Tensor3::Tensor3(){
	dim1 = 0;
	dim2 = 0;
	dim3 = 0;
}

Eigen::MatrixXd Tensor3::singletonDim1ToMatrix(void){
	if (this->dim1 == 1){
		return this->elements[0];
	}
	else{
		return this->elements[0]*sqrt(-1.0);
	}
}

//Tensor3 Tensor3::operator=(const Tensor3 tensor){
//	if (this == &tensor) return *this;
//
//	dim1 = tensor.dim1;
//	dim2 = tensor.dim2;
//	dim3 = tensor.dim3;
//
//	free(elements);
//	elements = (double*)malloc(sizeof(double)*dim1*dim2*dim3);
//	memcpy(elements,tensor.elements,sizeof(double)*dim1*dim2*dim3);
//	return *this;
//}

void Tensor3::set(unsigned int i, unsigned int j, unsigned int k, double value){
	if (i < dim1 && j < dim2 && k < dim3){
		elements[i](j,k) = value;
	}
}

double Tensor3::get(unsigned int i, unsigned int j, unsigned int k){
	if (i < dim1 && j < dim2 && k < dim3){
		return elements[i](j,k);
	}
	else{
		return sqrt(-1.0);
	}
}

Eigen::MatrixXd Tensor3::vectorTensor3ProductLeft(Eigen::VectorXd vector){
	assert(vector.size() == this->dim1);
	Eigen::MatrixXd result = Eigen::MatrixXd::Zero(dim2,dim3);
	for(unsigned int i = 0;i < this->dim1;i++){
		result = result + vector(i)*this->elements[i];
	}
	return result;
}

Eigen::VectorXi Tensor3::size(void){
	Eigen::VectorXi result = Eigen::VectorXi::Zero(3);
	result(0) = dim1;
	result(1) = dim2;
	result(2) = dim3;
	return result;
}

void Tensor3::print(void){
	for (unsigned int i=0;i<dim1;i++){
		std::cout << elements[i] << "\n\n";
	}
}

//This calculates the condition number of a matrix. It can be used to see if one can invert a matrix safely.
double conditionNumber(const Eigen::MatrixXd& A){
	Eigen::JacobiSVD<Eigen::MatrixXd> svd(A, Eigen::ComputeFullU | Eigen::ComputeFullV);
	Eigen::VectorXd singularValues = svd.singularValues();
	double singularMax = singularValues.maxCoeff();
	double singularMin = singularValues.minCoeff();
	return abs(singularMax/singularMin);
}


//This function inverts a matrix if invertible. Else it uses the pseudoinverse. As a last resort it will use tikhonov regularization.
Eigen::MatrixXd pseudoInverseWithRegularization(const Eigen::MatrixXd& matIn,double deltaForTikhonov){
	//For full rank matrix return its inverse.
	Eigen::FullPivLU<Eigen::MatrixXd> lu(matIn);
	if(lu.isInvertible()){
		return matIn.inverse();
	}

	//Pseudoinverse
	Eigen::MatrixXd normalMatrix = (matIn.adjoint())*matIn;
	lu = Eigen::FullPivLU<Eigen::MatrixXd>(normalMatrix);
	if(lu.isInvertible()){
		return (normalMatrix.inverse())*(matIn.adjoint());
	}
	
	int rows = normalMatrix.rows();
	int columns = normalMatrix.cols();
	Eigen::MatrixXd eye = Eigen::MatrixXd::Identity(rows,columns);

	//Tikhonov regularized form
	normalMatrix = normalMatrix + deltaForTikhonov*eye;
	return (normalMatrix.inverse())*(matIn.adjoint());

}


//This works correctly (SEAL OF APPROVAL)
//This calculates the Jacobian of a function using finite difference method.
Eigen::MatrixXd finDiffJacobian(Eigen::VectorXd (*fun)(const Eigen::VectorXd&),const Eigen::VectorXd& point,double delta){
	int size = point.size();
	Eigen::MatrixXd gradient;
	bool matrixCreated = 0;

	for(int row = 0; row<size; row++){
		Eigen::VectorXd rowMask = Eigen::VectorXd::Zero(size);
		rowMask(row) = delta;

		Eigen::VectorXd tmp = (*fun)(point+rowMask) - (*fun)(point-rowMask);
		tmp = tmp/(2.0*delta);

		if (!matrixCreated){
			gradient = Eigen::MatrixXd::Zero(tmp.size(),size);
			matrixCreated = 1;
		}

		for (int j=0; j < tmp.size();j++){
			gradient(j,row) = tmp(j);
		}

	}
	return gradient;
}


//This works correctly (SEAL OF APPROVAL)
//This calculates the Hessian matrix of a function
Eigen::MatrixXd finDiffHessian(double (*fun)(const Eigen::VectorXd&),const Eigen::VectorXd& point,double delta){
	int size = point.size();
	Eigen::MatrixXd resultMatrix(size,size);

	for(int column = 0; column<size; column++){
		Eigen::VectorXd columnMask = Eigen::VectorXd::Zero(size);
		columnMask(column) = delta;
		for(int row = 0; row < size; row++){
			Eigen::VectorXd rowMask = Eigen::VectorXd::Zero(size);
			rowMask(row) = delta;

			double tmp = (*fun)(point+columnMask+rowMask) - (*fun)(point+columnMask-rowMask) - (*fun)(point-columnMask+rowMask) + (*fun)(point-columnMask-rowMask);
			tmp = tmp/(4.0*delta*delta);
			resultMatrix(row,column) = tmp;

		}
	}
	return resultMatrix;
}

//ACHTUNG! This function not yet tested.
Tensor3 finDiffHessian(Eigen::VectorXd (*fun)(const Eigen::VectorXd&),Eigen::VectorXd point,double delta){
	int size = point.size();
	Tensor3 resultTensor;

	bool tensorCreated = false;

	for(int column = 0; column<size; column++){
		Eigen::VectorXd columnMask = Eigen::VectorXd::Zero(size);
		columnMask(column) = delta;
		for(int row = 0; row < size; row++){
			Eigen::VectorXd rowMask = Eigen::VectorXd::Zero(size);
			rowMask(row) = delta;


			Eigen::VectorXd tmp = (fun)(point+columnMask+rowMask);
			tmp += (-1.0)*(fun)(point+columnMask-rowMask);
			tmp += (-1.0)*(fun)(point-columnMask+rowMask);
			tmp += (fun)(point-columnMask-rowMask);
			tmp = tmp/(4.0*delta*delta);
			int thirdDim = tmp.size();
			if (!tensorCreated){
				tensorCreated = true;
				resultTensor = Tensor3(thirdDim,size,size);
			}
			for (int i=0;i<thirdDim;i++){
				resultTensor.set(i,row,column,tmp(i));
			}

		}
	}
	return resultTensor;
}

double smoothMax(double input, double beta){
	double tmp = sqrt(input*input + beta*beta);
	return (tmp + input)/2.0;
}

double clamp(const double input,const double upperLimit, const double lowerLimit){
	return std::min(std::max(input,lowerLimit),upperLimit);
}

Eigen::VectorXd clamp(const Eigen::VectorXd& input,const Eigen::VectorXd& upperLimits,const Eigen::VectorXd& lowerLimits){
	Eigen::VectorXd result = Eigen::VectorXd::Zero(input.size());
	for (int i=0;i<input.size();i++){
		result(i) = clamp(input(i),upperLimits(i),lowerLimits(i));
	}
	return result;
}

std::vector<int> findClampedIndeces(const Eigen::VectorXd& input,const Eigen::VectorXd& upperLimits,const Eigen::VectorXd& lowerLimits,const Eigen::VectorXd& gradient){
	std::vector<int> clampedIndeces;
	for (int i = 0;i<input.size();i++){
		if (input(i) >= upperLimits(i) && gradient(i) < 0.0){
			clampedIndeces.push_back(i);
		}
		if (input(i) <= lowerLimits(i) && gradient(i) > 0.0){
			clampedIndeces.push_back(i);
		}
	}
	return clampedIndeces;
}

bool hasMember(const std::vector<int>& set,int member){
	for (int i=0;i<(int)set.size();i++){
		if (set[i] == member){
			return true;
		}
	}
	return false;
}

std::vector<int> complementaryIndeces(const std::vector<int>& setOfIndeces,int indexAmount){
	std::vector<int> complementary;
	for (int i=0;i<indexAmount;i++){
		if (!hasMember(setOfIndeces,i)){
			complementary.push_back(i);
		}
	}
	return complementary;
}

void QuadraticForm::decomposeIndecesToFreeAndClamped(void){
	//Make state feasible first
	this->state = clamp(this->state,this->upperLimits,this->lowerLimits);
	
	Eigen::VectorXd g = this->gradient + this->hessian*this->state;
	this->clampedIndeces = findClampedIndeces(this->state,this->upperLimits,this->lowerLimits,g);
	this->freeIndeces = complementaryIndeces(this->clampedIndeces,this->state.size());
}

void QuadraticForm::decomposeSystem(void){
	this->decomposeIndecesToFreeAndClamped();

	this->state_f = Eigen::VectorXd::Zero(this->freeIndeces.size());
	this->state_c = Eigen::VectorXd::Zero(this->clampedIndeces.size());
	this->gradient_f = Eigen::VectorXd::Zero(this->freeIndeces.size());
	this->gradient_c = Eigen::VectorXd::Zero(this->clampedIndeces.size());
	this->hessian_ff = Eigen::MatrixXd::Zero(this->freeIndeces.size(),this->freeIndeces.size());
	this->hessian_cc = Eigen::MatrixXd::Zero(this->clampedIndeces.size(),this->clampedIndeces.size());
	this->hessian_cf = Eigen::MatrixXd::Zero(this->clampedIndeces.size(),this->freeIndeces.size());

	this->assemblingIndecesForClamped.clear();
	this->assemblingIndecesForFree.clear();
	for (int i=0;i<(int)this->freeIndeces.size();i++){
		this->state_f(i) = this->state(this->freeIndeces[i]);
		this->gradient_f(i) = this->gradient(this->freeIndeces[i]);
		this->assemblingIndecesForFree.push_back(this->freeIndeces[i]);
	}
	for (int i=0;i<(int)this->clampedIndeces.size();i++){
		this->state_c(i) = this->state(this->clampedIndeces[i]);
		this->gradient_c(i) = this->gradient(this->clampedIndeces[i]);
		this->assemblingIndecesForClamped.push_back(this->clampedIndeces[i]);
	}
	for (int i=0;i<(int)this->freeIndeces.size();i++){
		for (int j=0;j<(int)this->freeIndeces.size();j++){
			this->hessian_ff(i,j) = this->hessian(this->freeIndeces[i],this->freeIndeces[j]);
		}
	}
	for (int i=0;i<(int)this->clampedIndeces.size();i++){
		for (int j=0;j<(int)this->clampedIndeces.size();j++){
			this->hessian_cc(i,j) = this->hessian(this->clampedIndeces[i],this->clampedIndeces[j]);
		}
	}
	for (int i=0;i<(int)this->clampedIndeces.size();i++){
		for (int j=0;j<(int)this->freeIndeces.size();j++){
			this->hessian_cf(i,j) = this->hessian(this->clampedIndeces[i],this->freeIndeces[j]);
		}
	}
}

Eigen::VectorXd QuadraticForm::composeVector(Eigen::VectorXd input){
	Eigen::VectorXd output = Eigen::VectorXd::Zero(input.size());
	int j =0;
	for (int i=0;i<(int)this->assemblingIndecesForFree.size();i++){
		output(assemblingIndecesForFree[i]) = input(i);
		j++;
	}
	for (int i=0;i<(int)this->assemblingIndecesForClamped.size();i++){
		output(assemblingIndecesForClamped[i]) = input(i+j);
	}
	return output;
}

void QuadraticForm::getSearchDirectionInFreeSubspace(void){
	this->decomposeSystem();

	Eigen::VectorXd searchDirectionDecomp = Eigen::VectorXd::Zero(this->state.size());
	Eigen::VectorXd searchDirectionFreeSubspace = (this->hessian_cf.transpose())*this->state_c;
	searchDirectionFreeSubspace = this->gradient_f + searchDirectionFreeSubspace;
	//If there is room in the free space for a Newton step do it. Otherwise use the steepest descent which is the pre computed value.
	if (this->state_f.size() > 0){
		searchDirectionFreeSubspace = -pseudoInverseWithRegularization(this->hessian_ff)*searchDirectionFreeSubspace - this->state_f;
	}

	for (int i=0;i<searchDirectionFreeSubspace.size();i++){
		searchDirectionDecomp(i) = searchDirectionFreeSubspace(i);
	}

	this->searchDirection = composeVector(searchDirectionDecomp);

}

double QuadraticForm::evaluate(Eigen::MatrixXd point){
	return 0.5*((point.transpose())*(this->hessian)*point + (this->gradient.transpose())*point)(0);
}

Eigen::VectorXd QuadraticForm::backTrackingLineSearch(double reductionFactor, double treshold,double tolerance){
	float alpha = 0;
	getSearchDirectionInFreeSubspace();

	Eigen::VectorXd maxIncrement = findMaxPropagationInDirection();
	maxIncrement = maxIncrement - this->state;
	for (int i=0;i<maxIncrement.size();i++){
		alpha = std::max(abs(maxIncrement(i)/searchDirection(i)),abs(alpha));
	}

	Eigen::VectorXd g = this->gradient + (this->hessian)*this->state;
	//If we are not in a gradient
	if (g.dot(g) > tolerance){
		double val1 = this->evaluate(this->state);
		double val2 = this->evaluate(this->state + alpha*this->searchDirection);

		double backTrackingRatio = (this->evaluate(this->state) - this->evaluate(this->state + alpha*this->searchDirection))/(-(g.transpose())*(alpha*this->searchDirection));
		while(backTrackingRatio < treshold && alpha > 1e-30){
			alpha = alpha*reductionFactor;
			backTrackingRatio = (this->evaluate(this->state) - this->evaluate(this->state + alpha*this->searchDirection))/(-(g.transpose())*(alpha*this->searchDirection));
		}
	}
	else{
		alpha = 0.0;
	}

	g = this->state + alpha*this->searchDirection;
	return g;
}

Eigen::VectorXd QuadraticForm::findMaxPropagationInDirection(void){
	double tolerance = 1.0e-5;
	double alpha = tolerance;
	double alphaPrev = 0.0;

	if (this->searchDirection.isZero()){
		return this->state;
	}

	Eigen::VectorXd pt1 = this->state +alpha*this->searchDirection;
	Eigen::VectorXd pt2 = clamp(pt1,upperLimits,lowerLimits);
	
	Eigen::VectorXd diff = pt1-pt2;
	double dist = diff.dot(diff);
	dist = sqrt(dist);
	while (dist < tolerance){
		alphaPrev = alpha;
		alpha = alpha*2;
		
		pt1 = this->state +alpha*this->searchDirection;
		pt2 = clamp(pt1,upperLimits,lowerLimits);
		diff = pt1-pt2;
		dist = diff.dot(diff);
		dist = sqrt(dist);
	}
	double mid = (alpha + alphaPrev)*0.5;
	while (abs(alphaPrev-alpha)>tolerance){
		pt1 = this->state +mid*this->searchDirection;
		pt2 = clamp(pt1,upperLimits,lowerLimits);
		diff = pt1-pt2;
		dist = diff.dot(diff);
		dist = sqrt(dist);
		if (dist < tolerance){
			alphaPrev = mid;
		}
		else{
			alpha = mid;
		}
		mid = (alpha + alphaPrev)*0.5;
	}
	pt1 = this->state +alphaPrev*this->searchDirection;
	return pt1;
}

Eigen::VectorXd QuadraticForm::gradientOfQuadraticForm(Eigen::VectorXd point){
	Eigen::VectorXd g = this->gradient + (this->hessian)*point;
	return g;
}

Eigen::VectorXd QuadraticForm::splitSearch(double tolerance){
	getSearchDirectionInFreeSubspace();

	Eigen::VectorXd pt2 = this->state;
	Eigen::VectorXd pt1 =findMaxPropagationInDirection();

	Eigen::VectorXd diff = pt1-pt2;
	double dist = diff.dot(diff);
	dist = sqrt(dist);

	int safetyCheck = 1000;
	while (dist > tolerance){
		Eigen::VectorXd mid = (pt1+pt2)*0.5;
		

		if (gradientOfQuadraticForm(mid).dot(pt2-mid) > 0.0){
			pt2 = mid;
		}
		else{
			pt1 = mid;
		}
		safetyCheck--;
		if (safetyCheck<=0){
			break;
		}
		diff = pt1-pt2;
		dist = diff.dot(diff);
		dist = sqrt(dist);
	}
	return (pt1+pt2)*0.5;
}

Tensor3 finDiffCrossJacobian(Eigen::VectorXd (*fun)(const Eigen::VectorXd&,const Eigen::VectorXd&),const Eigen::VectorXd& coord1point,const Eigen::VectorXd& coord2point,double delta){
	Tensor3 jacobian;
	bool jacobCreated = false;
	for (int j=0;j<coord1point.size();j++){
		Eigen::VectorXd coord1Mask = Eigen::VectorXd::Zero(coord1point.size());
		coord1Mask(j) = delta;
		for(int k=0;k<coord2point.size();k++){
			Eigen::VectorXd coord2Mask = Eigen::VectorXd::Zero(coord2point.size());
			coord2Mask(k) = delta;

			Eigen::VectorXd diff = fun(coord1point+coord1Mask,coord2point+coord2Mask)-fun(coord1point+coord1Mask,coord2point-coord2Mask)-fun(coord1point-coord1Mask,coord2point+coord2Mask) + fun(coord1point-coord1Mask,coord2point-coord2Mask);
			diff = diff/(4*delta*delta);

			if(!jacobCreated){
				jacobian = Tensor3(diff.size(),coord1point.size(),coord2point.size());
				jacobCreated = true;
			}

			for (int i=0;i<diff.size();i++){
				jacobian.set(i,j,k,diff(i)); 
			}

		}
	}
	return jacobian;
}

Tensor3 finDiffCrossJacobian_invOrder(Eigen::VectorXd (*fun)(const Eigen::VectorXd&,const Eigen::VectorXd&),const Eigen::VectorXd& coord1point,const Eigen::VectorXd& coord2point,double delta){
	Tensor3 jacobian;
	bool jacobCreated = false;
	for (int j=0;j<coord1point.size();j++){
		Eigen::VectorXd coord1Mask = Eigen::VectorXd::Zero(coord1point.size());
		coord1Mask(j) = delta;
		for(int k=0;k<coord2point.size();k++){
			Eigen::VectorXd coord2Mask = Eigen::VectorXd::Zero(coord2point.size());
			coord2Mask(k) = delta;

			Eigen::VectorXd diff = fun(coord1point+coord1Mask,coord2point+coord2Mask)-fun(coord1point+coord1Mask,coord2point-coord2Mask)-fun(coord1point-coord1Mask,coord2point+coord2Mask) + fun(coord1point-coord1Mask,coord2point-coord2Mask);
			diff = diff/(4*delta*delta);

			if(!jacobCreated){
				jacobian = Tensor3(diff.size(),coord2point.size(),coord1point.size());
				jacobCreated = true;
			}

			for (int i=0;i<diff.size();i++){
				jacobian.set(i,k,j,diff(i)); 
			}

		}
	}
	return jacobian;
}

	//This computes the euclidean distance between two points
double euclidDistance(const Eigen::VectorXd& pt1,const Eigen::VectorXd& pt2){
	Eigen::VectorXd diff = pt1 - pt2;
	double result = diff.dot(diff);
	result = sqrt(result);
	return result;
}

//Returns the indices of the k nearest neighbors
std::vector<int> findKNearestNeighbors(const std::vector<Eigen::VectorXd>& dataSet, int centerIndex, int amountOfNeighbors){
	//Create vectors for distances and and the place indices.
	std::vector<double> distances;
	std::vector<int> indices;
	//Compute the distances to the required point
	for (int i = 0; i < (int)dataSet.size(); i++){
		double dist = euclidDistance(dataSet[centerIndex],dataSet[i]);
		if (dist > 0.0){
			distances.push_back(euclidDistance(dataSet[centerIndex],dataSet[i]));
			indices.push_back(i);
		}
	}

	//The vector to store the indices for the found k nearest neighbors
	std::vector<int> kNearestNeighborsIndices;

	//Find the k nearest neighbors
	for (int i = 0; i < std::min(amountOfNeighbors,(int)distances.size()); i++){
		//The index of the minimum element
		int min_index = std::min_element(distances.begin(), distances.end()) - distances.begin();
		//Store the minimum element
		kNearestNeighborsIndices.push_back(indices[min_index]);
		//Erase the found element from the list and continue the computation with the others
		distances[min_index] = std::numeric_limits<double>::max();
	}

	return kNearestNeighborsIndices;

}

//Computes the mean and the covariance of the given data. The data items are the columns of the input
void calcCovarWeighedVectorized(const Eigen::MatrixXd &input, const Eigen::VectorXd &inputWeights, Eigen::MatrixXd &out_covMat)
{
	double wSumInv = inputWeights.sum();
	wSumInv = 1.0f/wSumInv;

	Eigen::VectorXd out_mean = input.rowwise().mean();
	Eigen::MatrixXd temp = (input.colwise() - out_mean);

	for (int k=0;k<inputWeights.size();k++){
		temp.col(k) *= ((float)inputWeights(k))*wSumInv;
	}

	out_covMat = temp*temp.transpose();

	out_covMat = (1.0f/((float)inputWeights.size()-1.0f))*out_covMat;
}

//Computes the mean and the covariance of the given data from the given point. The data items are the columns of the input
void calcCovarFromPointWeighedVectorized(const Eigen::MatrixXd &input, const Eigen::VectorXd& center, const Eigen::VectorXd &inputWeights, Eigen::MatrixXd &out_covMat)
{
	double wSumInv = inputWeights.sum();
	wSumInv = 1.0f/wSumInv;

	Eigen::MatrixXd temp = (input.colwise() - center);

	for (int k=0;k<inputWeights.size();k++){
		temp.col(k) *= ((float)inputWeights(k))*wSumInv;
	}

	out_covMat = temp*temp.transpose();

	out_covMat = (1.0f/((float)inputWeights.size()-1.0f))*out_covMat;
}

Eigen::MatrixXd evaluateCovarianceMatrixFromKNearestNeighbors(const std::vector<Eigen::VectorXd>& dataSet, int centerIndex,const int& amountOfNeighbors,const std::vector<double>& weights){
	Eigen::VectorXd center = dataSet[centerIndex];
	std::vector<int> kNN = findKNearestNeighbors(dataSet,centerIndex,amountOfNeighbors);
	Eigen::MatrixXd dataIn = Eigen::MatrixXd::Zero(dataSet[0].size(),kNN.size());
	Eigen::MatrixXd covar = Eigen::MatrixXd::Zero(dataSet[0].size(),dataSet[0].size());
	Eigen::VectorXd weightVect = Eigen::VectorXd::Zero(kNN.size());
	for (int i = 0; i < (int)kNN.size(); i++){
		dataIn.col(i) = dataSet[kNN[i]];
		weightVect(i) = weights[kNN[i]];
	}

	calcCovarFromPointWeighedVectorized(dataIn,center,weightVect, covar);
	return covar;
}


} //AaltoGames
