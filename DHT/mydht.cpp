#include<bits/stdc++.h>
#include <cstdio>
#include<vector>

using namespace std;

typedef struct node{
  u_int N;
  vector<int> finger_table;
  vector<int> key_table;
} node_t;

int insert_node(vector<node_t>& ring, int N) {
    // Check for duplicates
    for (auto& i: ring) 
      if (i.N == N) { return 1; }

    node_t new_node; new_node.N = N;
    // Find the position to insert
    auto it = std::lower_bound(ring.begin(), ring.end(), new_node, 
        [](const node_t& a, const node_t& b) {
            return a.N < b.N;
        });

    // Insert the new node at the found position
    ring.insert(it, new_node);

    // TODO: Atualizar finger tables

    // TODO: Reatribuir os dados

    return 0;
}

int remove_node(vector<node_t> &ring_net, int N){

  // Busca node
  
  // Remove node

  // Atualiza finger tables
  
  // TODO: Redistribuir os dados

  return 0;
}

int insert_data(vector<node_t> ring_net){

  return 0;
}

int lookup_data(vector<node_t> ring_net){

  return 0;
}

void print_nodes(vector<node_t> ring_net){
  cout << endl << " NODE RING ----- " << ring_net.size() << endl;
  for (auto i:ring_net){
    cout << "    Node: " << i.N << endl;

    cout << "    Finger table {";
    for (auto j:i.finger_table){ cout << j << ", "; }

    cout << "}" << endl << "    Key table {";
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
    switch (cmd) {
      case 'E':
        scanf(" %c", &dummy);
        //cout << "Insert node: " << node <<  endl;
        insert_node(ring_net, Nid);
        print_nodes(ring_net);
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
