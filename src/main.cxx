#include "NodeTree.hxx"
#include "third_party/cxxopts/cxxopts.hpp"


int main(int argc, char* argv[])
{
	cxxopts::Options options(argv[0], "N-array tree description");

	options.add_options()
		("i,input", "Input serializable file tree", cxxopts::value<std::filesystem::path>())
		("o,output", "Output deserializable file tree", cxxopts::value<std::filesystem::path>())
		("g, generate", "Generate example json");

	auto result = options.parse(argc, argv);
	if (result.count("help"))
	{
		std::cout << options.help() << std::endl;
		return 0;
	}
	else if (result.count("generate"))
	{
		std::shared_ptr<continue_n_tree::NodeTree> root = std::make_shared<continue_n_tree::NodeTree>(8);

		std::shared_ptr<continue_n_tree::NodeTree> bar = std::make_shared<continue_n_tree::NodeTree>("bar");
		std::shared_ptr<continue_n_tree::NodeTree> baz = std::make_shared<continue_n_tree::NodeTree>("baz");

		root->addNode(1, bar);
		root->addNode(2, baz);

		std::shared_ptr<continue_n_tree::NodeTree> bar_float = std::make_shared<continue_n_tree::NodeTree>(2.015f);
		std::shared_ptr<continue_n_tree::NodeTree> bar_int = std::make_shared<continue_n_tree::NodeTree>(2015);
		std::shared_ptr<continue_n_tree::NodeTree> bar_string = std::make_shared<continue_n_tree::NodeTree>(2015);

		std::shared_ptr<continue_n_tree::NodeTree> baz_string_foo = std::make_shared<continue_n_tree::NodeTree>("foo");
		std::shared_ptr<continue_n_tree::NodeTree> baz_float = std::make_shared<continue_n_tree::NodeTree>(6.28318);
		std::shared_ptr<continue_n_tree::NodeTree> baz_float_string = std::make_shared<continue_n_tree::NodeTree>("hello");

		baz_float->addNode(1, baz_float_string);

		std::shared_ptr<continue_n_tree::NodeTree> bar_float_int = std::make_shared<continue_n_tree::NodeTree>(9);

		bar_float->addNode(1, bar_float_int);
		bar->addNode(1, bar_float);
		bar->addNode(2, bar_int);
		bar->addNode(3, bar_string);

		baz->addNode(1, baz_string_foo);
		baz->addNode(2, baz_float);

		std::ofstream json_writer("example_tree.json");
		json_writer << root->to_json().dump(4) << std::endl;
		return 0;

	}
	else if (result.count("input") && result.count("output"))
	{
		auto input_path = result["input"].as<std::filesystem::path>();
		auto output_path = result["output"].as<std::filesystem::path>();

		if (!std::filesystem::exists(input_path))
		{
			std::cout << "Input (file) param is not valid!" << std::endl;
			return 0;
		}
		if (!std::filesystem::exists(std::filesystem::absolute(output_path).parent_path()))
		{
			std::cout << "Output (file) param is not valid!" << std::endl;
			return 0;
		}
		std::shared_ptr<continue_n_tree::NodeTree> r = std::make_shared<continue_n_tree::NodeTree>(
			result["input"].as<std::filesystem::path>()
			);
		r->printTree();
		r->save_file(result["output"].as<std::filesystem::path>());
		return 0;
	}
	std::cout << options.help() << std::endl;
	return 0;
}