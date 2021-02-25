#include "huffman_test.h"
#include <time.h>
#include <iostream>

#define DO_CHECK(EXPR) check(EXPR, __FILE__, __LINE__);

// check huffman tree
void huffman_test::table_from_tree(std::array<std::vector<bool>, UINT8_MAX + 1> &cur_table, TreeNode* root, std::vector<bool> &b) {
	if (root->term) {
		// check correctness
		DO_CHECK(cur_table[root->c].size() == 0);
		for (int i = 0; i < 2; i++)
			DO_CHECK(root->l[i] == NULL);
	
		cur_table[root->c] = b;
		return;
	}
	for (int i = 0; i < 2; i++) {
		b.push_back(i);
		table_from_tree(cur_table, root->l[i], b);
		b.pop_back();
	}
}


// general check for initialization(return table)
std::array<std::vector<bool>, UINT8_MAX + 1> huffman_test::check_hufftree_init(std::array<int, UINT8_MAX + 1> &f) {
	std::array<std::vector<bool>, UINT8_MAX + 1> table_from_tree_res;

	// make HuffTree on frequency
	HuffTree hufftree(f);
	std::array<std::vector<bool>, UINT8_MAX + 1> table_ch = hufftree.get_table();
	TreeNode* tree = hufftree.get_node();
	// checking tree and table matches

	std::vector<bool> tmp;
	table_from_tree(table_from_tree_res, tree, tmp);
	DO_CHECK(table_from_tree_res == table_ch);

	// make HuffTree on table
	HuffTree hufftree_t(table_ch);
	TreeNode* tree_t = hufftree_t.get_node();

	// checking tree(on table) and table matches
	tmp.clear();
	for (int i = 0; i <= UINT8_MAX; i++)
		table_from_tree_res[i].clear();
	table_from_tree(table_from_tree_res, tree_t, tmp);
	DO_CHECK(table_from_tree_res == table_ch);

	return table_ch;
}

void huffman_test::hufftree_empty() {
	start_test();

	// empty input
	std::array<int, UINT8_MAX + 1> f;
	for (uint16_t i = 0; i <= UINT8_MAX; i++)
		f[i] = 0;

	std::array<std::vector<bool>, UINT8_MAX + 1> table_ch = check_hufftree_init(f);

	// table validation
	for (uint16_t i = 0; i <= UINT8_MAX; i++) {
		// code lengths must be the same
		DO_CHECK(table_ch[i].size() == table_ch[0].size());
	}

	end_test();
}

void huffman_test::hufftree_one_type() {
	start_test();

	// one type of symbol test
	std::array<int, UINT8_MAX + 1> f;
	for (uint16_t i = 0; i <= UINT8_MAX; i++)
		f[i] = 0;
	f[0] = 10000;

	std::array<std::vector<bool>, UINT8_MAX + 1> table_ch = check_hufftree_init(f);

	// table validation
	// code lengths must be the same, except 2ух(one - 1, one - 8), other - 9
	int cnt9 = 0, cnt8 = 0;
	for (uint16_t i = 1; i <= UINT8_MAX; i++) {
		if (table_ch[i].size() == 9)
			cnt9++;
		else if (table_ch[i].size() == 8)
			cnt8++;
	}
	DO_CHECK(cnt9 == UINT8_MAX + 1 - 2 && cnt8 == 1 && table_ch[0].size() == 1);

	end_test();
}


void huffman_test::hufftree_small_test() {
	start_test();

	// aaaaaabbbbbccccddd
	std::array<int, UINT8_MAX + 1> f;
	for (uint16_t i = 0; i <= UINT8_MAX; i++)
		f[i] = 0;
	f[0] = 6;
	f[1] = 5;
	f[2] = 4;
	f[3] = 3;
	std::vector<bool> ans[4];
	ans[0] = {1, 1};
	ans[1] = {1, 0};
	ans[2] = {0, 1};
	ans[3] = {0, 0, 1};

/*
	     18
	   7   11
	 3	
	0 3 4  5  6 
*/
	std::array<std::vector<bool>, UINT8_MAX + 1> table_ch = check_hufftree_init(f);

	// table validation
	int cnt11 = 0, cnt10 = 0;
	for (uint16_t i = 0; i <= UINT8_MAX; i++) {
		if (table_ch[i].size() == 11)
			cnt11++;
		else if (table_ch[i].size() == 10)
			cnt10++;
	}
	DO_CHECK(cnt11 == UINT8_MAX + 1 - 8 && cnt10 == 4 && table_ch[0].size() == 2 && table_ch[1].size() == 2 && table_ch[2].size() == 2 && table_ch[3].size() == 3);
	for (int i = 0; i < 4; i++)
		DO_CHECK(table_ch[i] == ans[i]);
	end_test();
}

#include <iostream>
void huffman_test::hufftree_small_test2() {
	start_test();

	// a^100+b^13+c^12+d^11+e^9+f^5
	std::array<int, UINT8_MAX + 1> f;
	for (uint16_t i = 0; i <= UINT8_MAX; i++)
		f[i] = 0;
	f[0] = 100;
	f[1] = 13;
	f[2] = 12;
	f[3] = 11;
	f[4] = 9;
	f[5] = 5;
	std::vector<bool> ans[6];
	ans[0] = {1};
	ans[1] = {0, 1, 0};
	ans[2] = {0, 0, 1};
	ans[3] = {0, 0, 0};
	ans[4] = {0, 1, 1, 1};
	ans[5] = {0, 1, 1, 0, 1};

	std::array<std::vector<bool>, UINT8_MAX + 1> table_ch = check_hufftree_init(f);

	// table validation
	int cnt13 = 0, cnt12 = 0;
	for (uint16_t i = 0; i <= UINT8_MAX; i++) {
		if (table_ch[i].size() == 13)
			cnt13++;
		else if (table_ch[i].size() == 12)
			cnt12++;
	}
	DO_CHECK(cnt13 == UINT8_MAX + 1 - 12 && cnt12 == 6);
	for (int i = 0; i < 6; i++) {
		DO_CHECK(table_ch[i] == ans[i]);
	}
	end_test();
}

void huffman_test::hufftree_all_symbols() {
	start_test();

	// all symbols, the same cnt
	std::array<int, UINT8_MAX + 1> f;
	for (uint16_t i = 0; i <= UINT8_MAX; i++)
		f[i] = 1024;
	
	std::array<std::vector<bool>, UINT8_MAX + 1> table_ch = check_hufftree_init(f);


	// table validation
	for (uint16_t i = 0; i <= UINT8_MAX; i++) {
		// code lengths must be the same
		DO_CHECK(table_ch[i].size() == 8);
	}

	end_test();
}

// archiver validation
void huffman_test::huffarch_empty() {
	start_test();

	// empty input
	std::vector<uint8_t> text;
	HuffmanArchiver arc(text);

	std::vector<uint8_t> compressed = arc.compression();
	HuffmanArchiver ard(compressed);

	std::vector<uint8_t> decompressed = ard.decompression();
	DO_CHECK(text == decompressed);

	end_test();
}

void huffman_test::huffarch_one_type() {
	start_test();

	// one type of symbol test
	std::vector<uint8_t> text;
	text.resize(5 * 1024 * 1024, 2);
	HuffmanArchiver arc(text);

	std::vector<uint8_t> compressed = arc.compression();
	HuffmanArchiver ard(compressed);

	std::vector<uint8_t> decompressed = ard.decompression();
	DO_CHECK(text == decompressed);

	end_test();
}


void huffman_test::huffarch_small_test() {
	start_test();

	// aaaaaabbbbbccccddd
	std::vector<uint8_t> text;
	for (int i = 0; i < 6; i++)
		text.push_back(5);
	for (int i = 0; i < 5; i++)
		text.push_back(8);
	for (int i = 0; i < 4; i++)
		text.push_back(18);
	for (int i = 0; i < 3; i++)
		text.push_back(107);
	HuffmanArchiver arc(text);

	std::vector<uint8_t> compressed = arc.compression();
	HuffmanArchiver ard(compressed);

	std::vector<uint8_t> decompressed = ard.decompression();
	DO_CHECK(text == decompressed);

	end_test();
}


void huffman_test::huffarch_random_test() {
	start_test();

	// small random input
	std::vector<uint8_t> text;
	for (int i = 0; i < 10000; i++)
		text.push_back(rand() % (UINT8_MAX + 1));
	HuffmanArchiver arc(text);

	std::vector<uint8_t> compressed = arc.compression();
	HuffmanArchiver ard(compressed);

	std::vector<uint8_t> decompressed = ard.decompression();
	DO_CHECK(text == decompressed);

	end_test();
}


void huffman_test::huffarch_big_random_test() {
	start_test();

	// big random input
	std::vector<uint8_t> text;
	for (int i = 0; i < 5 * 1024 * 1024; i++)
		text.push_back(rand() % (UINT8_MAX + 1));

	HuffmanArchiver arc(text);

	std::vector<uint8_t> compressed = arc.compression();
	HuffmanArchiver ard(compressed);


	std::vector<uint8_t> decompressed = ard.decompression();
	DO_CHECK(text == decompressed);

	end_test();
}



void huffman_test::huffarch_big_all_types_test() {
	start_test();

	// all types of symbols many times
	std::vector<uint8_t> text;
	text.resize(5 * 1024 * 1024);
	for (int i = 0; i < 5 * 1024 * 1024; i++)
		text[i] = (i % (UINT8_MAX + 1));

	HuffmanArchiver arc(text);

	std::vector<uint8_t> compressed = arc.compression();
	HuffmanArchiver ard(compressed);

	std::vector<uint8_t> decompressed = ard.decompression();
	DO_CHECK(text == decompressed);

	end_test();
}



void huffman_test::runAllTests() {
	hufftree_empty();
	hufftree_one_type();
	hufftree_small_test();
	hufftree_small_test2();
	hufftree_all_symbols();

	huffarch_empty();
	huffarch_one_type();
	huffarch_small_test();
	huffarch_random_test();
	huffarch_big_random_test();
	huffarch_big_all_types_test();
}
