#pragma once
#include "Optimization.h"
#include "SamplingTree.h"

namespace AaltoGames{


	class RandomForestSamplerPrivate; //forward declare for PIMPL

	
	class RandomForestSampler : public IOptimizer
	{
	public:
		RandomForestSampler();
		~RandomForestSampler();

		//////////////////////////////////////////////////////////////////////////
		//IOptimizer methods
		//////////////////////////////////////////////////////////////////////////
		virtual void init(int nInputs, int nResiduals, const float *minValues, const float *maxValues);
		virtual void landscapeUpdated(SampleArray *predictedSamples=NULL);
		virtual const float *getSample();
		virtual void putSample(const float *sample, const float *residuals, double objectiveFunctionValue);
		virtual const float *getBestSample();
		virtual double getBestObjectiveFuncValue();
		virtual void prune(int N);
		virtual SampleArray *getAllSamples();

		/////////////////////////////////////////////////////////////////////////
		//Random forest sampler -specific methods
		/////////////////////////////////////////////////////////////////////////
		double getDensity(const float *sample);
		double getDensity2(const float *sample);
		//Param:
		//samplingRelStd	Multiplier to obtain sample generation std from the distance from sample to hypercube borders. Default is 1.0f
		//volumeExp			Exponent of the hypercube volume used in selecting a sample to mutate. 1.0f yields unbiased importance sampling, 0 yields fully greedy optimization
		//splitStopRelWidth	A hypercube will not be split if its width in all dimensions is less than splitStopRelWidth * full parameter space width
		//nAutoInits		The number of automatic init samples to generate after init() or landscapeUpdated() calls. Includes random initializations and the last best sample
		//greedySamplePercentage	The proportion of samples devoted to searching around the current best sample. 
		//greedySamplingStdev	Maximum stdev (relative to parameter space dimensions) used in greedy sampling. Default is 0.01f
		void setParams(float samplingRelStd, float volumeExp, float splitStopRelWidth, int nAutoInits, float greedySamplePercentage, float greedySamplingStdev);
		//call this before init
		void setSize(int nTrees, int maxSamples);
#ifndef SWIG
		std::vector<SamplingTree *> &getForest();
#endif
		void localOptimizeAll(float kernelWidth, float regularization);
		int localOptimize(const float *sample, float *result, float kernelWidth, float regularization);
		//void meanshiftAll(float kernelWidth);
		//int meanShift(const float *sample, float *result, float kernelWidth); 
		void getProposalGaussian(float *mean, float *std, bool greedy=false);
	private:
		virtual void putAllSamples(SampleArray *samples, bool fitnessesValid);
		RandomForestSamplerPrivate *m;
	};

} //AaltoGames
