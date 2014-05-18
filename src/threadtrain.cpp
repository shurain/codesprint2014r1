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
#include <thread>
#include <atomic>
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


const string EXP_STR = ".thread_test.";


void multi_fit(Segmentor* seg, vector<string> sentences, vector<vector<int> > labels, chrono::milliseconds time_resource, chrono::system_clock::time_point genesis) {
    for (int i = 0; i < 1000000; ++i) {
        auto showtime = std::chrono::system_clock::now();
        auto time_left = time_resource - chrono::duration_cast<chrono::milliseconds>(showtime - genesis);

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        shuffle(sentences.begin(), sentences.end(), std::default_random_engine(seed));
        shuffle(labels.begin(), labels.end(), std::default_random_engine(seed));

        // this loop will terminate if we spent more than given time resource
        bool no_more_time = seg->fit(sentences, labels, time_left);

        if (no_more_time) {
            seg->model->average();
            break;
        }
    }
}


int main() {
    auto genesis = std::chrono::system_clock::now();

    NBUnigramBOW* nb = new NBUnigramBOW();

    int num_threads = 8;
    vector<Segmentor*> segment_vector;
    for (int i = 0; i < num_threads; ++i) {
        AveragedPerceptron* perceptron = new AveragedPerceptron();
        Featurizer* feat = new Featurizer();
        Segmentor* seg = new Segmentor(perceptron, feat);
        map<string, int> index_table;
        feat->index_table = index_table;

        segment_vector.push_back(seg);
    }

    vector<string> sentences;
    vector<vector<int> > labels;
    vector<vector<string> > tokens_list;

    ifstream training_data ("../data/train.txt");
    string line;
    stringstream ss;

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

    ifstream training_query ("../data/train.qry");
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

    ss.str("");
    ss.clear();

    ss << "models/nb";
    ss << EXP_STR;
    ss << "dat";

    ofstream os(ss.str(), ios::binary);
    boost::archive::binary_oarchive nb_ar(os);
    nb_ar << nb;

    ss.str("");
    ss.clear();

    // Averaged perceptron training
    std::random_device rd;

    /* chrono::milliseconds time_resource (1000 * 60 * 10 - 10000);  // 10 min - 10 sec */

    vector<thread> threads;

    chrono::milliseconds time_resource (1000 * 60 * 10 - 5000);  // 10 min - 5 sec = 9 min 55 sec
    /* chrono::milliseconds time_resource (1000 * 15 - 10000);  // 15 sec - 10 sec = 5 sec */
    for (int i = 0; i < 8; ++i) {
        Segmentor* seg = segment_vector[i];
        threads.push_back(thread(multi_fit, seg, sentences, labels, time_resource, genesis));
    }

    for (auto& t: threads) {
        t.join();
    }
//
//     vector<thread> query_threads;
//
//     auto mini_genesis = std::chrono::system_clock::now();
//     chrono::milliseconds train_query_time_resource (1000 * 30 - 5000);  // 30 sec - 5 sec
//
//     for (int i = 0; i < 8; ++i) {
//         Segmentor* seg = segment_vector[i];
//         query_threads.push_back(thread(multi_fit, seg, query_sentences, query_labels, train_query_time_resource, mini_genesis));
//     }
//
//     for (auto& t: query_threads) {
//         t.join();
//     }

    auto train_done = std::chrono::system_clock::now();

    cout << "Training done in " << chrono::duration_cast<chrono::milliseconds>(train_done - genesis).count() << "ms" <<  endl;

    for (auto seg: segment_vector) {
        cout << "# features : " << seg->feat->num << endl;
    }

    cout << "Dumping model..." << endl;

    for (int i = 0; i < num_threads; ++i) {
        Segmentor* seg = segment_vector[i];

        ss << "models/feature_index";
        ss << EXP_STR;
        ss << i;
        ss << ".dat";
        ofstream ofeature(ss.str(), ios::binary);
        boost::archive::binary_oarchive feature_ar(ofeature);
        feature_ar << seg->feat;

        ss.str("");
        ss.clear();

        ss << "models/perceptron";
        ss << EXP_STR;
        ss << i;
        ss << ".dat";

        ofstream operceptron(ss.str(), ios::binary);
        boost::archive::binary_oarchive perceptron_ar(operceptron);
        perceptron_ar << seg->model;

        ss.str("");
        ss.clear();
    }

    return 0;
};



