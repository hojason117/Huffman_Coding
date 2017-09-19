all: encoder decoder
encoder: encoder.cpp 4_Way_Cache_Optimized_Heap_Huffman.cpp
	g++ -std=c++11 encoder.cpp 4_Way_Cache_Optimized_Heap_Huffman.cpp -o encoder
decoder: decoder.cpp
	g++ -std=c++11 decoder.cpp -o decoder