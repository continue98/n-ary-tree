#include "NodeTree.hxx"

static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> g_converter;

continue_n_tree::NodeTree::NodeTree(const json& j)
{
	from_json(j);
}

continue_n_tree::NodeTree::NodeTree(std::filesystem::path input)
{
	std::ifstream json_reader(input);
	if (!json_reader.good())
	{
		return;
	}
	json json_tree;
	json_reader >> json_tree;
	from_json(json_tree);
}

bool continue_n_tree::NodeTree::addNode(int key, std::shared_ptr<NodeTree> node)
{
	if (m_childNodes.find(key) != m_childNodes.end())
	{
		return false;
	}
	m_childNodes.emplace(key, node);
	return true;
}

bool continue_n_tree::NodeTree::addNode(int key, tree_node_type data)
{
	std::shared_ptr<NodeTree> root = std::make_shared<NodeTree>(data);
	return addNode(key, root);
}

bool continue_n_tree::NodeTree::removeNode(int key)
{
	if (m_childNodes.find(key) == m_childNodes.end())
	{
		return false;
	}
	m_childNodes.erase(key);
	return true;
}

void continue_n_tree::NodeTree::setData(tree_node_type data)
{
	m_data = data;
}

continue_n_tree::tree_node_type continue_n_tree::NodeTree::getData()
{
	return m_data;
}

std::wstring continue_n_tree::NodeTree::getDataAsString()
{
	switch (m_data.index())
	{
	case node_types::STRING: {
		return g_converter.from_bytes(std::get<std::string>(m_data));
		break;
	}
	case node_types::INT: {
		return std::to_wstring(std::get<int>(m_data));
		break;
	}
	case node_types::DOUBLE: {
		return std::to_wstring(std::get<double>(m_data));
		break;
	}
	default:
		return std::wstring();
		break;
	}
}

std::shared_ptr<continue_n_tree::NodeTree> continue_n_tree::NodeTree::getChild(int key)
{
	return m_childNodes[key];
}

continue_n_tree::json continue_n_tree::NodeTree::to_json()
{
	json j;
	
	switch (m_data.index())
	{
	case node_types::STRING: {
		j["data"] = std::get<std::string>(m_data);
		break;
	}
	case node_types::INT: {
		j["data"] = std::get<int>(m_data);
		break;
	}
	case node_types::DOUBLE: {
		j["data"] = std::get<double>(m_data);
		break;
	}
	default:
		break;
	}
	for (const auto& [key, value] : m_childNodes)
	{
		j["childs"][std::to_string(key)] = value->to_json();
	}
	return j;
}

void continue_n_tree::NodeTree::from_json(const json& j)
{
	if (j["data"].is_string())
	{
		m_data = j["data"].get<std::string>();
	}
	else if (j["data"].is_number_integer())
	{
		m_data = j["data"].get<int>();
	}
	else if (j["data"].is_number_float())
	{
		m_data = j["data"].get<double>();
	}
	if (j.contains("childs"))
	{
		for (auto& [key, value] : j["childs"].items())
		{
			std::shared_ptr<NodeTree> node = std::make_shared<NodeTree>(value);
			addNode(std::stoi(key), node);
		}
	}
}

void continue_n_tree::NodeTree::save_file(std::filesystem::path output)
{
	std::ofstream json_writer(output);
	if (!json_writer.good())
	{
		return;
	}
	json_writer << std::setw(4) << to_json() << std::endl;
}

void continue_n_tree::NodeTree::printJson()
{
	std::wcout << g_converter.from_bytes(to_json().dump(4)) << std::endl;
}

void continue_n_tree::NodeTree::printTree(std::vector<bool> flags, const int depth, bool isLast)
{
	flags.push_back(true);

	for (int i = 1; i < depth; ++i) 
	{
		if (flags[i] == true) 
		{
			std::wcout << L"│   ";
		}
		else 
		{
			std::wcout << "    ";
		}
	}	
	if (depth == 0)
	{
		std::wcout << getDataAsString() << std::endl;
	}
	else if (isLast) 
	{
		std::wcout << L"└───" << getDataAsString() << std::endl;
		flags[depth] = false;
	}
	else 
	{
		std::wcout << L"├───" << getDataAsString() << std::endl;
	}
	for (auto it = m_childNodes.begin(); it != m_childNodes.end(); ++it)
	{
		auto it2 = it;
		it->second->printTree(flags, depth + 1, ++it2 == m_childNodes.end());
	}
	flags[depth] = true;
}

void continue_n_tree::NodeTree::printTree()
{
	std::vector<bool> flags;
	auto old_locale = std::locale::global(std::locale("en_US.UTF-8"));
	printTree(flags);
	std::locale::global(old_locale);
}