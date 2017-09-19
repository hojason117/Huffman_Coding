#pragma once

#include <fstream>

#define DOMAIN_SIZE 1000000

using namespace std;

class _4way_Heap_Huffman {
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
	private:
		Node** heap;
		int heapSize;

	public:
		_4Way_Heap(Node** uninit_heap, int size);
		~_4Way_Heap();
		void min_heapify(int index);
		void build_min_heap();
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
	_4way_Heap_Huffman();
	~_4way_Heap_Huffman();
	void build_tree_using_4way_heap(int freq_table[]);
	void output_code_table();
};