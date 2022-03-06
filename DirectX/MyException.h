#include <exception>
#include <string>

class MyException : std::exception {
public:
	MyException(int line, const char* file);
	const char* what();
private:
	int line;
	std::string file;
	std::string whatBuffer;
};