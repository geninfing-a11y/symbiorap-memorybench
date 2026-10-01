#include "comparator_support.hpp"
#include <sqlite3.h>
#include <stdexcept>

namespace {
void sql(sqlite3* db,const char* s){char* e=nullptr;if(sqlite3_exec(db,s,nullptr,nullptr,&e)!=SQLITE_OK){std::string m=e?e:"sqlite error";sqlite3_free(e);throw std::runtime_error(m);}}
}
int main(int argc,char**argv){
 try{
  const auto root=std::filesystem::path(m13cmp::arg(argc,argv,"--root","m13-sqlite"));
  const auto records=m13cmp::arg_u64(argc,argv,"--records",2048); const auto reps=m13cmp::arg_u64(argc,argv,"--repetitions",5);
  const auto payload_n=m13cmp::arg_u64(argc,argv,"--payload-bytes",1024); const auto profile=m13cmp::arg(argc,argv,"--profile","matched-full");
  const auto out=std::filesystem::path(m13cmp::arg(argc,argv,"--out","sqlite-result.json"));
  std::vector<std::string> samples; bool correct=true; std::filesystem::remove_all(root);std::filesystem::create_directories(root);
  for(std::uint64_t r=0;r<reps;++r){
   auto dbp=root/("run-"+std::to_string(r)+".sqlite"); sqlite3* db=nullptr; if(sqlite3_open(dbp.string().c_str(),&db)!=SQLITE_OK) throw std::runtime_error("sqlite3_open failed");
   sql(db,"PRAGMA journal_mode=WAL;"); sql(db, profile=="matched-full"?"PRAGMA synchronous=FULL;":"PRAGMA synchronous=NORMAL;");
   sql(db,"PRAGMA temp_store=MEMORY;"); sql(db,"CREATE TABLE kv(k INTEGER PRIMARY KEY, v BLOB NOT NULL);");
   sqlite3_stmt* st=nullptr; if(sqlite3_prepare_v2(db,"INSERT INTO kv(k,v) VALUES(?1,?2);",-1,&st,nullptr)!=SQLITE_OK) throw std::runtime_error("prepare failed");
   std::vector<unsigned char> payload(static_cast<std::size_t>(payload_n),0x5a); std::vector<std::uint64_t> lat;lat.reserve(static_cast<std::size_t>(records));
   auto all0=m13cmp::Clock::now();
   for(std::uint64_t i=0;i<records;++i){auto t0=m13cmp::Clock::now();sql(db,"BEGIN IMMEDIATE;");sqlite3_bind_int64(st,1,static_cast<sqlite3_int64>(i));sqlite3_bind_blob(st,2,payload.data(),static_cast<int>(payload.size()),SQLITE_STATIC);if(sqlite3_step(st)!=SQLITE_DONE)throw std::runtime_error("insert failed");sqlite3_reset(st);sqlite3_clear_bindings(st);sql(db,"COMMIT;");lat.push_back(m13cmp::ns_since(t0,m13cmp::Clock::now()));}
   const auto all1=m13cmp::Clock::now(); const auto sec=std::chrono::duration<double>(all1-all0).count(); samples.push_back(m13cmp::sample_json(static_cast<double>(records)/sec,lat));
   sqlite3_finalize(st); sqlite3_stmt* q=nullptr;sqlite3_prepare_v2(db,"SELECT count(*) FROM kv;",-1,&q,nullptr);if(sqlite3_step(q)!=SQLITE_ROW||static_cast<std::uint64_t>(sqlite3_column_int64(q,0))!=records)correct=false;sqlite3_finalize(q);sqlite3_close(db);
  }
  std::ostringstream j;j<<"{\n  \"schema_version\":\"1.0.0\",\n  \"adapter\":\"sqlite\",\n  \"external_version\":\""<<m13cmp::esc(sqlite3_libversion())<<"\",\n  \"configuration_class\":\""<<m13cmp::esc(profile)<<"\",\n  \"workload\":{\"name\":\"durable-single-key-kv\",\"records\":"<<records<<",\"payload_bytes\":"<<payload_n<<"},\n  \"semantics\":{\"durability_class\":\"journal\",\"ack_after_durable_commit\":"<<(profile=="matched-full"?"true":"false")<<",\"atomicity\":\"single-key-transaction\",\"sqlite_journal_mode\":\"WAL\",\"sqlite_synchronous\":\""<<(profile=="matched-full"?"FULL":"NORMAL")<<"\"},\n  \"samples\":[";
  for(std::size_t i=0;i<samples.size();++i){if(i)j<<",";j<<"\n    "<<samples[i];}j<<"\n  ],\n  \"correctness\":{\"pass\":"<<(correct?"true":"false")<<"},\n  \"claim_allowed\":false\n}\n";m13cmp::write_text(out,j.str());std::cout<<out.string()<<"\n";return correct?0:2;
 }catch(const std::exception&e){std::cerr<<"sqlite comparator: "<<e.what()<<"\n";return 1;}
}
