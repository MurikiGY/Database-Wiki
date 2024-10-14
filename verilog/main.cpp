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

vector<map<string,string>> build_table(string table_file){
  int leng;
  ifstream file(table_file);
  string line;
  vector<string> headers;
  vector<map<string,string>> data;
  if (getline(file, line)) {
        stringstream headerStream(line);
        string header;
        
        while (getline(headerStream, header, ',')) {
            headers.push_back(header);
        }
    }
  
  while (getline(file, line)) {
        stringstream lineStream(line);
        string cell;
        map<string, string> rows;   
        size_t i = 0;
        while (getline(lineStream, cell, ',')) {
            if (i < headers.size()) {
                rows[headers[i]] = cell;  
            }
            i++;
        }
        data.push_back(rows);
    }
    file.close();

  for (const auto& row : data) {
      for (const auto& pair : row) {
          cout << pair.first << ": " << pair.second << " | ";  
      }
      cout << endl;
  }
  cout<<endl;
  return data;

}




int main (int argc, char *argv[]) {
  int leng;
  vector<string> numeric;
  vector<string> categoric;
  vector<PLI<string>> categoric_plis;
  vector<PLI<int>> numeric_plis;
  vector<map<string,string>> data;
  // Read attributes
  cin >> leng;
  for (int i=0; i<leng ;i++){
    string line, att, type;
    cin >> line;
    att = line.substr(0, line.find(";"));
    type = line.substr(line.find(";")+1, line.length());
    if (type == "numeric") { 
      PLI<int> pli_auxn;
      pli_auxn.att=att;
      numeric.push_back(att); 
      numeric_plis.push_back(pli_auxn);  
      }
    else if (type == "categoric") { 
      PLI<string> pli_auxc;
      pli_auxc.att=att;
      categoric.push_back(att);
      categoric_plis.push_back(pli_auxc);
      }
  }

  gen_predicates(numeric, categoric);
  data = build_table("table.csv");

  return 0;
}
