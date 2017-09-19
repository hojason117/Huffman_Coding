#include "Binary_Heap_Huffman.h"
#include <iostream>
#include <string>
#include <climits>

using namespace std;

Binary_Heap_Huffman::Binary_Heap_Huffman() {
	huffman_tree = NULL;
}

Binary_Heap_Huffman::~Binary_Heap_Huffman() {
	free_tree(huffman_tree);
}

Binary_Heap_Huffman::Node::Node(int v, int f) {
	val = v;
	freq = f;
	left_child = NULL;
	right_child = NULL;
}

Binary_Heap_Huffman::Node::Node(int v, int f, Node *l, Node *r) {
	val = v;
	freq = f;
	left_child = l;
	right_child = r;
}

Binary_Heap_Huffman::Binary_Heap::Binary_Heap(Node** uninit_heap, int size) {
	heapSize = size;
	heap = uninit_heap;
	build_min_heap();
}

Binary_Heap_Huffman::Binary_Heap::~Binary_Heap() {
	delete[] heap;
}

void Binary_Heap_Huffman::Binary_Heap::min_heapify(int index) {
	int min = 0;
	int left = index * 2;
	int right = index * 2 + 1;

	if (left <= heapSize && heap[left]->freq < heap[index]->freq)
		min = left;
	else
		min = index;
	if (right <= heapSize && heap[right]->freq < heap[min]->freq)
		min = right;

	if (min != index) {
		Node *temp = heap[index];
		heap[index] = heap[min];
		heap[min] = temp;

		min_heapify(min);
	}
}

void Binary_Heap_Huffman::Binary_Heap::build_min_heap() {
	for (int i = heapSize; i > 0; i--)
		min_heapify(i);
}

Binary_Heap_Huffman::Node* Binary_Heap_Huffman::Binary_Heap::extract_min() {
	Node* min = heap[1];
	heap[1] = heap[heapSize--];
	min_heapify(1);

	return min;
}

void Binary_Heap_Huffman::Binary_Heap::insert(Node* in) {
	heap[++heapSize] = in;
	int i = heapSize;

	while (i > 1 && heap[i/2]->freq > heap[i]->freq) {
		Node* temp = heap[i/2];
		heap[i/2] = heap[i];
		heap[i] = temp;
		i = i / 2;
	}
}

int Binary_Heap_Huffman::Binary_Heap::heap_size() {
	return heapSize;
}

void Binary_Heap_Huffman::build_tree_using_binary_heap(int freq_table[]) {
	if (huffman_tree)
		free_tree(huffman_tree);

	int count = 0;
	for (int i = 0; i < DOMAIN_SIZE; i++) {
		if (freq_table[i] != 0)
			count++;
	}

	Node** uninit_heap = new Node*[count + 1];
	uninit_heap[0] = new Node(INT_MAX, INT_MAX);

	count = 0;
	for (int i = 0; i < DOMAIN_SIZE; i++) {
		if (freq_table[i] != 0)
			uninit_heap[++count] = new Node(i, freq_table[i]);
	}

	Binary_Heap min_heap(uninit_heap, count);

	while (min_heap.heap_size() > 1) {
		Node *node_1 = min_heap.extract_min();
		Node *node_2 = min_heap.extract_min();

		min_heap.insert(new Node(-1, node_1->freq + node_2->freq, node_1, node_2));
	}

	huffman_tree = min_heap.extract_min();
	
	//cout << huffman_tree->freq << endl;
	//show_WEPL();
}

void Binary_Heap_Huffman::output_code_table() {
	ofstream out;
	out.open("code_table.txt", ofstream::out);

	print_code(huffman_tree, out, "");

	out.close();
}

void Binary_Heap_Huffman::free_tree(Node* tree) {
	if (tree) {
		free_tree(tree->left_child);
		free_tree(tree->right_child);
		delete tree;
	}
}

void Binary_Heap_Huffman::print_code(Node *node, ofstream &out, string toPrint) {
	if (!node->left_child && !node->right_child)
		out << node->val << ' ' << toPrint << endl;

	if(node->left_child)
		print_code(node->left_child, out, toPrint + '0');
	if(node->right_child)
		print_code(node->right_child, out, toPrint + '1');
}

void Binary_Heap_Huffman::show_WEPL() {
	cout << "WEPL: " << print_WEPL(huffman_tree, 0) << endl;
}

int Binary_Heap_Huffman::print_WEPL(Node *node, int height) {
	if (!node->left_child && !node->right_child)
		return height * node->freq;

	return print_WEPL(node->left_child, height + 1) + print_WEPL(node->right_child, height + 1);
}