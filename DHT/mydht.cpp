#include <algorithm>
#include <bits/stdc++.h>
#include <cmath>
#include <vector>

using namespace std;


typedef struct node{
  int N;
  vector<int> finger_table;
  vector<int> key_table;
} node_t;


void print_nodes(vector<node_t> ring){
  cout << endl << " NODE RING ----- " << ring.size() << endl;
  for (auto i:ring){
    cout << "    Node: " << i.N << endl;

    cout << "    Finger table {";
    for (auto j:i.finger_table) { cout << j << ", "; }
    cout << "}" << endl;

    cout << "    Key table {";
    for (auto k:i.key_table) { cout << k << ", "; }
    cout << "}" << endl;
  }
  cout << " -------------- \n\n";
}


void update_finger_table(vector<node_t>& ring){
  // Find ring m from max value
  int m = ceil(log2(ring.back().N));

  for (auto& j: ring){
    j.finger_table.clear();
    for (int k=1; k<=m ;k++){
      int pow_k = 1 << (k-1), pow_m = 1 << m; // 2^(k-1) and 2^m
      int hash = (j.N + pow_k) % pow_m;       // Hashkey: (N+2^(k-1)) mod 2^m
      auto i = ring.begin();
      for (; hash > i->N ;i++){
        if (i == ring.end()) { i = ring.begin(); break; }
      }
      j.finger_table.push_back(i->N);
    }
  }
}


int insert_node(vector<node_t>& ring, int N){
  // Check for duplicates
  for (auto& i: ring) if (i.N == N) { return 1; }

  node_t new_node; new_node.N = N;

  // Find the position to insert
  auto it = lower_bound(ring.begin(), ring.end(), new_node, 
      [](const node_t& a, const node_t& b) { return a.N < b.N; });

  // Redistribute data from the next node
  if (ring.size() > 0){
    auto jt = it;
    if (jt == ring.end()) { jt = ring.begin(); }

    for (auto i = jt->key_table.begin(); i != jt->key_table.end(); ){
      if (*i <= N){
        new_node.key_table.push_back(*jt->key_table.erase(i));
      } else { i++; }
    }

    if (jt == ring.begin())
      for (auto i = jt->key_table.begin(); i != jt->key_table.end(); ){
        if (*i > ring.begin()->N){
          new_node.key_table.push_back(*jt->key_table.erase(i));
        } else { i++; }
      }
  }

  // Insert new node sorted
  ring.insert(it, new_node);

  // Update finger tables
  update_finger_table(ring);

  return 0;
}


int remove_node(vector<node_t>& ring, int N){
  // Find node
  auto it = find_if(ring.begin(), ring.end(), [N](const node_t &node){
      return node.N == N; });

  // Redistribute node data to the next node
  if (ring.size() > 1 && it != ring.end()){
    auto next = it; next++;
    if (next == ring.end()){ next = ring.begin(); }

    for (auto i = it->key_table.begin(); i != it->key_table.end(); ){
      auto pos = lower_bound(next->key_table.begin(), next->key_table.end(), *i);
      next->key_table.insert(pos, *i);
      i++;
    }
  } else { return 0; }  // Node not found

  ring.erase(it);

  // Update finger tables
  update_finger_table(ring);

  return 0;
}


int insert_data(vector<node_t>& ring, int N, int key){
  // Check empty ring
  if (ring.size() < 1) { cout << "Error: Trying to insert in a empty ring" << endl; return 1; }

  // TODO: Insercao errada, arrumar depois
  auto it = ring.begin();
  while (key > it->N){ 
    it++;
    if (it == ring.end()) {
      auto pos = lower_bound(ring.begin()->key_table.begin(), ring.begin()->key_table.end(), key);
      ring.begin()->key_table.insert(pos, key);
      return 0;
    }
  }
  auto pos = lower_bound(it->key_table.begin(), it->key_table.end(), key);
  it->key_table.insert(pos, key);

  //// Check if the node exists
  //auto it = find_if(ring.begin(), ring.end(), [N](const node_t &node){
  //    return node.N == N; });
  //if (it == ring.end()) { return 1; }

  //// Search node finger table
  //for (auto j: it->key_table){

  //}

  return 0;
}


int lookup_data(vector<node_t> ring, int N, int key){

  return 0;
}


int main (int argc, char *argv[]){
  vector<node_t> ring;
  int timestamp = 0;
  char cmd = 0, dummy = 0;
  int Nid = 0, key = 0;

  while (scanf("%d %c %d", &timestamp, &cmd, &Nid) != EOF){
    switch (cmd){
      case 'E':
        scanf(" %c", &dummy);
        cout << "Inserting node " << Nid << endl;
        insert_node(ring, Nid);
        break;

      case 'S':
        scanf(" %c", &dummy);
        cout << "Removing node " << Nid << endl;
        remove_node(ring, Nid);
        break;

      case 'I':
        scanf("%d", &key);
        cout << "Inserting key " << key << endl;
        insert_data(ring, Nid, key);
        break;

      case 'L':
        scanf("%d", &key);
        cout << "Searching for key " << key << endl;
        lookup_data(ring, Nid, key);
        break;

      default:
        cout << "Input error: Finishing program" << endl;
        exit(1);
    }
    print_nodes(ring);
  }

  return 0;
}
