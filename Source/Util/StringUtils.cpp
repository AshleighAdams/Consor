#include "StringUtils.hpp"

#include <numeric>

std::string Consor::Util::FormatTimeSpan(double Time)
{
	using namespace std;

	if(Time != Time || Time == std::numeric_limits<double>::infinity())
		return "\u221E";

	static string names[] = {
		"second",
		"minute",
		"hour",
		"day",
		"week",
		"year",
		"decade",
		"century", // centuries, ohwell
	};

	static double ajustment[] = {
		60, // seconds in a min
		60, // mins in an hour
		24, // hours in a day
		7,  // days in a week
		52, // weeks in a year
		10, // years in a decade
		10, // decades in a century
		1
	};

	string ret;
	double units = Time;
	
	for(unsigned int i = 0; i < sizeof(ajustment) / sizeof(ajustment[0]) - 1; i++)
	{
		long long unit = (long long)fmod(units, ajustment[i]);
		units = units / ajustment[i];

		string unit_name = names[i];

		if(unit != 1)
			unit_name.append("s");

		string str_unit;
		stringstream str;
		str << unit;
		str >> str_unit;
				
		ret = str_unit + " " + unit_name + " " + ret;

		if(units < 1)
			break;
	}

	return ret;
}




inline bool IsSeperatorChar(char x)
{
	switch(x)
	{
	case ' ':
	case '-':
	case '/':
	case '\\':
		return true;
	default:
		return false;
	}
}

std::string Consor::Util::WrapText(const std::string& input, size_t width, size_t* pOutWidth, size_t* pOutHeight) // wrap the text to the specified input, wrapping as seen fit
{
	using namespace std;
	stringstream ss;
	
	size_t pos = 0;
	size_t line_start = 0;
	size_t rwidth = 0;
	size_t rheight = 0;
	size_t line_len = 0;

	size_t inlen = input.length();

	while(true)
	{
		if(pos >= inlen)
			break;

		const char& x = input[pos];

		// the space may have wrapped around, if so, then just ignore it.
		if(line_len == 0 && x == ' ')
		{
			pos++;
			continue; // we don't want these anymore
		}

		if(line_len + 1 > width)
		{
			int retries = min<size_t>(width, 10);
			bool failed = false;
			size_t start_pos = pos;

			while(!IsSeperatorChar(input[pos]))
			{
				pos--;
				if(retries-- == 0 || pos == 0)
				{
					pos = start_pos - 2;
					failed = true;
					break;
				}
			}

			if(!failed)
				pos++; // else the space, or other crap gets in the way

			// linebreak
			string line = input.substr(line_start, pos - line_start);

			if(failed)
				ss << line << "-\n";
			else
				ss << line << '\n';

			if(rwidth < pos - line_start)
				rwidth = pos - line_start;

			line_start = pos;
			line_len = 0;

			rheight++;
		}
		else if(x == '\n')
		{
			// linebreak
			string line = input.substr(line_start, pos - line_start);
			ss << line;

			if(rwidth < pos - line_start)
				rwidth = pos - line_start;

			line_start = pos;
			line_len = 0;

			rheight++;
		}
		else
			line_len++;

		//if(rwidth < pos - line_start)
		//		rwidth = pos - line_start;

		if(pos == input.length())
			break;
		pos++;
	}

	if(line_start != pos)
	{
		string line = input.substr(line_start, pos - line_start);
		ss << line;
		rheight++;

		if(rwidth < pos - line_start)
			rwidth = pos - line_start;
	}

	if(pOutWidth)
		*pOutWidth = rwidth;
	if(pOutHeight)
		*pOutHeight = rheight;

	return ss.str();
}
