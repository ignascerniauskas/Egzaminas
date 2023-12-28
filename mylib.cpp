#include "mylib.h"

void lt_raides (string& str) {
    map<char, char> lt = {{'Ą', 'ą'},{'Č', 'č'},{'Ę', 'ę'},{'Ė', 'ė'},{'Į', 'į'},{'Š', 'š'},{'Ų', 'ų'},{'Ū', 'ū'},{'Ž', 'ž'}};

    transform(str.begin(), str.end(), str.begin(),

        [&lt](unsigned char c) -> char {
            auto it = lt.find(c);
            if (it != lt.end()) return it->second;
            return std::tolower(c);});
}

int lt_raidziu_dydis(const std::string& str) {
    int count = 0;
    for (unsigned char c : str) {
        if ((c & 0xC0)!=0x80) {++count;}
    }
    return count;
}

map<string, int> skaiciuoti_zodzius(const string& tekstas) {
    map<string, int> zodziu_skc;
    istringstream iss(tekstas);
    string zodis;

    while (iss >> zodis) {
        zodis.erase(remove_if(zodis.begin(), zodis.end(),
                [](unsigned char c) { return !isalpha(c) && c < 128; }), zodis.end());

        lt_raides(zodis);

        if (!zodis.empty() && zodis.find_first_of(".,-„“–“”!?/}{") == string::npos) {
                zodziu_skc[zodis]++;
        }
    }

    return zodziu_skc;
}

map<string, vector<pair<int, int>>> rasti_zodzius(const string& tekstas) {
    map<string, vector<pair<int, int>>> zodzio_vieta;
    istringstream iss(tekstas);
    string line;
    int eilute = 1;

    while (getline(iss, line)) {
        istringstream lineIss(line);
        string zodis;

        while (lineIss >> zodis) {
            // Patikriname, ar žodis turi bent vieną raidę
            if (any_of(zodis.begin(), zodis.end(), ::isalpha)) {
                // Šaliname nereikalingus simbolius
                zodis.erase(remove_if(zodis.begin(), zodis.end(),
                    [](unsigned char c) { return !isalpha(c) && c < 128; }), zodis.end());

                lt_raides(zodis);

                if (zodzio_vieta.find(zodis) == zodzio_vieta.end()) {
                    zodzio_vieta[zodis] = vector<pair<int, int>>();
                }

                // Tikriname, ar šis žodis jau buvo šioje eilutėje
                bool found = false;
                for (auto& pair : zodzio_vieta[zodis]) {
                    if (pair.first == eilute) {
                        pair.second++; // Jei taip, padidiname pasikartojimų skaičių
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    zodzio_vieta[zodis].push_back({eilute, 1}); // Jei ne, pridedame naują įrašą
                }
            }
        }

        eilute++;
    }

    return zodzio_vieta;
}


void Domenai(set<string>& domenas) { // domenu galuniu sudejimas i set
    ifstream domenaif;
    string tld;
    string failo_pav = "domenai.txt";

    while (true) {
        domenaif.open(failo_pav);
        if (domenaif.is_open()) {
            while (getline(domenaif, tld)) {//skaitymas ie eiles
                if (!tld.empty()) {
                    domenas.insert(tld);
                }
            }
            domenaif.close();
            break;
        }
        else {
            cerr << "Nepavyko atidaryti failo, iveskite pavadinima failo " << failo_pav;
            cin >> failo_pav;

            domenaif.clear();
        }
    }
}

void Adresas(const string& tekstas, const set<string>& domenas, vector<string>& urls) {
    string tldreguliari_israiska;//reguliari israiska
    for (const auto& tld : domenas) {//for ciklas iteruos per domenas
        if (!tldreguliari_israiska.empty()) {//jei netuscia
            tldreguliari_israiska += "|";
        }
        tldreguliari_israiska += "\\." + tld;//pasaliname taska
    }

    regex urlRegex("(https?://)?[a-zA-Z0-9\\-\\.]+(" + tldreguliari_israiska + ")\\b");// turi atitikti tldreguliari israiska su b atskiriame
    auto zodis_pradzia = sregex_iterator(tekstas.begin(), tekstas.end(), urlRegex);
    auto zodis_pabaiga = sregex_iterator();// auto iteruoja nuo pradzios iki pabaigos

    for (sregex_iterator i = zodis_pradzia; i != zodis_pabaiga; ++i) {
        urls.push_back((*i).str());//pridedame su push back'u
    }
}

//------------------------------------------------

void zodziu_pasikartojimas(const map<string, int>& zodziu_skaicius) {
    ofstream outputFile("Zodziu_skaicius.txt");

    outputFile<<"--------------------------------------------------------"<<endl;
    outputFile<<"| Žodis                    | Pasikartojimu skaicius    |" << std::endl;
    outputFile<<"--------------------------------------------------------"<<endl;

    for (const auto& pair : zodziu_skaicius) {
        if (pair.second > 1) {
            int dydis=pair.first.size() - lt_raidziu_dydis(pair.first);
            outputFile<<"| "<<left<<setw(25+dydis)<<pair.first<<"| "<<left<<setw(25)<<pair.second<<left<<" |"<<endl;
        }
    }

    outputFile<<"--------------------------------------------------------"<<endl;
    outputFile.close();
}


void adresai(const vector<string> urls){
    ofstream outputFile("Adresai.txt");
    outputFile<<"------------------------------------------------------"<<endl;
    outputFile<<"|                      Adresai                       |" <<endl;
    outputFile<<"------------------------------------------------------"<<endl;
    for (const string& url : urls) {
        outputFile<<"| "<<setw(50)<<left<<url<<" |"<<endl;
    }
    outputFile<<"------------------------------------------------------"<<endl;

}

void zodziu_eilutes(const map<string, vector<pair<int, int>>>& zodziu_vieta) {
    ofstream outputFile("Zodziu_eilutes.txt");
    outputFile << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
    outputFile << "| Žodis                    | Vieta                                                                                                                                                 |" << endl;
    outputFile << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

    for (const auto& pair : zodziu_vieta) {
        if (pair.second.size() > 1) {
            int dydis = pair.first.size() - lt_raidziu_dydis(pair.first);

            outputFile << "| " << left << setw(25 + dydis) << pair.first << "| " << setw(50);

            std::stringstream eilutes_srautas;
            for (size_t i = 0; i < pair.second.size(); ++i) {
                eilutes_srautas << pair.second[i].first << " (" << pair.second[i].second << "), ";

            }
            string eilute = eilutes_srautas.str();
            outputFile << left<< setw(150)<<eilute<<" |" << endl;
        }
    }
    outputFile << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
}



//--------------------------------------------------------------------------

void zodziu_pasikartojimas_ekrane(const map<string, int>& zodziu_skaicius){
    cout<<"--------------------------------------------------------"<<endl;
    cout<<"| Žodis                    | Pasikartojimu skaicius    |" << std::endl;
    cout<<"--------------------------------------------------------"<<endl;

    for (const auto& pair : zodziu_skaicius) {
        if (pair.second > 1) {
            int dydis=pair.first.size() - lt_raidziu_dydis(pair.first);
            cout<<"| "<<left<<setw(25+dydis)<<pair.first<<"| "<<left<<setw(25)<<pair.second<<left<<" |"<<endl;
            }
    }
    cout<<"--------------------------------------------------------"<<endl;
    cout<<'\n';

}

void adresai_ekrane(const vector<string> urls){
    cout<<"------------------------------------------------------"<<endl;
    cout<<"|                      Adresai                       |" <<endl;
    cout<<"------------------------------------------------------"<<endl;
    for (const string& url : urls) {
        cout<<"| "<<setw(50)<<left<<url<<" |"<<endl;
    }
    cout<<"------------------------------------------------------"<<endl;


}

void zodziu_eilutes_ekrane(const map<string, vector<pair<int, int>>>& zodziu_vieta) {
    cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "| Žodis                    | Vieta                                                                                                                                                 |" << endl;
    cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

    for (const auto& pair : zodziu_vieta) {
        if (pair.second.size() > 1) {
            int dydis = pair.first.size() - lt_raidziu_dydis(pair.first);

            cout << "| " << left << setw(25 + dydis) << pair.first << "| " << setw(50);

            std::stringstream eilutes_srautas;
            for (size_t i = 0; i < pair.second.size(); ++i) {
                eilutes_srautas << pair.second[i].first << " (" << pair.second[i].second << "), ";

            }
            string eilute = eilutes_srautas.str();
            cout << left<< setw(150)<<eilute<<" |" << endl;
        }
    }
    cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
    cout<<'\n';
}
