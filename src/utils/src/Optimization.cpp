#include "Optimization.h"
#include <exception>
#include<cstddef>

namespace AaltoGames
{


	float * OptimizerSample::getResiduals()
	{
		if (residuals.size()>0)
			return &residuals[0];
		else
			return 0;
	}

	float * OptimizerSample::getInputs()
	{
		return &inputs[0];
	}

	void OptimizerSample::setFitnessValid( bool val )
	{
		fitnessValid=val;
	}

	bool OptimizerSample::getFitnessValid() const
	{
		return fitnessValid;
	}

	void OptimizerSample::setFitness( double val )
	{
		fitness=val;
	}

	double OptimizerSample::getFitness() const
	{
		return fitness;
	}

	void OptimizerSample::setResidual( int idx, float val )
	{
		residuals[idx]=val;
	}

	float OptimizerSample::getResidual( int idx ) const
	{
		return residuals[idx];
	}

	void OptimizerSample::setInput( int idx, float val )
	{
		inputs[idx]=val;
	}

	float OptimizerSample::getInput( int idx ) const
	{
		return inputs[idx];
	}

	OptimizerSample::OptimizerSample(  )
	{
	}

	void OptimizerSample::init(int nInputs, int nResiduals, const float *inputs, const float *residuals, double fitness, bool fitnessValid )
	{
		this->inputs.resize(nInputs);
		this->residuals.resize(nResiduals); 
		this->fitness=fitness;
		this->fitnessValid=fitnessValid;
		if (this->residuals.size()>0)
			memcpy(&this->residuals[0],residuals,sizeof(float)*this->residuals.size());
		memcpy(&this->inputs[0],inputs,sizeof(float)*this->inputs.size());
	}


	OptimizerSample * SampleArray::getSample( int idx )
	{
		return &samples[idx];
	}

}
