#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Node {
public:
	int val;
	Node* left_child;
	Node* right_child;
	Node(int v);
};

Node* construct_decode_tree(ifstream &table);
void decode(Node* decode_tree, char *encoded);
void free_tree(Node *tree);

int main(int argc, char *argv[]) {
	//ifstream code_table("code_table.txt", ifstream::in);
	ifstream code_table(argv[2], ifstream::in);

	Node *decode_tree = construct_decode_tree(code_table);
	code_table.close();

	decode(decode_tree, argv[1]);
	free_tree(decode_tree);

	return 0;
}

Node* construct_decode_tree(ifstream &table) {
	Node *root = new Node(-1);
	Node **temp = &root;
	int num;
	string code;
	char consume;
	
	while (table >> num) {
		table.get(consume);
		getline(table, code);

		for (int i = 0; i < code.length() - 1; i++) {
			if (code[i] == '0') {
				if (!(*temp)->left_child)
					(*temp)->left_child = new Node(-1);

				temp = &((*temp)->left_child);
			}
			else {
				if (!(*temp)->right_child)
					(*temp)->right_child = new Node(-1);
				
				temp = &((*temp)->right_child);
			}
		}

		if (code[code.length() - 1] == '0') {
			if (!(*temp)->left_child)
				(*temp)->left_child = new Node(num);
			else
				(*temp)->left_child->val = num;
		}
		else {
			if (!(*temp)->right_child)
				(*temp)->right_child = new Node(num);
			else
				(*temp)->right_child->val = num;
		}

		temp = &root;
	}

	return root;
}

void decode(Node* decode_tree, char *encoded) {
	//ifstream encoded_input("encoded.bin", ios::binary | ifstream::in);
	ifstream encoded_input(encoded, ios::binary | ifstream::in);
	ofstream decoded_output("decoded.txt", ofstream::out);

	Node *temp = decode_tree;
	unsigned char buffer;
	int count = 0;
	while (encoded_input >> noskipws>> buffer) {
		for (int i = 0; i < 8; i++, buffer <<= 1) {
			if (!temp)
				goto no_matching_code;

			if (temp->val != -1) {
				decoded_output << temp->val << endl;
				temp = decode_tree;
			}
			
			if ((buffer & 128) == 128)
				temp = temp->right_child;
			else
				temp = temp->left_child;
		}
	}
	if (temp->val != -1)
		decoded_output << temp->val << endl;

no_matching_code:
	encoded_input.close();
	decoded_output.close();
}

Node::Node(int v) {
	val = v;
	left_child = NULL;
	right_child = NULL;
}

void free_tree(Node *tree) {
	if (tree) {
		free_tree(tree->left_child);
		free_tree(tree->right_child);
		delete tree;
	}
}