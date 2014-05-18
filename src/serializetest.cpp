#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "featurizer.h"


int main() {
    Featurizer* feat = new Featurizer();

    ifstream is("feature_index.dat", ios::binary);
    boost::archive::binary_iarchive iar(is);
    iar >> feat;

    vector<string> features;
    features.push_back("q0=a");
    features.push_back("q0=b");
    features.push_back("q0=c");

    cout << "Total feature of : " << feat->num << endl;
    for (auto i: feat->get_feature_indices(features)) {
        cout << i << endl;
    }

    return 0;
}
