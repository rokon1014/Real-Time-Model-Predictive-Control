//#pragma once
#include <vector>
#include<cstddef>
#include<cstring>

namespace AaltoGames
{

	//proxy class, can be instantiated for a float array given 
	class OptimizerSample
	{
	public:
		OptimizerSample();
		void init(int nInputs, int nResiduals, const float *inputs, const float *residuals, double fitness, bool fitnessValid);
		float getInput(int idx) const;
		void setInput(int idx, float val);
		float getResidual(int idx) const;
		void setResidual(int idx, float val);
		double getFitness() const;
		void setFitness(double val);
		bool getFitnessValid() const;
		void setFitnessValid(bool val);
		float *getInputs();
		float *getResiduals();
	protected:
		friend class IOptimizer;
		std::vector<float> inputs;
		std::vector<float> residuals;
		double fitness;
		bool fitnessValid;
	};


	//Wrapper for std::vector<OptimizerSample>. Since the class is needed only for SWIG that doesn't really work with STL, we have the wrapped container accessible as public.
	class SampleArray
	{
	public:
		OptimizerSample *getSample(int idx);
		int getCount() 
		{
			return (int)samples.size();
		}
#ifndef SWIG
		std::vector<OptimizerSample> samples;
#endif
	};

	
/*
//Pseudocode for using the IOptimizer interface in different applications

Tracking:

Program start:
init();

For each new image{
	prune();  //prune now to save CPU in the the prediction step below
	getAllSamples(); 
	predict samples for this timestep, e.g., based on optical flow;
	putAllSamples();
	landscapeUpdated(); //invalidate old fitness values, insert initial guesses based on image features
	while computing time not exceeded{
		getSampleToEvaluate();
		evaluate the model to image fit, update sample outputs;
		putEvaluatedSample();
	}
}


Offline animation optimization:

Start:
init();

on target poses updated{
	setObjectives();
	landscapeUpdated();
	while not converged and target poses not updated{
	    for (i=0; i<nThreads; i++){
			getSampleToEvaluate();
			queue sample for evaluation,
		}
		wait for threads;
		for (i=0; i<nThreads; i++)
			putEvaluatedSample();
	}
}


Online animation optimization:

Start:
init();

for each frame{
	setObjectives();
	landscapeUpdated();
	for (i=0; i<nThreads; i++){
		getSampleToEvaluate();
		queue sample for evaluation,
	}
	for  (int j=0; j<nBatches; j++){ //in training mode, use several batches per frame
		wait for threads;
		for (i=0; i<nThreads; i++)
			putEvaluatedSample();
	}
	getAllSamples(); 
	predict samples for this timestep, e.g., scroll splines in time;
	putAllSamples();
}
*/


	/*

	An optimizer interface that supports 
	
	1. Convex optimization with numerically evaluated gradients, Jacobians etc.
	2. Stochastic optimization, such as GA or evolution strategies
	3. Sequential Monte Carlo (SMC) optimization where the landscape changes, e.g., for each new timestep of a game simulation or each new video image in a computer vision tracking application.

	TODO: maybe use the batch interface and sample vector wrappers anyway, would remove many methods.

	*/
	class IOptimizer
	{
	public:
		//Init the optimizer
		virtual void init(int nInputs, int nResiduals, const float *minValues, const float *maxValues)=0;
		//Notify that the optimization landscape has changed, and all fitness or error values computed so far may be invalid.
		//This may happen, e.g., when getting a new camera image in a computer vision body tracking case, or
		//when advancing to the next frame in a game that uses the optimizer for motion planning.
		//The predictedSamples array, if not NULL, contains an initial set of samples as predicted for the modified landscape.
		//Note that if the client wants to predict the samples, it typically calls prune() and getAllSamples() before 
		//doing the prediction and calling landscapeUpdated()
		virtual void landscapeUpdated(SampleArray *predictedSamples=NULL)=0;
		//Returns a new sample for evaluation. For some optimizers, this can be called always. 
		//However, a gradient descend optimizer might return NULL because it is
		//not able to continue to the gradient direction before one calls putSample()
		//with all the samples that the optimizer uses for numeric gradient estimation.
		//NOTE: the sample returned may point to an internal buffer that gets overwritten at the next call to getSample()
		virtual const float *getSample()=0;
		//Stores an evaluated sample. Note that some optimizers may assume that samples are stored in the same order as they
		//are generated using getSample()
		virtual void putSample(const float *sample, const float *residuals, double objectiveFunctionValue)=0;
		//Returns the best sample so far
		virtual const float *getBestSample()=0;
		//Returns the objective function value for the best sample
		virtual double getBestObjectiveFuncValue()=0;
		//Prunes the internal sample representation to only contain N best samples
		virtual void prune(int N)=0;
		//Gets all samples. Useful if the client wants to modify the samples, e.g., to predict their values for the next time step
		virtual SampleArray *getAllSamples()=0;
	};


}
