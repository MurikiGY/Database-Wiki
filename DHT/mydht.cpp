#include <algorithm>
#include <bits/stdc++.h>
#include <cmath>
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
  int m = floor(log2(ring.back().N))+1;

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
      [](const node_t& a, const node_t& b) { 
      return a.N < b.N; 
    });

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
  if (it == ring.end()) { return 1; }

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


int insert_key(vector<node_t>& ring, int N, int key) {
  // Check empty ring
  if (ring.size() < 1) { cerr << "Error: Trying to insert in a empty ring" << endl; return 1; }

  vector<int> lookup_nodes;
  int Nit = N;

  // Find node
  auto it = find_if(ring.begin(), ring.end(), [Nit](const node_t &node) {
      return node.N == Nit; 
    });
  if (it == ring.end()) { return 1; }

  if (key == Nit || ring.size() == 1) {
    auto pos = lower_bound(it->key_table.begin(), it->key_table.end(), key);
    it->key_table.insert(pos, key);
    return 0;
  }

  while (1) {
    lookup_nodes.push_back(Nit);

    // Find node
    it = find_if(ring.begin(), ring.end(), [Nit](const node_t &node) {
        return node.N == Nit; 
      });

    int size = lookup_nodes.size();
    if (size > 1) {
      if ( (lookup_nodes[size-2] < key && key <= lookup_nodes[size-1]) ||
           (lookup_nodes[size-2] >= lookup_nodes[size-1] && key <= lookup_nodes[size-1]) ||
           (lookup_nodes[size-2] > lookup_nodes[size-1] && key > lookup_nodes[size-2])  ) {
        auto pos = lower_bound(it->key_table.begin(), it->key_table.end(), key);
        it->key_table.insert(pos, key);
        return 0;
      }
    }

    // Jumps to the next node
    int M = 1 << it->finger_table.size();;
    Nit = it->finger_table[std::__lg(((std::min(M, key) - Nit) + M) % M)];

#ifdef DEBUG
    cout << "Lookup: ";
    for (int i=0; i<(int)lookup_nodes.size() ;i++)
      cout << lookup_nodes[i] << " ";
    cout << endl;
#endif // DEBUG

//    // Check for loops
//    if (count(lookup_nodes.begin(), lookup_nodes.end(), Nit) >= 2) {
//#ifdef DEBUG
//      cout << "Error: loop found in " << Nit << " inserting key " << key << endl;
//#endif
//      return 1;
//    }
  }

  // ---

  //// Find the position to insert
  //auto it = lower_bound(ring.begin(), ring.end(), key, [](const node_t& a, int key) { 
  //    return a.N < key; });

  //if (it == ring.end()){ it = ring.begin(); }
  //auto pos = lower_bound(it->key_table.begin(), it->key_table.end(), key);
  //it->key_table.insert(pos, key);

  // ---

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


int lookup_key(vector<node_t> ring, int N, int key, int timestamp){
  vector<int> lookup_nodes;
  int Nit = N;

  while (1) {
    lookup_nodes.push_back(Nit);

    // Find node
    auto it = find_if(ring.begin(), ring.end(), [Nit](const node_t &node) {
        return node.N == Nit; 
      });
    if (it == ring.end()) { return 1; }

    // Search for key
    auto jt = find(it->key_table.begin(), it->key_table.end(), key);

    // Check if found key
    if (jt != it->key_table.end()) { 
      print_lookup(ring, lookup_nodes, key, timestamp); return 0; 
    }

    // Jumps to the next node
    int M = 1 << it->finger_table.size();;
    Nit = it->finger_table[std::__lg(((std::min(M, key) - Nit) + M) % M)];

    // Check for loops
    if (count(lookup_nodes.begin(), lookup_nodes.end(), Nit) >= 2) {
#ifdef DEBUG
      cout << "Error: key " << key << " not found" << endl;
#endif
      return 1;
    }
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
#ifdef DEBUG
        cout << "Inserting node " << Nid << endl;
#endif
        insert_node(ring, Nid);
        break;

      case 'S':
        scanf(" %c", &dummy);
#ifdef DEBUG
        cout << "Removing node " << Nid << endl;
#endif
        remove_node(ring, Nid);
        break;

      case 'I':
        scanf("%d", &key);
#ifdef DEBUG
        cout << "Inserting key " << key << " in node " << Nid << endl;
#endif
        insert_key(ring, Nid, key);
        break;

      case 'L':
        scanf("%d", &key);
#ifdef DEBUG
        cout << "Searching for key " << key << " in node " << Nid << endl;
#endif
        lookup_key(ring, Nid, key, timestamp);
        break;

      default:
        //cout << "Input error: Finishing program" << endl;
        exit(1);
    }

#ifdef DEBUG
    print_nodes(ring);
#endif
  }

  return 0;
}
