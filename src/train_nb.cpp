#include <iostream>
#include <regex>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <string>
#include <chrono>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "naivebayes.h"
#include "util.h"


using namespace std;


int main() {
    NBUnigramBOW* nb = new NBUnigramBOW();

    vector<vector<string> > tokens_list;

    ifstream training_data ("../data/train.txt");
    string line;

    if (training_data.is_open()) {
        while (getline(training_data, line)) {
            transform(line.begin(), line.end(), line.begin(), ::tolower);
            replace_if(line.begin(), line.end(), std::not1(std::ptr_fun(ofinterest)), ' ');
            line.erase(remove_if(line.begin(), line.end(), isapos), line.end());
            line = trim(line);

            if (line.empty()) {
                continue;
            }

            vector<string> tokens;

            vector<string> tokenized_strings = split(line, ' ');
            for (auto t: tokenized_strings) {
                if (t.empty()) {
                    continue;
                }
                tokens.push_back(t);
            }
            tokens_list.push_back(tokens);
        }
    }

    for (auto tokens: tokens_list) {
        nb->fit(tokens);
    }

    cout << "NB unigram count: " << nb->total << endl;

    tokens_list.clear();

    ifstream training_query ("../data/train1.qry");
    if (training_query.is_open()) {
        while (getline(training_query, line)) {
            transform(line.begin(), line.end(), line.begin(), ::tolower);
            replace_if(line.begin(), line.end(), std::not1(std::ptr_fun(ofinterest)), ' ');
            line.erase(remove_if(line.begin(), line.end(), isapos), line.end());
            line = trim(line);

            if (line.empty()) {
                continue;
            }

            vector<string> tokens;

            vector<string> tokenized_strings = split(line, ' ');
            int counter = 0;
            for (auto t: tokenized_strings) {
                counter++;
                if (counter == 1) {
                    continue;
                }

                if (t.empty()) {
                    continue;
                }
                tokens.push_back(t);
            }
            tokens_list.push_back(tokens);
        }
    }

    for (auto tokens: tokens_list) {
        nb->fit(tokens);
    }

    cout << "NB unigram count: " << nb->total << endl;

    ofstream os("naivebayes.dat", ios::binary);
    boost::archive::binary_oarchive nb_ar(os);
    nb_ar << nb;

    return 0;
}

