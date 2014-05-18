#include <iostream>
#include <regex>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <string>
#include <chrono>
#include "naivebayes.h"
#include "util.h"


using namespace std;


int main() {
    NBUnigramBOW* nb = new NBUnigramBOW();

    vector<vector<string> > tokens_list;

    vector<string> sentences;
    vector<vector<int> > labels;

    ifstream training_data ("../data/train.txt");
    string line;

    auto start = std::chrono::system_clock::now();

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

    auto end = std::chrono::system_clock::now();

    cout << "Data loading time: " << chrono::duration_cast<chrono::seconds>(end - start).count() << endl;

    start = chrono::system_clock::now();
    for (auto tokens: tokens_list) {
        nb->fit(tokens);
    }
    end = std::chrono::system_clock::now();

    cout << "Model fitting time: " << chrono::duration_cast<chrono::seconds>(end - start).count() << endl;

    ifstream test_query ("../data/dummytest.qry");

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
