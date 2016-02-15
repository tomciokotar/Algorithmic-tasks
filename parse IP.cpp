#include <cstdio>
#include <string>

using std::string;

bool dotsDigitsOnly(const string &text)
{
	for (unsigned int i = 0; i < text.size(); i++)
		if (text[i] != '.' && (text[i] < '0' || text[i] > '9'))
			return false;
	
	return true;
}

int countDots(const string &text)
{
	int res = 0;
	
	for (unsigned int i = 0; i < text.size(); i++)
		if (text[i] == '.')
			res++;
	
	return res;
}

bool isRangeCorrect(string text)
{
	return text.size() <= 3 && text.size() > 0 && (text.size() == 1 || text[0] != '0');
}

int stringToInt(string text)
{
	int res = 0;
	
	for (unsigned int i = 0; i < text.size(); i++)
		res = 10*res + (int)(text[i] - 48);
		
	return res;
}

bool isCorrectIP(const string &text)
{
	if (text.size() > 20 || !dotsDigitsOnly(text) || countDots(text) != 3)
		return false;
	
	int rangeBreaks[5];
	
	rangeBreaks[0] = -1;
	rangeBreaks[4] = text.size();
	
	for (int i = 0, currentRange = 1; i < (int) text.size(); i++)
		if (text[i] == '.')
			rangeBreaks[currentRange++] = i;
	
	for (int i = 0; i < 4; i++) {
		int rangeStart = rangeBreaks[i] + 1;
		int rangeSize = rangeBreaks[i+1] - rangeBreaks[i] - 1;
		
		if (!isRangeCorrect(text.substr(rangeStart, rangeSize)))
			return false;
		else if (stringToInt(text.substr(rangeStart, rangeSize)) > 255)
			return false;
	}
	
	return true;
}

int main()
{
	char charText[100];
	string text;
	
	while (scanf("%s", charText)) {
		text = charText;
		
		if (text == "-1")
			break;
		
		if (isCorrectIP(text))
			printf("YES\n");
		else
			printf("NO\n");
	}
	
	return 0;
}
