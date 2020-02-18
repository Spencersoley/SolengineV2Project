#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

//No constructor/destructor, so this is a static class. There's only ever going to be one instance of the methods.
//This is because we'll only need one IO manager. 

namespace SolengineV2
{
	class IOManager
	{
	public:
		//Turns file into buffer for image loading
		std::vector<unsigned char> ReadFileToVectorBuffer(std::string filePath)
		{
			std::vector<unsigned char> buffer;

			std::ifstream file(filePath, std::ios::binary);
			if (file.fail())
			{
				perror(filePath.c_str());
				throw std::invalid_argument("Failed to load file buffer");
			}

			//seek to the end
			file.seekg(0, std::ios::end);

			//get the file size
			unsigned int fileSize = (unsigned int)file.tellg();

			//seek to beginning 
			file.seekg(0, std::ios::beg);

			//reduce file size by any present header bytes
			fileSize -= (unsigned int)file.tellg();

			buffer.resize(fileSize);
			file.read((char*)&(buffer[0]), fileSize);
			file.close();

			return buffer;
		}

		//Turns file into string
		std::string ReadFileToStringBuffer(std::string filePath)
		{
			std::string buffer;

			std::ifstream file(filePath, std::ios::binary);
			if (file.fail())
			{
				perror(filePath.c_str());
				std::cout << "Failed to load file to string buffer!";
				return buffer;
			}

			//seek to the end
			file.seekg(0, std::ios::end);

			//get the file size
			unsigned int fileSize = (unsigned int)file.tellg();

			//seek to beginning 
			file.seekg(0, std::ios::beg);

			//reduce file size by any present header bytes
			fileSize -= (unsigned int)file.tellg();

			buffer.resize(fileSize);
			file.read((char*)&(buffer[0]), fileSize);
			file.close();

			return buffer;
		}

		//Turns a file into a vector of strings
		std::vector<std::string> ReadFileToStringVector(const std::string& fileName)
		{
			std::ifstream file;
			file.open(fileName);

			if (file.fail())
			{
				std::cout << fileName << " would not open";
			}

			std::vector<std::string> data;
			std::string temp;

			int test;
			file >> temp >> test;
			std::getline(file, temp);

			//Will read each line one by one and put it in temp
			while (std::getline(file, temp))
			{
				if (temp.size() > 0) data.push_back(temp);
			}

			return data;
		}
	};
}
