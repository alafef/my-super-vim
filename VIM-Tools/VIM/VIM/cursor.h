#
class Cursor
{
	// Char array
private:
	char* str;
	int cap;
	int len;
	// Constructor
public:
	MyString();
	Cursor();
	~Cursor();

	unsigned int cur_pos;
	unsigned int cur_line;
	unsigned int max_lines;
	MyString(const MyString& other);
	MyString(const char* new_str);
	MyString(std::initializer_list<char>& il);
	MyString(const std::string& other);
	MyString(char* new_str, int count);
	MyString(int count, char s);
	// Shift ostream and istream