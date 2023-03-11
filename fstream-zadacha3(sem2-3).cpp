#include <iostream>
#include <fstream>

const int MAX_LINE_SIZE = 110; //110 = max_size_firstName + max_size_lastName + max_size_email + max_size_fn + commas
const int MAX_NAME_SIZE = 32;
const int FN_SIZE = 11;			//10+1(for the '\0')
const int CHAR_FOR_MAJOR = 4;

enum class Major
{
	Informatics = 4,
	SoftwareEngineering = 6,
	ComputerScience = 8
};

struct Student
{
	Major major;
	char fristName[MAX_NAME_SIZE];
	char lastName[MAX_NAME_SIZE];
	char emial[MAX_NAME_SIZE];
	char fn[FN_SIZE];
};

unsigned getNumbeOfStudents(std::ifstream& file)
{
	char line[MAX_LINE_SIZE];
	unsigned numberOfStudents = 0;

	while (!file.eof())
	{
		file.getline(line, MAX_LINE_SIZE);
		numberOfStudents++;
	}
	file.seekg(0, std::ios::beg);
	return numberOfStudents - 1;
}

void getEachStudentData(const char*& source, char* destination)
{
	while (*source != ',' && *source != '\0')
	{
		*destination = *source;
		source++;
		destination++;
	}
	source++;
	*destination = '\0';
}

void getMajor(Student* students, const unsigned studentIndex)
{
	if (students[studentIndex].fn[CHAR_FOR_MAJOR] - '0' == (int)Major::Informatics)
	{
		students[studentIndex].major = Major::Informatics;
	}
	else if (students[studentIndex].fn[CHAR_FOR_MAJOR] - '0' == (int)Major::ComputerScience)
	{
		students[studentIndex].major = Major::ComputerScience;
	}
	else if (students[studentIndex].fn[CHAR_FOR_MAJOR] - '0' == (int)Major::SoftwareEngineering)
	{
		students[studentIndex].major = Major::SoftwareEngineering;
	}
}

void enterEachStudents(const char* line, Student* students, const unsigned studentIndex)
{
	char fristName[MAX_NAME_SIZE];

	getEachStudentData(line, students[studentIndex].fristName);
	getEachStudentData(line, students[studentIndex].lastName);
	getEachStudentData(line, students[studentIndex].emial);
	getEachStudentData(line, students[studentIndex].fn);

	getMajor(students, studentIndex);
}

void extractData(std::ifstream& file, Student* students)
{
	char line[MAX_LINE_SIZE];
	file.getline(line, MAX_LINE_SIZE); //vzema nachalniq red, koito ne ni trqbva

	unsigned studentIndex = 0; //s tazi promenliva sledim indexa na koito se namira suotvetniq student v masiva

	while (!file.eof())
	{
		file.getline(line, MAX_LINE_SIZE);
		enterEachStudents(line, students, studentIndex);
		studentIndex++;
	}
}

void getCommandInfo(const char* command, char* keyWord, char* fnOfFile, char* newMail)
{
	while (*command != ' ')
	{
		*keyWord = *command;
		keyWord++;
		command++;
	}
	*keyWord = '\0';
	command++;

	while (*command != ' ')
	{
		*fnOfFile = *command;
		fnOfFile++;
		command++;

		if (*command == '\0') //ako e "print" ili "save", zashtoto shte ima samo keyWord i fnOfFile
		{
			*fnOfFile = '\0';
			return;
		}
	}
	*fnOfFile = '\0';
	command++;

	while (*command != '\0') 
	{
		*newMail = *command;
		newMail++;
		command++;
	}
	*newMail = '\0';
	return;
}

void printCertainStudent(const unsigned numberOfStudents, const Student* students, const char* fnOrFile)
{
	bool availability = false;

	/*char fn[FN_SIZE];
	std::cout << "Faculti number: ";
	std::cin >> fn;
	std::cout << std::endl;*/

	for (size_t i = 0; i < numberOfStudents; i++)
	{
		if (strcmp(students[i].fn, fnOrFile) == 0)
		{
			availability = true;

			std::cout << "Name: " << students[i].fristName << " " << students[i].lastName << "; Email: " << students[i].emial << "; ";

			switch ((int)students[i].major)
			{
			case 4: std::cout << "Major: Informatics." << std::endl; break;
			case 6: std::cout << "Major: SoftwareEngineering." << std::endl; break;
			case 8: std::cout << "Major: ComputerScience." << std::endl; break;
			}
			break;
		}
	}

	if (!availability) std::cerr << "There is not a student with such a faculti number!";
}

void rewriteMail(Student& student, const char* newMail)
{
	size_t i = 0;

	for (i = 0; i < sizeof(newMail); i++)
	{
		student.emial[i] = newMail[i];
	}
	student.emial[i] = '\0';
}

void editMail(const unsigned numberOfStudents, Student* students, const char* fnOrFile, const char* newMail)
{
	bool availability = false;

	for (size_t i = 0; i < numberOfStudents; i++)
	{
		if (strcmp(students[i].fn, fnOrFile) == 0)
		{
			availability = true;

			rewriteMail(students[i], newMail);
		}
	}

	if (!availability) std::cerr << "There is not a student with such a faculti number!";
}

void saveNewInfo(std::ofstream& newFile, const unsigned numberOfStudents, const Student* students)
{
	newFile << "First name,Last name,Email address,FN" << std::endl;
	
	for (size_t i = 0; i < numberOfStudents-1; i++)
	{
		newFile << students[i].fristName << "," << students[i].lastName << "," << students[i].emial << "," << students[i].fn << std::endl;
	}
	newFile << students[numberOfStudents - 1].fristName << "," << students[numberOfStudents - 1].lastName << "," << students[numberOfStudents - 1].emial << "," << students[numberOfStudents - 1].fn << '\0';
}

int main()
{
	char fileName[MAX_NAME_SIZE];
	std::cout << "File name: " << std::endl;
	std::cin.getline(fileName, MAX_NAME_SIZE);

	std::ifstream file(fileName);
	if (!file.is_open())
	{
		std::cerr << "There is not a file, named \"" << fileName << "\"";
		return -1;
	}
	else std::cout << "File successfully opened!" << std::endl;

	unsigned numberOfStudents = getNumbeOfStudents(file);
	Student* students = new Student[numberOfStudents];

	extractData(file, students);

	std::cout << "Enter a command (print {FN} / edit {FN} {new EMAIL} / save):" << std::endl;
	char command[MAX_LINE_SIZE];
	std::cin.getline(command, MAX_LINE_SIZE);

	char keyWord[MAX_NAME_SIZE];
	char fnOrFile[MAX_NAME_SIZE];
	char newMail[MAX_NAME_SIZE];

	getCommandInfo(command, keyWord, fnOrFile, newMail);

	while (strcmp(keyWord, "save") != 0) //(keyWord != "save")
	{
		if (strcmp(keyWord, "print") == 0) //(keyWord == "print")
		{
			printCertainStudent(numberOfStudents, students, fnOrFile);
		}
		else if (strcmp(keyWord, "edit") == 0) //(keyWord == "edit")
		{
			editMail(numberOfStudents, students, fnOrFile, newMail);
		}

		std::cout << "Enter a command (print {FN} / edit {FN} {new EMAIL} / save):" << std::endl;
		std::cin.getline(command, MAX_LINE_SIZE);
		getCommandInfo(command, keyWord, fnOrFile, newMail);
	}

	std::ofstream newFile(fnOrFile);
	saveNewInfo(newFile, numberOfStudents, students);
	std::cout << "File" << fnOrFile << "successfully saved!" << std::endl;

	newFile.close();

	delete[] students;
	file.close();
}