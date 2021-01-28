#include <iostream>
#include <string>
#include "Iterative.hpp"
#include "FastVector.hpp"
#include "HashMap.hpp"
#include "Clique.hpp"
#include "Logistic.hpp"
#include "DatasetsFunc.hpp"
#include "Pairs.hpp"
#define BATCH_SIZE 1024
#define MAXLENGTH 2000


void Iterative::asVector(std::string str1,
                        std::string str2,
                        float** vectors,
                        HashMap<int>& ids,
                        FastVector<float>& result) {
    HashResult<int> res1;
    ids.get(str1, &res1);
    if (!res1.hasValue) {
        printf("something something %s\n", str1.c_str());
        exit(1);
    }
    HashResult<int> res2;
    ids.get(str2, &res2);
    if (!res2.hasValue) {
        printf("something something %s\n", str2.c_str());
        exit(1);
    }
    auto v1 = vectors[res1.value];
    auto v2 = vectors[res2.value];
    for (int i = 0; i < 1000; i++) {
        result.set(i, v1[i]);
        result.set(i + 1000, v2[i]);
    }
}

bool Iterative::exists(std::string str1, 
                        std::string str2, 
                        HashMap<int>& existing){
    HashResult<int> res1;
    existing.get(str1 + str2, &res1);
    HashResult<int> res2;
    existing.get(str2 + str1, &res2);
    if (res1.hasValue || res2.hasValue){
        return true;
    }
    return false;
}

void Iterative::resolve(Clique* clique, std::string path) {
    FILE* file = fopen(path.c_str(), "r");
    if (!file) {
        printf("Shit just got real, file: %s\n", path.c_str());
        exit(EXIT_FAILURE);
    }
    bool finished = false;
    char* line = nullptr;
    size_t len = 0;
    while (!finished) {
        int length = getline(&line, &len, file);
        if (length == -1) {
            finished = true;
            continue;
        }
        std::string tokens[3];
        Utils::splitStringLite(std::string(line), ",", tokens, 3);
        if (tokens[0].length() == 0 || tokens[1].length() == 0) {
            printf("Something really bad happened\n");
        }
        std::string str1 = tokens[0];
        std::string str2 = tokens[1];
        if (atoi(tokens[2].c_str()) == 1) {
            clique->setPair(str1, str2);
        } else {
            clique->NegativePair(str1, str2);
        }
    }
    if (line != nullptr) {
        free(line);
    }
    fclose(file);
}

void Iterative::train(Clique* clique,
                    Logistic<float>* model,
                    std::string *camera_ids,
                    FastVector<std::string>& dataset,
                    HashMap<int>& existing,
                    HashMap<int>& ids,
                    float** vectors,
                    int total_cameras) {
    float threshold = 0.01;
    float step = 0.1;
    bool flag = true;
    while (threshold < 0.5) {
        std::cout << "Threshod: " << threshold << std::endl;
        if (flag) {
            flag = false;
            int total = dataset.getLength() / BATCH_SIZE;
            FastVector<int> batches(total + 1);
            DatasetsFunc::split_in_batches(batches, dataset.getLength());
            model->fit(dataset, vectors, ids, batches, 0.01, 1);
        } else {
            auto positives = clique->getPositiveEntries();
            auto pos_unique = Pairs::RemoveDup(positives);
            auto negatives = clique->getNegativeEntries();
            auto new_pairs = Pairs::createDataset(pos_unique, negatives, existing);
            auto neg_unique = Pairs::RemoveDup(negatives);
            FastVector<std::string> new_dataset(100000);
            new_pairs->values(new_dataset);
            Pairs::deleteList(neg_unique);
            Pairs::deleteList(pos_unique);
            delete new_pairs;
            int total = dataset.getLength() / BATCH_SIZE;
            FastVector<int> batches(total + 1);
            DatasetsFunc::split_in_batches(batches, new_dataset.getLength());
            model->fit(new_dataset, vectors, ids, batches, 0.01, 1);
        }
        FILE* file;
        file = fopen("nes_set.csv", "w+");
        long m = 0;
        FastVector<float> vec(2000);
        int imin = rand() % (total_cameras - MAXLENGTH);
        int jmin = rand() % (total_cameras - MAXLENGTH);
        for (int i = imin; i < imin + MAXLENGTH; i++) {
            for (int j = jmin; j < jmin + MAXLENGTH; j++) {
                if(i == j) continue;
                if(exists(camera_ids[i], camera_ids[j], existing)) continue;
                vec.forceInit(0.0);
                asVector(camera_ids[i], camera_ids[j], vectors, ids, vec);
                auto prob = model->prob_no_threads(vec);
                if ((prob < threshold) || (prob > 1 - threshold)) {
                    if (prob >= 0.5) {
                        fprintf(file, "%s,%s,%d\n", camera_ids[i].c_str(), camera_ids[j].c_str(), 1);
                    } else {
                        fprintf(file, "%s,%s,%d\n", camera_ids[i].c_str(), camera_ids[j].c_str(), 0);
                    }
                }
            }
        }
        fclose(file);
        resolve(clique, "nes_set.csv");
        threshold += step;
        step *= 2;
        
    }
}