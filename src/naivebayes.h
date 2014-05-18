#ifndef NAIVEBAYES_H
#define NAIVEBAYES_H

#include <map>
#include <vector>
#include <string>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

using namespace std;

class NBUnigramBOW {
    public:
        friend class boost::serialization::access;
        template<class Archive>
            void serialize(Archive & ar, const unsigned int version) {
                ar & total;
                ar & max_len;
                ar & bigram_total;
                ar & Pw;
                ar & bigram;
                ar & memoize_cache;
            }
        int total;
        int max_len;
        int bigram_total;
        map<string, int> Pw;
        map<vector<string>, int> bigram;
        map<string, vector<string>> memoize_cache;

        NBUnigramBOW();
        void fit(vector<string> tokens);
        double fitness(vector<string> word_list);
        double word_prob(string word);
        vector<vector<string>> split_pairs(string word);
        vector<string> predict(string word);
};

#endif
