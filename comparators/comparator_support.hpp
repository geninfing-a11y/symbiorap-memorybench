#pragma once
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace m13cmp {
using Clock = std::chrono::steady_clock;
inline std::uint64_t ns_since(Clock::time_point a, Clock::time_point b) {
    return static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(b-a).count());
}
inline double percentile_ns(std::vector<std::uint64_t> v, double q) {
    if (v.empty()) return 0.0;
    std::sort(v.begin(), v.end());
    const auto idx = static_cast<std::size_t>(q * static_cast<double>(v.size()-1));
    return static_cast<double>(v[idx]);
}
inline std::string esc(std::string_view s) {
    std::string o; o.reserve(s.size()+8);
    for (char c: s) { if(c=='\\'||c=='"'){o.push_back('\\');o.push_back(c);} else if(c=='\n') o += "\\n"; else o.push_back(c); }
    return o;
}
inline std::string arg(int argc, char** argv, std::string_view key, std::string def={}) {
    for (int i = 1; i + 1 < argc; ++i) {
        if (std::string_view(argv[i]) == key) return argv[i + 1];
    }
    return def;
}
inline std::uint64_t arg_u64(int argc,char**argv,std::string_view key,std::uint64_t def){
    auto s=arg(argc,argv,key,""); if(s.empty())return def; return static_cast<std::uint64_t>(std::stoull(s));
}
inline void write_text(const std::filesystem::path& p, const std::string& s) { std::ofstream f(p,std::ios::binary); f<<s; }
inline std::string sample_json(double throughput,const std::vector<std::uint64_t>& lat){
    std::ostringstream o; o<<std::fixed<<std::setprecision(3)
      <<"{\"throughput_ops_s\":"<<throughput<<",\"latency_ns\":{\"p50\":"<<percentile_ns(lat,.50)
      <<",\"p95\":"<<percentile_ns(lat,.95)<<",\"p99\":"<<percentile_ns(lat,.99)<<"}}"; return o.str();
}
}
