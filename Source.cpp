#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

int main()
{
	//********FILE CREATION*******
	ifstream file("transitionTable.txt");
	int states = 0, cols = 0;
	string row, temp, column;
	stringstream ss;

	//******** COUNTING STATES******
	if (!file)
		cout << "File not found..." << endl;
	else
	{
		while (!file.eof())
		{
			getline(file, row);
			++states;
		}
	}
	file.close();
	cout << "States = " << states - 1 << endl;

	//***********COUNTING COLUMNS*******
	ifstream file1("transitionTable.txt");
	getline(file1, column);
	ss.clear();
	ss << column;

	while (ss)
	{
		ss >> temp;
		cols++;
	}
	//cols = 9;
	cout << "Columns = " << cols << endl;
	file1.close();

	//*******CREATING INITIAL ARRAY*********
	int **initialArray = new int*[states];
	for (int i = 0; i < states; i++)
	{
		initialArray[i] = new int[128];
		for (int j = 0; j < 128; j++)
			initialArray[i][j] = -1;
	}

	//********INPUT ELEMENTS********
	ifstream file2("transitionTable.txt");
	cout << "symbols:\n";
	int *symbols = new int[cols - 1];
	for (int i = 0; i < cols - 1; i++)
	{
		file2 >> symbols[i];
		cout << symbols[i] << ' ';
	}

	//******TRANSITION STATES********
	int **inputElements = new int*[states - 1];
	for (int i = 0; i < states; i++)
	{
		inputElements[i] = new int[cols];
		for (int j = 0; j < cols; j++)
			file2 >> inputElements[i][j];
	}

	//**********FINAL STATES************
	cout << "\nFinal States: ";
	int *finalStates = new int[states - 1];
	for (int i = 0; i < states - 1; i++)
	{
		finalStates[i] = inputElements[i][cols - 1];
		cout << finalStates[i] << ' ';
	}

	//***********MAPPING***************
	int val;
	for (int i = 0; i < cols - 1; i++)
	{
		val = symbols[i];
		for (int j = 0; j < states - 1; j++)
			initialArray[j][val] = inputElements[j][i];
	}

	//******COMPACTION***********
	int counter = 0;
	int identicalValueCounter;
	int indexArray[128];
	for (int i = 0; i < 128; i++)
		indexArray[i] = -1;

	for (int i = 0; i < 128; i++)
	{
		identicalValueCounter = 0;
		for (int j = 0; j < states - 1; j++)
		{
			if (initialArray[j][i] == -1)
				identicalValueCounter++;
		}
		if (identicalValueCounter == states - 1)
		{
			indexArray[i] = 0;
			counter++;
		}
	}

	int val1 = 0, val2 = 0, c = 0, counter1 = 0, ind2 = 0, ind2Val = -1;
	for (int i = 0; i < cols - 1; i++)
	{
		if (indexArray[symbols[i]] == -1)
		{
			val1 = symbols[i];
			for (int j = i + 1; j < cols - 1; j++)
			{
				val2 = symbols[j];
				if (indexArray[symbols[j]] == -1)
				{
					c = 0;
					for (int k = 0; k < states - 1; k++)            //comparing two columns
						if (initialArray[k][val1] == initialArray[k][val2])
							c++;

					if (c == states - 1)
					{
						if (val1 != ind2Val)
						{
							ind2++;
							indexArray[val1] = ind2;
							indexArray[val2] = ind2;
							ind2Val = val1;
							counter1++;
						}
						else
						{
							indexArray[val2] = indexArray[val1];
							counter1++;
						}
					}
				}
			}
		}
		if (indexArray[val1] == -1)
		{
			ind2++;
			indexArray[val1] = ind2;
		}
	}

	//******ARRAY AFTER COMPACTION******
	int newColumns, tempVal = 0;
	newColumns = 128 - counter - counter1 + 1;
	
	int **newArray = new int*[states - 1];
	for (int i = 0; i < states - 1; i++)
		newArray[i] = new int[newColumns];

	for (int i = 0; i < newColumns; i++)
	{
		for (int k = 0; k < 128; k++)
		{
			if (i == indexArray[k])
			{
				tempVal = k;
				break;
			}
		}
		for (int j = 0; j < states - 1; j++)
			newArray[j][i] = initialArray[j][tempVal];
	}
	//*********PRINTING COMPACTED ARRAY******
	cout << "\nTransition Table:" << endl;
	for (int i = 0; i < states - 1; i++)
	{
		for (int j = 0; j < newColumns; j++)
			cout << newArray[i][j] << " ";                
		cout << endl;
	}

	//*********STRING TRAVERSAL***********
	ifstream in("inputString.txt");
	string input;
	getline(in, input, '\0');
	int size, inCount = 0;
	size = input.length();

	cout << "Input String:\n" << input << endl;
	/*for (int i = 0; i < input.length(); i++)
	{
		if (input[i] == '\\' && input[i + 1] == 'n')
			inCount++;
		else if (input[i] == '\\' && input[i + 1] == 't')
			inCount++;
	}
	cout << "inCoutn = " << inCount << endl;
	
	string newInput = "";
	for (int i = 0; i < input.length(); i++)
	{
		if (input[i] == '\\' && input[i + 1] == 'n')
		{
			newInput =newInput+ '\n';
			i++;
		}
		else if (input[i] == '\\' && input[i + 1] == 't')
		{
			newInput = newInput + '\t';
			i++;
		}
		else 
			newInput=newInput+input[i];
	}
	cout << "nay string:\n";
	for (int i = 0; i < newInput.length(); i++)
	{
		if (input[i] == '\\n')
		cout << '\n';
		if (input[i] == '\\t')
			cout << '\t';
		if (input[i] == ' ')
			cout << "space";
		else
			cout << input[i];
	}*/

	cout << "\nTOKENS:  \n";
	int forward = 0, lexemeStart = 0, lastSeenFinalState = -1, remember = -1, lineNo = 0, currentState = lexemeStart;

	for (forward = 0; forward < input.length(); forward++)
	{
		currentState = newArray[currentState][indexArray[int(input[forward])]];
			if (finalStates[currentState] > 0)
			{
				lastSeenFinalState = finalStates[currentState];
				remember = forward;
			}

		if (currentState == -1)
		{
			if (lastSeenFinalState == -1)
			{
				cout << input[lexemeStart] << ",  error\n";
				forward=lexemeStart;
				lexemeStart = forward + 1;
			}

			else
			{
				cout << "<";
				for (int m = lexemeStart; m <= remember; m++)
					cout << input[m];
				cout << ",  " << lastSeenFinalState << ">\n";

				lexemeStart = remember + 1;
				forward = remember ;
			}
			currentState = 0;
			lastSeenFinalState = -1;
			remember = -1;
		}
	}
	if (forward == input.length())
	{
		if (lastSeenFinalState != -1)
		{
			cout << "<";
			for (int m = lexemeStart; m <= forward - 1; m++)
				cout << input[m];
			cout << ",  " << lastSeenFinalState << ">\n\n\n\n";
		}

		//else if (lastSeenFinalState == -1 && lexemeStart != input.length())
			//cout << "<" << input[lexemeStart] << ",  error>\n";
	}
	return 0;
}