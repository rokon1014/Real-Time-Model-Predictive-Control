#include "SamplingTree.h"
#include "Debug.h"
#include "ClippedGaussianSampling.h"
#include<cstring>

namespace AaltoGames
{
	SamplingTreeNodeAllocator::SamplingTreeNodeAllocator( int dim, int dataDim, int n )
	{
		nodes.resize(n);
		for (int i=0; i<n; i++)
		{
			nodes[i]=new SamplingTreeNode(dim,dataDim);
		}
		reset();
	}
	SamplingTreeNodeAllocator::~SamplingTreeNodeAllocator()
	{
		for (size_t i=0; i<nodes.size(); i++)
		{
			delete nodes[i];
		}
	}

	SamplingTreeNode * SamplingTreeNodeAllocator::allocateNode()
	{
		AALTO_ASSERT1(nextFreeNode<nodes.size());
		return nodes[nextFreeNode++];
	}

	void SamplingTreeNodeAllocator::reset()
	{
		nextFreeNode=0;

	}




	////////////////////////////////////////////////////////////////////////////////////
	SamplingTreeNode::SamplingTreeNode( int dim/*=0*/, int dataDim/*=0*/ )
	{
		allocateData(dim,dataDim);
		this->dim=dim;
		this->dataDim=dataDim;
		leastSignificantChild=this;
		init();
	}

	SamplingTreeNode::~SamplingTreeNode()
	{
		parent = children[0] = children[1] = NULL;
	}

	void SamplingTreeNode::allocateData( int dim,int dataDim )
	{
		data.resize(dataDim);
		sample.resize(dim);
		minCorner.resize(dim);
		maxCorner.resize(dim);
	}

	void SamplingTreeNode::init()
	{
		leastSignificantChild=this;
		sampleWeight=0;
		nSamples=0;
		probability=0;
		hasChildren=false;
		volumePow=1;
		volume=1;
		splitDim=0;
		parent=0;
		weightValid=false;
	}

	void SamplingTreeNode::setSample( const float *sample, double weight,const float *data, bool weightValid )
	{
		this->weightValid=weightValid;
		memcpy(&this->sample[0],sample,sizeof(float)*dim);
		if (data!=0)
			memcpy(&this->data[0],data,sizeof(float)*dataDim);
		this->sampleWeight=weight;
	}

	void  SamplingTreeNode::propagate()
	{
		if (hasChildren){
			//propagate info about density
			int i=0;
			probability=children[0]->probability+children[1]->probability;
			if (!validFloat(probability))
				probability=0;
			probability=clipDenormalizedToZero(probability);
			nSamples=children[0]->nSamples+children[1]->nSamples;

			//propagate info about least significant child, used for pruning the tree
			if (!children[0]->hasChildren && !children[1]->hasChildren){
				if (children[0]->significance < children[1]->significance)
					leastSignificantChild=children[0];
				else if (children[0]->significance == children[1]->significance)
					leastSignificantChild=children[rand01()];
				else
					leastSignificantChild=children[1];
			}
			else if (!children[0]->hasChildren && children[1]->hasChildren){
				leastSignificantChild=children[1]->leastSignificantChild;
			}
			else if (children[0]->hasChildren && !children[1]->hasChildren){
				leastSignificantChild=children[0]->leastSignificantChild;
			}
			else if (children[0]->hasChildren && children[1]->hasChildren){
				int i=0;
				if (children[1]->leastSignificantChild->significance==
					children[0]->leastSignificantChild->significance)
					i=rand01();
				else if (children[1]->leastSignificantChild->significance<
					children[0]->leastSignificantChild->significance)
					i=1;
				leastSignificantChild=children[i]->leastSignificantChild;
			}
		}
		if (parent!=0)
			parent->propagate();
	}

	void SamplingTreeNode::findOverlappingNodes( float *minValues, float *maxValues, std::vector<SamplingTreeNode *> &visited )
	{
		if (!hasChildren){
			visited.push_back(this);
		}
		else{
			float constraintMin=minValues[splitDim];
			float constraintMax=maxValues[splitDim];
			for (int i=0; i<2; i++){
				float childMin=children[i]->minCorner[splitDim];
				float childMax=children[i]->maxCorner[splitDim];
				if (!(constraintMax<childMin || constraintMin>childMax)){
					children[i]->findOverlappingNodes(minValues,maxValues,visited);
				}
			}
		}
	}

	SamplingTreeNode * SamplingTreeNode::findNodeAt( const float *pos )
	{
		if (hasChildren){
			if (pos[splitDim] < splitLoc)
			{
				return children[0]->findNodeAt(pos);
			}
			else
			{
				return children[1]->findNodeAt(pos);
			}
		}
		else{
			return this;
		}
	}

	SamplingTreeNode * SamplingTreeNode::getSibling()
	{
		if (parent==0)
			return 0;
		else{
			if (parent->children[0]!=this)
				return parent->children[0];
			else
				return parent->children[1];
		}
	}

	void SamplingTreeNode::copyFrom( SamplingTreeNode *src )
	{
		sampleWeight=src->sampleWeight;
		nSamples=src->nSamples;
		probability=src->probability;
		hasChildren=src->hasChildren;
		volumePow=src->volumePow;
		volume=src->volume;
		splitDim=src->splitDim;
		parent=src->parent;
		weightValid=src->weightValid;

		//see AllocateData() to understand the following
		memcpy(&sample[0],&src->sample[0],sizeof(float)*dim);	
		memcpy(&minCorner[0],&src->minCorner[0],sizeof(float)*dim);	
		memcpy(&maxCorner[0],&src->maxCorner[0],sizeof(float)*dim);	
		if (data.size()>0)
			memcpy(&data[0],&src->data[0],sizeof(float)*dataDim);
	}



	SamplingTree::SamplingTree()
	{
		volumeExp=1;
		probabilityMode=VW;
		splitStopRelWidth=0;
		root=NULL;
		allocatorOwned=false;
	}

	void SamplingTree::init( int dim, int dataDim, const float *minCorner, const float *maxCorner, int memSize/*=10000*/ )
	{
		allocator=new SamplingTreeNodeAllocator(dim,dataDim,memSize);
		init (dim,dataDim,minCorner,maxCorner,allocator);
		allocatorOwned=true;
	}

	void SamplingTree::init( int dim, int dataDim, const float *minCorner, const float *maxCorner, SamplingTreeNodeAllocator *allocator )
	{
		this->allocator=allocator;
		allocatorOwned=false;
		root=allocator->allocateNode();
		root->nSamples=0;
		root->hasChildren=false;
		memcpy(&root->minCorner[0],minCorner,sizeof(float)*dim);
		memcpy(&root->maxCorner[0],maxCorner,sizeof(float)*dim);
		if (valueRange.size()!=(size_t)dim)
			valueRange.resize(dim);
		maxSplitDim=dim;
		double volume=1.0;
		for (int i=0; i<dim; i++)
		{
			valueRange[i]=maxCorner[i]-minCorner[i];
			volume*=valueRange[i];
			root->sample[i]=0.5f*(minCorner[i]+maxCorner[i]); //init the sample to something reasonable
			
		}
		root->volumePow=volume;
	}


	void SamplingTree::initNodeProbabilities( SamplingTreeNode *n )
	{
		n->nSamples=1;
		
		if (probabilityMode==VW || n->parent==NULL)
		{
			//focus on regions of high weight (importance sampling, considering sample weight values as probability density or likelihood function measurements)
			n->probability=n->sampleWeight*n->volumePow;
		}
		else if (probabilityMode==VdW)
		{
			//further focus on regions where fitness alternates a lot
			n->probability=fabs(n->sampleWeight-n->getSibling()->sampleWeight)*n->volumePow;
		}
		else
		{
			n->probability=n->sampleWeight*fabs(n->sampleWeight-n->getSibling()->sampleWeight)*n->volumePow;
		}
		if (!validFloat(n->probability))
			n->probability=0;
		n->leastSignificantChild=n;
//		n->significance=n->sampleWeight;
		n->significance=n->probability;

		//check if we can split this node further, if not, set the selection probability to 0
		float maxRelWidth=0;
		for (int i=0; i<maxSplitDim; i++)
		{
			float w=n->maxCorner[i]-n->minCorner[i];
			w/=valueRange[i];
			maxRelWidth=_max(maxRelWidth,w);
		}
		if (maxRelWidth<splitStopRelWidth)
		{
			n->probability=0;
			return;
		}
	}


	void SamplingTree::split( SamplingTreeNode *parent, float splitLoc, int splitDim )
	{
		parent->splitLoc=splitLoc;
		//first, init children equal to parent
		SamplingTreeNode *childPtr[2];
		AALTO_ASSERT1(allocator);
		childPtr[0]=allocator->allocateNode();
		childPtr[1]=allocator->allocateNode();
		childPtr[0]->copyFrom(parent);
		childPtr[1]->copyFrom(parent);

		//when splitting, only one corner coordinate changes
		childPtr[0]->maxCorner[splitDim]=splitLoc;
		childPtr[1]->minCorner[splitDim]=splitLoc;

		double parentSplitWidth=parent->maxCorner[splitDim]-parent->minCorner[splitDim];
		for (int k=0; k<2; k++){
			SamplingTreeNode *child=childPtr[k];

			//link parent and children
			child->parent=parent;
			parent->children[k]=child;

			//update volume, required to compute probability
			//note that we update volume recursively based on the parent volume so that we don't have to multiply together all the dimensions
			AALTO_ASSERT1(child->maxCorner[splitDim]>=child->minCorner[splitDim]);//best to keep them sorted
			if (parent->volumePow!=0){
				double volumeMult=1;
				double splitWidth=child->maxCorner[splitDim]-child->minCorner[splitDim];
				if (splitWidth!=0 && parentSplitWidth!=0)
					volumeMult=splitWidth/parentSplitWidth;
				if (volumeExp!=0){
					child->volume*=volumeMult;
					child->volumePow=pow(child->volume,(double)volumeExp);
					AALTO_ASSERT1(_finite(child->volumePow));
					AALTO_ASSERT1(!_isnan(child->volumePow));
				}
				AALTO_ASSERT1(child->volumePow>=0);
			}
		}
	}

	void SamplingTree::computeOptimalSplit( SamplingTreeNode *target, const float *newSample, float &splitCoord, int &splitDim )
	{
		int dim=root->dim;
		//check that the sample is within the boundaries
		for (int j=0; j<dim; j++){
			AALTO_ASSERT1(newSample[j]<=target->maxCorner[j] && newSample[j]>=target->minCorner[j]);
		}

		//determine split dimension 
		
		splitDim=0;
		float maxDist=0;
		for (int j=0; j<maxSplitDim; j++){
			float dist=target->maxCorner[j]-target->minCorner[j];	//longest dimension gets split, preventing elongated cubes
			if (valueRange[j]!=0)
				dist/=valueRange[j];	//normalize according to valueRange
			else 
				dist=0;
			if (dist>maxDist){
				maxDist=dist;
				splitDim=j;
			}
		}

		splitCoord=0.5f*(target->sample[splitDim]+newSample[splitDim]);
		
		/*
		splitDim=0;
		float maxDist=0;
		for (int j=0; j<maxSplitDim; j++){
			splitCoord=0.5f*(target->sample[j]+newSample[j]);
			float distFromMiddle=fabs(splitCoord-target->minCorner[j]);
			distFromMiddle=_min(distFromMiddle,fabs(splitCoord-target->maxCorner[j]));
			if (valueRange[j]!=0)
				distFromMiddle/=valueRange[j];
			if (distFromMiddle>maxDist){
				maxDist=distFromMiddle;
				splitDim=j;
			}
		}
		
		splitCoord=0.5f*(target->sample[splitDim]+newSample[splitDim]);
		*/
	}

	void SamplingTree::initAndPropagateChildren( SamplingTreeNode *parent )
	{
		//Various initialization stuff. Requires that fitness and volume have been updated.
		initNodeProbabilities(parent->children[0]);
		initNodeProbabilities(parent->children[1]);
		parent->hasChildren=true;			//split complete
		parent->propagate();
	}

	void SamplingTree::updateNodeWeight( SamplingTreeNode *node, double weight )
	{
		node->sampleWeight=weight;
		initNodeProbabilities(node);
		node->propagate();
	}

	SamplingTreeNode * SamplingTree::putSample( const float *sample, double weight, const float *data/*=0*/ , bool fitnessValid)
	{
		for (int i=0; i<root->dim; i++)
		{
			if (sample[i]<root->minCorner[i] || sample[i]>root->maxCorner[i])
				Debug::throwError("Sample element %d=%f violates sampling tree root bounds, min=%f, max=%f!",i,sample[i],root->minCorner[i],root->maxCorner[i]);
		}
		if (root->nSamples==0){
			//if no samples yet, init the root node. No need to split anything here.
			root->setSample(sample,weight,data, fitnessValid);
			initNodeProbabilities(root); 
			return root;
		}

		SamplingTreeNode *parent=root->findNodeAt(sample);

		//check that same sample does not already exist, because it can cause the creation of a zero volume SamplingTreeNode
		if (memcmp(sample,&parent->sample[0],sizeof(float)*root->dim)==0){
			//already exists, just update fitness
			//if (fitness>parent->fitness)
			updateNodeWeight(parent,weight);

			return parent;
		}

		float splitCoord;
		int splitDim;
		computeOptimalSplit(parent,sample,splitCoord,splitDim);
		parent->splitDim=splitDim;

		//put the new sample inside the child it is closest to 
		int targetIndex=0;
		if (sample[splitDim]>splitCoord)
			targetIndex++;

		//split the parent and init the children
		split(parent,splitCoord,splitDim);
		AALTO_ASSERT1(parent->children[targetIndex]!=0);

		//update the fitness and data of the child with the new sample
		parent->children[targetIndex]->setSample(sample,weight,data,fitnessValid);
		AALTO_ASSERT1(parent->children[targetIndex]!=0);
		initAndPropagateChildren(parent);
		AALTO_ASSERT1(parent->children[targetIndex]!=0);
		return parent->children[targetIndex];
	}

	void SamplingTree::updateLeaves(std::vector<SamplingTreeNode *> &leaves)
	{
		updateLeavesRecursive(root,leaves);
	}

	void SamplingTree::updateLeavesRecursive( SamplingTreeNode *node,std::vector<SamplingTreeNode *> &leaves )
	{
		if (node->hasChildren){
			int i=rand01();
			updateLeavesRecursive(node->children[i],leaves);
			updateLeavesRecursive(node->children[1-i],leaves);
		}
		else if(node->nSamples>0){
			leaves.push_back(node);
		}
	}

	SamplingTreeNode * SamplingTree::sampleLeaf()
	{
		double total=root->probability/root->volumePow;
		SamplingTreeNode *target=root;
		while (target->hasChildren){
			double r;
			r=random()*target->probability;
			double threshold=target->children[0]->probability;
			int childIndex;
			if (r<threshold)
				childIndex=0;
			else if (r==threshold)
				childIndex=rand01(); 
			else
				childIndex=1;
			target=target->children[childIndex];
		}
		return target;
	}

	void SamplingTree::sampleNormal( float *out_sample, float relStd, float minRelStd )
	{
		SamplingTreeNode *node=sampleLeaf();
		int N=node->dim;
		for (int i=0; i<N; i++){
			float width=node->maxCorner[i]-node->minCorner[i];
			float minStd=valueRange[i]*minRelStd;
			float std=_max(width*relStd,minStd);
			AALTO_ASSERT1(width>=0);
			out_sample[i]=randGaussianClipped(node->sample[i],std,root->minCorner[i],root->maxCorner[i]);
		}

	}

	SamplingTreeNode * SamplingTree::findNodeAt( const float *pos )
	{
		return root->findNodeAt(pos);
	}

	void SamplingTree::setMaxSplittingDim( int maxSplitDim )
	{
		this->maxSplitDim=maxSplitDim;
	}

	SamplingTree::~SamplingTree()
	{
		if (allocatorOwned)
			delete allocator;
	}

	SamplingTreeNode * SamplingTree::getRoot()
	{
		return root;

	}

	void SamplingTree::setVolumeExp( float newVolumeExp )
	{
		if (newVolumeExp!=volumeExp)
		{
			volumeExp=newVolumeExp;	
			std::vector<SamplingTreeNode *> leaves;
			updateLeaves(leaves);
			for (size_t i=0; i<leaves.size(); i++)
			{
					
				SamplingTreeNode * s = leaves[i];
				if (s->parent!=NULL)
				{
					s->volumePow=pow(s->volume,(double)newVolumeExp);
					initAndPropagateChildren(s->parent);
				}
			}
		}

	}

	void SamplingTree::prune( int N )
	{
		while (root->nSamples>_max(N,1))
		{
			//get sample to prune
			SamplingTreeNode *worst=root->leastSignificantChild;
			if (worst==NULL)
				break;
			SamplingTreeNode *parent=worst->parent;
			if (parent==NULL)
				break;
			
			//remove the sibling pair, and copy the better sample to the parent
			parent->hasChildren=false;
			int sampleToKeep=1;
			if (parent->children[0]->sampleWeight > parent->children[1]->sampleWeight)
				sampleToKeep=0;
			const float *pData=NULL;
			if (root->dataDim>0)
				pData=&parent->children[sampleToKeep]->data[0];
			parent->setSample(&parent->children[sampleToKeep]->sample[0],parent->children[sampleToKeep]->sampleWeight,pData,parent->children[sampleToKeep]->weightValid);
			parent->hasChildren=false;
			initNodeProbabilities(parent);
			parent->propagate();
		}
		if (N==0)
		{
			//root is not deleted, but marked unused
			root->nSamples=0;
		}
	}

	void SamplingTree::clear(bool reAllocateRoot)
	{
		if (allocatorOwned){
			allocator->reset();
			if (!reAllocateRoot)
				allocator->allocateNode(); //keep same root node allocated
		}
		if (reAllocateRoot)
		{
			root=allocator->allocateNode();
		}
		root->nSamples=0;
		root->hasChildren=false;
	}

}
