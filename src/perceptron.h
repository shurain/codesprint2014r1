#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <string>
#include <map>
#include <vector>
#include <boost/serialization/vector.hpp>

class AveragedPerceptron {
    public:
        friend class boost::serialization::access;
        template<class Archive>
            void serialize(Archive & ar, const unsigned int version) {
                ar & size;
                ar & counter;
                ar & weights;
                ar & accumulates;
                ar & last_changed_at;
            }
        int size;
        int counter;
        std::vector<double> weights;
        std::vector<double> accumulates;
        std::vector<int> last_changed_at;

        AveragedPerceptron();
        int learn(std::vector<int> indices, int label);
        int predict(std::vector<int> indices);
        double predict_w(std::vector<int> indices);
        void average();
};
#endif
