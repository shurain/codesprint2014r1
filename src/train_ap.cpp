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
#include "util.h"

using namespace std;


int main() {
    AveragedPerceptron* perceptron = new AveragedPerceptron();
    Featurizer* feat = new Featurizer();
    Segmentor* seg = new Segmentor(perceptron, feat);
    map<string, int> index_table;
    feat->index_table = index_table;

    vector<string> sentences;
    vector<vector<int> > labels;

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

            vector<int> label;
            vector<string> tokens;

            vector<string> tokenized_strings = split(line, ' ');
            for (auto t: tokenized_strings) {
                if (t.empty()) {
                    continue;
                }
                vector<int> tmp(t.size()-1, -1);
                label.insert(label.end(), tmp.begin(), tmp.end());
                label.insert(label.end(), 1);

                tokens.push_back(t);
            }

            line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());

            sentences.push_back(line);
            labels.push_back(label);
        }
    }

    // Averaged perceptron training
    std::random_device rd;

    for (int i = 0; i < 1; ++i) {
        seg->fit(sentences, labels);

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        shuffle(sentences.begin(), sentences.end(), std::default_random_engine(seed));
        shuffle(labels.begin(), labels.end(), std::default_random_engine(seed));
    }

    ofstream ofeature("feature_index.dat", ios::binary);
    boost::archive::binary_oarchive feature_ar(ofeature);
    feature_ar << feat;

    ofstream operceptron("perceptron.dat", ios::binary);
    boost::archive::binary_oarchive perceptron_ar(operceptron);
    perceptron_ar << perceptron;

    return 0;
};

