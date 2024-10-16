#include<iostream>
#include<bits/stdc++.h>
#include <vector>
#include <string>
using namespace std;
typedef map<string,vector<int>> clusters_c; // cluster do PLI composto por <distinct_att,vector de ids de tuplas>
typedef map<int,vector<int>,greater<int>> clusters_n; // cluster do PLI composto por <distinct_att,vector de ids de tuplas>
typedef vector<pair<int,pair<int,int>>> predicates; //vetor de predicados composto por <operação,<tx,ty>>
// typedef vector<vector<pair<int,pair<int,int>>>> evidences;
// Generate the predicates using the attributes
// The predicates are only between attributes with the same domain

void print_all(predicates pred_n,predicates pred_c,map<string,clusters_n> plis_n,map<string,clusters_c> plis_c){
  vector<string> op_to_string {
     "=", "!=", "<", "<=", ">", ">="
  };
  for(auto& [op,tuples]: pred_c){  
    auto& [tx, ty] = tuples;
    cout << tx << " " << op_to_string[op] << " " << ty << endl;
  }
  for(auto& [op,tuples]: pred_n){  
    auto& [tx, ty] = tuples;
    cout << tx << " " << op_to_string[op] << " " << ty << endl;
  }
  for(const auto& [att,clusters]: plis_n){
    cout << att << ":" << endl;
    for (const auto& [element,ids] : clusters) {
        cout << element << "->";
      for (const auto& id : ids) {
        cout << id << "|";
    }
    cout << endl;
    }
  }
  for(const auto& [att,clusters]: plis_c){
    cout << att << ":" << endl;
    for (const auto& [element,ids] : clusters) {
        cout << element << "->";
      for (const auto& id : ids) {
        cout << id << "|";
    }
    cout << endl;
    }
  }
}


void gen_predicates(map<string,int>& att_n, map<string,int>& att_c,predicates& pred_n,predicates& pred_c){
  for (auto it1 = att_n.begin(); it1 != att_n.end() ;++it1){
    for (auto it2 = att_n.begin(); it2 != att_n.end() ;++it2){
      for(int k =0;k<6;++k){
        pred_n.push_back({k,{it1->second,it2->second}});  //Adiciona ao vetor de predicados um par <operação,tuplas de att>
      }
    }
  }
  
  for (auto it1 = att_c.begin(); it1 != att_c.end() ;++it1){
    for (auto it2 = att_c.begin(); it2 != att_c.end() ;++it2){
      for(int k =0;k<2;++k){
        pred_c.push_back({k,{it1->second,it2->second}});  //Adiciona ao vetor de predicados um par <operação,tuplas de att>
      }
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
  for (const auto& [att,elements] : data) {
    for (const auto& value : elements) {
      if(att_n.find(att) !=att_n.end()){
        plis_n[att][stoi(value)].push_back(index); 
        index++;
      }
      else{
        if(att_c.find(att) !=att_c.end()){
          plis_c[att][value].push_back(index);
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
