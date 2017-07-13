#pragma once


#pragma once
#include <vector> 
#include <MathUtils.h>

namespace AaltoGames
{

	//TODO: pit�isk� tehd� template<class DataType> class KdTreeNode, jossa DataType -memberiss� probabilityt yms.

	class SamplingTreeNode{
	public:
		///The weight function as measured for this sample. This can be a likelihood, radiosity, fitness or a probability density value, depending on the case 
		double sampleWeight;
		///Selection probability of this SamplingTreeNode when considering the tree as a probability density model. 
		///probability is propagated upwards in the tree.
		double probability;	
		///number of samples (nodes) in the subtree
		int nSamples;
		///true if this node has been split (has children)
		bool hasChildren;
		///the sample vector pointing inside this SamplingTreeNode
		std::vector<float> sample;
		///vectors pointing to the minimum and maximum corner of this SamplingTreeNode
		std::vector<float> minCorner;
		std::vector<float> maxCorner;

		///a data vector stored in this SamplingTreeNode. In particle filters, the current particle
		///location is stored in sampleLoc, and the data vector stores the previous
		///locations for prediction
		std::vector<float> data;
		///volume of this SamplingTreeNode
		double volumePow;	
		double volume;
		///dimension along which this SamplingTreeNode has been split
		int splitDim;
		///splitting postiing along the variable denoted by splitDim
		float splitLoc;
		///pointer to the parent of this SamplingTreeNode
		SamplingTreeNode *parent;
		///pointers to the children of this SamplingTreeNode
		SamplingTreeNode *children[2];
		///if false, the weight for this sample may be only a prediction waiting to be verified
		bool weightValid;
		///number of variable dimensions
		int dim;
		///number of data dimensions
		int dataDim;
		SamplingTreeNode *leastSignificantChild;
		double significance;
		///constructs the SamplingTreeNode and calls init()
		SamplingTreeNode(int dim=0, int dataDim=0);
		~SamplingTreeNode();
		///Normally, dim and dataDim are passed as arguments to the constructor. However, when
		///declaring arrays of SamplingTreeNodes, dim and dataDim may be initially zero and allocateData can be called
		///later. Note that allocateData should only be called once with nonzero arguments.
		void allocateData(int dim,int dataDim);
		///init the SamplingTreeNode with default values
		void init();
		///set the sample, fitness and data. sample and data vectors are copied
		///instead of simply updating the pointers
		void setSample(const float *sample, double weight, const float *data, bool weightValid);
		///Propagate the selection (sampling) probabilities upwards in the tree.
		///Needs to be called after setSample() and initProbabilities() to
		///prepare the tree for sampling.
		void  propagate();
		///Recursive depth-first browsing, child is visited only if its intersection with the
		///constraint SamplingTreeNode defined by minValues and maxValues is nonzero.
		///IKdTreeBrowse::browseCallback() is called for each visited leaf node
		void findOverlappingNodes(float *minValues, float *maxValues, std::vector<SamplingTreeNode *> &visited);
		SamplingTreeNode *findNodeAt(const float *pos);
		///returns the sibling of this SamplingTreeNode
		SamplingTreeNode *getSibling();
		void copyFrom(SamplingTreeNode *src);
	};

	class SamplingTreeNodeAllocator
	{
	public:
		SamplingTreeNodeAllocator(int dim, int dataDim, int n);
		~SamplingTreeNodeAllocator();
		//allocates a new node from the internal pool
		SamplingTreeNode *allocateNode();
		void reset();
	private:
		std::vector<SamplingTreeNode *> nodes;
		size_t nextFreeNode;
	};

	class SamplingTree {
	public:
		std::vector<float> valueRange;
		enum ProbabilityMode
		{
			VW=0,	//volume * sample weight
			VdW,    //volume * difference of weight of siblings 
			VWdW	//volume * sample weight * difference of weight of siblings 
		};
		ProbabilityMode probabilityMode;
		float splitStopRelWidth;
		///Construct the instance setting dim=dimensions
		SamplingTree();
	public:
		~SamplingTree();
		//allocate memory for everything, and init the root hypercube bounds
		void init(int dim, int dataDim, const float *minCorner, const float *maxCorner, int memSize=10000);
		//allocate memory for everything, and init the root hypercube bounds
		void init(int dim, int dataDim, const float *minCorner, const float *maxCorner, SamplingTreeNodeAllocator *allocator);
		//sample a leaf (hypercube) according to the current selection probabilities
		SamplingTreeNode *sampleLeaf();
		//Sample from a normal distribution centered around the sample of the selected hypercube
		//The normal distribution has diagonal covariances cov[i]=squared(width[i]*relStd), where width[i] is the hypercube width in the dimension i
		void sampleNormal(float *out_sample, float relStd, float minRelStd=0);
		//add a new sample to the tree. Note that if the exact same sample is already in the tree, its weight is updated and no new node is added.
		SamplingTreeNode *putSample(const float *sample, double weight, const float *data=0, bool fitnessValid=true);
		///Updates the public leaves array. Useful, e.g., for visualizing all the leaves
		void updateLeaves(std::vector<SamplingTreeNode *> &leaves);
		SamplingTreeNode *findNodeAt(const float *pos);
		///Override default splitting behavior
		void setMaxSplittingDim(int maxSplitDim);
		SamplingTreeNode *getRoot();
		///See above. By default, volumeExp is set to 1, which  
		///leads to unbiased importance sampling. However,
		///if one wants the sampling/optimization to proceed more greedily, favoring high
		///fitnesses regardless of the volume of the SamplingTreeNode, one may decrease greedyVolumeExp.
		///Especially, if one wants the samples to accumulate as in a multidimensional histogram, one
		///may set greedyVolumeExp to zero. This is useful if the fitness value for a parameter vector
		///is uncertain.
		void setVolumeExp(float newVolumeExp);
		//prunes away the least probable leaves.
		void prune(int N);
		//Clear the tree. Faster than calling init again
		void clear(bool reAllocateRoot);
	protected:
		///Updates node selection probability in sampling
		void initNodeProbabilities(SamplingTreeNode *n);
		///Split the SamplingTreeNode parent at splitCoord along splitDim
		void split(SamplingTreeNode *parent, float splitCoord, int splitDim);
		///Compute optimal splitting dimension of a SamplingTreeNode based on the new sample and the existing sample
		void computeOptimalSplit(SamplingTreeNode *target, const float *newSample, float &splitCoord, int &splitDim);
		///Init the probabilities of children of the parent SamplingTreeNode and propagate the selection probability and significance
		///up the tree. Called after storing a sample.
		void initAndPropagateChildren(SamplingTreeNode *parent);
		///Update the fitness of the SamplingTreeNode. Re-computes and propagates selection probability.
		void updateNodeWeight(SamplingTreeNode *node, double weight);
		///Recursion initialized by updateLeafCubes()
		void updateLeavesRecursive(SamplingTreeNode *node, std::vector<SamplingTreeNode *> &leaves );
	private:
		/// Private Copy-constructor - copying this object is not allowed
		SamplingTree(const SamplingTree & src);
		/// Private assignment operator - copying this object is not allowed
		SamplingTree & operator=(const SamplingTree & src);
		int maxSplitDim;
		SamplingTreeNodeAllocator *allocator;
		bool allocatorOwned;
		SamplingTreeNode *root;
		float volumeExp;
	};



}
