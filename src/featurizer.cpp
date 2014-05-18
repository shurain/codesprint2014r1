#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "featurizer.h"

using namespace std;


Featurizer::Featurizer() {
    this->num = 0;
}

int Featurizer::add_feature(string feature) {
    // possibly slow because of duplicate check
    auto it = this->index_table.find(feature);
    if (it != this->index_table.end()) {
        return it->second; // pass silently
    }
    this->index_table[feature] = this->num;
    this->num += 1;
    return this->num - 1;
}

vector<int> Featurizer::get_feature_indices(vector<string> features) {
    vector<int> indices;
    for (auto x: features) {
        auto it = this->index_table.find(x);
        if (it == this->index_table.end()) {
            int index = this->add_feature(x);
            indices.push_back(index);
        }
        else {
            indices.push_back(it->second);
        }
    }
    return indices;
}

vector<string> Featurizer::featurize(int i, vector<char> query, vector<int> prev_tag) {
    char q0 = query[i];
    char q1_ = query[i-1];
    char q2_ = query[i-2];
    char q3_ = query[i-3];
    char q4_ = query[i-4];
    char q5_ = query[i-5];
    char q1 = query[i+1];
    char q2 = query[i+2];
    char q3 = query[i+3];
    char q4 = query[i+4];
    char q5 = query[i+5];

    int p1 = prev_tag[0];
    int p2 = prev_tag[1];
    int p3 = prev_tag[2];
    int p4 = prev_tag[3];
    int p5 = prev_tag[4];

    vector<string> feature;
    stringstream ss;

    ss << "bias";
    feature.push_back(ss.str()); ss.str(""); ss.clear();

    // ss << "q3_,q2_,q1_=" << q3_ << "," << q2_ << "," << q1_;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q2_,q0=" << q3_ << "," << q2_ << "," << q0;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q2_,q1=" << q3_ << "," << q2_ << "," << q1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q2_,q2=" << q3_ << "," << q2_ << "," << q2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q2_,q3=" << q3_ << "," << q2_ << "," << q3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q2_,p1=" << q3_ << "," << q2_ << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q2_,p2=" << q3_ << "," << q2_ << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q2_,p3=" << q3_ << "," << q2_ << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q1_,q0=" << q3_ << "," << q1_ << "," << q0;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q1_,q1=" << q3_ << "," << q1_ << "," << q1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q1_,q2=" << q3_ << "," << q1_ << "," << q2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q1_,q3=" << q3_ << "," << q1_ << "," << q3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q1_,p1=" << q3_ << "," << q1_ << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q1_,p2=" << q3_ << "," << q1_ << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q1_,p3=" << q3_ << "," << q1_ << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q0,q1=" << q3_ << "," << q0 << "," << q1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q0,q2=" << q3_ << "," << q0 << "," << q2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q0,q3=" << q3_ << "," << q0 << "," << q3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q0,p1=" << q3_ << "," << q0 << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q0,p2=" << q3_ << "," << q0 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q0,p3=" << q3_ << "," << q0 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q1,q2=" << q3_ << "," << q1 << "," << q2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q1,q3=" << q3_ << "," << q1 << "," << q3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q1,p1=" << q3_ << "," << q1 << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q1,p2=" << q3_ << "," << q1 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q1,p3=" << q3_ << "," << q1 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q2,q3=" << q3_ << "," << q2 << "," << q3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q2,p1=" << q3_ << "," << q2 << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q2,p2=" << q3_ << "," << q2 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q2,p3=" << q3_ << "," << q2 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q3,p1=" << q3_ << "," << q3 << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q3,p2=" << q3_ << "," << q3 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,q3,p3=" << q3_ << "," << q3 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,p1,p2=" << q3_ << "," << p1 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,p1,p3=" << q3_ << "," << p1 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3_,p2,p3=" << q3_ << "," << p2 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q1_,q0=" << q2_ << "," << q1_ << "," << q0;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q1_,q1=" << q2_ << "," << q1_ << "," << q1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q1_,q2=" << q2_ << "," << q1_ << "," << q2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q1_,q3=" << q2_ << "," << q1_ << "," << q3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q1_,p1=" << q2_ << "," << q1_ << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q1_,p2=" << q2_ << "," << q1_ << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q1_,p3=" << q2_ << "," << q1_ << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q0,q1=" << q2_ << "," << q0 << "," << q1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q0,q2=" << q2_ << "," << q0 << "," << q2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q0,q3=" << q2_ << "," << q0 << "," << q3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q0,p1=" << q2_ << "," << q0 << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q0,p2=" << q2_ << "," << q0 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q0,p3=" << q2_ << "," << q0 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q1,q2=" << q2_ << "," << q1 << "," << q2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q1,q3=" << q2_ << "," << q1 << "," << q3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q1,p1=" << q2_ << "," << q1 << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q1,p2=" << q2_ << "," << q1 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q1,p3=" << q2_ << "," << q1 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q2,q3=" << q2_ << "," << q2 << "," << q3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q2,p1=" << q2_ << "," << q2 << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q2,p2=" << q2_ << "," << q2 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q2,p3=" << q2_ << "," << q2 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q3,p1=" << q2_ << "," << q3 << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q3,p2=" << q2_ << "," << q3 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q3,p3=" << q2_ << "," << q3 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,p1,p2=" << q2_ << "," << p1 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,p1,p3=" << q2_ << "," << p1 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,p2,p3=" << q2_ << "," << p2 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1_,q0,q1=" << q1_ << "," << q0 << "," << q1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1_,q0,q2=" << q1_ << "," << q0 << "," << q2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1_,q0,q3=" << q1_ << "," << q0 << "," << q3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1_,q0,p1=" << q1_ << "," << q0 << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1_,q0,p2=" << q1_ << "," << q0 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1_,q0,p3=" << q1_ << "," << q0 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1_,q1,q2=" << q1_ << "," << q1 << "," << q2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1_,q1,q3=" << q1_ << "," << q1 << "," << q3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1_,q1,p1=" << q1_ << "," << q1 << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1_,q1,p2=" << q1_ << "," << q1 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1_,q1,p3=" << q1_ << "," << q1 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1_,q2,q3=" << q1_ << "," << q2 << "," << q3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1_,q2,p1=" << q1_ << "," << q2 << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1_,q2,p2=" << q1_ << "," << q2 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1_,q2,p3=" << q1_ << "," << q2 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1_,q3,p1=" << q1_ << "," << q3 << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1_,q3,p2=" << q1_ << "," << q3 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1_,q3,p3=" << q1_ << "," << q3 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1_,p1,p2=" << q1_ << "," << p1 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1_,p1,p3=" << q1_ << "," << p1 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1_,p2,p3=" << q1_ << "," << p2 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q0,q1,q2=" << q0 << "," << q1 << "," << q2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q0,q1,q3=" << q0 << "," << q1 << "," << q3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q0,q1,p1=" << q0 << "," << q1 << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q0,q1,p2=" << q0 << "," << q1 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q0,q1,p3=" << q0 << "," << q1 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q0,q2,q3=" << q0 << "," << q2 << "," << q3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q0,q2,p1=" << q0 << "," << q2 << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q0,q2,p2=" << q0 << "," << q2 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q0,q2,p3=" << q0 << "," << q2 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q0,q3,p1=" << q0 << "," << q3 << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q0,q3,p2=" << q0 << "," << q3 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q0,q3,p3=" << q0 << "," << q3 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q0,p1,p2=" << q0 << "," << p1 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q0,p1,p3=" << q0 << "," << p1 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q0,p2,p3=" << q0 << "," << p2 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1,q2,q3=" << q1 << "," << q2 << "," << q3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1,q2,p1=" << q1 << "," << q2 << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1,q2,p2=" << q1 << "," << q2 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1,q2,p3=" << q1 << "," << q2 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1,q3,p1=" << q1 << "," << q3 << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1,q3,p2=" << q1 << "," << q3 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1,q3,p3=" << q1 << "," << q3 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1,p1,p2=" << q1 << "," << p1 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1,p1,p3=" << q1 << "," << p1 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1,p2,p3=" << q1 << "," << p2 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2,q3,p1=" << q2 << "," << q3 << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2,q3,p2=" << q2 << "," << q3 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2,q3,p3=" << q2 << "," << q3 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2,p1,p2=" << q2 << "," << p1 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2,p1,p3=" << q2 << "," << p1 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2,p2,p3=" << q2 << "," << p2 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3,p1,p2=" << q3 << "," << p1 << "," << p2;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3,p1,p3=" << q3 << "," << p1 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q3,p2,p3=" << q3 << "," << p2 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "p1,p2,p3=" << p1 << "," << p2 << "," << p3;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();

    ss << "q0=" << q0;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q1_=" << q1_;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q2_=" << q2_;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q3_=" << q3_;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q4_=" << q4_;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q5_=" << q5_;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q1=" << q1;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q2=" << q2;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q3=" << q3;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q4=" << q4;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q5=" << q5;
    feature.push_back(ss.str()); ss.str(""); ss.clear();

    ss << "q2_,q1_,q0=" << q2_ << "," << q1_ << "," << q0;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q1_,q0,q1=" << q1_ << "," << q0 << "," << q1;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q0,q1,q2=" << q0 << "," << q1 << "," <<  q2;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q1_,q0=" << q1_ << "," << q0;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q0,q1=" << q0 << "," << q1;
    feature.push_back(ss.str()); ss.str(""); ss.clear();

    ss << "q3_,q2_,q1_,q0=" << q3_ << "," << q2_ << "," << q1_ << "," << q0;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q2_,q1_,q0,q1=" << q2_ << "," << q1_ << "," << q0 << "," << q1;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q1_,q0,q1,q2=" << q1_ << "," << q0 << "," << q1 << "," << q2;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q0,q1,q2,q3=" << q0 << "," << q1 << "," << q2 << "," << q3;
    feature.push_back(ss.str()); ss.str(""); ss.clear();

    ss << "q4_,q3_,q2_,q1_,q0=" << q4_ << "," << q3_ << "," << q2_ << "," << q1_ << "," << q0;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q3_,q2_,q1_,q0,q1=" << q3_ << "," << q2_ << "," << q1_ << "," << q0 << "," << q1;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q2_,q1_,q0,q1,q2=" << q2_ << "," << q1_ << "," << q0 << "," << q1 << "," << q2;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q1_,q0,q1,q2,q3=" << q1_ << "," << q0 << "," << q1 << "," << q2 << "," << q3;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q0,q1,q2,q3,q4=" << q0 << "," << q1 << "," << q2 << "," << q3 << "," << q4;
    feature.push_back(ss.str()); ss.str(""); ss.clear();

    ss << "p1=" << p1;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "p2=" << p2;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "p3=" << p3;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "p4=" << p4;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "p5=" << p5;
    feature.push_back(ss.str()); ss.str(""); ss.clear();

    ss << "p2,p1,q0=" << p2 << "," << p1 << "," << q0;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "p1,q0,q1=" << p1 << "," << q0 << "," << q1;
    feature.push_back(ss.str()); ss.str(""); ss.clear();

    ss << "q5_,p5,q0=" << q5_ << "," << p5 << "," << q0;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q4_,p4,q0=" << q4_ << "," << p4 << "," << q0;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q3_,p3,q0=" << q3_ << "," << p3 << "," << q0;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q2_,p2,q0=" << q2_ << "," << p2 << "," << q0;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q1_,p1,q0=" << q1_ << "," << p1 << "," << q0;
    feature.push_back(ss.str()); ss.str(""); ss.clear();

    ss << "q2_,q1_,p2,p1,q0=" << q2_ << "," << q1_ << "," << p2 << "," << p1 << "," << q0;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q1_,q0,q1,q2,p1=" << q1_ << "," << q0 << "," << q1 << "," << q2 << "," << p1;
    feature.push_back(ss.str()); ss.str(""); ss.clear();

    // ss << "q3_,q2_,q1_,q0,p3,p2,p1=" << q3_ << "," << q2_ << "," << q1_ << "," << q0 << "," << p3 << "," << p2 << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q2_,q1_,q0,q1,p2,p1=" << q2_ << "," << q1_ << "," << q0 << "," << q1 << "," << p2 << "," << p1;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q1_,q0,q1,q2,p2,p1=" << q2_ << "," << q1_ << "," << q0 << "," << q1 << "," << q2 << "," << p2 << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    ss << "q1_,q0,q1,q2,q3,p1=" << q1_ << "," << q0 << "," << q1 << "," << q2 << "," << q3 << "," << p1;
    feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q1_,q0,q1,q2,q3,q4,p1=" << q1_ << "," << q0 << "," << q1 << "," << q2 << "," << q3 << "," << q4 << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();

    // ss << "q3_,q2_,q1_,q0,q1,p3,p2,p1=" << q3_ << "," << q2_ << "," << q1_ << "," << q0 << "," << q1 << "," << p3 << "," << p2 << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();
    // ss << "q2_,q1_,q0,q1,q2,q3,p2,p1=" << q2_ << "," << q1_ << "," << q0 << "," << q1 << "," << q2 << "," << q3 << "," << p2 << "," << p1;
    // feature.push_back(ss.str()); ss.str(""); ss.clear();


    ss << "p1,q0=" << p1 << "," << q0;
    feature.push_back(ss.str()); ss.str(""); ss.clear();

    return feature;
}
