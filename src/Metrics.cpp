#include "Metrics.hpp"
#include "FastVector.hpp"

float Metrics::f1_score(FastVector<int> *y_true, int *y_pred){
    int tp = 0;
    int fp = 0;
    int fn = 0;
    int size = y_true->getLength();
    for(int i = 0; i < size; i++){
        if(y_true->get(i) == y_pred[i] && y_pred[i] == 1){
            tp++;
        }
        if(y_true->get(i) != y_pred[i] && y_pred[i] == 1){
            fp++;
        }
        if(y_true->get(i) == y_pred[i] && y_pred[i] == 0){
            fn++;
        }
    }
    float result = tp;
    result /= (tp + 0.5 * (fp + fn));
    return result;
}

float Metrics::precision_score(FastVector<int> *y_true, int *y_pred){
    int tp = 0;
    int fp = 0;
    int size = y_true->getLength();
    for(int i = 0; i < size; i++){
        if(y_true->get(i) == y_pred[i] && y_pred[i] == 1){
            tp++;
        }
        if(y_true->get(i) != y_pred[i] && y_pred[i] == 1){
            fp++;
        }
    }
    float result = tp;
    result /= (tp + fp);
    return result;
}

float Metrics::recall_score(FastVector<int> *y_true, int *y_pred){
    int tp = 0;
    int fn = 0;
    int size = y_true->getLength();
    for(int i = 0; i < size; i++){
        if(y_true->get(i) == y_pred[i] && y_pred[i] == 1){
            tp++;
        }
        if(y_true->get(i) != y_pred[i] && y_pred[i] == 0){
            fn++;
        }
    }
    float result = tp;
    result /= (tp + fn);
    return result;
}

float Metrics::accuracy_score(FastVector<int> *y_true, int *y_pred){
    int match = 0;
    int size = y_true->getLength();
    for(int i = 0; i < size; i++){
        if(y_true->get(i) == y_pred[i]){
            match ++;
        }
    }
    float result = match / size;
    return result;
}