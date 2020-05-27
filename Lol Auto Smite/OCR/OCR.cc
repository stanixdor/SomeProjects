#include "OCR.hh"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>


std::string GetOCRs(const char* filename)
{
	std::string command = "tesseract --dpi 300 " + std::string(filename) + " temp nobatch digits";
	system(command.c_str());

	std::ifstream tempfile("temp.txt");
	std::stringstream buffer;
	buffer << tempfile.rdbuf();
	std::string fileContent = buffer.str();

	if (std::remove("temp.txt") != 0) // delete file
		std::cout << "error deleting temp file";

	return fileContent;
}

int GetOCRi(const char* filename)
{
	std::string command = "tesseract --dpi 300 " + std::string(filename) + " temp";
	system(command.c_str());

	std::ifstream tempfile("temp.txt");
	std::stringstream buffer;
	buffer << tempfile.rdbuf();
	tempfile.close();
	int number;
	buffer >> number;

	/*if (std::remove("temp.txt") != 0) // delete file
		std::cout << "error deleting temp file";*/

	return number;
}
