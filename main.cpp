/*
AnnMarie Kolbl : Homework 2
ak192@zips.uakron.edu | 3079881
JSON Parser for Object Oriented
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

//prototype for parse
void parse(ifstream &is, char &next);

//to hold weight
int weight = 0;

struct Value
{
	virtual ~Value() = default;
	virtual int weight() const {return 1;}
};

struct Null : Value //for null values
{
    string value;
	int weight() const {return 1;}
};

struct Bool : Value //for boolean values
{
    bool value;
	int weight() const {return 1;}
};

struct Number : Value //for integer values
{
    int value;
	int weight() const {return 1;}
};

struct String : Value, std::string //for string values
{
    string value;
	int weight() const {return 1;}
};

struct Object : Value //for object values
{
    char value;
    int weight() const {return 1;}
};

struct Array : Value, vector<Value *> //for arrays
{
	using std::vector<Value *>::vector;
	Array() {
		vector<Value *> vec;
	}

	~Array() {
		for (Value * v : *this)
			delete v;
	}
	char value;
	int weight() const {return 1;}
};

/* parseObject function accepts an ifstream object
and a character. Creates an object of Object (obj)
and adds obj.weight to weight, then calls parse. 
Returns nothing */
void parseObject(ifstream &is, char next)
{
    Object obj;

    if(next == '}')
    {
        obj.value = next;
        is.get(next);
        parse(is, next);
    }
    else
    {
        obj.value = next;

        weight += obj.weight();

        is.get(next);

        parse(is, next);
    }
}

/* parseString function accepts an ifstream object
and a character. Creates an object of String (str)
and adds str.weight to weight, then reads in the
rest of the string and gets the character after
that. Returns nothing. */
void parseString(ifstream &is, char &next)
{
    String str;

    weight += str.weight();

    getline(is, str.value, '"');
    is.get(next);
}

/* parseArray function accepts an ifstream object
and a character. Creates an object of Array (arr)
and adds arr.weight to weight, then calls parse.
Returns nothing */
void parseArray(ifstream &is, char next)
{
    Array arr;

    if(next == ']')
    {
        arr.value = next;
        is.get(next);
        parse(is, next);
    }
    else
    {
        arr.value = next;

        weight += arr.weight();

        is.get(next);

        parse(is, next);
    }
}

/* parseBool function accepts an ifstream object
and a character, both passed by reference. Creates 
an object of Bool (bl) and adds bl.weight to weight,
then reads in the rest of the boolean value and 
gets the character after that. Returns nothing. */
void parseBool(ifstream &is, char &next)
{
    Bool bl;
    string value;

    weight += bl.weight();

    getline(is, value, 'e');

    if(next == 't')
        bl.value = true;
    else
        bl.value = false;

    is.get(next);
}

/* parseNull function accepts an ifstream object
and a character, both passed by reference. Creates 
an object of Null (null) and adds null.weight to 
weight, then reads in the rest of the string and 
gets the character after that. Returns nothing. */
void parseNull(ifstream &is, char &next)
{
    Null null;

    weight += null.weight();

    getline(is, null.value, ',');
    is.get(next);
}

/* parseNumber function accepts an ifstream object
and a character, both passed by reference. Creates 
an object of Number (num) and adds str.weight to 
weight, then reads in the rest of the string and 
gets the character after that. Returns nothing. */
void parseNumber(ifstream &is, char &next)
{
    Number num;

    weight += num.weight();

    is.get(next);
    while (isdigit(next) || next == '.')
    {
        num.value = next;
        is.get(next);
    }

    is.get(next);
}

/* parse function accepts an ifstream object
and a character, both passed by reference.
Checks the value in next and calls the correct
function. Returns nothing. */
void parse(ifstream &is, char &next)
{
	while (!is.eof())
	{
		if (next == '{' || next == '}')
		{
		    parseObject(is, next);
        }
        else if (next == '"')
        {
            parseString(is, next);
        }
        else if (next == '[' || next == ']')
        {
            parseArray(is, next);
        }
        else if (next == 't' || next == 'f')
        {
            parseBool(is, next);
        }
        else if (next == 'n')
        {
            parseNull(is, next);
        }
        else if (isdigit(next))
        {
            parseNumber(is, next);
        }
        else
            is.get(next);
	}
}


int main()
{
	ifstream is;
	char next;

	is.open("JSONText.txt");

	if(!is.is_open())
    {
        cout << "Error opening file." << endl;
        return 0;
    }

    cout << "The parser is parsing files..." << endl;

    is.get(next);
	parse(is, next);

	is.close();

	cout << "Weight of the document: " << weight << endl;

	return 0;
}
