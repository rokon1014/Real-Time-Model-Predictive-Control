/*

Aalto University Game Tools license

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/


#ifndef CONTROLPBP_H
#define CONTROLPBP_H
//#include <Eigen>
#include <vector>
#include "DiagonalGMM.h"
#include "DynamicPdfSampler.h"

#ifdef SWIG
#define __stdcall  //SWIG doesn't understand __stdcall, but fortunately c# assumes it for virtual calls
#endif


namespace AaltoGames
{
	//Defines how controls affect system state at each step
	typedef void (*TransitionFunction)(int step, const float *state, const float *controls, float *out_state);
	//Evaluates the cost for a state, e.g., penalizes not hitting a target. Returned in the form of squared cost, which can be transformed into a potential as exp(-sqCost) by probabilistic optimizers.
	typedef double (*StateCostFunction)(int stepIdx, const float *state);
	//Optional notification callback after the update algorithm has processed a single step (node) of the graph
	typedef void (*OnStepDone)();


	//Note: although we use Eigen vectors internally, the public interface uses only plain float arrays for maximal portability
	class ControlPBP
	{
	public:
		virtual ~ControlPBP();
		//minValues and maxValues contain first the bounds for state variables, then for control variables
		//stateKernelStd==NULL corresponds to the special case of Q=0
		void init(int nParticles, int nSteps, int nStateDimensions, int nControlDimensions, const float *controlMinValues, const float *controlMaxValues, const float *controlMean, const float *controlPriorStd, const float *controlDiffPriorStd, const float *controlDiffDiffPriorStd, float controlMutationStdScale, const float *stateKernelStd);
		//Note that this is just a convenience method that internally calls startIteration(), getControl() etc.
		virtual void  update(const float *currentState, TransitionFunction transitionFwd, StateCostFunction statePotential, OnStepDone onStepDone=NULL);
		virtual double  getSquaredCost();
		virtual void  getBestControl(int timeStep, float *out_control);
		virtual void  setSamplingParams(const float *controlPriorStd, const float *controlDiffPriorStd, const float *controlDiffDiffPriorStd, float controlMutationStdScale, const float *stateKernelStd);

		//returns the prior GMM for the given time step
#ifndef SWIG
		void getConditionalControlGMM(int timeStep, const Eigen::VectorXf &state, DiagonalGMM &dst);
#endif
		/*
		Below, an interface for operation without callbacks. This is convenient for C# integration and custom multithreading. See InvPendulumTest.cpp for the usage.
		*/
		virtual void startIteration(bool advanceTime, const float *initialState);
		virtual void startPlanningStep(int stepIdx);
		//typically, this just returns sampleIdx. However, if there's been a resampling operation, multiple new samples may link to the same previous sample (and corresponding state)
		virtual int  getPreviousSampleIdx(int sampleIdx);
		//samples a new control, considering an optional gaussian prior with diagonal covariance (this corresponds to the \mu_p, \sigma_p, C_u in the paper, although the C_u is computed on Unity side and raw stdev and mean arrays are passed to the optimizer)
		virtual void  getControl(int sampleIdx, float *out_control, const float *priorMean=0, const float *priorStd=0);
		virtual void  updateResults(int sampleIdx, const float *finalControl, const float *newState, double squaredStateCost, const float *priorMean=0, const float *priorStd=0);
		virtual void  endPlanningStep(int stepIdx);
		virtual void  endIteration();
		virtual void  setParams(double uniformBias, double resampleThreshold, bool useGaussianBackPropagation,float gbpRegularization);
		//this function is public only for debug drawing
		void gaussianBackPropagation(TransitionFunction transitionFwd=NULL, OnStepDone onStepDone=NULL);
		virtual int  getBestSampleLastIdx();
		float getGBPRegularization();
	private:
		virtual void  getControlWithoutStateKernel(int sampleIdx, float *out_control, const float *priorMean=0, const float *priorStd=0);

		class MarginalSample
		{
		public:
			Eigen::VectorXf state;
			Eigen::VectorXf previousState;
			Eigen::VectorXf control;
			Eigen::VectorXf previousControl;
			Eigen::VectorXf previousPreviousControl;
			//double weight;
			double forwardBelief;
			double belief;
			double fwdMessage;
			double bwdMessage;
			double stateCost;
			double statePotential;
			double fullCost;
			double fullControlCost;
			double controlCost;
			double controlPotential;
			double bestStateCost;
			double stateDeviationCost;
			int fullSampleIdx;
			int previousMarginalSampleIdx;
			//only used in the no kernels mode
			int priorSampleIdx;

			void init(int nStateDimensions,int nControlDimensions){
				state.resize(nStateDimensions);
				previousState.resize(nStateDimensions);
				control.resize(nControlDimensions);
				control.setZero();
				previousControl.resize(nControlDimensions);
				previousControl.setZero();
				//weight=1.0;
				fullCost=0.0;
				stateCost=0;
				statePotential=1;
				fullControlCost=0;
				bestStateCost=0;
				controlCost=0;
				stateDeviationCost=0;
				belief=fwdMessage=bwdMessage=1;
				forwardBelief=1.0;
			}
		};
		//a vector of MarginalSamples for each graph node, representing a GMM of joint state and control
		std::vector<std::vector<MarginalSample> > marginals;
		std::vector<Eigen::VectorXf> fullSamples;
		std::vector<MarginalSample> oldBest;
		std::vector<double> fullSampleCosts;
		double bestCost;
		Eigen::VectorXf gaussianBackPropagated;
		std::vector<DiagonalGMM> prior;
		Eigen::VectorXf controlMin,controlMax,controlMean, controlPriorStd,controlDiffPriorStd,controlDiffDiffPriorStd,controlMutationStd,stateKernelStd;
		int nSteps;
		DiagonalGMM staticPrior;
		int nSamples;
		int nStateDimensions;
		int nControlDimensions;
		int iterationIdx;
		bool resample;
		int currentStep;
		int nextStep;
		int nInitialGuesses;
		bool useStateKernels;
		int bestFullSampleIdx;
		bool timeAdvanced;
		DynamicPdfSampler *selector;
		DynamicPdfSampler *conditionalSelector;
		void finalizeWeights(int stepIdx);
	};

} //namespace AaltoGames


#endif //CONTROLPBP_H

