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
    map<string, int> index_table;
    feat->index_table = index_table;

    ifstream feature_is("feature_index.dat", ios::binary);
    boost::archive::binary_iarchive feature_ar(feature_is);
    feature_ar >> feat;

    ifstream perceptron_is("perceptron.dat", ios::binary);
    boost::archive::binary_iarchive perceptron_ar(perceptron_is);
    perceptron_ar >> perceptron;

    Segmentor* seg = new Segmentor(perceptron, feat);

    cout << "Total feature of : " << seg->feat->num << endl;

    ifstream test_query ("../data/dummytest.qry");
    string line;

    vector<string> test_texts;

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

    ofstream prediction_file ("../prediction/ap_cpp.txt");

    for (auto x: test_texts) {
        vector<int> prediction = seg->predict(x);
        stringstream ss;
        for (int i = 0; i < prediction.size(); ++i) {
            ss << x[i];
            if (prediction[i] == 1) {
                ss << " ";
            }
        }
        string result = ss.str();
        prediction_file << trim(result) << endl;
        ss.str("");
        ss.clear();
    }

    return 0;
};
