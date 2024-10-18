#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <sstream>
#include "../driver/atmo_driver.hh"
#include "../ast/nodes/statement_list_node.hh"

class ParserTest : public ::testing::Test {
protected:
	AtmoDriver driver;
	void SetUp() override {
		Error::InTest = true;
	}

	std::shared_ptr<StatementListNode> parse(const std::string& input) {
		std::istringstream stream(input);
 		driver.parse_only(stream);
		return driver.ast_root;
	}
};