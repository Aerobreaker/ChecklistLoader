#pragma once

#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>

class Checklist;

class Node {
private:

protected:

public:
	std::string key {};
	std::string value {};
	Checklist *sublist {};

	Node();
	Node(const std::string &key);
	Node(const std::string &key, const std::string &val);

	bool operator< (const Node &other) const;
	bool operator> (const Node &other) const;
	bool operator== (const Node &other) const;
	bool operator<= (const Node &other) const;
	bool operator>= (const Node &other) const;
	bool operator!= (const Node &other) const;
};

class Checklist : public std::unordered_map<std::string, Node *> {
private:
	std::vector<Node *> ordered_nodes {};

public:
	static Checklist *from_file(const std::string &fname);
	Checklist &add(std::string &key, Node *node);
	void update_order();

	Node *operator[] (size_t ind);
};
