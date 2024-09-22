#include "../src/wda.cpp"
#include <iostream>
#include <string>

int fails = 0;

void test(std::string test_case, std::string message, bool expression) {
  if (expression == false) {
    std::cout << "Test Case: " << test_case << " - Assertion \"" << message << "\" failed!" << std::endl;
    fails++;
  } else {
    std::cout << "Test Case: " << test_case << " - Assertion \"" << message << "\" passed." << std::endl;
  }
}

namespace tests {
  void util() {
    std::string sentence = "This has some words.";
    std::vector<std::string> split_sentence = {"This", "has", "some", "words."};
    test("Util Test 1", "join_many_to_one_str works", join_many_to_one_str(split_sentence.begin(), split_sentence.end(), " ") == sentence);
    test("Util Test 2", "split works", split_into_substrings(sentence, ' ') == split_sentence);
  }

  void cli() {
    std::vector<std::string> args = split_into_substrings("executable --foo=1 --bar --baz=value", ' ');
    test("CLI Test 1", "has_option detects boolean option", cli::has_option(args, "--bar"));
    test("CLI Test 2", "has_option detects lack of boolean option", cli::has_option(args, "--not-there") == false);
    test("CLI Test 3", "get_option finds value at end", cli::get_option(args, "--baz") == "value");
    test("CLI Test 4", "get_option finds value at middle", cli::get_option(args, "--foo") == "1");
  }

  std::string raw_save = R"""""(100 45.5000 -73.6000 Montreal
150 47.6000 -122.3000 Seattle
200 48.8000 2.3000 Paris, France
2000 44.8400 -0.5800 Bordeaux, France
)""""";
  std::string french = R"""""(200 48.8000 2.3000 Paris, France
2000 44.8400 -0.5800 Bordeaux, France
)""""";
  void location_save() {
    std::stringstream ss;
    ss << raw_save;
    LocationList save(ss);
    test("Location Save Test 1", "constructs and serialises", save.serialise() == raw_save);

    test("Location Save Test 2", "resolve_location resolves full name",
         save.match_entry("Paris, France").value_or(-1) == 200);
    test("Location Save Test 3", "resolve_location resolves ID", save.match_entry("100").value_or(-1) == 100);
    test("Location Save Test 4", "resolve_location returns nullopt if no name match",
         save.match_entry("Berlin") == std::nullopt);
    test("Location Save Test 5", "resolve_location returns nullopt if no id match", save.match_entry("123") == std::nullopt);

    test("Location Save Test 6", "list_filtered works on partials and is "
         "case-insensitive",
         save.list_filtered("francE") == french);
    test("Location Save Test 7", "list_filtered works on id",
         save.list_filtered("200") == "200 48.8000 2.3000 Paris, France\n");
    test("Location Save Test 8", "list_filtered returns empty string when no matches", save.list_filtered("xxxxx") == "");

    test("Location Save Test 9", "remove_location returns false when nothing removed",
         save.remove_entry("France") == false && save.remove_entry("123") == false);
    test("Location Save Test 10", "remove_location doesn't remove by partial name or "
         "by invalid ID",
         save.serialise() == raw_save);
    // This test mutates the data
    test("Location Save Test 11", "remove_location returns true when removing by "
         "existing id",
         save.remove_entry("100") == true);
    // This test mutates the data
    test("Location Save Test 12", "remove_location returns true when removing by "
         "existing full name",
         save.remove_entry("Seattle") == true);
    test("Location Save Test 13", "remove_location removes by full name and ID", save.serialise() == french);
  }
} // namespace tests

void run_tests() {
  tests::util();
  tests::cli();
  tests::location_save();
}

int main() {
  run_tests();
  if (fails == 0)
    std::cout << "All assertions passed." << std::endl;
  else
    std::cout << std::to_string(fails) << " assertions failed." << std::endl;
  return fails == 0 ? 0 : 1;
}