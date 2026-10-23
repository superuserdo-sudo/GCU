#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cerrno>

namespace {
std::string read_input(const std::string &name, bool &ok) {
    if (name == "-") return std::string(std::istreambuf_iterator<char>(std::cin), {});
    std::ifstream f(name, std::ios::binary);
    if (!f) { ok = false; return {}; }
    std::string s(std::istreambuf_iterator<char>(f), {});
    if (f.bad()) ok = false;
    return s;
}
std::string hex(const std::vector<unsigned char> &v) {
    std::ostringstream out;
    out << std::hex << std::setfill('0');
    for (auto b : v) out << std::setw(2) << unsigned(b);
    return out.str();
}
uint32_t rotr(uint32_t x, unsigned n) { return (x >> n) | (x << (32 - n)); }
std::vector<unsigned char> sha256(const std::string &input, bool is224) {
    static constexpr uint32_t K[64] = {
      0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
      0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
      0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
      0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
      0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
      0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
      0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
      0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2};
    uint32_t h[8] = {is224?0xc1059ed8u:0x6a09e667u,is224?0x367cd507u:0xbb67ae85u,is224?0x3070dd17u:0x3c6ef372u,is224?0xf70e5939u:0xa54ff53au,is224?0xffc00b31u:0x510e527fu,is224?0x68581511u:0x9b05688cu,is224?0x64f98fa7u:0x1f83d9abu,is224?0xbefa4fa4u:0x5be0cd19u};
    std::vector<unsigned char> m(input.begin(), input.end()); uint64_t bits=uint64_t(m.size())*8; m.push_back(0x80); while(m.size()%64!=56)m.push_back(0); for(int i=7;i>=0;--i)m.push_back((bits>>(i*8))&255);
    for(size_t off=0;off<m.size();off+=64){uint32_t w[64];for(int i=0;i<16;++i)w[i]=(uint32_t(m[off+i*4])<<24)|(uint32_t(m[off+i*4+1])<<16)|(uint32_t(m[off+i*4+2])<<8)|m[off+i*4+3];for(int i=16;i<64;++i){uint32_t s0=rotr(w[i-15],7)^rotr(w[i-15],18)^(w[i-15]>>3),s1=rotr(w[i-2],17)^rotr(w[i-2],19)^(w[i-2]>>10);w[i]=w[i-16]+s0+w[i-7]+s1;}uint32_t a=h[0],b=h[1],c=h[2],d=h[3],e=h[4],f=h[5],g=h[6],z=h[7];for(int i=0;i<64;++i){uint32_t S1=rotr(e,6)^rotr(e,11)^rotr(e,25),ch=(e&f)^(~e&g),t1=z+S1+ch+K[i]+w[i],S0=rotr(a,2)^rotr(a,13)^rotr(a,22),maj=(a&b)^(a&c)^(b&c),t2=S0+maj;z=g;g=f;f=e;e=d+t1;d=c;c=b;b=a;a=t1+t2;}h[0]+=a;h[1]+=b;h[2]+=c;h[3]+=d;h[4]+=e;h[5]+=f;h[6]+=g;h[7]+=z;}
    std::vector<unsigned char> out; int count=is224?7:8;for(int i=0;i<count;++i)for(int j=3;j>=0;--j)out.push_back((h[i]>>(j*8))&255);return out;
}
std::vector<unsigned char> sha1(const std::string &input) {
    uint32_t h[5]={0x67452301,0xefcdab89,0x98badcfe,0x10325476,0xc3d2e1f0};std::vector<unsigned char>m(input.begin(),input.end());uint64_t bits=uint64_t(m.size())*8;m.push_back(0x80);while(m.size()%64!=56)m.push_back(0);for(int i=7;i>=0;--i)m.push_back((bits>>(i*8))&255);
    for(size_t off=0;off<m.size();off+=64){uint32_t w[80];for(int i=0;i<16;++i)w[i]=(uint32_t(m[off+i*4])<<24)|(uint32_t(m[off+i*4+1])<<16)|(uint32_t(m[off+i*4+2])<<8)|m[off+i*4+3];for(int i=16;i<80;++i)w[i]=rotr(w[i-3]^w[i-8]^w[i-14]^w[i-16],31);uint32_t a=h[0],b=h[1],c=h[2],d=h[3],e=h[4];for(int i=0;i<80;++i){uint32_t f,k;if(i<20){f=(b&c)|(~b&d);k=0x5a827999;}else if(i<40){f=b^c^d;k=0x6ed9eba1;}else if(i<60){f=(b&c)|(b&d)|(c&d);k=0x8f1bbcdc;}else{f=b^c^d;k=0xca62c1d6;}uint32_t t=rotr(a,27)+f+e+k+w[i];e=d;d=c;c=rotr(b,2);b=a;a=t;}h[0]+=a;h[1]+=b;h[2]+=c;h[3]+=d;h[4]+=e;}
    std::vector<unsigned char>o;for(auto x:h)for(int j=3;j>=0;--j)o.push_back((x>>(j*8))&255);return o;
}
std::vector<unsigned char> md5(const std::string &input) {
    static constexpr uint32_t K[64]={0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,0xa679438e,0x49b40821,0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391};
    static constexpr unsigned S[64]={7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21};
    uint32_t h[4]={0x67452301,0xefcdab89,0x98badcfe,0x10325476};std::vector<unsigned char>m(input.begin(),input.end());uint64_t bits=uint64_t(m.size())*8;m.push_back(0x80);while(m.size()%64!=56)m.push_back(0);for(int i=0;i<8;++i)m.push_back((bits>>(i*8))&255);
    for(size_t off=0;off<m.size();off+=64){uint32_t w[16];for(int i=0;i<16;++i)w[i]=uint32_t(m[off+i*4])|(uint32_t(m[off+i*4+1])<<8)|(uint32_t(m[off+i*4+2])<<16)|(uint32_t(m[off+i*4+3])<<24);uint32_t a=h[0],b=h[1],c=h[2],d=h[3];for(int i=0;i<64;++i){uint32_t f,g;if(i<16){f=(b&c)|(~b&d);g=i;}else if(i<32){f=(d&b)|(~d&c);g=(5*i+1)%16;}else if(i<48){f=b^c^d;g=(3*i+5)%16;}else{f=c^(b|~d);g=(7*i)%16;}uint32_t t=d;d=c;c=b;b+=rotr(a+f+K[i]+w[g],32-S[i]);a=t;}h[0]+=a;h[1]+=b;h[2]+=c;h[3]+=d;}
    std::vector<unsigned char>o;for(auto x:h)for(int j=0;j<4;++j)o.push_back((x>>(j*8))&255);return o;
}
uint64_t rotr64(uint64_t x, unsigned n) { return (x >> n) | (x << (64 - n)); }
std::vector<unsigned char> sha512(const std::string &input, bool is384) {
    static constexpr uint64_t K[80]={0x428a2f98d728ae22ULL,0x7137449123ef65cdULL,0xb5c0fbcfec4d3b2fULL,0xe9b5dba58189dbbcULL,0x3956c25bf348b538ULL,0x59f111f1b605d019ULL,0x923f82a4af194f9bULL,0xab1c5ed5da6d8118ULL,0xd807aa98a3030242ULL,0x12835b0145706fbeULL,0x243185be4ee4b28cULL,0x550c7dc3d5ffb4e2ULL,0x72be5d74f27b896fULL,0x80deb1fe3b1696b1ULL,0x9bdc06a725c71235ULL,0xc19bf174cf692694ULL,0xe49b69c19ef14ad2ULL,0xefbe4786384f25e3ULL,0x0fc19dc68b8cd5b5ULL,0x240ca1cc77ac9c65ULL,0x2de92c6f592b0275ULL,0x4a7484aa6ea6e483ULL,0x5cb0a9dcbd41fbd4ULL,0x76f988da831153b5ULL,0x983e5152ee66dfabULL,0xa831c66d2db43210ULL,0xb00327c898fb213fULL,0xbf597fc7beef0ee4ULL,0xc6e00bf33da88fc2ULL,0xd5a79147930aa725ULL,0x06ca6351e003826fULL,0x142929670a0e6e70ULL,0x27b70a8546d22ffcULL,0x2e1b21385c26c926ULL,0x4d2c6dfc5ac42aedULL,0x53380d139d95b3dfULL,0x650a73548baf63deULL,0x766a0abb3c77b2a8ULL,0x81c2c92e47edaee6ULL,0x92722c851482353bULL,0xa2bfe8a14cf10364ULL,0xa81a664bbc423001ULL,0xc24b8b70d0f89791ULL,0xc76c51a30654be30ULL,0xd192e819d6ef5218ULL,0xd69906245565a910ULL,0xf40e35855771202aULL,0x106aa07032bbd1b8ULL,0x19a4c116b8d2d0c8ULL,0x1e376c085141ab53ULL,0x2748774cdf8eeb99ULL,0x34b0bcb5e19b48a8ULL,0x391c0cb3c5c95a63ULL,0x4ed8aa4ae3418acbULL,0x5b9cca4f7763e373ULL,0x682e6ff3d6b2b8a3ULL,0x748f82ee5defb2fcULL,0x78a5636f43172f60ULL,0x84c87814a1f0ab72ULL,0x8cc702081a6439ecULL,0x90befffa23631e28ULL,0xa4506cebde82bde9ULL,0xbef9a3f7b2c67915ULL,0xc67178f2e372532bULL,0xca273eceea26619cULL,0xd186b8c721c0c207ULL,0xeada7dd6cde0eb1eULL,0xf57d4f7fee6ed178ULL,0x06f067aa72176fbaULL,0x0a637dc5a2c898a6ULL,0x113f9804bef90daeULL,0x1b710b35131c471bULL,0x28db77f523047d84ULL,0x32caab7b40c72493ULL,0x3c9ebe0a15c9bebcULL,0x431d67c49c100d4cULL,0x4cc5d4becb3e42b6ULL,0x597f299cfc657e2aULL,0x5fcb6fab3ad6faecULL,0x6c44198c4a475817ULL};
    uint64_t h[8]; if(is384){uint64_t q[8]={0xcbbb9d5dc1059ed8ULL,0x629a292a367cd507ULL,0x9159015a3070dd17ULL,0x152fecd8f70e5939ULL,0x67332667ffc00b31ULL,0x8eb44a8768581511ULL,0xdb0c2e0d64f98fa7ULL,0x47b5481dbefa4fa4ULL};std::copy(q,q+8,h);}else{uint64_t q[8]={0x6a09e667f3bcc908ULL,0xbb67ae8584caa73bULL,0x3c6ef372fe94f82bULL,0xa54ff53a5f1d36f1ULL,0x510e527fade682d1ULL,0x9b05688c2b3e6c1fULL,0x1f83d9abfb41bd6bULL,0x5be0cd19137e2179ULL};std::copy(q,q+8,h);}
    std::vector<unsigned char> m(input.begin(),input.end());uint64_t lo=uint64_t(m.size())*8,hi=uint64_t(m.size())>>61;m.push_back(0x80);while(m.size()%128!=112)m.push_back(0);for(int i=7;i>=0;--i)m.push_back((hi>>(i*8))&255);for(int i=7;i>=0;--i)m.push_back((lo>>(i*8))&255);
    for(size_t off=0;off<m.size();off+=128){uint64_t w[80];for(int i=0;i<16;++i){w[i]=0;for(int j=0;j<8;++j)w[i]=(w[i]<<8)|m[off+i*8+j];}for(int i=16;i<80;++i){uint64_t s0=rotr64(w[i-15],1)^rotr64(w[i-15],8)^(w[i-15]>>7),s1=rotr64(w[i-2],19)^rotr64(w[i-2],61)^(w[i-2]>>6);w[i]=w[i-16]+s0+w[i-7]+s1;}uint64_t a=h[0],b=h[1],c=h[2],d=h[3],e=h[4],f=h[5],g=h[6],z=h[7];for(int i=0;i<80;++i){uint64_t S1=rotr64(e,14)^rotr64(e,18)^rotr64(e,41),ch=(e&f)^(~e&g),t1=z+S1+ch+K[i]+w[i],S0=rotr64(a,28)^rotr64(a,34)^rotr64(a,39),maj=(a&b)^(a&c)^(b&c),t2=S0+maj;z=g;g=f;f=e;e=d+t1;d=c;c=b;b=a;a=t1+t2;}h[0]+=a;h[1]+=b;h[2]+=c;h[3]+=d;h[4]+=e;h[5]+=f;h[6]+=g;h[7]+=z;}
    std::vector<unsigned char>o;int count=is384?6:8;for(int i=0;i<count;++i)for(int j=7;j>=0;--j)o.push_back((h[i]>>(j*8))&255);return o;
}
std::vector<unsigned char> blake2b(const std::string &input) {
    static constexpr uint64_t IV[8]={0x6a09e667f3bcc908ULL,0xbb67ae8584caa73bULL,0x3c6ef372fe94f82bULL,0xa54ff53a5f1d36f1ULL,0x510e527fade682d1ULL,0x9b05688c2b3e6c1fULL,0x1f83d9abfb41bd6bULL,0x5be0cd19137e2179ULL};
    static constexpr unsigned char P[12][16]={{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{14,10,4,8,9,15,13,6,1,12,0,2,11,7,5,3},{11,8,12,0,5,2,15,13,10,14,3,6,7,1,9,4},{7,9,3,1,13,12,11,14,2,6,5,10,4,0,15,8},{9,0,5,7,2,4,10,15,14,1,11,12,6,8,3,13},{2,12,6,10,0,11,8,3,4,13,7,5,15,14,1,9},{12,5,1,15,14,13,4,10,0,7,6,3,9,2,8,11},{13,11,7,14,12,1,3,9,5,0,15,4,8,6,2,10},{6,15,14,9,11,3,0,8,12,2,13,7,1,4,10,5},{10,2,8,4,7,6,1,5,15,11,9,14,3,12,13,0},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{14,10,4,8,9,15,13,6,1,12,0,2,11,7,5,3}};
    uint64_t h[8];std::copy(IV,IV+8,h);h[0]^=0x01010040ULL;size_t off=0;uint64_t total=0;
    do {size_t n=std::min<size_t>(128,input.size()-off);bool last=off+n==input.size();unsigned char block[128]{};if(n)std::memcpy(block,input.data()+off,n);total+=n;uint64_t m[16];for(int i=0;i<16;++i){m[i]=0;for(int j=7;j>=0;--j)m[i]=(m[i]<<8)|block[i*8+j];}uint64_t v[16];for(int i=0;i<8;++i){v[i]=h[i];v[i+8]=IV[i];}v[12]^=total;if(last)v[14]=~v[14];auto G=[&](int a,int b,int c,int d,uint64_t x,uint64_t y){v[a]=v[a]+v[b]+x;v[d]=rotr64(v[d]^v[a],32);v[c]+=v[d];v[b]=rotr64(v[b]^v[c],24);v[a]=v[a]+v[b]+y;v[d]=rotr64(v[d]^v[a],16);v[c]+=v[d];v[b]=rotr64(v[b]^v[c],63);};for(int r=0;r<12;++r){const auto*s=P[r];G(0,4,8,12,m[s[0]],m[s[1]]);G(1,5,9,13,m[s[2]],m[s[3]]);G(2,6,10,14,m[s[4]],m[s[5]]);G(3,7,11,15,m[s[6]],m[s[7]]);G(0,5,10,15,m[s[8]],m[s[9]]);G(1,6,11,12,m[s[10]],m[s[11]]);G(2,7,8,13,m[s[12]],m[s[13]]);G(3,4,9,14,m[s[14]],m[s[15]]);}for(int i=0;i<8;++i)h[i]^=v[i]^v[i+8];off+=n;if(last)break;}while(true);
    std::vector<unsigned char>out;for(auto x:h)for(int j=0;j<8;++j)out.push_back((x>>(j*8))&255);return out;
}
uint32_t cksum(const std::string &s) {uint32_t c=0;for(unsigned char b:s){c^=uint32_t(b)<<24;for(int i=0;i<8;++i)c=(c&0x80000000u)?(c<<1)^0x04c11db7u:c<<1;}uint64_t n=s.size();while(n){c^=uint32_t(n&255)<<24;for(int i=0;i<8;++i)c=(c&0x80000000u)?(c<<1)^0x04c11db7u:c<<1;n>>=8;}return ~c;}
std::string digest_value(const std::string&cmd,const std::string&data){if(cmd=="sh256")return hex(sha256(data,false));if(cmd=="sh224")return hex(sha256(data,true));if(cmd=="sh1c")return hex(sha1(data));if(cmd=="md5c")return hex(md5(data));if(cmd=="sh384")return hex(sha512(data,true));if(cmd=="sh512")return hex(sha512(data,false));if(cmd=="b2ck")return hex(blake2b(data));if(cmd=="cksm")return std::to_string(cksum(data))+" "+std::to_string(data.size());if(cmd=="bsum"){uint16_t sum=0;for(unsigned char b:data){sum=(sum>>1)|(sum<<15);sum=(sum+b)&0xffff;}std::ostringstream out;out<<std::setw(5)<<std::setfill('0')<<sum<<std::setfill(' ')<<" "<<(data.size()+1023)/1024;return out.str();}return {};}

}
extern "C" int gcu_checksum_run(const char *command, int argc, char **argv) {
    std::string cmd=command?command:"";bool verify=false;std::vector<std::string>files;
    for(int i=1;i<argc;++i){std::string x=argv[i];if(x=="--check"||x=="-c")verify=true;else if(x=="--help"){std::cout<<"Usage: "<<cmd<<" [FILE...]\nChecksums are for integrity/compatibility, not a substitute for authentication.\n";return 0;}else if(x=="--version"){std::cout<<cmd<<" (GCU) 0.2.0\n";return 0;}else files.push_back(x);}
    if(files.empty())files.push_back("-");int rc=0;
    if(verify){
        for(const auto&manifest:files){bool mok=true;std::string content=read_input(manifest,mok);if(!mok){std::cerr<<cmd<<": cannot read manifest '"<<manifest<<"'\n";rc=1;continue;}std::istringstream lines(content);std::string line;while(std::getline(lines,line)){if(line.empty())continue;std::istringstream row(line);std::string expected,second;row>>expected;if(expected.empty())continue;std::string name;if(cmd=="cksm"||cmd=="bsum"){row>>second;std::getline(row,name);if(name.empty()){std::cerr<<cmd<<": malformed checksum line\n";rc=1;continue;}if(name[0]==' ')name.erase(0,1);expected+=" "+second;}else{std::getline(row,name);while(!name.empty()&&(name[0]==' '||name[0]=='\t'))name.erase(0,1);if(!name.empty()&&name[0]=='*')name.erase(0,1);}if(name.empty()){std::cerr<<cmd<<": malformed checksum line\n";rc=1;continue;}bool ok=true;std::string data=read_input(name,ok);if(!ok){std::cout<<name<<": FAILED open or read\n";rc=1;continue;}std::string actual=digest_value(cmd,data);if(actual==expected)std::cout<<name<<": OK\n";else{std::cout<<name<<": FAILED\n";rc=1;}}}return rc;
    }
    for(const auto &f:files){bool ok=true;std::string data=read_input(f,ok);if(!ok){std::cerr<<cmd<<": cannot read '"<<f<<"'\n";rc=1;continue;}std::string value=digest_value(cmd,data);if(value.empty()){std::cerr<<cmd<<": unsupported digest\n";return 2;}std::cout<<value<<(cmd=="cksm"||cmd=="bsum"?" ":"  ")<<(f=="-"?"-":f)<<"\n";}
    return rc;
}
