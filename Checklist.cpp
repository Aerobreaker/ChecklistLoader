#include "Checklist.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>

// Source: https://github.com/timsort/cpp-TimSort
#include <gfx/timsort.hpp>

Node::Node() {}
Node::Node(const std::string &ky) : key(ky) {}
Node::Node(const std::string &ky, const std::string &val) : key(ky), value(val) {}

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

std::shared_ptr<Checklist> Checklist::from_file(const std::string &fname) {
	std::filesystem::path fil {fname};
	std::shared_ptr<Checklist> outp = std::make_shared<Checklist>();
	if (!std::filesystem::is_regular_file(fil) || !std::filesystem::exists(fil)) return outp;

	std::ifstream infil {fil};
	if (!infil.is_open()) return nullptr;
	
	std::string key, stepkey, stepval, stepkey_lower;
	bool inp {};
	std::vector<std::pair<int, std::string>> ws;
	int ws_cnt, note_count = 0;

	do {
		ws_cnt = 0;
		while (infil.good() && std::isspace(infil.peek())) {
			++ws_cnt;
			// For some reason with README.md, seekg() gets stuck at character 118?  get() seeks right past it though.  It looks like an issue with character 10 (\n)
			// infil.seekg(1, ios_base::cur);
			infil.get();
		}
		if (!infil.good()) break;
		infil >> stepkey;
		if (stepkey.empty()) break;
		stepkey_lower = stepkey;
		std::transform(stepkey_lower.begin(), stepkey_lower.end(), stepkey_lower.begin(), ::tolower);
		if (stepkey_lower == "-note:") {
			stepkey = "Note " + std::to_string(++note_count);
		} else {
			note_count = 0;
		}
		while (infil.good() && std::isspace(infil.peek())) infil.get();
		if (!infil.good()) break;
		inp = static_cast<bool>(std::getline(infil, stepval));
		while (!ws.empty() && ws_cnt <= ws.back().first) ws.pop_back();
		key.clear();
		for (std::pair<int, std::string> &it : ws) key += it.second.back() == '.' ? it.second : (it.second + '.');
		std::shared_ptr<Node> node;
		if (stepkey_lower == "-load:") {
			while (key.back() == '.') key.pop_back();
			if (outp->contains(key)) {
				node = outp->at(key);
				node->sublist = Checklist::from_file(stepval);
			}
		} else {
			key += stepkey;
			node = std::make_shared<Node>(std::move(key), std::move(stepval));
		}
		if (node) {
			ws.push_back(std::make_pair(std::move(ws_cnt), std::move(stepkey)));
			outp->add(node->key, node);
		}
		stepkey.clear();
	} while (inp && infil.good());

	// The 3 fail bits are badbit, failbit, and eofbit.  Eofbit is fine, the other two are not
	if (infil.bad() || infil.fail()) return nullptr;

	// If nothing found, return a bad pointer
	if (outp->empty()) return nullptr;

	outp->update_order();

	return outp;
}

Checklist &Checklist::add(std::string &key, std::shared_ptr<Node> node) {
	while (key.back() == '.') key.pop_back();

	std::string basekey, subkey;
	size_t len = key.find('.');
	if (len == std::string::npos) {
		basekey = key;
	} else {
		basekey = std::move(key.substr(0, len));
		subkey = std::move(key.substr(len + 1));
	}

	if (subkey.empty()) {
		if (contains(basekey)) node->sublist = at(basekey)->sublist;
		insert_or_assign(move(basekey), node);
	} else {
		if (!(contains(basekey))) {
			std::shared_ptr<Node> nod = std::make_shared<Node>(basekey, "");
			insert({basekey, nod});
		}
		if (!(at(basekey)->sublist)) at(basekey)->sublist = std::make_shared<Checklist>();
		at(basekey)->sublist->add(subkey, node);
	}

	return *this;
}

void Checklist::update_order() {
	ordered_nodes.clear();

	for (std::pair<const std::string, std::shared_ptr<Node>> &it : *this) {
		ordered_nodes.push_back(it.second);
		if (it.second->sublist) it.second->sublist->update_order();
	}

	std::stable_sort(ordered_nodes.begin(), ordered_nodes.end(), [](std::shared_ptr<Node> a, std::shared_ptr<Node> b) {return *a < *b; });
}

std::shared_ptr<Node> Checklist::operator[] (size_t ind) {
	return ordered_nodes[ind];
}
