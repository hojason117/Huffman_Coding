#include "Pairing_Heap_Huffman.h"
#include <iostream>
#include <string>

using namespace std;

Pairing_Heap_Huffman::Pairing_Heap_Huffman() {
	huffman_tree = NULL;
}

Pairing_Heap_Huffman::~Pairing_Heap_Huffman() {
	free_tree(huffman_tree);
}

Pairing_Heap_Huffman::Node::Node(int v, int f) {
	val = v;
	freq = f;
	left_child = NULL;
	right_child = NULL;
}

Pairing_Heap_Huffman::Node::Node(int v, int f, Node *l, Node *r) {
	val = v;
	freq = f;
	left_child = l;
	right_child = r;
}

Pairing_Heap_Huffman::Pairing_Heap::Pairing_Heap() {
	heap = NULL;
	heapSize = 0;
}

Pairing_Heap_Huffman::Pairing_Heap::pairing_node::pairing_node(Node *node) {
	data = node;
	child_count = 0;
	child = NULL;
	left_sib = NULL;
	right_sib = NULL;
}

void Pairing_Heap_Huffman::Pairing_Heap::pairing_node::add_leftmost_child(pairing_node *n) {
	if (child)
		child->left_sib = n;
	n->right_sib = child;
	n->left_sib = this;
	child = n;
	child_count++;
}

Pairing_Heap_Huffman::Pairing_Heap::pairing_node* Pairing_Heap_Huffman::Pairing_Heap::meld(Pairing_Heap_Huffman::Pairing_Heap::pairing_node *n0, Pairing_Heap_Huffman::Pairing_Heap::pairing_node *n1) {
	pairing_node *small = (n0->data->freq < n1->data->freq) ? n0 : n1;
	pairing_node *big = (small == n0) ? n1 : n0;

	small->add_leftmost_child(big);
	
	return small;
}

/*Pairing_Heap_Huffman::Node* Pairing_Heap_Huffman::Pairing_Heap::extract_min() {
	Node* min = heap->data;
	int child_num = heap->child_count;
	pairing_node *children = heap->child;
	delete heap;
	heapSize--;

	if (heapSize == 0) {
		heap = NULL;
		return min;
	}

	vector<pairing_node*> children_subtree;
	children_subtree.reserve(child_num);
	
	while (children) {
		pairing_node *temp = children;
		children = children->right_sib;
		temp->left_sib = NULL;
		temp->right_sib = NULL;
		children_subtree.push_back(temp);
	}
	
	if (children_subtree.size() == 1)
		heap = children_subtree.front();
	else {
		vector<pairing_node*> pass_1;
		pass_1.reserve(child_num / 2);

		for (int i = 0; i + 1 < children_subtree.size(); i += 2)
			pass_1.push_back(meld(children_subtree[i], children_subtree[i + 1]));

		if (children_subtree.size() % 2 == 1)
			pass_1.back() = meld(pass_1.back(), children_subtree.back());

		for (int i = pass_1.size()-1; i > 0; i--)
			pass_1[i-1] = meld(pass_1[i], pass_1[i-1]);

		heap = pass_1[0];
	}
	
	return min;
}*/

Pairing_Heap_Huffman::Node* Pairing_Heap_Huffman::Pairing_Heap::extract_min() {
	Node* min = heap->data;
	int child_num = heap->child_count;
	pairing_node *children = heap->child;
	delete heap;
	heapSize--;

	if (heapSize == 0) {
		heap = NULL;
		return min;
	}

	pairing_node** children_subtree = new pairing_node*[child_num];

	for (int i = 0; i < child_num; i++) {
		pairing_node *temp = children;
		children = children->right_sib;
		temp->left_sib = NULL;
		temp->right_sib = NULL;
		children_subtree[i] = temp;
	}

	if (child_num == 1)
		heap = children_subtree[0];
	else {
		pairing_node** pass_1 = new pairing_node*[child_num / 2];

		for (int i = 0; i + 1 < child_num; i += 2) {
			pairing_node* temp = meld(children_subtree[i], children_subtree[i + 1]);
			pass_1[i/2] = temp;
		}

		if (child_num % 2 == 1)
			pass_1[child_num / 2 - 1] = meld(pass_1[child_num / 2 - 1], children_subtree[child_num - 1]);

		for (int i = child_num / 2 - 1; i > 0; i--)
			pass_1[i - 1] = meld(pass_1[i], pass_1[i - 1]);

		heap = pass_1[0];

		delete[] pass_1;
	}

	delete[] children_subtree;
	
	return min;
}

void Pairing_Heap_Huffman::Pairing_Heap::insert(Node* in) {
	pairing_node *p_node = new pairing_node(in);
	if (heap == NULL)
		heap = p_node;
	else
		heap = meld(heap, p_node);

	heapSize++;
}

int Pairing_Heap_Huffman::Pairing_Heap::heap_size() {
	return heapSize;
}

void Pairing_Heap_Huffman::build_tree_using_pairing_heap(int freq_table[]) {
	if (huffman_tree)
		free_tree(huffman_tree);

	Pairing_Heap p_heap;

	for (int i = 0; i < DOMAIN_SIZE; i++) {
		if (freq_table[i] != 0)
			p_heap.insert(new Node(i, freq_table[i]));
	}

	while (p_heap.heap_size() > 1) {
		Node *node_1 = p_heap.extract_min();
		Node *node_2 = p_heap.extract_min();

		p_heap.insert(new Node(-1, node_1->freq + node_2->freq, node_1, node_2));
	}

	huffman_tree = p_heap.extract_min();

	//cout << huffman_tree->freq << endl;
	//show_WEPL();
}

void Pairing_Heap_Huffman::output_code_table() {
	ofstream out;
	out.open("code_table.txt", ofstream::out);

	print_code(huffman_tree, out, "");

	out.close();
}

void Pairing_Heap_Huffman::free_tree(Node* tree) {
	if (tree) {
		free_tree(tree->left_child);
		free_tree(tree->right_child);
		delete tree;
	}
}

void Pairing_Heap_Huffman::print_code(Node *node, ofstream &out, string toPrint) {
	if (!node->left_child && !node->right_child)
		out << node->val << ' ' << toPrint << endl;

	if (node->left_child)
		print_code(node->left_child, out, toPrint + '0');
	if (node->right_child)
		print_code(node->right_child, out, toPrint + '1');
}

void Pairing_Heap_Huffman::show_WEPL() {
	cout << "WEPL: " << print_WEPL(huffman_tree, 0) << endl;
}

int Pairing_Heap_Huffman::print_WEPL(Node *node, int height) {
	if (!node->left_child && !node->right_child)
		return height * node->freq;

	return print_WEPL(node->left_child, height + 1) + print_WEPL(node->right_child, height + 1);
}