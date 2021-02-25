#pragma once

#include <vector>
#include <map>
const int BITS = 8;

struct TreeNode{
	TreeNode* l[2];
	bool term;
	uint8_t c;
	TreeNode();
	TreeNode(const int _c);
	TreeNode(TreeNode* l0, TreeNode* l1);
	~TreeNode();
};

class HuffTree {
public:
	HuffTree() = delete;
	HuffTree& operator=(const HuffTree& a) = delete;
	HuffTree(const std::array<int, UINT8_MAX + 1> &f);
	HuffTree(const std::array<std::vector<bool>, UINT8_MAX + 1> &_table);
	~HuffTree();
	std::array<std::vector<bool>, UINT8_MAX + 1> get_table() const;
	TreeNode* get_node() const;
private:
	std::array<std::vector<bool>, UINT8_MAX + 1> table;
	TreeNode* node;
	void dfs_for_table(TreeNode* root, std::vector<bool> &b);
	void insert_letter(TreeNode* &root, std::vector<bool> &b, size_t id, uint8_t c);

friend class huffman_test;
};

class HuffmanArchiver {
public:
	HuffmanArchiver() = delete;
	HuffmanArchiver(const std::vector<uint8_t> &_text);
	std::vector<uint8_t> compression();
	std::vector<uint8_t> decompression();
	int get_compressed_size() const;
	int get_decompressed_size() const;
	int get_table_size() const;
private:
	std::vector<uint8_t> text;
	int compressed_size, decompressed_size, table_size;
	static std::vector<char>& boolv_to_uint8v(const std::vector<bool> &v);

friend class huffman_test;
};

class HuffmanException : public std::logic_error {
public:
	HuffmanException(const char* _error);
};

