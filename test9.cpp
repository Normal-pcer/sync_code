#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>

using namespace std;

namespace MD5 {
	typedef struct
	{
		unsigned int count[2];
		unsigned int state[4];
		unsigned char buffer[64];   
	}MD5_CTX;
	 
							 
	#define F(x,y,z) ((x & y) | (~x & z))
	#define G(x,y,z) ((x & z) | (y & ~z))
	#define H(x,y,z) (x^y^z)
	#define I(x,y,z) (y ^ (x | ~z))
	#define ROTATE_LEFT(x,n) ((x << n) | (x >> (32-n)))
	#define FF(a,b,c,d,x,s,ac) \
			  { \
			  a += F(b,c,d) + x + ac; \
			  a = ROTATE_LEFT(a,s); \
			  a += b; \
			  }
	#define GG(a,b,c,d,x,s,ac) \
			  { \
			  a += G(b,c,d) + x + ac; \
			  a = ROTATE_LEFT(a,s); \
			  a += b; \
			  }
	#define HH(a,b,c,d,x,s,ac) \
			  { \
			  a += H(b,c,d) + x + ac; \
			  a = ROTATE_LEFT(a,s); \
			  a += b; \
			  }
	#define II(a,b,c,d,x,s,ac) \
			  { \
			  a += I(b,c,d) + x + ac; \
			  a = ROTATE_LEFT(a,s); \
			  a += b; \
			  }                                            
	void MD5Init(MD5_CTX *context);
	void MD5Update(MD5_CTX *context,unsigned char *input,unsigned int inputlen);
	void MD5Final(MD5_CTX *context,unsigned char digest[16]);
	void MD5Transform(unsigned int state[4],unsigned char block[64]);
	void MD5Encode(unsigned char *output,unsigned int *input,unsigned int len);
	void MD5Decode(unsigned int *output,unsigned char *input,unsigned int len);
	  
	unsigned char PADDING[]={0x80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
							 
	void MD5Init(MD5_CTX *context)
	{
		 context->count[0] = 0;
		 context->count[1] = 0;
		 context->state[0] = 0x67452301;
		 context->state[1] = 0xEFCDAB89;
		 context->state[2] = 0x98BADCFE;
		 context->state[3] = 0x10325476;
	}
	void MD5Update(MD5_CTX *context,unsigned char *input,unsigned int inputlen)
	{
		unsigned int i = 0,index = 0,partlen = 0;
		index = (context->count[0] >> 3) & 0x3F;
		partlen = 64 - index;
		context->count[0] += inputlen << 3;
		if(context->count[0] < (inputlen << 3))
		   context->count[1]++;
		context->count[1] += inputlen >> 29;
		
		if(inputlen >= partlen)
		{
		   memcpy(&context->buffer[index],input,partlen);
		   MD5Transform(context->state,context->buffer);
		   for(i = partlen;i+64 <= inputlen;i+=64)
			   MD5Transform(context->state,&input[i]);
		   index = 0;        
		}  
		else
		{
			i = 0;
		}
		memcpy(&context->buffer[index],&input[i],inputlen-i);
	}
	void MD5Final(MD5_CTX *context,unsigned char digest[16])
	{
		unsigned int index = 0,padlen = 0;
		unsigned char bits[8];
		index = (context->count[0] >> 3) & 0x3F;
		padlen = (index < 56)?(56-index):(120-index);
		MD5Encode(bits,context->count,8);
		MD5Update(context,PADDING,padlen);
		MD5Update(context,bits,8);
		MD5Encode(digest,context->state,16);
	}
	void MD5Encode(unsigned char *output,unsigned int *input,unsigned int len)
	{
		unsigned int i = 0,j = 0;
		while(j < len)
		{
			 output[j] = input[i] & 0xFF;  
			 output[j+1] = (input[i] >> 8) & 0xFF;
			 output[j+2] = (input[i] >> 16) & 0xFF;
			 output[j+3] = (input[i] >> 24) & 0xFF;
			 i++;
			 j+=4;
		}
	}
	void MD5Decode(unsigned int *output,unsigned char *input,unsigned int len)
	{
		 unsigned int i = 0,j = 0;
		 while(j < len)
		 {
			   output[i] = (input[j]) |
						   (input[j+1] << 8) |
						   (input[j+2] << 16) |
						   (input[j+3] << 24);
			   i++;
			   j+=4; 
		 }
	}
	void MD5Transform(unsigned int state[4],unsigned char block[64])
	{
		 unsigned int a = state[0];
		 unsigned int b = state[1];
		 unsigned int c = state[2];
		 unsigned int d = state[3];
		 unsigned int x[64];
		 MD5Decode(x,block,64);
		 FF(a, b, c, d, x[ 0], 7, 0xd76aa478); /* 1 */
	 FF(d, a, b, c, x[ 1], 12, 0xe8c7b756); /* 2 */
	 FF(c, d, a, b, x[ 2], 17, 0x242070db); /* 3 */
	 FF(b, c, d, a, x[ 3], 22, 0xc1bdceee); /* 4 */
	 FF(a, b, c, d, x[ 4], 7, 0xf57c0faf); /* 5 */
	 FF(d, a, b, c, x[ 5], 12, 0x4787c62a); /* 6 */
	 FF(c, d, a, b, x[ 6], 17, 0xa8304613); /* 7 */
	 FF(b, c, d, a, x[ 7], 22, 0xfd469501); /* 8 */
	 FF(a, b, c, d, x[ 8], 7, 0x698098d8); /* 9 */
	 FF(d, a, b, c, x[ 9], 12, 0x8b44f7af); /* 10 */
	 FF(c, d, a, b, x[10], 17, 0xffff5bb1); /* 11 */
	 FF(b, c, d, a, x[11], 22, 0x895cd7be); /* 12 */
	 FF(a, b, c, d, x[12], 7, 0x6b901122); /* 13 */
	 FF(d, a, b, c, x[13], 12, 0xfd987193); /* 14 */
	 FF(c, d, a, b, x[14], 17, 0xa679438e); /* 15 */
	 FF(b, c, d, a, x[15], 22, 0x49b40821); /* 16 */
	 
	 /* Round 2 */
	 GG(a, b, c, d, x[ 1], 5, 0xf61e2562); /* 17 */
	 GG(d, a, b, c, x[ 6], 9, 0xc040b340); /* 18 */
	 GG(c, d, a, b, x[11], 14, 0x265e5a51); /* 19 */
	 GG(b, c, d, a, x[ 0], 20, 0xe9b6c7aa); /* 20 */
	 GG(a, b, c, d, x[ 5], 5, 0xd62f105d); /* 21 */
	 GG(d, a, b, c, x[10], 9,  0x2441453); /* 22 */
	 GG(c, d, a, b, x[15], 14, 0xd8a1e681); /* 23 */
	 GG(b, c, d, a, x[ 4], 20, 0xe7d3fbc8); /* 24 */
	 GG(a, b, c, d, x[ 9], 5, 0x21e1cde6); /* 25 */
	 GG(d, a, b, c, x[14], 9, 0xc33707d6); /* 26 */
	 GG(c, d, a, b, x[ 3], 14, 0xf4d50d87); /* 27 */
	 GG(b, c, d, a, x[ 8], 20, 0x455a14ed); /* 28 */
	 GG(a, b, c, d, x[13], 5, 0xa9e3e905); /* 29 */
	 GG(d, a, b, c, x[ 2], 9, 0xfcefa3f8); /* 30 */
	 GG(c, d, a, b, x[ 7], 14, 0x676f02d9); /* 31 */
	 GG(b, c, d, a, x[12], 20, 0x8d2a4c8a); /* 32 */
	 
	 /* Round 3 */
	 HH(a, b, c, d, x[ 5], 4, 0xfffa3942); /* 33 */
	 HH(d, a, b, c, x[ 8], 11, 0x8771f681); /* 34 */
	 HH(c, d, a, b, x[11], 16, 0x6d9d6122); /* 35 */
	 HH(b, c, d, a, x[14], 23, 0xfde5380c); /* 36 */
	 HH(a, b, c, d, x[ 1], 4, 0xa4beea44); /* 37 */
	 HH(d, a, b, c, x[ 4], 11, 0x4bdecfa9); /* 38 */
	 HH(c, d, a, b, x[ 7], 16, 0xf6bb4b60); /* 39 */
	 HH(b, c, d, a, x[10], 23, 0xbebfbc70); /* 40 */
	 HH(a, b, c, d, x[13], 4, 0x289b7ec6); /* 41 */
	 HH(d, a, b, c, x[ 0], 11, 0xeaa127fa); /* 42 */
	 HH(c, d, a, b, x[ 3], 16, 0xd4ef3085); /* 43 */
	 HH(b, c, d, a, x[ 6], 23,  0x4881d05); /* 44 */
	 HH(a, b, c, d, x[ 9], 4, 0xd9d4d039); /* 45 */
	 HH(d, a, b, c, x[12], 11, 0xe6db99e5); /* 46 */
	 HH(c, d, a, b, x[15], 16, 0x1fa27cf8); /* 47 */
	 HH(b, c, d, a, x[ 2], 23, 0xc4ac5665); /* 48 */
	 
	 /* Round 4 */
	 II(a, b, c, d, x[ 0], 6, 0xf4292244); /* 49 */
	 II(d, a, b, c, x[ 7], 10, 0x432aff97); /* 50 */
	 II(c, d, a, b, x[14], 15, 0xab9423a7); /* 51 */
	 II(b, c, d, a, x[ 5], 21, 0xfc93a039); /* 52 */
	 II(a, b, c, d, x[12], 6, 0x655b59c3); /* 53 */
	 II(d, a, b, c, x[ 3], 10, 0x8f0ccc92); /* 54 */
	 II(c, d, a, b, x[10], 15, 0xffeff47d); /* 55 */
	 II(b, c, d, a, x[ 1], 21, 0x85845dd1); /* 56 */
	 II(a, b, c, d, x[ 8], 6, 0x6fa87e4f); /* 57 */
	 II(d, a, b, c, x[15], 10, 0xfe2ce6e0); /* 58 */
	 II(c, d, a, b, x[ 6], 15, 0xa3014314); /* 59 */
	 II(b, c, d, a, x[13], 21, 0x4e0811a1); /* 60 */
	 II(a, b, c, d, x[ 4], 6, 0xf7537e82); /* 61 */
	 II(d, a, b, c, x[11], 10, 0xbd3af235); /* 62 */
	 II(c, d, a, b, x[ 2], 15, 0x2ad7d2bb); /* 63 */
	 II(b, c, d, a, x[ 9], 21, 0xeb86d391); /* 64 */
		 state[0] += a;
		 state[1] += b;
		 state[2] += c;
		 state[3] += d;
	}
	string MD5Hash(const string& text) {
		char hash[32];
	    unsigned char digest[16];
	    MD5_CTX ctx;
	    
	    MD5Init(&ctx);
	    MD5Update(&ctx, (unsigned char *)text.c_str(), text.size());
	    MD5Final(&ctx, digest);
	    
	    sprintf(hash, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
			digest[0], digest[1], digest[2], digest[3], digest[4],
			digest[5], digest[6], digest[7], digest[8], digest[9],
			digest[10], digest[11], digest[12], digest[13], digest[14], digest[15]);
		
		return hash;
	}
}

using namespace MD5;

// URL decoding function
string decode_url(const string& url) {
    string decoded_url;
    for (size_t i = 0; i < url.length(); ++i) {
        if (url[i] == '%') {
            if (i + 2 < url.length()) {
                int value = 0;
                stringstream ss;
                ss << hex << url.substr(i + 1, 2);
                ss >> value;
                decoded_url += static_cast<char>(value);
                i += 2; // Skip the encoded characters
            }
        } else if (url[i] == '+') {
            decoded_url += ' '; // Convert '+' to space
        } else {
            decoded_url += url[i]; // Directly add unencoded characters
        }
    }
    return decoded_url;
}

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
using namespace Formatter;

// Simulated database
map<int, string> database;

// Register user
string registerUser(const string& name) {
    int uid = database.size() + 10000;
    InfoTable userInfo;
    // STOP CRACKING MD5 PLEASE THIS IS NOT WHAT WE WANT
    userInfo["admin"] = MD5Hash(name) == "f1d1c50c0e066b430a6d8ac375b75c57" ? "true" : "false";
    userInfo["time"] = std::to_string(time(0));
    userInfo["name"] = name;
    
    // Store user information
    database[uid] = toJson(userInfo);
    return "Registered user. UID is " + std::to_string(uid);
}

// Execute user command
string info(const string& uid) {
	int currentUser = atoi(uid.c_str());
    if (database.find(currentUser) == database.end()) throw (string)"User not found";

    InfoTable userInfo = toTable(database[currentUser]);
    string result = "";
    result += "Name: " + userInfo["name"] + "\n\n";
    result += "Registered at: " + userInfo["time"] + "\n\n";
    result += "You are ";
    result += userInfo["admin"] == "true" ? "admin" : "user";
    
    return result;
}

// Process request
string workWithRequest(const string& request) {
    try {
        string r = decode_url(request);
        check(r);
        if (r.size() >= 9 && r.substr(0, 9) == "register/") {
            return registerUser(r.substr(9));
        } else if (r.size() >= 5 && r.substr(0, 5) == "info/") {
            return info(r.substr(5));
        } else {
            return "not found";
		}
    } catch (string s) {
        return "ERROR:\n- " + s;
    }
}

// Handle client request
void handle_request(SOCKET client_socket) {
    char buffer[1024];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        string request(buffer);
        
        string method;
        string uri;
        istringstream request_stream(request);
        request_stream >> method;
        request_stream >> uri;

        if (method == "GET") {
            string decoded_uri = decode_url(uri.substr(1));
            string response_body = workWithRequest(decoded_uri);
            string response_header = "HTTP/1.1 200 OK\r\n"
                                     "Content-Type: text/plain\r\n"
                                     "Content-Length: " + to_string(response_body.size()) + "\r\n\r\n";

            string full_response = response_header + response_body;
            send(client_socket, full_response.c_str(), full_response.size(), 0);
        }
    }
    closesocket(client_socket);
}

// Start the server
void start_server(int port) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    listen(server_socket, SOMAXCONN);

    cout << "Server is running on port " << port << "..." << endl;

    while (true) {
        SOCKET client_socket = accept(server_socket, nullptr, nullptr);
        thread(handle_request, client_socket).detach();
    }

    closesocket(server_socket);
    WSACleanup();
}

int main() {
    start_server(0xff0); // Start the server
    return 0;
}
