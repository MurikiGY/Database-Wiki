#include<iostream>
#include<bits/stdc++.h>
#include <vector>
#include <string>
#include "pli.h"
using namespace std;

// Generate the predicates using the attributes
// The predicates are only between attributes with the same domain
void gen_predicates(vector<string> numeric, vector<string> categoric){
  for (int i=0; i<numeric.size() ;i++)
    for (int j=i; j<numeric.size() ;j++){
      cout << numeric[i] << " = " << numeric[j] << endl;
      cout << numeric[i] << " != " << numeric[j] << endl;
      cout << numeric[i] << " < " << numeric[j] << endl;
      cout << numeric[i] << " <= " << numeric[j] << endl;
      cout << numeric[i] << " >  " << numeric[j] << endl;
      cout << numeric[i] << " >= " << numeric[j] << endl;
    }

  cout << endl;

  for (int i=0; i<categoric.size() ;i++)
    for (int j=i; j<categoric.size() ;j++){
      cout << categoric[i] << " = " << categoric[j] << endl;
      cout << categoric[i] << " != " << categoric[j] << endl;
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
        
        while (getline(headerStream, header, ',')) {
            headers.push_back(header);
            data[header] = vector<string>();  
        }
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

void build_pli(vector<string> numeric, vector<string> categoric,map<string,PLI<string>> categoric_plis,map<string,PLI<int>> numeric_plis,map<string,vector<string>> data){
  int index=0;
  for (const auto& pair : data) {
    for (const auto& value : pair.second) {
      auto it = find(numeric.begin(),numeric.end(),pair.first);
      if (it != numeric.end()){
        numeric_plis[pair.first].clusters[stoi(value)].push_back(index);
        index++;
      }
      else{
        it = find(categoric.begin(),categoric.end(),pair.first);
        if (it != categoric.end()){
          categoric_plis[pair.first].clusters[value].push_back(index);
          index++;
        }
      }
    }
    index=0;
  } 
  for(const auto& pair: numeric_plis){
    cout << pair.first << ":" << endl;
    for (const auto& key : pair.second.clusters) {
        cout << key.first << "->";
      for (const auto& value : key.second) {
        cout << value << "|";
    }
    cout << endl;
    }
  }
  for(const auto& pair: categoric_plis){
    cout << pair.first << ":" << endl;
    for (const auto& key : pair.second.clusters) {
        cout << key.first << "->";
      for (const auto& value : key.second) {
        cout << value << "|";
    }
    cout << endl;
    }
  }
}


int main (int argc, char *argv[]) {
  int leng;
  vector<string> numeric;
  vector<string> categoric;
  map<string,PLI<string>> categoric_plis;
  map<string,PLI<int>> numeric_plis;
  map<string,vector<string>> data;
  // Read attributes
  cin >> leng;
  for (int i=0; i<leng ;i++){
    string line, att, type;
    cin >> line;
    att = line.substr(0, line.find(";"));
    type = line.substr(line.find(";")+1, line.length());
    if (type == "numeric") 
      numeric.push_back(att); 
    else if (type == "categoric")
      categoric.push_back(att);
  }

  gen_predicates(numeric, categoric);
  data = build_table("table.csv");
  build_pli(numeric,categoric,categoric_plis,numeric_plis,data);
  return 0;
}
