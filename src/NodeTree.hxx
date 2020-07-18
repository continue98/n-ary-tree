#pragma once 

#include <memory>
#include <variant>
#include <unordered_map>
#include "third_party/nlohmann/json.hpp"
#include "third_party/fifo-map/fifo_map.hpp"
#include <filesystem>
#include <locale>
#include <codecvt>
#include <fstream>
#include <iostream>

namespace continue_n_tree
{

	// https://github.com/nlohmann/json#order-of-object-keys
	template<class K, class V, class dummy_compare, class A>
	using my_workaround_fifo_map = nlohmann::fifo_map<K, V, nlohmann::fifo_map_compare<K>, A>;
	using json = nlohmann::basic_json<my_workaround_fifo_map>;

	typedef std::variant<std::string, int, double> tree_node_type;
	enum node_types
	{
		STRING,
		INT,
		DOUBLE
	};
	class NodeTree
	{
	public:
		NodeTree() = default;
		NodeTree(tree_node_type data) : m_data(data) { };
		NodeTree(std::string_view data) : m_data(std::string(data)) { };
		NodeTree(const char* data) : m_data(std::string(data)) { };
		NodeTree(int data) : m_data(data) { };
		NodeTree(double data) : m_data(data) { };
		NodeTree(const json& j);
		NodeTree(std::filesystem::path input);

		bool addNode(int key, std::shared_ptr<NodeTree> node);
		bool addNode(int key, tree_node_type data);
		bool removeNode(int key);
		void setData(tree_node_type data);
		tree_node_type getData();
		std::wstring getDataAsString();
		std::shared_ptr<NodeTree> getChild(int key);
		json to_json();
		void from_json(const json& j);
		void save_file(std::filesystem::path output);
		void printJson();
		void printTree();
		void printTree(std::vector<bool> flag, const int depth = 0, bool isLast = false);

	private:
		std::unordered_map<int, std::shared_ptr<NodeTree>> m_childNodes;
		tree_node_type m_data;
	};
}