#include <vector>
#include <algorithm>
#include <cfloat>
#include <vector>

#include "caffe/common.hpp"
#include "caffe/layer.hpp"
#include "caffe/syncedmem.hpp"
#include "caffe/layers/guided_crf_layer.hpp"
#include "caffe/layer.hpp"
#include "caffe/util/math_functions.hpp"
using std::max;
using std::min;
namespace caffe 
{

template <typename Dtype>
void GuidedCRFLayer<Dtype>::LayerSetUp(const vector<Blob<Dtype>*>& bottom,
                                       const vector<Blob<Dtype>*>& top)
{
//  gpu_id_ = 0;

//  maxIter = this->layer_param_.crf_param().max_iter();
//  radius=this->layer_param_.crf_param().radius();
//  alpha=this->layer_param_.crf_param().alpha();
//  eps=this->layer_param_.crf_param().eps();
//  nodeBel.resize(maxIter);	

//----------------------------------------	
//	myworkspace_.resize(3);
//	myworkspace_[0] = static_cast<Blob<Dtype> *>(Caffe::parallel_workspace_[0*1+gpu_id_]);
//	myworkspace_[1] = static_cast<Blob<Dtype> *>(Caffe::parallel_workspace_[1*1+gpu_id_]);
//	myworkspace_[2] = static_cast<Blob<Dtype> *>(Caffe::parallel_workspace_[2*1+gpu_id_]);
//----------------------------------------	

  for(int iter=0;iter<maxIter;iter++)
    nodeBel[iter]=new Blob<Dtype>();

  if (this->blobs_.size() > 0)
    LOG(INFO)<<"skip initialization";
  else
  {
    int channels = bottom[0]->channels();
    this->blobs_.resize(1);
    this->blobs_[0].reset(new Blob<Dtype>(1,1,channels,channels));
    caffe_set(this->blobs_[0]->count(),Dtype(1),this->blobs_[0]->mutable_cpu_data());
    for(int c=0;c<channels;c++)
      this->blobs_[0]->mutable_cpu_data()[c*channels+c]=0;
  }
}

template <typename Dtype>
void GuidedCRFLayer<Dtype>::Reshape(const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top)
{

  top[0]->ReshapeLike(*bottom[0]);
  int num = bottom[0]->num();
  int maxStates = bottom[0]->channels();
  int channels = bottom[1]->channels();
  int height = bottom[0]->height();
  int width = bottom[0]->width();


  filterPot.Reshape(num,maxStates,height,width);
  compatPot.Reshape(num,maxStates,height,width);
  tempPot.Reshape(num,maxStates,height,width);
  output_p1.Reshape(num,maxStates,height,width);
  output_p2.Reshape(num,maxStates,height,width);
  for(int iter=0;iter<maxIter;iter++)
    nodeBel[iter]->Reshape(num,maxStates,height,width);
  
  mean_I.Reshape(num,channels,height,width);
  II.Reshape(num,channels*channels,height,width);
  mean_II.Reshape(num,channels*channels,height,width);
  var_I.Reshape(num,channels*channels,height,width);
  inv_var_I.Reshape(num,channels*channels,height,width);
  mean_p.Reshape(num*maxStates,1,height,width);
  b.Reshape(num*maxStates,1,height,width);
  mean_b.Reshape(num*maxStates,1,height,width);
  buffer_image.Reshape(num,channels,height,width);
  buffer_score.Reshape(num,maxStates,height,width);
  buffer_image_image.Reshape(num,channels*channels,height,width);

//----------------------- workspace -------------------------
//	myworkspace_[0]->Reshape(num*maxStates,channels,height,width);
//	myworkspace_[1]->Reshape(num*maxStates,channels,height,width);
//	myworkspace_[2]->Reshape(num*maxStates,channels,height,width);
	
//  Ip = myworkspace_[0]->mutable_gpu_data();
//  mean_Ip = myworkspace_[0]->mutable_gpu_diff();
  
//  cov_Ip = myworkspace_[1]->mutable_gpu_data();
//  a = myworkspace_[1]->mutable_gpu_diff();
  
//  mean_a = myworkspace_[2]->mutable_gpu_data();
//  buffer_image_score  = myworkspace_[2]->mutable_gpu_diff();
//----------------------- workspace -------------------------  
}


template <typename Dtype>
GuidedCRFLayer<Dtype>::~GuidedCRFLayer()
{
	for(int iter=0;iter<maxIter;iter++)
    delete nodeBel[iter];
  nodeBel.clear();
}

template <typename Dtype>
void GuidedCRFLayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*>& top,
    const vector<Blob<Dtype>*>& bottom) {
  NOT_IMPLEMENTED;
}

template <typename Dtype>
void GuidedCRFLayer<Dtype>::Backward_cpu(const vector<Blob<Dtype>*>& top,
    const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom) {
  NOT_IMPLEMENTED;
}

INSTANTIATE_CLASS(GuidedCRFLayer);
REGISTER_LAYER_CLASS(GuidedCRF);
}  // namespace caffe
