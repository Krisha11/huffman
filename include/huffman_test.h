#ifndef HUFFMAN_TEST_H
#define HUFFMAN_TEST_H

#include "huffman.h"
#include "autotest.h"
	
class huffman_test : public Test {
private:
	// help functions
	void table_from_tree(std::array<std::vector<bool>, UINT8_MAX + 1> &cur_table, TreeNode* root, std::vector<bool> &b);
	std::array<std::vector<bool>, UINT8_MAX + 1> check_hufftree_init(std::array<int, UINT8_MAX + 1> &f);

	// check structure of the tree
	void hufftree_empty();
	void hufftree_one_type();
	void hufftree_small_test();
	void hufftree_small_test2();
	void hufftree_all_symbols();

	// general tests
	void huffarch_empty();
	void huffarch_one_type();
	void huffarch_small_test();
	void huffarch_random_test();
	void huffarch_big_random_test();
	void huffarch_big_all_types_test();
// ...
    
public:
	void runAllTests();
    // ...
};

#endif


