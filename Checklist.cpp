#include "Checklist.hpp"

#include <filesystem>
#include <fstream>

//Source: https://github.com/timsort/cpp-TimSort
#include "..\cpp-TimSort\include\gfx\timsort.hpp"

using namespace std;

Node::Node() {}
Node::Node(const string &ky) : key(ky) {}
Node::Node(const string &ky, const string &val) : key(ky), value(val) {}

bool Node::operator< (const Node &other) const {
	if (key.length() < other.key.length()) return true;
	if (key.length() > other.key.length()) return false;
	if (key < other.key) return true;
	if (key > other.key) return false;
	if (value.length() < other.value.length()) return true;
	if (value.length() > other.value.length()) return false;
	if (value < other.value) return true;
	if (value > other.value) return false;
	return false;
};
bool Node::operator> (const Node &other) const {
	if (key.length() > other.key.length()) return true;
	if (key.length() < other.key.length()) return false;
	if (key > other.key) return true;
	if (key < other.key) return false;
	if (value.length() > other.value.length()) return true;
	if (value.length() < other.value.length()) return false;
	if (value > other.value) return true;
	if (value < other.value) return false;
	return false;
};
bool Node::operator== (const Node &other) const { return key == other.key && value == other.value; };
bool Node::operator<= (const Node &other) const {
	if (key.length() < other.key.length()) return true;
	if (key.length() > other.key.length()) return false;
	if (key < other.key) return true;
	if (key > other.key) return false;
	if (value.length() < other.value.length()) return true;
	if (value.length() > other.value.length()) return false;
	if (value < other.value) return true;
	if (value > other.value) return false;
	return true;
};
bool Node::operator>= (const Node &other) const {
	if (key.length() > other.key.length()) return true;
	if (key.length() < other.key.length()) return false;
	if (key > other.key) return true;
	if (key < other.key) return false;
	if (value.length() > other.value.length()) return true;
	if (value.length() < other.value.length()) return false;
	if (value > other.value) return true;
	if (value < other.value) return false;
	return true;
};
bool Node::operator!= (const Node &other) const { return key != other.key || value != other.value; };

Checklist *Checklist::from_file(const string &fname) {
	using namespace filesystem;
	path fil {fname};
	Checklist *outp = new Checklist;
	if (!is_regular_file(fil) || !exists(fil)) return outp;

	ifstream infil {fil};
	if (!infil.is_open()) return outp;
	
	string key, stepkey, stepval;
	bool inp {};
	vector<pair<int, string>> ws;
	int ws_cnt;

	do {
		ws_cnt = 0;
		while (isspace(infil.peek())) {
			++ws_cnt;
			infil.seekg(1, ios_base::cur);
		}
		infil >> stepkey;
		if (stepkey.empty()) break;
		while (isspace(infil.peek())) infil.seekg(1, ios_base::cur);
		inp = getline(infil, stepval) ? true : false;
		while (!ws.empty() && ws_cnt <= ws.back().first) ws.pop_back();
		key.clear();
		for (pair<int, string> &it : ws) key += it.second.back() == '.' ? it.second : (it.second + '.');
		key += stepkey;
		Node *node = new Node {move(key), move(stepval)};
		ws.push_back(make_pair(move(ws_cnt), move(stepkey)));
		outp->add(node->key, node);
		stepkey.clear();
	} while (inp);

	outp->update_order();

	return outp;
}

Checklist &Checklist::add(string &key, Node *node) {
	while (key.back() == '.') key.pop_back();

	string basekey, subkey;
	size_t len = key.find('.');
	if (len == string::npos) {
		basekey = key;
	} else {
		basekey = move(key.substr(0, len));
		subkey = move(key.substr(len + 1));
	}

	if (subkey.empty()) {
		if (contains(basekey)) node->sublist = at(basekey)->sublist;
		insert_or_assign(move(basekey), node);
	} else {
		if (!(contains(basekey))) {
			Node *nod = new Node(basekey, "");
			insert({basekey, nod});
		}
		if (!(at(basekey)->sublist)) at(basekey)->sublist = new Checklist;
		at(basekey)->sublist->add(subkey, node);
	}

	return *this;
}

void Checklist::update_order() {
	ordered_nodes.clear();

	for (pair<const string, Node *> &it : *this) {
		ordered_nodes.push_back(it.second);
		if (it.second->sublist) it.second->sublist->update_order();
	}

	gfx::timsort(ordered_nodes, [](Node *a, Node *b) {return *a < *b; });
}

Node *Checklist::operator[] (size_t ind) {
	return ordered_nodes[ind];
}
