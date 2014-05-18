#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "segmentor.h"
#include "featurizer.h"

using namespace std;

Segmentor::Segmentor(AveragedPerceptron* model, Featurizer* feat) {
    this->look_ahead = 5;
    this->model = model;
    this->feat = feat;
}

bool Segmentor::fit(vector<string> sentences, vector<vector<int> > labels, chrono::milliseconds time_left) {
    auto train_start = std::chrono::system_clock::now();

    bool no_more_time = false;

    for (int i = 0; i < sentences.size(); ++i) {
        // dummy sentence
        string sentence = sentences[i];
        vector<int> label = labels[i];

        vector<char> dummy_sentence(sentence.begin(), sentence.end());
        vector<int> dummy_label(label);

        // beginning of sentence
        dummy_label.insert(dummy_label.begin(), 1);

        // beginning of sentences
        dummy_sentence.insert(dummy_sentence.begin(), '!');
        dummy_sentence.insert(dummy_sentence.begin(), '@');
        dummy_sentence.insert(dummy_sentence.begin(), '#');
        dummy_sentence.insert(dummy_sentence.begin(), '$');
        dummy_sentence.insert(dummy_sentence.begin(), '%');

        // end of sentences
        dummy_sentence.push_back('^');
        dummy_sentence.push_back('&');
        dummy_sentence.push_back('*');
        dummy_sentence.push_back('(');
        dummy_sentence.push_back(')');

        for (int j = 0; j < this->look_ahead; ++j) {
            dummy_label.insert(dummy_label.begin(), -1);
            dummy_label.push_back(-1);
        }
        // remove one redundant dummy label
        dummy_label.erase(dummy_label.begin());

        // exploit that beginning/end of sentence will have implicit spaces
        vector<int> prev_tag = {1, -1, -1, -1, -1};

        // move single cursor
        for (int j = 0; j < sentence.size(); ++j) {
            auto feat = this->feat->featurize(j + this->look_ahead, dummy_sentence, prev_tag);
            auto f_indices = this->feat->get_feature_indices(feat);
            int true_label = this->model->learn(f_indices, dummy_label[j + this->look_ahead]);
            prev_tag.insert(prev_tag.begin(), true_label);
            prev_tag.pop_back();
        }
        if (i % 100 == 0) {
            this->model->average();
        }
        auto now = std::chrono::system_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - train_start);
        if (elapsed > time_left) {
            no_more_time = true;
            break;
        }
    }
    this->model->average();
    return no_more_time;
}

vector<int> Segmentor::predict(string sentence) {
    vector<char> dummy_sentence(sentence.begin(), sentence.end());

    // beginning of sentences
    dummy_sentence.insert(dummy_sentence.begin(), '!');
    dummy_sentence.insert(dummy_sentence.begin(), '@');
    dummy_sentence.insert(dummy_sentence.begin(), '#');
    dummy_sentence.insert(dummy_sentence.begin(), '$');
    dummy_sentence.insert(dummy_sentence.begin(), '%');

    // end of sentences
    dummy_sentence.push_back('^');
    dummy_sentence.push_back('&');
    dummy_sentence.push_back('*');
    dummy_sentence.push_back('(');
    dummy_sentence.push_back(')');

    vector<int> prev_tag = {1, -1, -1, -1, -1};

    vector<int> prediction;

    for (int i = 0; i < sentence.size(); ++i) {
        auto feat = this->feat->featurize(i + this->look_ahead, dummy_sentence, prev_tag);
        auto f_indices = this->feat->get_feature_indices(feat);
        int guess = this->model->predict(f_indices);
        // FIXME could possibly do better if greedy search is done
        prediction.push_back(guess);
        prev_tag.insert(prev_tag.begin(), guess);
        prev_tag.pop_back();
    }

    return prediction;
}

vector<double> Segmentor::predict_w(string sentence) {
    vector<char> dummy_sentence(sentence.begin(), sentence.end());

    // beginning of sentences
    dummy_sentence.insert(dummy_sentence.begin(), '!');
    dummy_sentence.insert(dummy_sentence.begin(), '@');
    dummy_sentence.insert(dummy_sentence.begin(), '#');
    dummy_sentence.insert(dummy_sentence.begin(), '$');
    dummy_sentence.insert(dummy_sentence.begin(), '%');

    // end of sentences
    dummy_sentence.push_back('^');
    dummy_sentence.push_back('&');
    dummy_sentence.push_back('*');
    dummy_sentence.push_back('(');
    dummy_sentence.push_back(')');

    vector<int> prev_tag = {1, -1, -1, -1, -1};

    vector<double> prediction;

    for (int i = 0; i < sentence.size(); ++i) {
        auto feat = this->feat->featurize(i + this->look_ahead, dummy_sentence, prev_tag);
        auto f_indices = this->feat->get_feature_indices(feat);
        int guess = this->model->predict(f_indices);
        double guess_w = this->model->predict_w(f_indices);
        // FIXME could possibly do better if greedy search is done
        prediction.push_back(guess_w);
        prev_tag.insert(prev_tag.begin(), guess);
        prev_tag.pop_back();
    }

    return prediction;
}
