#include<iostream>
#include<bits/stdc++.h>
#include <vector>
#include <string>

using namespace std;

int main (int argc, char *argv[]) {
  int leng;
  vector<string> numeric;
  vector<string> categoric;

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


  
  return 0;
}
