#include <bits/stdc++.h>
using namespace std;

namespace StringChecker {
    const string BANNED_CHAR = "\n\a\t\b\r\v\f";

    // Check if the input string contains illegal characters
    void check(const string& input) {
        for (auto c : BANNED_CHAR) {
            if (input.find(c) != string::npos) {
                throw (string)"Attack detected. Don't use illegal chars.";
            }
        }
    }

    // Escape illegal characters in the string
    string escape(const string& input) {
        string result = "";
        for (auto c : input) {
            if (c == '\"' || c == '\\') result.push_back('\\');
            result.push_back(c); 
        }
        return result;
    }

    // Unescape the escaped characters in the string
    string unescape(const string& str) {
        string unescaped;
        for (int i = 0; i < str.size(); ++i) {
            if (str[i] == '\\' && i + 1 < str.size()) i++; // Skip escape character
            unescaped += str[i];
        }
        return unescaped;
    }
}
using namespace StringChecker;

namespace Formatter {
    typedef map<string, string> InfoTable;

    // Convert info table to JSON format string
    string toJson(const InfoTable& m) {
        string result = "{";
        bool first = true;
        for (const auto& entry : m) {
            if (first) first = false;
            else result += ',';
            result += '"' + escape(entry.first) + '"';
            result += ':';
            result += '"' + escape(entry.second) + '"';
        }
        return result + "}";
    }

    // Parse JSON string to info table
    InfoTable toTable(const string& json) {
        InfoTable m;
        int idx = 0;

        auto readChar = [&](char c) {
            while (idx < json.size() && isspace(json[idx])) idx++;
            if (idx >= json.size() || (c && json[idx] != c)) {
                throw (string)"Invalid data: expected '" + string(1, c) + "', not '" + json[idx] + "'";
            }
            idx++;
        };

        auto readStr = [&]() {
            string result = "";
            char terminator = 0;

            if (idx < json.size() && (json[idx] == '"' || json[idx] == '\'')) {
                terminator = json[idx];
                idx++;
            } else {
                throw (string)"Invalid data: expected a string, not '" + json[idx] + "'";
            }
            while (idx < json.size()) {
                if (json[idx] == terminator) break;
                if (json[idx] == '\\') {
                    idx++;
                    if (idx >= json.size()) throw (string)"Invalid data: unterminated string";
                }
                
                // GBK Encoding
                if (json[idx] & 0x80) {
                    result.push_back(json[idx]);
                    idx++;
                }
                result.push_back(json[idx]);
                idx++;
            }
            readChar(terminator);
            return result;
        };

        readChar('{');
        while (idx < json.size() && (json[idx] == '"' || json[idx] == '\'')) {
            string key = readStr();
            readChar(':');
            string value = readStr();
            m[key] = value;
            if (idx < json.size() && json[idx] == ',') readChar(',');
        }
        readChar('}');
        return m;
    }
}

int main() {
	using Formatter::toTable;
	try {
		string s = R"({"key":"value\"\\\""})";
		auto t = toTable(s);
		cout << t["key"] << endl;
	} catch (string const &s) {
		cerr << s << endl;
	}
}