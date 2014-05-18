#ifndef SEGMENTOR_H
#define SEGMENTOR_H

#include <vector>
#include <string>
#include <chrono>
#include "perceptron.h"
#include "featurizer.h"

using namespace std;

class Segmentor {
    public:
        int look_ahead;
        AveragedPerceptron* model;
        Featurizer* feat;

        Segmentor(AveragedPerceptron* model, Featurizer* feat);
        vector<string> featurize(int i, vector<char> query, vector<int> prev_tag);
        bool fit(vector<string> sentences, vector<vector<int> > labels, chrono::milliseconds time_left);
        vector<int> predict(string sentence);
        vector<double> predict_w(string sentences);
};


#endif
