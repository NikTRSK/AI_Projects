/*
	Author: Nikica Trajkovski
	https://github.com/NikTRSK/Cpp-Utility-Classes
*/

#ifndef STRING_UTILS
#define STRING_UTILS

#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>

#include <iostream>
/* TODO: Create StringUtils namespace */
namespace StringUtils
{
	/* Returns a vector of tokens  */
 	static inline std::vector<std::string> SplitString(const std::string &input, const std::string &delimiter) noexcept
	{
		std::vector<std::string> tokens;
		std::string token;
		size_t position = 0, found = 0;

		while ((found = input.find_first_of(delimiter, position)) != std::string::npos)
		{
			tokens.push_back(input.substr(position, found - position));
			position = found + 1;
		}
		tokens.push_back(input.substr(position));
		return tokens;
	}

	/* Trims whitespace from the sides of a string */
	static inline void TrimInPlace(std::string& input) noexcept
	{
		// Remove whitespace on the left
		auto left = input.find_first_not_of(' ');
		if (left != std::string::npos)
		{
			input.erase(0, left);
		}
		// Remove whitespace on the right
		auto right = input.find_last_not_of(' ');
		if (right != std::string::npos)
		{
			input.erase(right + 1, input.length() - right - 1);
		}
	}

	/* Returns a new string with the whitespace trimmed */
	static inline std::string Trim(const std::string& input) noexcept
	{
		// Remove whitespace on the left
		auto left = input.find_first_not_of(' ');
		if (left == std::string::npos)
		{
			left = 0;
		}
		// Remove whitespace on the right
		auto right = input.find_last_not_of(' ');
		if (right == std::string::npos)
		{
			right = 0;
		}
		return input.substr(left, right - left + 1);
	}

	/* Removes whitespace from beginning of the string */
	static inline std::string TrimFront(const std::string& input) noexcept
	{
		// Remove whitespace on the left
		auto left = input.find_first_not_of(' ');
		if (left == std::string::npos)
			return input;
		
		return input.substr(left, input.size());
	}

	/* Removes whitespace from end of the string */
	static inline std::string TrimBack(const std::string& input) noexcept
	{
		// Remove whitespace on the right
		auto right = input.find_last_not_of(' ');
		if (right == std::string::npos)
			return input;

		return input.substr(right, input.size());
	}

	/* Returns true if the passed in string is an integer number */
	static inline bool IsNumber(const std::string& input) noexcept
	{
		return !input.empty() && std::all_of(input.begin(), input.end(), ::isdigit);
	}

	/* Returns a string of the passed in number formated with decimal spaces */
	template <typename T>
	static inline std::string ToStringWithPrecision(const T& number, const int decimalPlaces) noexcept
	{
		std::ostringstream toOutput;
		toOutput << std::fixed << std::setprecision(decimalPlaces) << number;
		return toOutput.str();
	}

	/* Checks if the substring is contained anywhere in the strung */
	static inline bool Contains(std::string input, std::string substring) noexcept
	{
		if (substring.size() > input.size()) return false;
		return input.find(substring) != std::string::npos;
	}

	/* Check if the a string starts with a certain substring */
	static inline bool StartsWith(std::string input, std::string substring) noexcept
	{
		if (substring.size() > input.size()) return false;
		return std::equal(substring.begin(), substring.end(), input.begin());
	}

	/* Check if the a string ends with a certain substring */
	static inline bool EndsWith(std::string input, std::string substring) noexcept
	{
		if (substring.size() > input.size()) return false;
		return std::equal(substring.rbegin(), substring.rend(), input.rbegin());
	}

	/* Checks whether a character is a lowercase */
	static inline bool IsLower(char c) noexcept
	{
		return (c >= 'a' && c <= 'z');
	}

	/* Checks whether a character is an uppercase */
	static inline bool IsUpper(char c) noexcept
	{
		return (c >= 'A' && c <= 'Z');
	}

	/* Checks if a letter is an ASCII character */
	static inline bool IsChar(char c) noexcept
	{
		return (IsUpper(c) || IsLower(c));
	}

	/* Converts a string to all uppercase */
	static inline std::string ToUpper(const std::string &toConvert) noexcept
	{
		std::string toReturn;

		for (const char & c : toConvert)
		{
			if (IsLower(c))
				toReturn.push_back(c - 32);
			else
				toReturn.push_back(c);
		}

		return toReturn;
	}

	/* Converts a string to all lowercase */
	static inline std::string ToLower(const std::string &toConvert) noexcept
	{
		std::string toReturn;

		for (const char & c : toConvert)
		{
			if (IsUpper(c))
				toReturn.push_back(c + 32);
			else
				toReturn.push_back(c);
		}

		return toReturn;
	}

	/* Format strings with equal spacing between tokens */
	/* BASED ON: https://stackoverflow.com/questions/14765155/how-can-i-easily-format-my-data-table-in-c */
	static inline std::string centerString(const std::string & s, const int w) {
		std::stringstream ss, spaces;
		int padding = w - s.size();
		for(int i = 0; i < padding / 2; ++i)
			spaces << " ";
		ss << spaces.str() << s << spaces.str();
		// Add one extra space for odd paddings
		if(padding > 0 && padding % 2 != 0)
			ss << " ";
		return ss.str();
	}
}

#endif