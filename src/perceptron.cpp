#include <iostream>
#include <map>
#include <string>
#include "perceptron.h"

using namespace std;


AveragedPerceptron::AveragedPerceptron() {
    // FIXME perhaps this should be dynamically determined
    this->size = 20000000;
    this->counter = 0;

    vector<double> weights(this->size, 0.0);
    this->weights = weights;

    vector<double> accumulates(this->size, 0.0);
    this->accumulates = accumulates;

    vector<int> last_changed_at(this->size, 0);
    this->last_changed_at = last_changed_at;
}

int AveragedPerceptron::learn(vector<int> indices, int label) {
    int guess = this->predict(indices);

    if (guess * label <= 0) {
        for (auto i: indices) {
            int time_delta = this->counter - this->last_changed_at[i];
            this->last_changed_at[i] = this->counter;
            this->accumulates[i] +=  label * time_delta * 1.0;
            this->weights[i] += label * 1.0;
        }
    }
    this->counter += 1;
    return label;
}

int AveragedPerceptron::predict(vector<int> indices) {
    // FIXME need to check out of bound
    double product = 0.0;
    for (auto j: indices) {
        double w = this->weights[j];
        product += w;
    }
    if (product > 0.0) {
        return 1;
    }
    else {
        return -1;
    }
}

double AveragedPerceptron::predict_w(vector<int> indices) {
    double product = 0.0;
    for (auto j: indices) {
        double w = this->weights[j];
        product += w;
    }
    return product;
}

void AveragedPerceptron::average() {
    for (int j = 0; j < this->accumulates.size(); ++j) {
        int time_delta = this->counter - this->last_changed_at[j];
        double total_weight = this->accumulates[j];
        total_weight += time_delta * this->weights[j];
        this->weights[j] = total_weight / this->counter;
    }
    /* cout << "Averaging weights" << endl; */
}
