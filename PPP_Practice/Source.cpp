/*#include "std_lib_facilities.h"

int main()
{
	cout << "Greetings, program!" << endl;
	
	return 0;
	
}*/   


#include "std_lib_facilities.h"

//------------------------------------------------------------------------------

/*grammar for code

Statement:
    Expression
    Print
    Quit

Print:
    =
	
Quit:
    x

Expression:
    Term
    Expression + Term
    Expression � Term

Term:
    Primary
    Term * Primary
    Term / Primary
    Term % Primary

Primary:
    Number
    ( Expression )
    � Primary
    + Primary

Number:
    floating-point-literal

Input comes from cin through the Token_stream called ts.
*/

class Token
{
public:
	char kind;        // what kind of token
	double value;     // for numbers: a value

	Token(char ch) : kind(ch), value(0)    // make a Token from a char
	{}
	Token(char ch, double val) : kind(ch), value(val)    // make a Token from a char and a double
	{}
};

//------------------------------------------------------------------------------

class Token_stream
{
public:
	Token_stream();           // make a Token_stream that reads from cin
	Token get();              // get a Token (get() is defined elsewhere)
	void putback(Token t);    // put a Token back

private:
	bool full = false;        // is there a Token in the buffer?
	Token buffer;     // here is where we keep a Token put back using putback()
};

//------------------------------------------------------------------------------

// The constructor just sets full to indicate that the buffer is empty:
Token_stream::Token_stream() : full(false), buffer(0)    // no Token in buffer
{}

//------------------------------------------------------------------------------

// The putback() member function puts its argument back into the Token_stream's buffer:
void Token_stream::putback(Token t)
{
	if (full) error("putback() into a full buffer");
	buffer = t;       // copy t to buffer
	full = true;      // buffer is now full
}

//------------------------------------------------------------------------------

const char number = '8'; //t.kind == '8' means that t is a number token
const char print = '='; //t.kind == '8' means that t is a print token
const char quit = 'x'; //t.kind == 'x' means that t is a exit token

Token Token_stream::get()
{
	if (full)            // do we already have a Token ready?
	{
		full = false;   // remove token from buffer
		return buffer;
	}

	char ch = 0;
	cin >> ch;    // note that >> skips whitespace (space, newline, tab, etc.)

	switch (ch)
	{
	case print:    //wont recognize this as default case.
	case quit:    //  wont recognize this as default case.

	case '(': //let each character represent themselves
	case ')': 
	case '+': 
	case '-': 
	case '*': 
	case '/': 
	case '%':
		return Token(ch);        // A token is created of that character. let each character represent itself

	case '.':   //wont recognize this as default case.

	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	{
		cin.putback(ch);         // put digit back into the input stream
		double val = 0;
		cin >> val;                   // read a floating-point number. previously read char is converted into double.
		return Token(number, val);   
	}
	default:
		error("Bad token");
		return NULL;
	}
}

//------------------------------------------------------------------------------

Token_stream ts;        // provides get() and putback()

						//------------------------------------------------------------------------------

double expression();    // declaration so that primary() can call expression()

						//------------------------------------------------------------------------------

double primary()       // deal with numbers and parentheses
{
	Token t = ts.get();
	switch (t.kind)
	{
	case '(':    // handle '(' expression ')'
	{
		double d = expression();
		t = ts.get();
		if (t.kind != ')') error("')' expected");
		return d;
	}
	case number:            // we use '8' to represent a number
		return t.value;  // return the number's value

	case '-':
		return - primary();

	case '+':
		return + primary();

	default:
		error("primary expected");
		return NULL;
	}
}

//------------------------------------------------------------------------------

// deal with *, /, and %
double term()
{
	double left = primary();
	Token t = ts.get();        // get the next token from token stream

	while (true)
	{
		switch (t.kind)
		{
		case '*':
			left *= primary();
			t = ts.get();
			break;
		case '/':
		{
			double d = primary();
			if (d == 0) error("divide by zero");
			left /= d;
			t = ts.get();
			break;
		}
		case '%':
		{
			double d = primary();
			if (d == 0) error("divide by zero");
			left = fmod(left, d); //fmod(cmath) = floting point modulo 
			
			/* or prohibit the modulo of floting point integers by narrowcasting them to int
			int i1 = narrow_cast<int>(left);
			int i2 = narrow_cast<int>(primary());
			left = i1 % i2;
			*/
			
			t = ts.get();
			break;
		}
		default:
			ts.putback(t);     // put t back into the token stream
			return left;
		}
	}
}

//------------------------------------------------------------------------------

// deal with + and -
double expression()
{
	double left = term();      // read and evaluate a Term
	Token t = ts.get();        // get the next token from token stream

	while (true)
	{
		switch (t.kind)
		{
		case '+':
			left += term();    // evaluate Term and add
			t = ts.get();
			break;
		case '-':
			left -= term();    // evaluate Term and subtract
			t = ts.get();
			break;
		default:
			ts.putback(t);     // put t back into the token stream
			return left;       // finally: no more + or -: return the answer
		}
	}
}

//------------------------------------------------------------------------------

void calculate()
{
	double val = 0;
	cout << "('=' to print, 'x' to quit) \n" << "> ";
	while (cin) //always will ask for input. infinite loop.
	{
		try
		{
			Token t = ts.get();

			while (t.kind == print)
				t = ts.get();

			if (t.kind == quit)
				return;

			ts.putback(t);
			cout << "=" << expression() << '\n';
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}

int main()
{
	try
	{
		calculate();
		keep_window_open();
		return 0;
	}
	catch (exception& e)
	{
		cerr << "error: " << e.what() << '\n';
		keep_window_open("~~");
		/*char ch;
		cout << "Type ~ to close the window" << endl;
			while (cin >> ch)
			{
				if (ch == '~')
					return 1;
			}*/
		return 1; //this return wouldn't be called because the previous loop will never end until '~' is passed to it.
	}
	catch (...)
	{
		cerr << "Oops: unknown exception!\n";
		keep_window_open("~~");
		return 2;
	}

}
//------------------------------------------------------------------------------



