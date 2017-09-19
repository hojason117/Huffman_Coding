#pragma once

#include <fstream>
#include <unordered_map>

#define DOMAIN_SIZE 1000000

using namespace std;

class _4way_Cache_Optimized_Heap_Huffman {
public:
	class hash_table_node {
	public:
		unsigned int code;
		int code_length;
		hash_table_node(int c, int l);
	};

	_4way_Cache_Optimized_Heap_Huffman();
	~_4way_Cache_Optimized_Heap_Huffman();
	void build_tree_using_4way_heap(int freq_table[]);
	void output_code_table_n_build_hash_table();
	hash_table_node* get_hash_table_entry(int num);

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

	class _4Way_Heap {
	public:
		_4Way_Heap(Node** uninit_heap, int size);
		~_4Way_Heap();
		void min_heapify(int index);
		void build_min_heap();
		Node* extract_min();
		void insert(Node* in);
		int heap_size();

	private:
		Node** heap;
		int heapSize;
	};

	Node* huffman_tree;
	unordered_map<int, hash_table_node*> hashmap;
	void free_tree(Node *tree);
	void print_code_n_build_hash_table(Node *node, ofstream &out, string toPrint);
};