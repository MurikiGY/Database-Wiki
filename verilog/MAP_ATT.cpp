#include<iostream>
#include<bits/stdc++.h>
#include <vector>
#include <string>
#include "pli.h"
using namespace std;
typedef map<string,vector<int>> clusters_c;
typedef map<int,vector<int>,greater<int>> clusters_n;
// Generate the predicates using the attributes
// The predicates are only between attributes with the same domain
void gen_predicates(map<string,int> att_n, map<string,int> att_c,map<string,vector<string>> pred_n,map<string,vector<string>> pred_c){
  for (int i=0; i<att_n.size() ;i++)
    for (int j=i; j<att_n.size() ;j++){
      cout << att_n[i] << " = " << att_n[j] << endl;
      cout << att_n[i] << " != " << att_n[j] << endl;
      cout << att_n[i] << " < " << att_n[j] << endl;
      cout << att_n[i] << " <= " << att_n[j] << endl;
      cout << att_n[i] << " >  " << att_n[j] << endl;
      cout << att_n[i] << " >= " << att_n[j] << endl;
    }

  cout << endl;

  for (int i=0; i<att_c.size() ;i++)
    for (int j=i; j<att_c.size() ;j++){
      cout << att_c[i] << " = " << att_c[j] << endl;
      cout << att_c[i] << " != " << att_c[j] << endl;
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

    // for (const auto& pair : data) {
    //     cout << "Attribute: " << pair.first << " -> ";
    //     for (const auto& value : pair.second) {
    //         cout << value << " ";
    //     }
    //     cout << endl;
    // }
    return data;
}

void build_pli(map<string,int> att_n, map<string,int> att_c,map<string,clusters_c> plis_c,map<string,clusters_n> plis_n,map<string,vector<string>> data){
  int index=0;
  for (const auto& pair : data) {
    for (const auto& value : pair.second) {
      //USAR O MAP PRA ACHAR O ATRIBUTO
      auto it = find(att_n.begin(),att_n.end(),pair.first);
      if (it != att_n.end()){
        // att_plis_n[pair.first].clusters[stoi(value)].push_back(index);
        plis_n[pair.first][stoi(value)].push_back(index);
        index++;
      }
      else{
        it = find(att_c.begin(),att_c.end(),pair.first);
        if (it != att_c.end()){
          // att_plis_c[pair.first].clusters[value].push_back(index);
          plis_c[pair.first][value].push_back(index);
          index++;
        }
      }
    }
    index=0;
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

  // map<string,PLI<int>> att_plis_n;
  // map<string,PLI<string>> att_plis_c;

int main (int argc, char *argv[]) {
  int leng;
  map<string,int> att_n;
  map<string,int> att_c;
  map<string,vector<string>> pred_n;
  map<string,vector<string>> pred_c;
  map<string,clusters_c> plis_c;
  map<string,clusters_n> plis_n;
  map<string,vector<string>> data;
  // Read attributes
  cin >> leng;
    //FAZER LÓGICA DE INSERIR NO VETOR COM O ID
  
  for (int i=0; i<leng ;i++){
    string line, att, type;
    cin >> line;
    att = line.substr(0, line.find(";"));
    type = line.substr(line.find(";")+1, line.length());
    if (type == "numeric") 
      att_n[i]=att; 
    else if (type == "categoric")
      att_c[i]=att;
  }

  for (const auto& pair : att_n) {
    cout << pair.first <<": " << pair.second<<endl;
  }
  
  for (const auto& pair : att_c) {
    cout << pair.first <<": " << pair.second<<endl;
  }
  

  gen_predicates(att_n, att_c,pred_n,pred_c);
  data = build_table("table.csv");
  build_pli(att_n,att_c,plis_c,plis_n,data);
  return 0;
}
