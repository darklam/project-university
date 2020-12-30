#ifndef LOGISTIC_HPP
#include "CustomVector.hpp"
#include "FastVector.hpp"
#include <string>
#include "Types.hpp"
#include "HashMap.hpp"
#include "Utils.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>

template <typename T>
class Logistic {
    public:
        Logistic(int vocab_size){
            this->b0 = 1.0;
            this->b1 = new float[vocab_size];
            this->loss_history = new FastVector<float>(10);
            for(int i = 0; i < vocab_size; i++){
                this->b1[i] = 1.0;
            }
            this->size = vocab_size;
        };

        ~Logistic(){
            delete[] this->b1;
            delete this->loss_history;
        }

        void fit(FastVector<std::string>& dataset, T** vectors, HashMap<int>& ids, int size, float learning_rate, int epocs = 1){
            this->learning_rate = learning_rate;
            for(int e = 0; e < epocs; e++){
                float total_loss = 0.0;
                for(int i = 0; i < size; i++){
                    FastVector<float> vec(this->size);
                    auto row = dataset.get(i);
                    int y_true = this->getVector(row, vectors, ids, vec);
                    auto pred = this->make_pred(vec);
                    auto _loss = this->cost_function(y_true, pred);
                    this->update_weights(vec, y_true, pred);
                    total_loss += _loss;
                }
                std::cout << "Epoch: " << e << " Loss: " << total_loss / size << std::endl;
                this->loss_history->append(total_loss / size);
            }
        }

        void fit(FastVector<float*>& dataset, FastVector<int>& labels, int size, float learning_rate, int epocs = 1){
            this->learning_rate = learning_rate;
            for(int e = 0; e < epocs; e++){
                float total_loss = 0.0;
                for(int i = 0; i < size; i++){
                    auto vec = dataset[i];
                    auto y_true = labels[i];
                    auto pred = this->make_pred(vec);
                    auto _loss = this->cost_function(y_true, pred);
                    this->update_weights(vec, y_true, pred);
                    total_loss += _loss;
                }
                std::cout << "Epoch: " << e << " Loss: " << total_loss / size << std::endl;
                this->loss_history->append(total_loss / size);
            }
        }
        

        int* predict(FastVector<std::string>& dataset, T** vectors, HashMap<int>& ids, int size, int start, FastVector<int>& target){
            int *predictions = new int[size-start];
            for(int i = start; i < size; i++){
                FastVector<float> vec(this->size);
                auto row = dataset.get(i);
                int y_true = this->getVector(row, vectors, ids, vec);
                target.append(y_true);
                auto pred = this->make_pred(vec);
                if(pred >= 0.5){
                    predictions[i - start] = 1;
                }else{
                    predictions[i - start] = 0;
                }
            }
            return predictions;
        }

        int* predict(FastVector<float*> dataset, int end, int start){
            int *predictions = new int[end - start];
            for(int i = start; i < end; i++){
                auto row = dataset[i];
                auto pred = this->make_pred(row);
                if(pred >= 0.5){
                    predictions[i - start] = 1;
                }else{
                    predictions[i - start] = 0;
                }
            }
            return predictions;
        }

        FastVector<float>* getLoss(){
            return this->loss_history;
        }

    private:
        float b0;
        int size;
        float *b1;
        FastVector<float> *loss_history;
        float learning_rate;

        int getVector(std::string row, T** vectors, HashMap<int>& ids, FastVector<float>& result){
            std::string tokens[3];
            Utils::splitStringLite(std::string(row), ",", tokens, 3);
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
            return atoi(tokens[2].c_str());
        }


        void update_weights(FastVector<float>& x, int y_true, float pred){
            this->b0 = this->b0 - this->learning_rate * (pred - y_true);    
            for(int i = 0; i < this->size; i++){
                this->b1[i] = this->b1[i] - this->learning_rate * (pred - y_true) * x.get(i);
            }
        }

        void update_weights(float* x, int y_true, float pred){
            this->b0 = this->b0 - this->learning_rate * (pred - y_true);    
            for(int i = 0; i < this->size; i++){
                this->b1[i] = this->b1[i] - this->learning_rate * (pred - y_true) * x[i];
            }
        }


        float make_pred(FastVector<float>& x){
            float p = this->b0;
            // float p = 0;
            for(int i = 0; i < this->size; i++){
                p += this->b1[i] * x.get(i);
            }
            float sigmoid = 1 / (1 + exp(-p));
            return sigmoid;
        }

        float make_pred(float* x){
            float p = this->b0;
            // float p = 0;
            for(int i = 0; i < this->size; i++){
                p += this->b1[i] * x[i];
            }
            float sigmoid = 1 / (1 + exp(-p));
            return sigmoid;
        }

        float cost_function(int y, float pred){
            return (-y * log(pred) - (1 - y) * log(1 - pred));
        }
};


#endif 