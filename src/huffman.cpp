#include "huffman.h"

// TreeNode
TreeNode::TreeNode() : term(0), c(0) {
	for (int i = 0; i < 2; i++)
		l[i] = nullptr;
}
TreeNode::TreeNode(const int _c) : term(1), c(_c) {
	for (int i = 0; i < 2; i++)
		l[i] = nullptr;
}
TreeNode::TreeNode(TreeNode* l0, TreeNode* l1) : term(0), c(0) {
	l[0] = l0;
	l[1] = l1;
}
TreeNode::~TreeNode() {
	for (int i = 0; i < 2; i++) {
		delete l[i];
		l[i] = nullptr;
	}
}

// HuffTree
void HuffTree::dfs_for_table(TreeNode* root, std::vector<bool> &b) {
	if (root->term) {
		table[root->c] = b;
		return;
	}
	for (int i = 0; i < 2; i++) {
		b.push_back(i);
		dfs_for_table(root->l[i], b);
		b.pop_back();
	}
}

HuffTree::HuffTree(const std::array<int, UINT8_MAX + 1> &f) {
	// make forest
	std::multimap <int, TreeNode*> forest;
	for (int i = 0; i <= UINT8_MAX; i++) {
		forest.insert(std::make_pair(f[i], new TreeNode(i)));
	}	

	// join forest into one tree
	while(forest.size() > 1) {
		std::pair<int, TreeNode*> u = *forest.begin();
		forest.erase(forest.begin());

		std::pair<int, TreeNode*> v = *forest.begin();
		forest.erase(forest.begin());
		forest.insert(std::make_pair(u.first + v.first, new TreeNode(u.second, v.second)));
	}
	node = (*forest.begin()).second;	

	// make table
	std::vector<bool> b;
	dfs_for_table(node, b);
}

void HuffTree::insert_letter(TreeNode* &root, std::vector<bool> &b, size_t id, uint8_t c) {
	if (root->term) 
		throw HuffmanException("Wrong Input (HuffmanTable)");
	if (id == b.size()) {
		root->term = 1;
		root->c = c;
		return;
	}
	if (root->l[b[id]] == nullptr)
		root->l[b[id]] = new TreeNode();
	insert_letter(root->l[b[id]], b, id + 1, c);
}

HuffTree::HuffTree(const std::array<std::vector<bool>, UINT8_MAX + 1> &_table) {
	table = _table;
	node = new TreeNode();
	for (int i = 0; i <= UINT8_MAX; i++) {
		insert_letter(node, table[i], 0, i);
	}
}

HuffTree::~HuffTree() {
	delete node;
}

std::array<std::vector<bool>, UINT8_MAX + 1> HuffTree::get_table() const{
	return table;
}
TreeNode* HuffTree::get_node() const{
	return node;
}



// HuffmanArchiver
HuffmanArchiver::HuffmanArchiver(const std::vector<uint8_t> &_text) : text(_text), compressed_size(-1), decompressed_size(-1), table_size(-1) {}

std::vector<uint8_t> HuffmanArchiver::compression() {
	decompressed_size = text.size();

	// make frequency
	std::array<int, UINT8_MAX + 1> m;
	for (int i = 0; i <= UINT8_MAX; i++)
		m[i] = 0;
	for (size_t i = 0; i < text.size(); i++)
		m[text[i]]++;


	// make HuffTree and table
	HuffTree tree(m);
	std::array<std::vector<bool>, UINT8_MAX + 1> table = tree.get_table();

	// make result in bits
	// info
	std::vector<bool> bits;
	for (int i = 0; i <= UINT8_MAX; i++) {
		// push len of code
		uint8_t len = table[i].size();
		for (int j = BITS * sizeof(uint8_t) - 1; j >= 0; j--)
			bits.push_back((len & (1 << j)) != 0);

		// push code
		bits.insert(bits.end(), table[i].begin(), table[i].end());
	}

	while(bits.size() % BITS != 0)
		bits.push_back(0);
	table_size = bits.size() / BITS;

	// data
	// push cnt of symbols
	uint32_t len = text.size();
	for (int j = BITS * sizeof(int) - 1; j >= 0; j--)
		bits.push_back((len & (1 << j)) != 0);

	for (size_t i = 0; i < text.size(); i++) {
		bits.insert(bits.end(), table[text[i]].begin(), table[text[i]].end());
	}

	while(bits.size() % BITS != 0)
		bits.push_back(0);

	// bits into bytes
	std::vector<uint8_t> res;
	for (size_t i = 0; i < bits.size(); i += 8) {
		uint8_t c1 = 0;
		for (size_t j = 0; j < sizeof(uint8_t) * BITS; j++)
			c1 = (c1 << 1) + bits[i + j];
		res.push_back(c1);
	}
	compressed_size = res.size() - table_size;
	
	return res;
}

std::vector<uint8_t> HuffmanArchiver::decompression() {
	// bytes into bits
	std::vector <bool > bits;
	for (size_t i = 0; i < text.size(); i++)
		for (int j = BITS * sizeof(uint8_t) - 1; j >= 0; j--)
			bits.push_back((text[i] & (1 << j)) != 0);
	
	// get table
	std::array<std::vector<bool>, UINT8_MAX + 1> table;
	size_t l = 0;	
	for (int i = 0; i <= UINT8_MAX; i++) {
		// get len
		uint8_t len = 0;
		for (size_t i = 0; i < BITS * sizeof(uint8_t); i++)
			if (l == bits.size()) 
				throw HuffmanException("Wrong Input (HuffmanTable)");
			else
				len = len * 2 + bits[l++];

		// get code
		for (int j = 0; j < len; j++)
			if (l == bits.size()) 
				throw HuffmanException("Wrong Input (HuffmanTable)");
			else
				table[i].push_back(bits[l++]);
	}

	while(l % BITS != 0)
		l++;
	table_size = l / BITS;
	compressed_size = text.size() - table_size;


	// make HuffTree
	HuffTree tree(table);

	// decompression
	// get len
	uint32_t len = 0;
	for (size_t i = 0; i < BITS * sizeof(int); i++)
		if (l == bits.size()) 
			throw HuffmanException("Wrong Input (Text)");
		else
			len = len * 2 + bits[l++];

	std::vector<uint8_t> res;
	TreeNode *st = tree.get_node(), *cur = st;	

	res.resize(len);
	size_t i, j;	
	for (i = l, j = 0; (i < bits.size() && j < res.size()); i++) {
		if (cur->l[bits[i]] == nullptr) 
			throw HuffmanException("Wrong Input (Text)");
		else
			cur = cur->l[bits[i]];

		if (!cur->term) 
			continue;
		res[j++] = cur->c;
		cur = st;
	}

	if (bits.size() % 8 != 0 || bits.size() - i > 7 || j != res.size())
		throw HuffmanException("Wrong Input (Text)");
	
	decompressed_size = res.size();
	return res;
}

// statistics
int HuffmanArchiver::get_compressed_size() const {
	return compressed_size;
}

int HuffmanArchiver::get_table_size() const {
	return table_size;
}

int HuffmanArchiver::get_decompressed_size() const {
	return decompressed_size;
}


HuffmanException::HuffmanException(const char* _error) : logic_error(_error) {}
