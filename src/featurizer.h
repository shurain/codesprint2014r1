#ifndef FEATURIZER_H
#define FEATURIZER_H

#include <map>
#include <string>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>

using namespace std;

class Featurizer {
    public:
        friend class boost::serialization::access;
        template<class Archive>
            void serialize(Archive & ar, const unsigned int version) {
                ar & num;
                ar & index_table;
            }
        int num;
        std::map<std::string, int> index_table;

        Featurizer();
        int add_feature(string feature);
        vector<int> get_feature_indices(vector<string> features);
        vector<string> featurize(int i, vector<char> query, vector<int> prev_tag);
};
#endif
