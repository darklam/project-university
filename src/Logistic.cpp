#include "Logistic.hpp"
#include "CustomVector.hpp"
#include "FastVector.hpp"
#include "Types.hpp"
#include "math.h"
#include "cstdlib"

Logistic::Logistic(int size){
    this->b0 = 0;
    this->b1 = new CustomVector<float> ();
    for(int i = 0; i < size; i++){
        this->b1->add(0.0);
    }
    this->size = size;
}

Logistic::~Logistic(){
    delete this->b1;
    delete this->loss;
}

void Logistic::update_weights(FastVector<float> *x, float loss, float pred){
    this->b0 = this->b0 - this->learning_rate * loss * pred * (1 - pred);
    for(int i = 0; i < this->size; i++){
        auto _b1 = this->b1->get(i) + this->learning_rate * loss * pred * (1 - pred) * x->get(i);
        this->b1->set(i, _b1);
    }
}


float Logistic::calculate_pred(FastVector<float> *x){
    float p = this->b0;
    for(int i = 0; i < this->size; i++){
        p += this->b1->get(i) * x->get(i);
    }
    float pred = 1 / (1 + exp(-p));
    return pred;
}

float Logistic::cost_function(float y, float pred){
    return (-y * log(pred) - (1 - y) * log(1 - pred));
}

void Logistic::fit(Vector2DFloat x_train, FastVector<float> *y_train, float learning_rate){
    this->learning_rate = learning_rate;
    int epocs = 1;
    int size = x_train->getLength();
    for(int e = 0; e < epocs; e++){
        for(int i = 0; i < size; i++){
            auto xi = x_train->get(i);
            auto yi = y_train->get(i);
            auto pred = this->calculate_pred(xi);
            auto loss = this->cost_function(yi, pred);
            this->loss->add(loss);
            this->update_weights(xi, loss, pred);
        }
    }
}


int* Logistic::predict(Vector2DFloat x_test){
    int size = x_test->getLength();
    int *predictions = new int(size);
    for(int i = 0; i < size; i++){
        auto xi = x_test->get(i);
        auto pred = this->calculate_pred(xi);
        if(pred >= 0.5){
            predictions[i] = 1;
        }else{
            predictions[i] = 0;
        }
    }
    return predictions;
}