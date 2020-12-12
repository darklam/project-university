#ifndef LOGISTIC_HPP
#include "CustomVector.hpp"
#include "FastVector.hpp"
#include <string>
#include "Types.hpp"

class Logistic {
    public:
        Logistic(int size);
        ~Logistic();
        void fit(Vector2DFloat, FastVector<int> *, float);
        int* predict(Vector2DFloat);
    private:
        float b0;
        int size;
        CustomVector<float> *b1;
        CustomVector<float> *loss;
        float learning_rate;
        void update_weights(FastVector<float> *, float, float);
        float calculate_pred(FastVector<float> *);
        float cost_function(int, float);
};


#endif 