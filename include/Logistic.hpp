#ifndef LOGISTIC_HPP
#include "CustomVector.hpp"
#include "FastVector.hpp"
#include <string>
#include "Types.hpp"
#include "HashMap.hpp"
#include "Utils.hpp"
#include <cmath>
#include <cstdlib>

template <typename T>
class Logistic {
    public:
        Logistic(int vocab_size){
            this->b0 = 0;
            this->b1 = new FastVector<float> (vocab_size);
            this->b2 = new FastVector<float> (vocab_size);
            for(int i = 0; i < vocab_size; i++){
                this->b1->append(0.0);
            }
            this->size = vocab_size;
        };

        ~Logistic(){
            delete this->b1;
            delete this->loss;
        }

        void fit(FastVector<std::string>& dataset, T** vectors, HashMap<int>& ids, int size, float learning_rate){
            this->learning_rate = learning_rate;
            int epocs = 1;
            for(int e = 0; e < epocs; e++){
                for(int i = 0; i < size; i++){
                    FastVector<float> vec(this->size);
                    auto row = dataset.get(i);
                    int y_true = this->getVector(row, vectors, ids, vec);
                    auto pred = this->make_pred(vec);
                    auto loss = this->cost_function(y_true, pred);
                    this->update_weights(vec, loss, pred);
                    this->loss->append(loss);
                }
            }
        }

        int* predict(FastVector<std::string>& dataset, T** vectors, HashMap<int>& ids, int size, int start, FastVector<int>& target){
            int *predictions = new int(size);
            for(int i = start; i < size; i++){
                FastVector<float> vec(this->size);
                auto row = dataset.get(i);
                int y_true = this->getVector(row, vectors, ids, vec);
                target.append(y_true);
                auto pred = this->make_pred(vec);
                auto loss = this->cost_function(y_true, pred);
                if(pred >= 0.5){
                    predictions[i] = 1;
                }else{
                    predictions[i] = 0;
                }
            }
        }

    private:
        float b0;
        int size;
        FastVector<float> *b1;
        FastVector<float> *loss;
        float learning_rate;

        int getVector(std::string row, T** vectors, HashMap<int>& ids, FastVector<float>& result){
            std::string tokens[3];
            Utils::splitStringLite(std::string(line), ",", tokens, 3);
            HashResult<int> res1;
            ids.get(tokens[0], &res1);
            if(!res1.hasValue){
                printf("something bad\n");
                exit(1);
            }
            HashResult<int> res2;
            ids.get(tokens[1], &res2);
            if(!res2.hasValue){
                printf("something bad\n");
                exit(1);
            }
            auto v1 = vectors[res1.value];
            auto v2 = vectors[res2.value];
            for(int i = 0; i < this->size; i++){
                float min = v1[i] + v2[i];
                min /= 2;
                result.append(min);
            }
            return atoi(tokens[2]);

        }
        void update_weights(FastVector<float>& x, float loss, float pred){
            this->b0 = this->b0 - this->learning_rate * loss * pred * (1 - pred);
            for(int i = 0; i < this->size; i++){
                auto _b1 = this->b1->get(i) + this->learning_rate * loss * pred * (1 - pred) * x.get(i);
                this->b1->set(i, _b1);
            }
        }

        float make_pred(FastVector<float>& x){
            float p = this->b0;
            for(int i = 0; i < this->size; i++){
                p += this->b1->get(i) * x.get(i);
            }
            float pred = 1 / (1 + exp(-p));
            return pred;
        }

        float cost_function(int y, float pred){
            return (-y * log(pred) - (1 - y) * log(1 - pred));
        }
};


#endif 