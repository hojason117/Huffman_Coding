#include <iostream>
#include <fstream>
#include <string>
#include "4_Way_Cache_Optimized_Heap_Huffman.h"

#define DOMAIN_SIZE 1000000

using namespace std;

void construct_freq_table(ifstream &input, int freq_table[]);
void encode(ifstream &input_file);
int output_code(int num, _4way_Cache_Optimized_Heap_Huffman *_4H, ofstream &out);
int write_bits(int bit, ofstream &out);
void flush_bits(int zeros_to_append, ofstream &out);

int main(int argc, char *argv[]) {
	std::ios::sync_with_stdio(false);

	ifstream input;
	//input.open("sample_input_large.txt", ifstream::in);
	//input.open("sample_input_small.txt", ifstream::in);
	input.open(argv[1], ifstream::in);

	encode(input);

	return 0;
}

void construct_freq_table(ifstream &input, int freq_table[]) {
	for (int i = 0; i < DOMAIN_SIZE; i++)
		freq_table[i] = 0;

	int temp;
	while (input >> temp)
		freq_table[temp]++;
}

void encode(ifstream &input_file) {
	int *freq_table = new int[DOMAIN_SIZE];
	construct_freq_table(input_file, freq_table);
	
	_4way_Cache_Optimized_Heap_Huffman _4H;
	_4H.build_tree_using_4way_heap(freq_table);
	_4H.output_code_table_n_build_hash_table();

	ofstream encoded_output;
	encoded_output.open("encoded.bin", ofstream::out | ios::binary);

	input_file.clear();
	input_file.seekg(0, ios::beg);

	int current_buffer_bit = 0;
	int temp;
	int count = 0;
	while (input_file >> temp)
		current_buffer_bit = output_code(temp, &_4H, encoded_output);

	if (current_buffer_bit != 0)
		flush_bits(8 - current_buffer_bit, encoded_output);

	input_file.close();
	encoded_output.close();
}

int output_code(int num, _4way_Cache_Optimized_Heap_Huffman *_4H, ofstream &out) {
	_4way_Cache_Optimized_Heap_Huffman::hash_table_node* target = _4H->get_hash_table_entry(num);

	int length = target->code_length;
	unsigned int code = target->code;

	unsigned int current_bit = 0;
	for (int i = length - 1; i >= 0; i--)
		current_bit = write_bits((code >> i) & 1, out);

	return current_bit;
}

int write_bits(int bit, ofstream &out) {
	static int current_bit = 0;
	static unsigned char bit_buffer = 0;

	bit_buffer = bit_buffer << 1 | bit;

	current_bit++;
	if (current_bit == 8) {
		out << bit_buffer;
		current_bit = 0;
		bit_buffer = 0;
	}

	return current_bit;
}

void flush_bits(int zeros_to_append, ofstream &out) {
	for (int i = 0; i < zeros_to_append; i++)
		write_bits(0, out);
}