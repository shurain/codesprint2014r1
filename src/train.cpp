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
    auto genesis = std::chrono::system_clock::now();

    NBUnigramBOW* nb = new NBUnigramBOW();

    AveragedPerceptron* perceptron = new AveragedPerceptron();
    Featurizer* feat = new Featurizer();
    Segmentor* seg = new Segmentor(perceptron, feat);
    map<string, int> index_table;
    feat->index_table = index_table;

    vector<string> sentences;
    vector<vector<int> > labels;
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
            tokens_list.push_back(tokens);
        }
    }

    // train Naive Bayes model
    for (auto tokens: tokens_list) {
        nb->fit(tokens);
    }

    cout << "NB unigram count: " << nb->total << endl;

    tokens_list.clear();
    vector<string> query_sentences;
    vector<vector<int> > query_labels;

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

            vector<int> label;
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
                vector<int> tmp(t.size()-1, -1);
                label.insert(label.end(), tmp.begin(), tmp.end());
                label.insert(label.end(), 1);

                tokens.push_back(t);
            }
            line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());

            query_sentences.push_back(line);
            query_labels.push_back(label);
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

    // Averaged perceptron training
    std::random_device rd;

    chrono::milliseconds time_resource (1000 * 60 * 9 + 1000 * 30 - 5000);  // 10 min - 10 sec
    /* chrono::milliseconds time_resource (1000);  // 1s */
    /* chrono::milliseconds time_resource (1000 * 15 - 10000);  // 15 sec - 10 sec = 5 sec */

    for (int i = 0; i < 1000000; ++i) {
        auto showtime = std::chrono::system_clock::now();
        auto time_left = time_resource - chrono::duration_cast<chrono::milliseconds>(showtime - genesis);

        // this loop will terminate if we spent more than given time resource
        bool no_more_time = seg->fit(sentences, labels, time_left);

        if (no_more_time) {
            seg->model->average();
            break;
        }

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        shuffle(sentences.begin(), sentences.end(), std::default_random_engine(seed));
        shuffle(labels.begin(), labels.end(), std::default_random_engine(seed));
    }

    cout << "AP feature count: " << seg->feat->num << endl;

    auto mini_genesis = std::chrono::system_clock::now();
    cout << chrono::duration_cast<chrono::milliseconds> (mini_genesis - genesis).count() << endl;
    /* chrono::milliseconds train_query_time_resource (1000 * 70 - 10000);  // 70 sec - 10 sec = 60 sec */
    chrono::milliseconds train_query_time_resource (1000 * 30 - 5000);  // 10 min - 10 sec
    /* chrono::milliseconds train_query_time_resource (1000);  // 70 sec - 10 sec = 60 sec */

    for (int i = 0; i < 1000000; ++i) {
        cout << i << endl;
        auto showtime = std::chrono::system_clock::now();
        auto time_left = train_query_time_resource - chrono::duration_cast<chrono::milliseconds>(showtime - mini_genesis);

        // this loop will terminate if we spent more than given time resource
        bool no_more_time = seg->fit(query_sentences, query_labels, time_left);

        if (no_more_time) {
            seg->model->average();
            break;
        }

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        shuffle(query_sentences.begin(), query_sentences.end(), std::default_random_engine(seed));
        shuffle(query_labels.begin(), query_labels.end(), std::default_random_engine(seed));
    }

    cout << "AP feature count: " << seg->feat->num << endl;

    auto finished = std::chrono::system_clock::now();
    cout << chrono::duration_cast<chrono::milliseconds> (finished - mini_genesis).count() << endl;

    ofstream ofeature("feature_index.dat", ios::binary);
    boost::archive::binary_oarchive feature_ar(ofeature);
    feature_ar << feat;

    ofstream operceptron("perceptron.dat", ios::binary);
    boost::archive::binary_oarchive perceptron_ar(operceptron);
    perceptron_ar << perceptron;

    return 0;
};


