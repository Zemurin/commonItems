#ifndef PARSER_H
#define PARSER_H


#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <regex>
#include <set>



// A parser for reading Paradox Script.

namespace commonItems
{
using parsingFunction = std::function<void(const std::string&, std::istream&)>;
using parsingFunctionStreamOnly = std::function<void(std::istream&)>;
void absorbBOM(std::istream& theStream);

class parser
{
  public:
	parser() = default;
	virtual ~parser() = default;
	parser(const parser&) = default;
	parser(parser&&) noexcept = default;
	parser& operator=(const parser&) = default;
	parser& operator=(parser&&) = default;

	void registerKeyword(const std::string& keyword, const parsingFunctionStreamOnly& function);
	void registerKeyword(const std::string& keyword,
		 const parsingFunction& function); // for the few keywords that need to be returned
	void registerRegex(const std::string& keyword, const parsingFunction& function);
	void IgnoreUnregisteredItems();
	void IgnoreAndLogUnregisteredItems();
	void IgnoreAndStoreUnregisteredItems(std::set<std::string>& ignored_tokens);

	void clearRegisteredKeywords() noexcept;

	void parseStream(std::istream& theStream);
	void parseFile(std::filesystem::path filename);

	std::optional<std::string> getNextToken(std::istream& theStream);
	static std::optional<std::string> getNextTokenWithoutMatching(std::istream& theStream);
	// as a function object, parse the stream.
	virtual void operator()(std::istream& theStream);

  private:
	inline bool tryToMatchAgainstKeywords(const std::string& toReturn, const std::string& strippedLexeme, bool isLexemeQuoted, std::istream& theStream);
	inline bool tryToMatchAgainstRegexes(const std::string& toReturn, const std::string& strippedLexeme, bool isLexemeQuoted, std::istream& theStream);

	std::map<std::string, parsingFunctionStreamOnly> registeredKeywordStringsStreamOnly;
	std::map<std::string, parsingFunction> registeredKeywordStrings;
	std::vector<std::pair<std::regex, parsingFunction>> generatedRegexes;
};
} // namespace commonItems
#endif // PARSER_H