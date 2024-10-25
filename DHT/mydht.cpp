#include <bits/stdc++.h>
#include <cmath>
#include <vector>

using namespace std;

typedef struct node{
  int N;
  vector<int> finger_table;
  vector<int> key_table;
} node_t;

void update_finger_table(vector<node_t>& ring){
  // Hashkey: (N+2^(k-1)) mod 2^m
  int m = ceil(log2(ring.back().N));

  for (auto& i: ring){
    i.finger_table.clear();
    for (int k=1; k<=m ;k++){
      int pow_k = 1 << (k-1), pow_m = 1 << m;
      int hash = (i.N + pow_k) % pow_m;
      i.finger_table.push_back(hash);
    }
  }
}

int insert_node(vector<node_t>& ring, int N) {
  // Check for duplicates
  for (auto& i: ring) 
    if (i.N == N) { return 1; }

  node_t new_node; new_node.N = N;
  // Find the position to insert
  auto it = lower_bound(ring.begin(), ring.end(), new_node, 
      [](const node_t& a, const node_t& b) {
      return a.N < b.N;
      });

  // Insert new node sorted
  ring.insert(it, new_node);

  // Update finger tables
  update_finger_table(ring);

  // TODO: Redistribute data

  return 0;
}

int remove_node(vector<node_t> &ring, int N){

  // TODO: Redistribute node data

  // Remove node
  auto it = find_if(ring.begin(), ring.end(), [N](const node_t &node) {
      return node.N == N;
      });
  ring.erase(it);

  // Update finger tables
  update_finger_table(ring);

  return 0;
}

int insert_data(vector<node_t> ring_net){

  return 0;
}

int lookup_data(vector<node_t> ring_net){

  return 0;
}

void print_nodes(vector<node_t> ring){
  cout << endl << " NODE RING ----- " << ring.size() << endl;
  for (auto i:ring){
    cout << "    Node: " << i.N << endl;

    cout << "    Finger table {";
    for (auto j:i.finger_table){ cout << j << ", "; }
    cout << "}" << endl;

    cout << "    Key table {";
    for (auto k:i.key_table){ cout << k << ", "; }
    cout << "}" << endl;
  }
  cout << " -------------- \n\n";
}

int main (int argc, char *argv[]) {
  vector<node_t> ring_net;
  int timestamp = 0;
  char cmd = 0, dummy = 0;
  int Nid = 0, data = 0;

  while (scanf("%d %c %d", &timestamp, &cmd, &Nid) != EOF) {
    print_nodes(ring_net);
    switch (cmd) {
      case 'E':
        scanf(" %c", &dummy);
        //cout << "Insert node: " << node <<  endl;
        insert_node(ring_net, Nid);
        break;

      case 'S':
        scanf(" %c", &dummy);
        //cout << "Remove node: " << node << endl;
        remove_node(ring_net, Nid);
        break;

      case 'I':
        scanf("%d", &data);
        //cout << "Insert in node " << node << " the data " << data << endl;
        insert_data(ring_net);
        break;

      case 'L':
        scanf("%d", &data);
        //cout << "Lookup in node " << node << " for data " << data << endl;
        lookup_data(ring_net);
        break;

      default:
        cout << "Input error: Finishing program" << endl;
        exit(1);
    }
  }

  return 0;
}
