#include "RandomForestSampler.h"
#include "DynamicPdfSampler.h"
#include "SamplingTree.h"
#include "IndexRandomizer.h"
#include "Debug.h"
#include "EigenMathUtils.h"
#include "ClippedGaussianSampling.h"
using namespace Eigen;
namespace AaltoGames{

	class RandomForestSamplerPrivate{
	public:
		int maxSamples;
		float greedySamplePercentage;
		float greedySamplingStdev;
		bool landscapeUpdatedAtLeastOnce;
		IndexRandomizer indexRandomizer;
		//Note: this array only holds pointers to nodes stored in trees - no need to delete the nodes in the destructor
		std::vector<SamplingTreeNode *> leaves;
		SamplingTreeNodeAllocator *allocator;
		SampleArray allSamples;
		SampleArray newSamples;
		SamplingTree masterTree;	//we need a master tree that stores all samples for pruning and updating operations
		std::vector<SamplingTree *> forest;
		RandomForestSamplerPrivate()
		{
			maxSamples=0;
			treeSelector=NULL;
			allocator=0;
		}
		~RandomForestSamplerPrivate()
		{
			delete allocator;
			for (size_t i=0; i<forest.size(); i++)
				delete forest[i];

			delete treeSelector;
		}
		DynamicPdfSampler *treeSelector;
		int nTrees;
		int nInputs;
		int nResiduals;
		float samplingRelStd;
		std::vector<float> tempSample;
		std::vector<float> minValues,maxValues;
		std::vector<float> best;
		double bestFunctionValue;
		int nRandomInitsTotal;
		int nRandomInitsCreated;
		void updateBest(const float *sample, double objectiveFunctionValue)
		{
			if (objectiveFunctionValue > bestFunctionValue )
			{
				bestFunctionValue=objectiveFunctionValue;
				memcpy(&best[0],sample,sizeof(float)*nInputs);
			}
		}
		void computeLeafSamplingStds(SamplingTreeNode *leaf, int varIdx, float &out_min, float &out_max){
			float oldSample=leaf->sample[varIdx];
			//WAY 1: the distance to hypercube borders
			//out_min=leaf->minCorner[varIdx];
			//out_max=leaf->maxCorner[varIdx];
			
			//WAY 2: average hypercube width
			out_min=0;
			out_max=0;
			float std=0;
			for (size_t i=0; i<forest.size(); i++){
				SamplingTree *tree = forest[i];
				SamplingTreeNode *leaf2=tree->findNodeAt(&leaf->sample[0]);
				std+=(leaf2->maxCorner[varIdx]-leaf2->minCorner[varIdx]);
			}
			std/=(float)nTrees;
			std*=samplingRelStd;

			
			float minStd=0.0001f*(maxValues[varIdx]-minValues[varIdx]); //0.0001f
			float maxStd=1.0f*(maxValues[varIdx]-minValues[varIdx]);
			out_min=clipMinMaxf(std,minStd,maxStd);
			out_max=clipMinMaxf(std,minStd,maxStd);	
			
			//WAY 3: average distance to hypercube borders in all trees
/*			out_min=0;
			out_max=0;
			for (SamplingTree *tree : forest)
			{
				SamplingTreeNode *leaf2=tree->findNodeAt(&leaf->sample[0]);
				out_min+=leaf2->minCorner[varIdx];
				out_max+=leaf2->maxCorner[varIdx];
			}
			out_min/=(float)nTrees;
			out_max/=(float)nTrees;
		

			float minStd=0.0001f*(maxValues[varIdx]-minValues[varIdx]); //0.0001f
			float maxStd=1.0f*(maxValues[varIdx]-minValues[varIdx]);
			out_min=clipMinMaxf(samplingRelStd*(oldSample-out_min),minStd,maxStd);
			out_max=clipMinMaxf(samplingRelStd*(out_max-oldSample),minStd,maxStd);	

			*/
		}
		void buildSampleArray()
		{
			leaves.clear();
			masterTree.updateLeaves(leaves);
			allSamples.samples.resize(leaves.size());
			for (size_t i=0; i<leaves.size(); i++)
			{
				allSamples.samples[i].init(nInputs,nResiduals,&leaves[i]->sample[0],leaves[i]->data.size()>0 ? &leaves[i]->data[0] : NULL,leaves[i]->sampleWeight,leaves[i]->weightValid);
			}
		}
		//Currently not in use. Using this may cause samples to form clusters that sample locally and don't track big changes in the landscape
//		const float *generateSampleBasedOnNeighbors(const float *pos)
//		{
//			//TODO: should we normalize the variance, not every sample vector?
//			//Kernel weighting? Best sample as the center instead of mean?
//
//			//first prepare the neighbor sample vectors and their weights as OpenCV matrices
//			int N=nTrees*2;
//			
//			MatrixXf samples(N,nInputs,CV_32FC1);
//			StackDoubleVector(weights,N);
//			int rowIdx=0;
//			for (size_t i=0; i<forest.size(); i++){
//				SamplingTree *tree = forest[i];
//				SamplingTreeNode *leaf=tree->findNodeAt(pos); 
//				CvFloatVector s(nInputs,&leaf->sample[0]);
//				cv::transpose(s,samples.row(rowIdx));
//				weights[rowIdx]=leaf->sampleWeight;
//				rowIdx++;
//
//				SamplingTreeNode *leaf2=leaf->getSibling();
//				if (leaf2!=NULL)
//					leaf=leaf2;
//				CvFloatVector s2(nInputs,&leaf->sample[0]);
//				cv::transpose(s2,samples.row(rowIdx));
//				weights[rowIdx]=leaf->sampleWeight;
//				rowIdx++;
//			}
//			
//			//compute weighed mean
//			StackFloatVector(mean,nInputs);
//			for (int j=0; j<nInputs; j++){
//				double avg=0;
//				double wSum=0;
//				for (int i=0; i<N; i++){
//					double w=weights[i];
//					avg+=w*samples.at<float>(i,j);
//					wSum+=w;
//				}
//				avg/=wSum;
//				mean[j]=(float)avg;
//			}
//
//			//transform samples to origin and normalize their lengths so that we get a default sampling covariance independent of the sample distribution, only scaled by sample weights
//			for (int row=0; row<N; row++)
//			{
//				//transform to origin, normalize, compute squared length
//				float sqLen=0;
//				for (int col=0; col<nInputs; col++)
//				{
//					float centeredSample=samples.at<float>(row,col)-mean[col];
//					float normalizedSample=centeredSample/(maxValues[col]-minValues[col]);
//					sqLen+=squared(normalizedSample);
//					samples.at<float>(row,col)=normalizedSample;
//				}
//				//normalize length
//				/*
//				float len=sqrtf(sqLen);
//				if (len>0.0001f)
//				{
//					float newLength=0.005f; //in normalized coordinates
//					float scale=newLength/len;
//					for (int col=0; col<nInputs; col++)
//					{
//						samples.at<float>(row,col)*=scale*(maxValues[col]-minValues[col]);
//					}
//				}
//				*/
//			}
//
//
//			//sample (see GaussianSampling.m matlab file)
//			StackDoubleVector(sample,nInputs);
//			sample.setTo(0);
//			double totalWeights=0;
//			for (int i=0; i<N; i++)
//			{
//				double r=randGaussianClipped(0,1,-1000,1000);
//				double w=weights[i];
//				totalWeights+=w;
//				for (int j=0; j<nInputs; j++)
//				{
//					sample[j]+=w*r*(double)(samples.at<float>(i,j));
//				}
//			}
//			sample.scale(1.0/sqrt(totalWeights));
//			
//			//add a normally distributed sample (diagonal variance) so that the effective variance is never zero
//			for (int i=0; i<nInputs; i++)
//			{
//				float r=randGaussian(0,0.001f*(maxValues[i]-minValues[i]));
//				tempSample[i]=clipMinMaxf(r+mean[i]+(float)sample[i],minValues[i],maxValues[i]);
//			}
///*
//			for (int i=0; i<nInputs; i++)
//			{
//				float r=NormalDistributionTable::getInstance().sample(0,0.005f*(maxValues[i]-minValues[i]),-FLT_MAX,FLT_MAX);
//				tempSample[i]=clipMinMaxf(best[i]+r,minValues[i],maxValues[i]);
//			}
//			*/
//			return &tempSample[0];
//		}
		double normalizedSqDist(const float *sample1, const float *sample2)
		{
			double result=0;
			for (int i=0; i<nInputs; i++)
			{
				float range=maxValues[i]-minValues[i];
				float val1=(sample1[i]-minValues[i])/range;
				float val2=(sample2[i]-minValues[i])/range;
				result+=squared(val1-val2);
			}
			return result;
		}
		int localOptimize(const float *sample, float *result, float kernelWidth, float regularization)
		{
			int n=allSamples.getCount();
			//Build a matrix of samples, one at each row, and a vector of objective function values
			//Note: we convert the fitness function values to equal quadratic minimization obj. func. values, assuming local unimodality
			MatrixXf residuals(n,nResiduals);
			VectorXf weights(n);
			MatrixXf sampleMatrix(n,nInputs);
			VectorXf scales(nInputs);
			const VectorXf center=Map<const VectorXf>(sample,nInputs);
			const VectorXf centerResiduals=Map<const VectorXf>(&masterTree.findNodeAt(sample)->data[0],nResiduals);
			for (int i=0; i<nInputs; i++)
			{
				scales(i)=maxValues[i]-minValues[i];
			}
			int nNeighbors=0;
			for (int i=0; i<n; i++)
			{
				OptimizerSample *s=allSamples.getSample(i);
				if (s->getFitnessValid())
				{
					VectorXf sv=Map<VectorXf>(s->getInputs(),nInputs);
					VectorXf diff=sv-center;
					diff=diff.cwiseQuotient(scales);
					float dist=diff.squaredNorm();
					if (dist<kernelWidth)
					{
						weights(nNeighbors)=expf(-0.5f*squared(dist)/squared(kernelWidth*0.5f));
						VectorXf rv=Map<VectorXf>(s->getResiduals(),nResiduals);
						residuals.row(nNeighbors)=rv.transpose();
						sampleMatrix.row(nNeighbors)=sv.transpose();
						nNeighbors++;
					}
				}
			}
			VectorXf solved=Map<VectorXf>(result,nInputs);
			if (nNeighbors>2)
			{
				gaussNewtonFromSamplesWeighed(center,centerResiduals,sampleMatrix.block(0,0,nNeighbors,sampleMatrix.cols()),weights.block(0,0,nNeighbors,weights.cols()),residuals.block(0,0,nNeighbors,residuals.cols()),regularization,solved);
				for (int i=0; i<nInputs; i++)
				{
					if (!validFloat(result[i]))
						result[i]=0.5f*(minValues[i]+maxValues[i]);
					result[i]=clipMinMaxf(result[i],minValues[i],maxValues[i]);
				}
			}
			else
			{
				solved=center;
			}
			return nNeighbors;
		}

		void localOptimizeAll(float kernelWidth, float regularization)
		{
			buildSampleArray();
			int n=allSamples.getCount();
			newSamples=allSamples;
			for (int i=0; i<n; i++)
			{
				localOptimize(allSamples.getSample(i)->getInputs(),newSamples.getSample(i)->getInputs(),kernelWidth,regularization);
			}
		}
		int meanShift(const float *center, float *result, float kernelWidth)
		{
			int nNeighbors=0;
/*			int nSolvedOutputs=1;
			gaussian.init(nSolvedOutputs,nInputs);
			int dim=nInputs+nSolvedOutputs;
			static MatrixXf sampleMat;
			sampleMat.create(1000,dim,CV_32F);
			static MatrixXf sampleWeights;
			sampleWeights.create(1000,1,CV_32F);
			*/
			static VectorXd mean(nInputs);
			//	if (allSamples.getSample(centerIdx)->getFitness()!=bestFunctionValue)
			//		continue;
			//prepare sample and weight matrices
			mean.setZero();
			double wTot=0;
			for (int neighborIdx=0; neighborIdx<allSamples.getCount(); neighborIdx++)
			{
				if (allSamples.getSample(neighborIdx)->getFitnessValid())
				{
					double fitness=allSamples.getSample(neighborIdx)->getFitness();
					if (fitness<1e-40)
						continue;
					float *neighbor=allSamples.getSample(neighborIdx)->getInputs();
					//compute the weight (kernel function) for the sample
					float sqDist=0;
					for (int i=0; i<nInputs; i++)
					{
						float valueRange=maxValues[i]-minValues[i];
						//according to tests so far, using the sampling stdev as the weight kernel stdev works best
						sqDist+=squared((neighbor[i]-center[i])/valueRange);
					}
					double w=_max(0,kernelWidth*kernelWidth-sqDist);
					w*=fitness;
					if (w>0)
					{
						for (int i=0; i<nInputs; i++)
						{
							mean(i)+=w*neighbor[i];
						}
						wTot+=w;
						nNeighbors++;
					}
				} //if fitness valid
			}
			if (wTot>0)
			{
				for (int i=0; i<nInputs; i++)
				{
					result[i]=(float)(mean(i)/wTot);
					result[i]=clipMinMaxf(result[i],minValues[i],maxValues[i]);
				}
			}
			else
			{
				memcpy(result,center,sizeof(float)*nInputs);
			}
			return nNeighbors;
		}

		//void meanShiftStep(float kernelWidth)
		//{
		//	buildSampleArray();
		//	//create a new sample array for the outputs
		//	newSamples=allSamples;
		//	int nSolvedOutputs=1;
		//	gaussian.init(nSolvedOutputs,nInputs);
		//	int dim=nInputs+nSolvedOutputs;
		//	static MatrixXf sampleMat;
		//	sampleMat.create(1000,dim,CV_32F);
		//	static MatrixXf sampleWeights;
		//	sampleWeights.create(1000,1,CV_32F);
		//	static MatrixXf mean(nInputs,1,CV_64F);
		//	for (int centerIdx=0; centerIdx<allSamples.getCount(); centerIdx++)
		//	{
		//		int nUsed=0;
		//	//	if (allSamples.getSample(centerIdx)->getFitness()!=bestFunctionValue)
		//	//		continue;
		//		//prepare sample and weight matrices
		//		float *center=allSamples.getSample(centerIdx)->getInputs();
		//		mean.setTo(0);
		//		double wTot=0;
		//		for (int neighborIdx=0; neighborIdx<allSamples.getCount(); neighborIdx++)
		//		{
		//			double fitness=allSamples.getSample(neighborIdx)->getFitness();
		//			if (fitness<1e-40)
		//				continue;
		//			float *neighbor=allSamples.getSample(neighborIdx)->getInputs();
		//			//compute the weight (kernel function) for the sample
		//			float sqDist=0;
		//			for (int i=0; i<nInputs; i++)
		//			{
		//				float valueRange=maxValues[i]-minValues[i];
		//				//according to tests so far, using the sampling stdev as the weight kernel stdev works best
		//				sqDist+=squared((neighbor[i]-center[i])/valueRange);
		//			}
		//			double w=_max(0,kernelWidth*kernelWidth-sqDist);
		//			w*=fitness;
		//			if (w>0)
		//			{
		//				for (int i=0; i<nInputs; i++)
		//				{
		//					mean.at<double>(i,0)+=w*neighbor[i];
		//				}
		//				wTot+=w;
		//			}
		//		}
		//		if (wTot>0)
		//		{
		//			float *newSample=newSamples.samples[centerIdx].getInputs();
		//			for (int i=0; i<nInputs; i++)
		//			{
		//				newSample[i]=(float)(mean.at<double>(i,0)/wTot);
		//				newSample[i]=clipMinMaxf(newSample[i],minValues[i],maxValues[i]);
		//			}
		//		}
		//	}
		//}
	};

	RandomForestSampler::RandomForestSampler()
	{
		m=new RandomForestSamplerPrivate();
		m->samplingRelStd=0.1f;
		m->nRandomInitsTotal=0;
		m->greedySamplePercentage=20.0f;
		m->greedySamplingStdev=0.01f;
	}
		
	RandomForestSampler::~RandomForestSampler()
	{
		delete m;
	}

	void RandomForestSampler::init( int nInputs, int nResiduals, const float *minValues, const float *maxValues )
	{	
		AALTO_ASSERT1(m->maxSamples!=0);
		if (m->allocator!=NULL)
			delete m->allocator;
		m->landscapeUpdatedAtLeastOnce=false;
		m->allocator=new SamplingTreeNodeAllocator(nInputs,nResiduals,m->maxSamples*2*(m->nTrees+1)); //*2 to allow for other nodes except leaves, +1 because of the master tree
		m->nInputs=nInputs;
		m->nResiduals=nResiduals;
//		m->nSamples=0;
//		m->allSamples.samples.resize(maxSamples);
		m->forest.resize(m->nTrees);
		m->tempSample.resize(nInputs);
		m->minValues.resize(nInputs);
		memcpy(&m->minValues[0],minValues,sizeof(float)*nInputs);
		m->maxValues.resize(nInputs);
		memcpy(&m->maxValues[0],maxValues,sizeof(float)*nInputs);
		if (m->treeSelector!=NULL)
			delete m->treeSelector;
		m->treeSelector=new DynamicPdfSampler(m->nTrees);
		m->best.resize(nInputs);
		m->bestFunctionValue=-1;
		m->nRandomInitsCreated=0;
		m->masterTree.init(nInputs,nResiduals,minValues,maxValues,m->allocator);
		m->masterTree.probabilityMode=SamplingTree::VW;
		m->leaves.reserve(m->maxSamples);
		for (int i=0; i<m->nTrees; i++)
		{
			if (m->forest[i]!=NULL)
				delete m->forest[i];
			m->forest[i]=new SamplingTree();
			m->forest[i]->init(nInputs,nResiduals,minValues,maxValues,m->allocator);
			m->treeSelector->setDensity(i,0); 
			m->forest[i]->probabilityMode=SamplingTree::VW; //better for landscapes containing sharp cliffs without the real optima. Worse for landscapes containing a lot of good enough area
		}
	}	

	void RandomForestSampler::setSize(int nTrees, int maxSamples)
	{
		m->maxSamples=maxSamples;
		m->nTrees=nTrees;
	}


	
	void RandomForestSampler::prune( int N )
	{
		m->masterTree.prune(N);
		for (size_t i=0; i<m->forest.size(); i++){
			SamplingTree *tree = m->forest[i];
			tree->prune(N);
		}
		m->buildSampleArray();
		m->allocator->reset();
		m->bestFunctionValue=-1;
		/*discount past fitnesses in sample generation (similar to CMA-ES..)
		for (size_t i=0; i<m->allSamples.samples.size(); i++)
		{
			m->allSamples.samples[i].setFitness(m->allSamples.samples[i].getFitness()*0.5);
		}
		*/
		putAllSamples(&m->allSamples,true);

	}

	void RandomForestSampler::landscapeUpdated(SampleArray *predictedSamples)
	{
		//copy the samples to the sample array (needed so that we can reset the allocator, as the tree init calls will reallocate the root nodes)
		m->landscapeUpdatedAtLeastOnce=true;
		m->nRandomInitsCreated=0;
		if (predictedSamples==NULL)
		{
			m->buildSampleArray();
			predictedSamples=&m->allSamples;
		}
		m->allocator->reset();
		m->bestFunctionValue=-1;
		putAllSamples(predictedSamples,false);
	}

	const float * RandomForestSampler::getSample()
	{
		//first check if random inits still needed
		if (m->nRandomInitsCreated<m->nRandomInitsTotal)
		{
			//if (m->nRandomInitsCreated==0 && m->landscapeUpdatedAtLeastOnce)
			//{
			//	//after a landscape update, add the old best
			//	memcpy(&m->tempSample[0],&m->best[0],sizeof(float)*m->nInputs);				
			//}
			//else{
				for (int i=0; i<m->nInputs; i++)
				{
					m->tempSample[i]=m->minValues[i]+randomf()*(m->maxValues[i]-m->minValues[i]);
				}
			//}
			m->nRandomInitsCreated++;
			return &m->tempSample[0];
		}

		//Select tree
		int treeIdx=m->treeSelector->sample();

		//Select hypercube / leaf
		SamplingTreeNode *leaf=NULL;
		//The user may specify that we should spend a proportion of samples for greedy search around the current best candidate
		//TODO: implement gradient ascend, gaussian sampling based on neighbors etc here.
		float maxRelStd=1.0f;
		if (randomf()>(1.0f-0.01f*m->greedySamplePercentage) && m->bestFunctionValue>0)
		{
			//In greedy mode
			leaf=m->masterTree.findNodeAt(&m->best[0]);
			maxRelStd=m->greedySamplingStdev;
		}
		else
		{
			leaf=m->forest[treeIdx]->sampleLeaf();
		}

		//uncomment if you want to use neighbor-based gaussian approximation as the sampling density
		//return m->generateSampleBasedOnNeighbors(&leaf->sample[0]);

		//If sample weight not valid (e.g., the weight is a prediction from a landscape that has since changed), 
		//return the sample as is, otherwise generate a new one. Note that we return the sample only when it gets selected the first time to 
		//prevent generating and evaluating the same sample multiple times if the client gets multiple samples before calling putsample
		SamplingTreeNode *nodeInMasterTree=m->masterTree.findNodeAt(&leaf->sample[0]);
		bool &weightValid=nodeInMasterTree->weightValid;
//		AALTO_ASSERT1(memcmp(&nodeInMasterTree->sample[0],&leaf->sample[0],sizeof(float)*m->nInputs)==0);
		if (!weightValid)
		{
			weightValid=true;
			return &leaf->sample[0];
		}
		else
		{
			//mutate the sample in the leaf by asymmetric sampling
			//1. randomly select whether we generate a larger or smaller value related to the original
			//2. sample from a half of a symmetric sampling distribution with std relative to the distance between
			//the leaf hypercube wall and the original sample
			for (int i=0; i<m->nInputs; i++)
			{
				float leftStd,rightStd;
				m->computeLeafSamplingStds(leaf,i,leftStd,rightStd);
				float maxStd=maxRelStd*(m->maxValues[i]-m->minValues[i]);
				leftStd=_min(maxStd,leftStd);
				rightStd=_min(maxStd,rightStd);
				float dir=randomf()*(leftStd+rightStd)-leftStd;
//				float dir=randomf()-0.5f;
				float oldSample=leaf->sample[i];
				if (dir<0)
				{
					m->tempSample[i]=randGaussianClipped(oldSample,leftStd,m->minValues[i],oldSample);
				}
				else
				{
					m->tempSample[i]=randGaussianClipped(oldSample,rightStd,oldSample, m->maxValues[i]);
				}
			}
			return &m->tempSample[0];
		}
	}

	void RandomForestSampler::putSample( const float *sample, const float *residuals, double objectiveFunctionValue )
	{
		for (int i=0; i<m->nInputs; i++)
		{ 
			//if (!validFloat(sample[i]))
				//throw std::exception("Sample with invalid floats!");
		}
		m->masterTree.putSample(sample,objectiveFunctionValue,residuals);
		if (m->nTrees==1)
		{
			m->forest[0]->putSample(sample,objectiveFunctionValue,residuals);
			m->treeSelector->setDensity(0,m->forest[0]->getRoot()->probability);
		}
		else
		{
			//Store samples in some trees
			int nStored=0;
			for (int i=0; i<m->nTrees; i++)
			{
				//Currently, we store to all trees to maximize resolution. The benefit of multiple trees only emerges from 
				//rebuilding the trees in random order at each time step
				//if (i!=treeToSkip)
				//if (randomf()>0.5f)
				//{
					m->forest[i]->putSample(sample,objectiveFunctionValue,residuals);
					m->treeSelector->setDensity(i,m->forest[i]->getRoot()->probability);
					nStored++;
				//}
			}
			//if we didn't yet select any tree, select at least one
			if (nStored==0)
			{
				int treeIdx=randInt(0,m->nTrees-1);
				m->forest[treeIdx]->putSample(sample,objectiveFunctionValue,residuals);
				m->treeSelector->setDensity(treeIdx,m->forest[treeIdx]->getRoot()->probability);
			}
		}

		//update best
		m->updateBest(sample,objectiveFunctionValue);
	}

	const float * RandomForestSampler::getBestSample()
	{
		return &m->best[0];
	}

	double RandomForestSampler::getBestObjectiveFuncValue()
	{
		return m->bestFunctionValue;
	}


	double RandomForestSampler::getDensity( const float *sample )
	{

		double result=0;
		for (int i=0; i<m->nTrees; i++)
		{
			//float acc=0;
			float density=1.0f;
			SamplingTreeNode *leaf=m->forest[i]->findNodeAt(sample);
			for (int varIdx=0; varIdx<m->nInputs; varIdx++)
			{
				float leftStd,rightStd;
				m->computeLeafSamplingStds(leaf,varIdx,leftStd,rightStd);
				float std=sample[varIdx] < leaf->sample[varIdx] ? leftStd : rightStd; 
				density*=exp(-0.5f*squared(sample[varIdx]-leaf->sample[varIdx])/squared(std));
			}
			result+=density*leaf->sampleWeight;
			leaf=leaf->getSibling();
			if (leaf!=NULL)
			{
				for (int varIdx=0; varIdx<m->nInputs; varIdx++)
				{
					float leftStd,rightStd;
					m->computeLeafSamplingStds(leaf,varIdx,leftStd,rightStd);
					float std=sample[varIdx] < leaf->sample[varIdx] ? leftStd : rightStd; 
					density*=exp(-0.5f*squared(sample[varIdx]-leaf->sample[varIdx])/squared(std));
					result+=density*leaf->sampleWeight;
				}
			}
		}
		result/=(double)m->nTrees;
		return result;
	}

	double RandomForestSampler::getDensity2( const float *sample )
	{

		double result=0;
		for (int i=0; i<m->nTrees; i++)
		{
			result+=m->forest[i]->findNodeAt(sample)->sampleWeight;
		}
		result/=(double)m->nTrees;
		return result;

	}

	void RandomForestSampler::setParams( float samplingRelStd, float volumeExp, float splitStopRelWidth, int nRandomInits, float greedySamplePercentage, float greedySamplingStdev )
	{
		m->samplingRelStd=samplingRelStd;
		m->greedySamplePercentage=greedySamplePercentage;
		m->nRandomInitsTotal=nRandomInits;
		m->greedySamplingStdev=greedySamplingStdev;
		for (int i=0; i<m->nTrees; i++)
		{
			m->forest[i]->probabilityMode=SamplingTree::VW;
			m->forest[i]->setVolumeExp(volumeExp);
			m->forest[i]->splitStopRelWidth=splitStopRelWidth;
		}
	}

	std::vector<SamplingTree *> &RandomForestSampler::getForest()
	{
		return m->forest;
	}

	SampleArray * RandomForestSampler::getAllSamples()
	{
		m->buildSampleArray();
		return &m->allSamples;
	}

	void RandomForestSampler::putAllSamples( SampleArray *samples, bool fitnessesValid )
	{

		//reinit the trees and add samples in random order
		/* version that doesn't store all samples to all trees - 
		int N=m->allSamples.samples.size();
		m->indexRandomizer.init(N);
		m->masterTree.init(m->nInputs,0,&m->minValues[0],&m->maxValues[0],m->allocator);
		for (SamplingTree *tree : m->forest)
		{
			m->indexRandomizer.init(N);
			tree->init(m->nInputs,0,&m->minValues[0],&m->maxValues[0],m->allocator);
		}
		for (int i=0; i<N; i++)
		{
			int idx=m->indexRandomizer.get();
			OptimizerSample *sample=m->allSamples.getSample(idx);
			putSample(sample->getInputs(),sample->getOutputs(),false);
		}*/
		
		
		int N=samples->samples.size();
		m->indexRandomizer.init(N);
		m->masterTree.init(m->nInputs,m->nResiduals,&m->minValues[0],&m->maxValues[0],m->allocator);
		for (int i=0; i<N; i++)
		{
			int idx=m->indexRandomizer.get();
			OptimizerSample *sample=samples->getSample(idx);
			m->masterTree.putSample(sample->getInputs(),sample->getFitness(),sample->getResiduals(),fitnessesValid);
		}

		for (size_t iter=0; iter<m->forest.size(); iter++){
			SamplingTree *tree = m->forest[iter];
			m->indexRandomizer.init(N);
			tree->init(m->nInputs,m->nResiduals,&m->minValues[0],&m->maxValues[0],m->allocator);
			for (int i=0; i<N; i++)
			{
				int idx=m->indexRandomizer.get();
				OptimizerSample *sample=samples->getSample(idx);
				tree->putSample(sample->getInputs(),sample->getFitness(),sample->getResiduals(),fitnessesValid);
			}
		}
		
	}
	
	void RandomForestSampler::localOptimizeAll(float kernelWidth, float regularization)
	{
		m->localOptimizeAll(kernelWidth, regularization);
		m->allocator->reset();
		putAllSamples(&m->newSamples,false);
	}
	//void RandomForestSampler::meanshiftAll( float kernelWidth )
	//{
	//	m->meanShiftStep(kernelWidth);
	//	m->allocator->reset();
	//	putAllSamples(&m->newSamples,false);

	//}

	int RandomForestSampler::localOptimize( const float *sample, float *result, float kernelWidth, float regularization )
	{
		m->buildSampleArray();
		return m->localOptimize(sample,result,kernelWidth,regularization);
	}

	//int RandomForestSampler::meanShift( const float *sample, float *result, float kernelWidth )
	//{
	//	m->buildSampleArray();
	//	return m->meanShift(sample,result,kernelWidth);
	//}

	void RandomForestSampler::getProposalGaussian( float *mean, float *std, bool greedy/*=false*/ )
	{
		//Select tree
		int treeIdx=m->treeSelector->sample();

		//Select hypercube / leaf
		SamplingTreeNode *leaf=NULL;
		float maxRelStd=1.0f;
		if (greedy)
		{
			leaf=m->masterTree.findNodeAt(&m->best[0]);
			maxRelStd=m->greedySamplingStdev;
		}
		else
		{
			leaf=m->forest[treeIdx]->sampleLeaf();
		}
		for (int i=0; i<m->nInputs; i++)
		{
			float leftStd,rightStd;
			m->computeLeafSamplingStds(leaf,i,leftStd,rightStd);
			mean[i]=leaf->sample[i];
			std[i]=0.5f*(leftStd+rightStd); //asymmetric proposals not currently supported
		}
	}


} //namespace AaltoGames
