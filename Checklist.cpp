#include "Checklist.hpp"

#include <filesystem>
#include <fstream>

using namespace std;

Node::Node() {}
Node::Node(const string &ky) : key(ky) {}
Node::Node(const string &ky, const string &val) : key(ky), value(val) {}

bool Node::operator< (const Node &other) const { return this->key < other.key ? true : (this->key == other.key ? this->value < other.value : false); };
bool Node::operator> (const Node &other) const { return this->key > other.key ? true : (this->key == other.key ? this->value > other.value : false); };
bool Node::operator== (const Node &other) const { return this->key == other.key && this->value == other.value; };
bool Node::operator<= (const Node &other) const { return !(*this > other); };
bool Node::operator>= (const Node &other) const { return !(*this < other); };
bool Node::operator!= (const Node &other) const { return !(*this == other); };

Checklist Checklist::from_file(const string &fname) {
	using namespace filesystem;
	path fil {fname};
	Checklist outp {};
	if (!is_regular_file(fil) || !exists(fil)) return outp;

	ifstream infil {fil};
	if (!infil.is_open()) return outp;
	
	string stepkey, stepval;
	bool inp {};

	do {
		infil >> stepkey;
		if (stepkey.empty()) break;
		while (isspace(infil.peek())) infil.seekg(1, ios_base::cur);
		inp = getline(infil, stepval) ? true : false;
		Node *node = new Node {move(stepkey), move(stepval)};
		outp.add(node);
		stepkey.clear();
	} while (inp);

	return outp;
}

Checklist &Checklist::add(Node *node) {
	while (node->key.back() == '.') node->key.pop_back();

	string key, subkey;
	size_t len = node->key.find('.');
	if (len == string::npos) {
		key = node->key;
	} else {
		key = move(node->key.substr(0, len));
		subkey = move(node->key.substr(len + 1));
	}

	if (subkey.empty()) {
		if (contains(key)) node->sublist = at(key)->sublist;
		insert_or_assign(move(key), node);
	} else {
		node->key = move(subkey);
		if (!(contains(key))) {
			Node *nod = new Node(key, "");
			insert({key, nod});
		}
		if (!(at(key)->sublist)) at(key)->sublist = new Checklist;
		at(key)->sublist->add(node);
	}

	update_order();

	return *this;
}

void Checklist::update_order() {
	ordered_nodes.clear();

	for (pair<const string, Node *> &it : *this) {
		ordered_nodes.push_back(it.second);
		if (it.second->sublist) it.second->sublist->update_order();
	}
}

Node *Checklist::operator[] (size_t ind) {
	return ordered_nodes[ind];
}
