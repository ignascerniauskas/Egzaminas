#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <regex>
#include <iomanip>
#include <locale>
#include <set>
#include <cctype>
#include <codecvt>
#include <Windows.h>


//using namespace std;
using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::map;
using std::set;
using std::vector;
using std::pair;
using std::ifstream;
using std::stringstream;
using std::istringstream;
using std::regex;
using std::smatch;
using std::ofstream;
using std::left;
using std::setw;
using std::cerr;
using std::sregex_iterator;



void lt_raides (string& str);
int lt_raidziu_dydis(const string& str);
map<string, int> skaiciuoti_zodzius(const string& tekstas);
map<string, vector<pair<int, int>>> rasti_zodzius(const string& tekstas);
vector<string> findURLs(const string& tekstas);
void Adresas(const string& tekstas, const set<string>& domenas, vector<string>& urls);
void Domenai(set<string>& domenas);
void zodziu_pasikartojimas(const map<string, int>& zodziu_skaicius);
void adresai(const vector<string> urls);
void zodziu_eilutes(const map<string, vector<pair<int, int>>>& zodziu_vieta);
void zodziu_pasikartojimas_ekrane(const map<string, int>& zodziu_skaicius);
void adresai_ekrane(const vector<string> urls);
void zodziu_eilutes_ekrane(const map<string, vector<pair<int, int>>>& zodziu_vieta);
