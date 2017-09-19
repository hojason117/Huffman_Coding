#pragma once

#include <fstream>

#define DOMAIN_SIZE 1000000

using namespace std;

class Pairing_Heap_Huffman {
private:
	class Node {
	public:
		int val;
		int freq;
		Node* left_child;
		Node* right_child;
		Node(int v, int f);
		Node(int v, int f, Node *l, Node *r);
	};

	class Pairing_Heap {
	private:
		class pairing_node {
		public:
			Node *data;
			int child_count;
			pairing_node *child;
			pairing_node *left_sib;
			pairing_node *right_sib;
			pairing_node(Node *node);
			void add_leftmost_child(pairing_node *n);
		};

		pairing_node *heap;
		int heapSize;

	public:
		Pairing_Heap();
		pairing_node* meld(pairing_node *n0, pairing_node *n1);
		Node* extract_min();
		void insert(Node* in);
		int heap_size();
	};

	Node* huffman_tree;
	void free_tree(Node* tree);
	void print_code(Node *node, ofstream &out, string toPrint);
	void show_WEPL();
	int print_WEPL(Node *node, int height);

public:
	Pairing_Heap_Huffman();
	~Pairing_Heap_Huffman();
	void build_tree_using_pairing_heap(int freq_table[]);
	void output_code_table();
};