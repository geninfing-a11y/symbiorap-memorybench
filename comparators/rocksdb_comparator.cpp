#include "comparator_support.hpp"
#include <rocksdb/db.h>
#include <rocksdb/version.h>
#include <rocksdb/options.h>
#include <memory>
#include <stdexcept>
#include <string>

namespace {
template <class OptionsT>
std::unique_ptr<rocksdb::DB> open_rocksdb(const OptionsT& opt, const std::string& path) {
    std::unique_ptr<rocksdb::DB> db;
    rocksdb::Status status;
    if constexpr (requires { rocksdb::DB::Open(opt, path, &db); }) {
        status = rocksdb::DB::Open(opt, path, &db);
    } else {
        rocksdb::DB* raw = nullptr;
        status = rocksdb::DB::Open(opt, path, &raw);
        db.reset(raw);
    }
    if (!status.ok()) throw std::runtime_error(status.ToString());
    return db;
}
}
int main(int argc,char**argv){
 try{
  const auto root=std::filesystem::path(m13cmp::arg(argc,argv,"--root","m13-rocksdb"));const auto records=m13cmp::arg_u64(argc,argv,"--records",2048);const auto reps=m13cmp::arg_u64(argc,argv,"--repetitions",5);const auto payload_n=m13cmp::arg_u64(argc,argv,"--payload-bytes",1024);const auto profile=m13cmp::arg(argc,argv,"--profile","matched-sync");const auto out=std::filesystem::path(m13cmp::arg(argc,argv,"--out","rocksdb-result.json"));
  std::vector<std::string> samples;bool correct=true;std::filesystem::remove_all(root);std::filesystem::create_directories(root);
  for(std::uint64_t r=0;r<reps;++r){auto p=root/("run-"+std::to_string(r));rocksdb::Options opt;opt.create_if_missing=true;auto db=open_rocksdb(opt,p.string());rocksdb::WriteOptions wo;wo.sync=(profile=="matched-sync");std::string payload(static_cast<std::size_t>(payload_n),'Z');std::vector<std::uint64_t>lat;lat.reserve(static_cast<std::size_t>(records));auto a=m13cmp::Clock::now();for(std::uint64_t i=0;i<records;++i){auto t=m13cmp::Clock::now();auto st=db->Put(wo,"k"+std::to_string(i),payload);if(!st.ok())throw std::runtime_error(st.ToString());lat.push_back(m13cmp::ns_since(t,m13cmp::Clock::now()));}auto b=m13cmp::Clock::now();samples.push_back(m13cmp::sample_json(static_cast<double>(records)/std::chrono::duration<double>(b-a).count(),lat));std::string v;for(std::uint64_t i=0;i<records;++i){auto st=db->Get(rocksdb::ReadOptions{},"k"+std::to_string(i),&v);if(!st.ok()||v.size()!=payload.size()){correct=false;break;}}}
  std::ostringstream j;j<<"{\n  \"schema_version\":\"1.0.0\",\n  \"adapter\":\"rocksdb\",\n  \"external_version\":\""<<ROCKSDB_MAJOR<<"."<<ROCKSDB_MINOR<<"."<<ROCKSDB_PATCH<<"\",\n  \"configuration_class\":\""<<m13cmp::esc(profile)<<"\",\n  \"workload\":{\"name\":\"durable-single-key-kv\",\"records\":"<<records<<",\"payload_bytes\":"<<payload_n<<"},\n  \"semantics\":{\"durability_class\":\"journal\",\"ack_after_durable_commit\":"<<(profile=="matched-sync"?"true":"false")<<",\"atomicity\":\"single-key-write\",\"rocksdb_writeoptions_sync\":"<<(profile=="matched-sync"?"true":"false")<<"},\n  \"samples\":[";for(std::size_t i=0;i<samples.size();++i){if(i)j<<",";j<<"\n    "<<samples[i];}j<<"\n  ],\n  \"correctness\":{\"pass\":"<<(correct?"true":"false")<<"},\n  \"claim_allowed\":false\n}\n";m13cmp::write_text(out,j.str());std::cout<<out.string()<<"\n";return correct?0:2;
 }catch(const std::exception&e){std::cerr<<"rocksdb comparator: "<<e.what()<<"\n";return 1;}
}
