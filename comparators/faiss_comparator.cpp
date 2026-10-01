#include "comparator_support.hpp"
#include <faiss/IndexFlat.h>
#include <faiss/index_io.h>
#include <cmath>
#include <random>
#include <stdexcept>
#include <limits>
#ifndef M13_FAISS_VERSION
#define M13_FAISS_VERSION "unknown-package-version"
#endif
int main(int argc,char**argv){
 try{
  const auto vectors=m13cmp::arg_u64(argc,argv,"--vectors",100000);
  const auto queries=m13cmp::arg_u64(argc,argv,"--queries",100);
  const auto dims=m13cmp::arg_u64(argc,argv,"--dims",128);
  const auto reps=m13cmp::arg_u64(argc,argv,"--repetitions",5);
  const auto out=std::filesystem::path(m13cmp::arg(argc,argv,"--out","faiss-result.json"));
  const auto root=std::filesystem::path(m13cmp::arg(argc,argv,"--root","m13-faiss"));
  std::filesystem::remove_all(root); std::filesystem::create_directories(root);
  std::mt19937_64 rng(130015); std::uniform_real_distribution<float>d(-1.f,1.f);
  std::vector<float> xb(static_cast<std::size_t>(vectors*dims)); for(auto&x:xb)x=d(rng);
  std::vector<std::string>samples; bool correct=true;
  std::uint64_t label_mismatches=0,equivalent_ties=0,retrieval_failures=0;
  double max_reconstructed_l2=0.0; float max_reported_distance=0.0f;
  for(std::uint64_t r=0;r<reps;++r){
    faiss::IndexFlatL2 idx(static_cast<faiss::idx_t>(dims));
    auto b0=m13cmp::Clock::now(); idx.add(static_cast<faiss::idx_t>(vectors),xb.data()); auto b1=m13cmp::Clock::now();
    auto fp=root/("run-"+std::to_string(r)+".faiss"); faiss::write_index(&idx,fp.string().c_str());
    std::vector<float>xq(static_cast<std::size_t>(queries*dims));
    for(std::uint64_t q=0;q<queries;++q) std::copy_n(xb.data()+static_cast<std::ptrdiff_t>((q%vectors)*dims),static_cast<std::size_t>(dims),xq.data()+static_cast<std::ptrdiff_t>(q*dims));
    std::vector<faiss::idx_t>I(static_cast<std::size_t>(queries)); std::vector<float>D(static_cast<std::size_t>(queries));
    std::vector<std::uint64_t>lat; lat.reserve(static_cast<std::size_t>(queries)); std::vector<float>reconstructed(static_cast<std::size_t>(dims));
    for(std::uint64_t q=0;q<queries;++q){
      auto t=m13cmp::Clock::now(); idx.search(1,xq.data()+static_cast<std::ptrdiff_t>(q*dims),1,D.data()+q,I.data()+q); lat.push_back(m13cmp::ns_since(t,m13cmp::Clock::now()));
      const auto expected=static_cast<faiss::idx_t>(q%vectors); max_reported_distance=std::max(max_reported_distance,std::fabs(D[q]));
      bool equivalent=false; double reconstructed_l2=std::numeric_limits<double>::infinity();
      if(I[q]>=0&&I[q]<static_cast<faiss::idx_t>(vectors)){
        idx.reconstruct(I[q],reconstructed.data()); reconstructed_l2=0.0;
        for(std::uint64_t k=0;k<dims;++k){const double delta=static_cast<double>(reconstructed[static_cast<std::size_t>(k)])-static_cast<double>(xq[static_cast<std::size_t>(q*dims+k)]);reconstructed_l2+=delta*delta;}
        max_reconstructed_l2=std::max(max_reconstructed_l2,reconstructed_l2); equivalent=(reconstructed_l2==0.0);
      }
      if(I[q]!=expected){++label_mismatches;if(equivalent)++equivalent_ties;}
      if(!equivalent){++retrieval_failures;correct=false;}
    }
    auto build=std::chrono::duration<double>(b1-b0).count(); std::ostringstream s;
    s<<std::fixed<<std::setprecision(3)<<"{\"throughput_ops_s\":"<<(static_cast<double>(queries)/(std::accumulate(lat.begin(),lat.end(),0.0)/1e9))<<",\"build_records_per_sec\":"<<(static_cast<double>(vectors)/build)<<",\"latency_ns\":{\"p50\":"<<m13cmp::percentile_ns(lat,.5)<<",\"p95\":"<<m13cmp::percentile_ns(lat,.95)<<",\"p99\":"<<m13cmp::percentile_ns(lat,.99)<<"}}"; samples.push_back(s.str());
  }
  std::ostringstream j; j<<"{\n  \"schema_version\":\"1.0.1\",\n  \"adapter\":\"faiss\",\n  \"external_version\":\""<<M13_FAISS_VERSION<<"\",\n  \"configuration_class\":\"IndexFlatL2-default\",\n  \"workload\":{\"name\":\"exact-vector-retrieval\",\"vectors\":"<<vectors<<",\"queries\":"<<queries<<",\"dims\":"<<dims<<"},\n  \"semantics\":{\"durability_class\":\"retrieval-only\",\"index\":\"IndexFlatL2\"},\n  \"samples\":[";
  for(std::size_t i=0;i<samples.size();++i){if(i)j<<",";j<<"\n    "<<samples[i];}
  j<<"\n  ],\n  \"correctness\":{\"pass\":"<<(correct?"true":"false")<<",\"label_mismatches\":"<<label_mismatches<<",\"equivalent_ties\":"<<equivalent_ties<<",\"retrieval_failures\":"<<retrieval_failures<<",\"max_abs_reported_distance\":"<<max_reported_distance<<",\"max_reconstructed_l2\":"<<max_reconstructed_l2<<"},\n  \"claim_allowed\":false\n}\n";
  m13cmp::write_text(out,j.str()); std::cout<<out.string()<<"\n"; return correct?0:2;
 }catch(const std::exception&e){std::cerr<<"faiss comparator: "<<e.what()<<"\n";return 1;}
}
