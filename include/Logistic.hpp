#ifndef LOGISTIC_HPP
#include "CustomVector.hpp"
#include "FastVector.hpp"
#include <string>
#include "Types.hpp"

class Logistic {
    public:
        Logistic(int size);
        ~Logistic();
        void fit(Vector2DFloat, FastVector<float> *, float);
        int* predict(Vector2DFloat);
    private:
        float b0;
        int size;
        CustomVector<float> *b1;
        CustomVector<float> *loss;
        float e = 2.71828;
        float learning_rate;
        void update_weights(FastVector<float> *, float, float);
        float calculate_pred(FastVector<float> *);
        float cost_function(float, float);
};


#endif 