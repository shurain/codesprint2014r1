#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include "naivebayes.h"

using namespace std;


NBUnigramBOW::NBUnigramBOW() {
    this->total = 0;
    this->max_len = 20;
    this->bigram_total = 0;
}

void NBUnigramBOW::fit(vector<string> tokens) {
    string prev = "--";
    for (auto token: tokens) {
        auto it = this->Pw.find(token);
        if (it != this->Pw.end()) {
            this->Pw[token] += 1;
        }
        else {
            this->Pw[token] = 1;
        }
        this->total += 1;

        if (prev == "--") {
            prev = token;
        }
        else {
            vector<string> ngram;
            ngram.push_back(prev);
            ngram.push_back(token);

            auto it = this->bigram.find(ngram);
            if (it != this->bigram.end()) {
                it->second += 1;
            }
            else {
                this->bigram[ngram] = 1;
            }
            prev = token;
        }
    }
}

double NBUnigramBOW::fitness(vector<string> word_list) {
    double total = 0.0;
    for (auto word: word_list) {
        total += log(this->word_prob(word));
    }
    return total;
}

double NBUnigramBOW::word_prob(string word) {
    auto it = this->Pw.find(word);
    if (it != this->Pw.end()) {
        return ((double) it->second) / this->total;
    }
    return 1.0 / (this->total * pow(10, word.size()));
}

vector<string> NBUnigramBOW::predict(string word) {
    auto it = this->memoize_cache.find(word);
    if (it != this->memoize_cache.end()) {
        return it->second;
    }

    vector<string> result;
    if (word == "") {
        result.push_back("");
        return result;
    }
    vector<vector<string>> segments;

    vector<vector<string>> splits = this->split_pairs(word);
    for (auto split: splits) {
        vector<string> segment;
        segment.push_back(split[0]);
        for (auto post: this->predict(split[1])) {
            segment.push_back(post);
        }
        segments.push_back(segment);
    }

    double max_fitness = -100000.0;
    int index = -1;

    for (int i = 0; i < segments.size(); ++i) {
        double segment_fitness = this->fitness(segments[i]);

        if (index == -1) {
            max_fitness = segment_fitness;
            index = i;
        }
        else {
            if (segment_fitness > max_fitness) {
                max_fitness = segment_fitness;
                index = i;
            }
        }
    }

    this->memoize_cache[word] = segments[index];

    return segments[index];
}

vector<vector<string>> NBUnigramBOW::split_pairs(string word) {
    vector<vector<string>> splits;
    for (int i = 0; i < min((int)word.size(), this->max_len); ++i) {
        vector<string> temp;
        temp.push_back(word.substr(0, i+1));
        temp.push_back(word.substr(i+1, word.size()));
        splits.push_back(temp);
    }
    return splits;
}
