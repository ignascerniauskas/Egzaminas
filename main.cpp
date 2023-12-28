#include "mylib.h"


int main() {
    setlocale(LC_ALL, "en_US.UTF-8");
    SetConsoleOutputCP(CP_UTF8);

    string failo_pav;
    cout<<"Iveskite failo pavadinima: ";
    cin>> failo_pav;
    ifstream inputFile(failo_pav);
    stringstream buffer;
    buffer << inputFile.rdbuf();
    string tekstas = buffer.str();

    map<string, int> zodziu_skaicius = skaiciuoti_zodzius(tekstas);
    map<string, vector<pair<int, int>>> zodziu_vieta = rasti_zodzius(tekstas);
    vector<string> url;
    set<string> domenas;
    Domenai(domenas);
    Adresas(tekstas, domenas, url);

    char pasirinkimas;
    cout<<"Ar norite rezultatus matyti ekrane-E, ar faile-F? :";
    cin>>pasirinkimas;

    if(toupper(pasirinkimas)=='F'){
        zodziu_pasikartojimas(zodziu_skaicius);
        zodziu_eilutes(zodziu_vieta);
        adresai(url);
    }

    else if(toupper(pasirinkimas)=='E'){
        zodziu_pasikartojimas_ekrane(zodziu_skaicius);
        zodziu_eilutes_ekrane(zodziu_vieta);
        adresai_ekrane(url);
    }

    else{cout<<"Neteisingas pasirinkimas"<<endl;}



    return 0;
}

