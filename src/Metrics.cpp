#include "Metrics.hpp"
#include "FastVector.hpp"

float Metrics::f1_score(FastVector<int>& y_true, int *y_pred){
    int tp = 0;
    int fp = 0;
    int fn = 0;
    int size = y_true.getLength();
    for(int i = 0; i < size; i++){
        if(y_true.get(i) == y_pred[i] && y_pred[i] == 1){
            tp++;
        }
        if(y_true.get(i) != y_pred[i] && y_pred[i] == 1){
            fp++;
        }
        if(y_true.get(i) != y_pred[i] && y_pred[i] == 0){
            fn++;
        }
    }
    if((tp + 0.5 * (fp + fn)) == 0){
        return 0.0;
    }
    float result = tp;
    result /= (tp + 0.5 * (fp + fn));
    return result;
}

float Metrics::precision_score(FastVector<int>& y_true, int *y_pred){
    int tp = 0;
    int fp = 0;
    int size = y_true.getLength();
    for(int i = 0; i < size; i++){
        if(y_true.get(i) == y_pred[i] && y_pred[i] == 1){
            tp++;
        }
        if(y_true.get(i) != y_pred[i] && y_pred[i] == 1){
            fp++;
        }
    }
    if((tp + fp) == 0){
        return 0.0;
    }
    float result = tp;
    result /= (tp + fp);
    return result;
}

float Metrics::recall_score(FastVector<int>& y_true, int *y_pred){
    int tp = 0;
    int fn = 0;
    int size = y_true.getLength();
    for(int i = 0; i < size; i++){
        if(y_true.get(i) == y_pred[i] && y_pred[i] == 1){
            tp++;
        }
        if(y_true.get(i) != y_pred[i] && y_pred[i] == 0){
            fn++;
        }
    }
    if((tp + fn) == 0){
        return 0.0;
    }
    float result = tp;
    result /= (tp + fn);
    return result;
}

float Metrics::accuracy_score(FastVector<int>& y_true, int *y_pred){
    int match = 0;
    int size = y_true.getLength();
    for(int i = 0; i < size; i++){
        if(y_true.get(i) == y_pred[i]){
            match ++;
        }
    }
    float result = (float)((float)(match) / (float)(size));
    return result;
}

void Metrics::printMetrics(FastVector<int>& y_true, int* pred){
  std::cout << "\nF1: " << Metrics::f1_score(y_true, pred) << std::endl;
  std::cout << "Precision: " << Metrics::precision_score(y_true, pred) << std::endl;
  std::cout << "Recall: " << Metrics::recall_score(y_true, pred) << std::endl;
  std::cout << "Accuracy: " << Metrics::accuracy_score(y_true, pred) << std::endl;
}