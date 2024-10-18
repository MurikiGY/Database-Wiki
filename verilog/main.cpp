#include<iostream>
#include<bits/stdc++.h>
#include <vector>
#include <string>
using namespace std;
typedef map<string,vector<int>> clusters_c; // cluster do PLI composto por <distinct_att,vector de ids de tuplas>
typedef map<int,vector<int>,greater<int>> clusters_n; // cluster do PLI composto por <distinct_att,vector de ids de tuplas>
typedef vector<pair<int,pair<int,int>>> predicates; //vetor de predicados composto por <operação,<Ax,Ay>>
typedef map<pair<int,pair<int,int>>, int> predicates_index; //mapeia cada predicado para seu index no vetor
// #define MAXPREDICATES (1LL<<8)
// #define ISCATEGORIC(X) (X) & (1LL<<7)

//PENSAR EM COMO FAZER UM VETOR SÓ PRAS PLI

// Generate the predicates using the attributes
// The predicates are only between attributes with the same domain
// vector<string> op_to_string {"=", "!=", "<", "<=", ">", ">="};

void print_all(vector<vector<int>> B,predicates P,map<int,clusters_n> plis_n,map<int,clusters_c> plis_c){
  vector<string> op_to_string {"=", "!=", "<", "<=", ">", ">="};
  for(auto& [op,tuples]: P){  
    auto& [Ax, Ay] = tuples;
    cout << Ax << " " << op_to_string[op] << " " << Ay << endl;
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
  // for(auto& x:B){
  //   for(auto& e:x){
  //    cout << e << endl;
  //   }
  //   break;
  // }
}

vector<vector<int>> build_B(predicates& P,int size){
  vector<int> eahead;
  vector<vector<int>>B(size*size);
  int op;
  for(int i=0;i<P.size();++i){
    op = P[i].first;
    if(1 <= op && op <= 3){ // != || < || <=
      eahead.push_back(i);
    } 
  }
  for(auto& e:B){
    e=eahead;
  }
  return B;
  
}

// MUDAR OS ATTS DE <STRING,INT> PARA <INT,STRING>


map<int,vector<int>> build_T(predicates& P,predicates_index& PI,map<int,clusters_c> plis_c,map<int,clusters_n> plis_n){
  int op;
  map<int,vector<int>> T;
  for(auto& [att,cluster]:plis_c){
    for(auto& [element,ids]:cluster){
      vector<int> t;
      for(int i=0;i<ids.size();++i){
        for(int j=i+1;j<ids.size();++j){
        }
      }
    }
    
  }

  // if(op == 0 || op == 4 ){ // = || >
  //   eahead.push_back(i);
  // } 


}


//int check_30() //Check if there is 30% of similar elements

void gen_predicates(map<string,int> att_type,map<string,int> atts,predicates& P,predicates_index& PI){
  int i=0;
  for(auto& [att1,id1]:atts){
    for(auto& [att2,id2]:atts){
      if(att_type[att1] && att_type[att2]){//Se os 2 são categoricos
        //if check_30()
        for(int k =0;k<2;++k){
          P.push_back({k,{id1,id2}});  //Adiciona ao vetor de predicados um par <operação,tuplas de att>
          PI[{k,{id1,id2}}] = i;
          i++;  
        }
      }
      else if(!att_type[att1] && !att_type[att2]){//Se os 2 são numericos
        //if check_30()
        for(int k =0;k<6;++k){
          P.push_back({k,{id1,id2}});  //Adiciona ao vetor de predicados um par <operação,tuplas de att>
          PI[{k,{id1,id2}}] = i;
          i++;  
        }
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
typedef map<string,vector<int>> clusters_c; // cluster do PLI composto por <distinct_att,vector de ids de tuplas>

void build_pli(map<string,int> att_type,map<string,int> atts,map<int,clusters_c>& plis_c,map<int,clusters_n>& plis_n,map<string,vector<string>>& data){
  int i =0;
  for (const auto& [att,elements] : data) {
    for (const auto& value : elements) {
      if(atts.find(att) !=atts.end()){
        if(!att_type[att])
          plis_n[atts[att]][stoi(value)].push_back(i); 
        else
          plis_c[atts[att]][value].push_back(i); 
      i++;
      }
    }
    i=0;
  }
}

int main (int argc, char *argv[]) {
  int leng;
  map<string,int> atts;
  map<int,string> atts_index;
  map<string,int> att_type;
  predicates P;
  predicates_index PI;
  map<int,clusters_c> plis_c;
  map<int,clusters_n> plis_n;
  map<string,vector<string>> data;
  vector<vector<int>> B;
  map<int,vector<int>> T;

  // Read attributes
  cin >> leng;
  for (int i=0; i<leng ;i++){
    string line, att, type;
    cin >> line;
    att = line.substr(0, line.find(";"));
    type = line.substr(line.find(";")+1, line.length());
    if (type == "numeric"){
      atts[att]=i;
      atts_index[i]=att;
      att_type[att]=0;
    }
    else if (type == "categoric"){
      atts[att]=i;
      atts_index[i]=att;
      att_type[att]=1;
    }
  }

  gen_predicates(att_type,atts,P,PI);
  data = build_table("table.csv");
  build_pli(att_type,atts,plis_c,plis_n,data);
  // B = build_B(P,att_n.size()+att_c.size());
  // T = build_T(P,PI,plis_c,plis_n);
  print_all(B,P,plis_n,plis_c);
  return 0;
}
