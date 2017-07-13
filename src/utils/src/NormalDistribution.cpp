#include "NormalDistribution.h"
#include "LinAlgUtilities.h"
#include <iostream>
#include <Eigen/Cholesky>

namespace AaltoGames
{



	void NormalDistribution::setScaling(){
		double dimension = (double)this->mu.size();
		double tmp = this->covariance.determinant();
		tmp = sqrt(pow(2*M_PI,dimension)*tmp);
		this->scaling = tmp;
	}

	NormalDistribution::NormalDistribution(){
		this->mu = Eigen::VectorXd::Zero(1,1);
		this->covariance = Eigen::MatrixXd::Ones(1,1);
		this->precisionMatrix = Eigen::MatrixXd::Ones(1,1);
		this->setScaling();
		this->choleskyComputed = false;
	}

	NormalDistribution::NormalDistribution(Eigen::VectorXd mu, Eigen::MatrixXd covariance){
		this->mu = mu;
		this->covariance = covariance;
		this->precisionMatrix = pseudoInverseWithRegularization(covariance);
		this->setScaling();
		this->choleskyComputed = false;
	}

	NormalDistribution NormalDistribution::productWithNormalDistribution(NormalDistribution& dist){
		NormalDistribution product;

		Eigen::MatrixXd covar1 = pseudoInverseWithRegularization(dist.covariance); // 1/sigma1^2


		Eigen::MatrixXd covar2 = pseudoInverseWithRegularization(this->covariance); //1/sigma2^2


		Eigen::MatrixXd tmp = (covar1 + covar2);
		product.covariance = pseudoInverseWithRegularization(tmp);


		Eigen::MatrixXd tmp1 = covar1*dist.mu;

		Eigen::MatrixXd tmp2 = covar2*(this->mu);

		product.mu = product.covariance*(tmp1 + tmp2);


		product.setScaling();
		product.choleskyComputed = false;
		return product;
	}

	double NormalDistribution::evaluateAtPoint(const Eigen::VectorXd& x){
		Eigen::VectorXd diff = x - this->mu;
		double argument = diff.transpose() *(this->precisionMatrix * diff);
		return exp(-0.5*argument)/this->scaling;
	}

	NormalDistribution braketNotationToStandard(const Eigen::MatrixXd& precisionMatrix,const Eigen::VectorXd& shift){
		NormalDistribution normalForm;
		normalForm.covariance = pseudoInverseWithRegularization(precisionMatrix);
		normalForm.mu = normalForm.covariance*shift;
		normalForm.setScaling();
		return normalForm;
	}

	std::vector<double> BoxMuller(unsigned int amount){
		std::vector<double> result;
		for(unsigned int i = 0;i<amount;i = i + 2){
			double rand1 = ((double) rand() / (RAND_MAX));
			double rand2 = ((double) rand() / (RAND_MAX));
			double module = sqrt(-2.0*log(rand1));
			result.push_back(module*cos(2*M_PI*rand2));
			result.push_back(module*sin(2*M_PI*rand2));
		}
		while (result.size() > amount){
			result.pop_back();
		}
		return result;
	}

	std::vector<Eigen::VectorXd> NormalDistribution::generateSamples(int amount){
		if (!this->choleskyComputed){
			Eigen::FullPivLU<Eigen::MatrixXd> lu(this->covariance);
			assert(lu.isInvertible());
			Eigen::LLT<Eigen::MatrixXd> llt;
			llt.compute(this->covariance);
			this->choleskyOfCovarianceMatrix = llt.matrixL();
			this->choleskyComputed = true;
		}
		
		int dim = this->mu.size();
		std::vector<Eigen::VectorXd> samples;
		for (int i = 0; i < amount; i++){
			std::vector<double> randomGaussians = BoxMuller(dim);
		
			Eigen::VectorXd sample = Eigen::VectorXd::Zero(dim);
			for (int i=0;i<dim;i++){
				sample(i) = randomGaussians[i];
			}

			sample = this->mu + this->choleskyOfCovarianceMatrix * sample;
			samples.push_back(sample);
		}

		return samples;

	}

	NormalDistribution NormalDistribution::conditionPart(NormalDistribution normIn, std::vector<int>& conditioningIndices, Eigen::VectorXd& values){
		//System size
		int inSize = normIn.mu.size();
		assert(conditioningIndices.size() == values.size());
		
		std::vector<int> nonConditioningIndices;
		//Finding out the indices that do not belong to the conditioning set
		for (int i=0;i<inSize;i++){
			if (*std::find(conditioningIndices.begin(), conditioningIndices.end(), i) != i){
				nonConditioningIndices.push_back(i);
			}
		}

		//Initialize everything to zero
		Eigen::MatrixXd sigmaNonCond = Eigen::MatrixXd::Zero(nonConditioningIndices.size(), nonConditioningIndices.size());
		Eigen::VectorXd muNonCond = Eigen::VectorXd::Zero(nonConditioningIndices.size());
		Eigen::MatrixXd sigmaCond = Eigen::MatrixXd::Zero(conditioningIndices.size(), conditioningIndices.size());
		Eigen::VectorXd muCond = Eigen::VectorXd::Zero(conditioningIndices.size());
		Eigen::MatrixXd sigmaCross = Eigen::MatrixXd::Zero(nonConditioningIndices.size(),conditioningIndices.size());

		int row,column;
		for (unsigned int i = 0; i<nonConditioningIndices.size(); i++){
			row = nonConditioningIndices[i];
			
			//Form the mean vector of the non-conditioning part
			muNonCond[i] = normIn.mu[row];

			//Form the covariance matrix of the non-conditioning part
			for (unsigned int j=0; j<nonConditioningIndices.size(); j++){
				column = nonConditioningIndices[j];
				sigmaNonCond(i,j) = normIn.covariance(row,column);
			}

			//Form the cross covariance matrix
			for (unsigned int j=0; j<conditioningIndices.size(); j++){
				column = conditioningIndices[j];
				sigmaCross(i,j) = normIn.covariance(row,column);
			}

		}


		for (unsigned int i = 0; i<conditioningIndices.size(); i++){
			row = conditioningIndices[i];
		
			//Get the mean of the conditioning part
			muCond[i] = normIn.mu[row];

			//Get the covariance matrix of the conditioning part
			for (unsigned int j=0; j<conditioningIndices.size(); j++){
				column = conditioningIndices[j];
				sigmaCond(i,j) = normIn.covariance(row,column);
			}

		}

		//Get precision matrix of the conditioning part
		Eigen::MatrixXd sigmaCondInv = pseudoInverseWithRegularization(sigmaCond);

		//Compute the mean and covariance matrix when conditioned
		muNonCond += sigmaCross*sigmaCondInv*(values - muCond);
		sigmaNonCond -= sigmaCross*sigmaCondInv*(sigmaCross.transpose());

		//Form a NormalDistribution from the result
		NormalDistribution result(muNonCond,sigmaCond);
		return result;

	}



} //AaltoGames