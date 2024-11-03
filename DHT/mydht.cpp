#include <algorithm>
#include <bits/stdc++.h>
#include <cmath>
#include <iterator>
#include <vector>

using namespace std;


typedef struct node {
  int N;
  vector<int> finger_table;
  vector<int> key_table;
} node_t;


void print_nodes(vector<node_t> ring) {
  cout << endl << " NODE RING ----- " << ring.size() << endl;
  for (auto i:ring){
    int vec_size = 0;
    cout << "    Node: " << i.N << endl;

    vec_size = i.finger_table.size();
    cout << "    Finger table {";
    for (int j=0; j<vec_size-1 ;j++) { cout << i.finger_table[j] << ", "; }
    if (vec_size > 0) { cout << i.finger_table[vec_size-1]; }
    cout << "}" << endl;

    vec_size = i.key_table.size();
    cout << "    Key table {";
    for (int j=0; j<vec_size-1 ;j++) { cout << i.key_table[j] << ", "; }
    if (vec_size > 0) { cout << i.key_table[vec_size-1]; }
    cout << "}" << endl;
    cout << "    ---\n";
  }
  cout << " -------------- \n\n";
}


void update_finger_table(vector<node_t>& ring) {
  // Find ring m from max value
  int m = ceil(log2(ring.back().N));

  for (auto& j: ring){
    j.finger_table.clear();
    for (int k=1; k<=m ;k++) {
      int pow_k = 1 << (k-1), pow_m = 1 << m; // 2^(k-1) and 2^m
      int hash = (j.N + pow_k) % pow_m;       // Hashkey: (N+2^(k-1)) mod 2^m
      auto i = ring.begin();
      while (i != ring.end() && hash > i->N) { ++i; }
      if (i == ring.end()) { i = ring.begin(); }
      j.finger_table.push_back(i->N);
    }
  }
}


int insert_node(vector<node_t>& ring, int N) {
  // Check for duplicates
  for (auto& i: ring) if (i.N == N) { return 1; }

  node_t new_node; new_node.N = N;

  // Find the position to insert
  auto it = lower_bound(ring.begin(), ring.end(), new_node, 
      [](const node_t& a, const node_t& b) { return a.N < b.N; });

  // Redistribute data from the next node
  if (ring.size() > 0) {
    if (it == ring.end()){ // Insert at end
      for (auto i = ring.begin()->key_table.begin(); i != ring.begin()->key_table.end(); )
        if (*i > ring.begin()->N && *i <= N) {
          new_node.key_table.push_back(*i);
          ring.begin()->key_table.erase(i);
        } else { i++; }
    } else if (it == ring.begin()) { // Insert at start
      for (auto i = ring.begin()->key_table.begin(); i != ring.begin()->key_table.end(); )
        if (*i > ring.begin()->N || *i <= N) {
          new_node.key_table.push_back(*i);
          ring.begin()->key_table.erase(i);
        } else { i++; }
    } else { // Insert at middle
      for (auto i = it->key_table.begin(); i != it->key_table.end(); )
        if (*i <= N) {
          new_node.key_table.push_back(*i);
          it->key_table.erase(i);
        } else { i++; }
    }
  }

  // Insert new node sorted
  ring.insert(it, new_node);

  // Update finger tables
  update_finger_table(ring);

  return 0;
}


int remove_node(vector<node_t>& ring, int N) {
  // Find node
  auto it = find_if(ring.begin(), ring.end(), [N](const node_t &node) {
      return node.N == N; 
    });

  // Redistribute node data to the next node
  if (ring.size() > 1 && it != ring.end()) {
    auto next = it; next++;
    if (next == ring.end()){ next = ring.begin(); }

    for (auto i = it->key_table.begin(); i != it->key_table.end(); ) {
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


int insert_data(vector<node_t>& ring, int N, int key) {
  // Check empty ring
  if (ring.size() < 1) { cout << "Error: Trying to insert in a empty ring" << endl; return 1; }

  // TODO: Insercao errada, arrumar depois
  auto it = ring.begin();
  while (key > it->N) { 
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


void print_lookup(vector<node_t> ring, vector<int> lookup_nodes, int key, int timestamp){
  int vec_size;

  // Print lookup nodes
  vec_size = lookup_nodes.size();
  cout << timestamp << " L " << key << " {";
  for (int i=0; i<vec_size-1 ;i++) { cout << lookup_nodes[i] << ","; }
  cout << lookup_nodes[vec_size-1] << "}" << endl;

  // Print finger tables
  for (auto i: lookup_nodes){
    auto it = find_if(ring.begin(), ring.end(), [i](const node_t &node){
        return node.N == i; 
      });
    cout << timestamp << " T "<< it->N << " {";
    vec_size = it->finger_table.size();
    for (int i=0; i<vec_size-1 ;i++)
      cout << it->finger_table[i] << ",";
    if (vec_size > 0) { cout << it->finger_table[vec_size-1] << "}" << endl; }
  }

}


int lookup_data(vector<node_t> ring, int N, int key, int timestamp){
  vector<int> lookup_nodes;
  int Nit = N;

  while (1) {
    lookup_nodes.push_back(Nit);

    // Search for node
    auto it = find_if(ring.begin(), ring.end(), [Nit](const node_t &node) {
        return node.N == Nit; 
      });

    // Search for key
    auto jt = find(it->key_table.begin(), it->key_table.end(), key);

    // Check if found key
    if (jt != it->key_table.end()) { 
      print_lookup(ring, lookup_nodes, key, timestamp); return 0; 
    }

    // Jumps to the next node (two pointers logic)
    auto prev_it = it->finger_table.begin(), next_it = next(prev_it);
    while (next_it!=it->finger_table.end() && Nit<*next_it && key>*next_it) {
      prev_it = next_it;
      next_it++;
    }
    if (next_it == it->finger_table.end()){ Nit = *prev_it; }
    else { Nit = *next_it; }
  }

  return 0;
}


int main (int argc, char *argv[]) {
  vector<node_t> ring;
  int timestamp = 0;
  char cmd = 0, dummy = 0;
  int Nid = 0, key = 0;

  while (scanf("%d %c %d", &timestamp, &cmd, &Nid) != EOF) {
    switch (cmd) {
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
        lookup_data(ring, Nid, key, timestamp);
        break;

      default:
        cout << "Input error: Finishing program" << endl;
        exit(1);
    }
    print_nodes(ring);
  }

  return 0;
}
