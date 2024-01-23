#pragma once

#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>

class Checklist;

class Node {
public:
	std::string key {};
	std::string value {};
	std::string notes {};
	std::shared_ptr<Checklist> sublist {};

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

class Checklist : public std::unordered_map<std::string, std::shared_ptr<Node>> {
protected:
	std::vector<std::shared_ptr<Node>> ordered_nodes{};

public:
	static std::shared_ptr<Checklist> from_file(const std::string &fname);
	Checklist &add(std::string &key, std::shared_ptr<Node> node);
	bool has(std::string &key);
	std::shared_ptr<Node> get(std::string &key);
	void update_order();
	std::vector<std::shared_ptr<Node>> all_nodes();

	std::shared_ptr<Node> operator[] (size_t ind);
};
