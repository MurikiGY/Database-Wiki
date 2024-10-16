#include<iostream>
#include<bits/stdc++.h>
#include <vector>
#include <string>
#include "pli.h"
using namespace std;
typedef map<string,vector<int>> clusters_c;
typedef map<int,vector<int>,greater<int>> clusters_n;
typedef map<int,vector<pair<int,int>>> predicates;
// Generate the predicates using the attributes
// The predicates are only between attributes with the same domain

void print_all(predicates pred_n,predicates pred_c,map<string,clusters_n> plis_n,map<string,clusters_c> plis_c){
  for(auto& pair: pred_c){  
    for(auto& pred:pair.second){
      switch(pair.first){
      case 0:
        cout <<pred.first  << " = " << pred.second << endl;
        break;
      case 1:
          cout <<pred.first  << " != " << pred.second << endl;
          break;
      default:
        break;
      }
    }
  }
  for(auto& pair: pred_n){  
    for(auto& pred:pair.second){
      switch(pair.first){
      case 0:
        cout <<pred.first  << " = " << pred.second << endl;
        break;
      case 1:
          cout <<pred.first  << " != " << pred.second << endl;
          break;
      case 2:
        cout <<pred.first  << " < " << pred.second << endl;
          break;
      case 3:
        cout <<pred.first  << " <= " << pred.second << endl;
          break;
      case 4:
        cout <<pred.first  << " > " << pred.second << endl;
          break;
      case 5:
        cout <<pred.first  << " >= " << pred.second << endl;
          break;
      default:
        break;
      }
    }
  }
  for(const auto& pair: plis_n){
    cout << pair.first << ":" << endl;
    for (const auto& key : pair.second) {
        cout << key.first << "->";
      for (const auto& value : key.second) {
        cout << value << "|";
    }
    cout << endl;
    }
  }
  for(const auto& pair: plis_c){
    cout << pair.first << ":" << endl;
    for (const auto& key : pair.second) {
        cout << key.first << "->";
      for (const auto& value : key.second) {
        cout << value << "|";
    }
    cout << endl;
    }
  }
}


void gen_predicates(map<string,int>& att_n, map<string,int>& att_c,predicates& pred_n,predicates& pred_c){
  for (auto it1 = att_n.begin(); it1 != att_n.end() ;++it1){
    for (auto it2 = it1; it2 != att_n.end() ;++it2){
      pred_n[0].push_back({it1->second,it2->second});
      pred_n[1].push_back({it1->second,it2->second});
      pred_n[2].push_back({it1->second,it2->second});
      pred_n[3].push_back({it1->second,it2->second});
      pred_n[4].push_back({it1->second,it2->second});
      pred_n[5].push_back({it1->second,it2->second});
    }
}
  for (auto it1 = att_c.begin(); it1 != att_c.end() ;++it1){
    for (auto it2 = it1; it2 != att_c.end() ;++it2){
      pred_c[0].push_back({it1->second,it2->second});
      pred_c[1].push_back({it1->second,it2->second});
    }
  }
}

map<string,vector<string>> build_table(string table_file){
    ifstream file(table_file);
    string line;
    vector<string> headers;    
    map<string, vector<string>> data;  

    if (getline(file, line)) {
        stringstream headerStream(line);
        string header;
        while (getline(headerStream, header, ','))
            headers.push_back(header);
    }

    while (getline(file, line)) {
        stringstream lineStream(line);
        string cell;
        int i = 0;
        while (getline(lineStream, cell, ',')) {
            if (i < headers.size()) {
                data[headers[i]].push_back(cell);  
            }
            i++;
        }
    }
    file.close();
    return data;
}

void build_pli(map<string,int>& att_n, map<string,int>& att_c,map<string,clusters_c>& plis_c,map<string,clusters_n>& plis_n,map<string,vector<string>>& data){
  int index=0;
  for (const auto& pair : data) {
    for (const auto& value : pair.second) {
      if(att_n.find(pair.first) !=att_n.end()){
        plis_n[pair.first][stoi(value)].push_back(index);
        index++;
      }
      else{
        if(att_c.find(pair.first) !=att_c.end()){
          plis_c[pair.first][value].push_back(index);
          index++;
        }
      }
    }
    index=0;
  } 
}

int main (int argc, char *argv[]) {
  int leng;
  map<string,int> att_n;
  map<string,int> att_c;
  predicates pred_n;
  predicates pred_c;
  map<string,clusters_c> plis_c;
  map<string,clusters_n> plis_n;
  map<string,vector<string>> data;
  // Read attributes
  cin >> leng;
  for (int i=0; i<leng ;i++){
    string line, att, type;
    cin >> line;
    att = line.substr(0, line.find(";"));
    type = line.substr(line.find(";")+1, line.length());
    if (type == "numeric") 
      att_n[att]=i; 
    else if (type == "categoric")
      att_c[att]=i;
  }
  gen_predicates(att_n, att_c,pred_n,pred_c);
  data = build_table("table.csv");
  build_pli(att_n,att_c,plis_c,plis_n,data);
  print_all(pred_n,pred_c,plis_n,plis_c);
  return 0;
}
