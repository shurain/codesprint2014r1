#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <chrono>
#include <functional>
#include <cctype>
#include <locale>
#include <algorithm>
#include <regex>
#include <iterator>
#include <vector>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "naivebayes.h"
#include "util.h"

using namespace std;


int main() {
    NBUnigramBOW* nb = new NBUnigramBOW();

    ifstream is("naivebayes.dat", ios::binary);
    boost::archive::binary_iarchive nb_ar(is);
    nb_ar >> nb;

    cout << "Total number of unigrams : " << nb->total << endl;

    ifstream test_query ("../data/dummytest.qry");

    vector<string> test_texts;

    string line;

    if (test_query.is_open()) {
        while (getline(test_query, line)) {
            transform(line.begin(), line.end(), line.begin(), ::tolower);
            line = trim(line);

            if (line.empty()) {
                continue;
            }

            test_texts.push_back(line);
        }
    }

    ofstream prediction_file ("../prediction/nb_cpp.txt");

    int THRESHOLD = 6;

    for (auto x: test_texts) {
        vector<string> prediction = nb->predict(x);

        // bigram correction
        vector<string> correction;
        for (auto word: prediction) {
            vector<vector<string>> splits = nb->split_pairs(word);

            bool corrected = false;
            for (auto pair: splits) {
                vector<string> ngram;
                ngram.push_back(pair[0]);
                ngram.push_back(pair[1]);

                // this will not search the entire space
                // will pick bi-gram greedily
                auto it = nb->bigram.find(ngram);
                if (it != nb->bigram.end()) {
                    if (it->second > THRESHOLD) {
                        // this bigram has to show up plenty of times
                        if ((pair[1].size() != 1) && (pair[1].size() != 2)) {
                            // will ignore single/double characters since they are usually a word
                            // segmented because they contain apostrophes
                            correction.push_back(pair[0]);
                            correction.push_back(pair[1]);
                            corrected = true;
                            break;
                        }
                    }
                }
            }

            if (not corrected) {
                // reaching here means no correction was made
                correction.push_back(word);
            }
        }

        stringstream ss;
        for (auto word: correction) {
            ss << word << " ";
        }
        string result = ss.str();

        prediction_file << trim(result) << endl;
        ss.str("");
        ss.clear();
    }
    return 0;
}
