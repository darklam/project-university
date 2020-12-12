#ifndef METRICS_HPP
#include "FastVector.hpp"

namespace Metrics {
    float f1_score(FastVector<int>* y_true, int* y_pred);
    float recall_score(FastVector<int>* y_true, int* y_pred);
    float precision_score(FastVector<int>* y_true, int* y_pred);
    float accuracy_score(FastVector<int>* y_true, int* y_pred);
}

#endif