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
#include "perceptron.h"
#include "segmentor.h"
#include "featurizer.h"
#include "naivebayes.h"
#include "util.h"

using namespace std;


int main() {
    int THRESHOLD = 6;

    NBUnigramBOW* nb = new NBUnigramBOW();

    ifstream is("naivebayes.dat", ios::binary);
    boost::archive::binary_iarchive nb_ar(is);
    nb_ar >> nb;

    AveragedPerceptron* perceptron = new AveragedPerceptron();
    Featurizer* feat = new Featurizer();
    map<string, int> index_table;
    feat->index_table = index_table;

    ifstream feature_is("feature_index.dat", ios::binary);
    boost::archive::binary_iarchive feature_ar(feature_is);
    feature_ar >> feat;

    ifstream perceptron_is("perceptron.dat", ios::binary);
    boost::archive::binary_iarchive perceptron_ar(perceptron_is);
    perceptron_ar >> perceptron;

    Segmentor* seg = new Segmentor(perceptron, feat);

    ifstream test_query ("../data/test1.qry");

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

    ofstream prediction_file ("../prediction/nb_ap_ensemble.txt");

    for (auto x: test_texts) {
        vector<string> prediction = nb->predict(x);
        double loglikelihood = nb->fitness(prediction);

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
                correction.push_back(word);
            }
        }

        // Averaged Perceptron to the rescue!
        // It's better off to resort to averaged perceptron when dealing with non-words
        bool is_equal = false;
        if (prediction.size() < correction.size()) {
            is_equal = std::equal ( prediction.begin(), prediction.end(), correction.begin() );
        }
        else {
            is_equal = std::equal ( correction.begin(), correction.end(), prediction.begin() );
        }

        if (is_equal) {
            // reaching here means no correction was made
            if (prediction.size() == 2 && loglikelihood < -10.0) {
                // if the final prediction was a single word
                // it is quite likely that this word a english non-word
                // but it might still be segmented
                // fallback to averaged perceptron
                correction.clear();
                vector<int> ap_prediction = seg->predict(prediction[0]);
                stringstream ss;
                for (int i = 0; i < ap_prediction.size(); ++i) {
                    ss << prediction[0][i];
                    if (ap_prediction[i] == 1) {
                        correction.push_back(ss.str());
                        ss.str("");
                        ss.clear();
                    }
                }
                if (not ss.str().empty()) {
                    correction.push_back(ss.str());
                    ss.str("");
                    ss.clear();
                }
            }

        }

        bool anomaly_detected = false;
        for (auto word: prediction) {
            // handle single character anomalies
            // only single character that appears many times
            // so that it will affect the result is 'a'
            if (word.size() == 1 && word[0] != 'a') {
                anomaly_detected = true;
            }
        }

        if (anomaly_detected) {
            // fallback to averaged perceptron
            correction.clear();

            vector<int> ap_prediction = seg->predict(x);
            stringstream ss;
            for (int i = 0; i < ap_prediction.size(); ++i) {
                ss << x[i];
                if (ap_prediction[i] == 1) {
                    correction.push_back(ss.str());
                    ss.str("");
                    ss.clear();
                }
            }
            if (not ss.str().empty()) {
                correction.push_back(ss.str());
                ss.str("");
                ss.clear();
            }
        }

        // if (prediction.size() > 5 && loglikelihood < -50.0) {
        //     for (auto w: prediction) {
        //         cout << w << " ";
        //     }
        //     cout << loglikelihood << endl;
        //     // correction.clear();
        //     // vector<int> ap_prediction = seg->predict(x);
        //     // stringstream ss;
        //     // for (int i = 0; i < ap_prediction.size(); ++i) {
        //     //     ss << x[i];
        //     //     if (ap_prediction[i] == 1) {
        //     //         correction.push_back(ss.str());
        //     //         ss.str("");
        //     //         ss.clear();
        //     //     }
        //     // }
        //     // if (not ss.str().empty()) {
        //     //     correction.push_back(ss.str());
        //     //     ss.str("");
        //     //     ss.clear();
        //     // }
        // }






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
};
