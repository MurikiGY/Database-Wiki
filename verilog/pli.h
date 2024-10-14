#ifndef PLI_H
#define PLI_H

#include<iostream>
#include<bits/stdc++.h>
#include <vector>
#include <string>
using namespace std;

template <typename T>
class PLI {
public:
    struct ClusterEntry {
        T key;                  
        std::vector<int> ids;    
        // // Constructor for ClusterEntry
        // ClusterEntry(T k, const std::vector<int>& tupleIds) : key(k), ids(tupleIds) {
        //     // Sort the ids in ascending order
        //     std::sort(ids.begin(), ids.end());
        // }
    };
    string att;
    std::vector<ClusterEntry> clusters; 

    

    // Constructor
    PLI() {}


    // Add a new cluster entry (key and list of ids)
    void addClusterEntry(T key, const std::vector<int>& tupleIds) {
        clusters.push_back(ClusterEntry(key, tupleIds));
    }

    // Get all clusters
    const std::vector<ClusterEntry>& getClusters() const {
        return clusters;
    }
};

#endif // PLI_H
