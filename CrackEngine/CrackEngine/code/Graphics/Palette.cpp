#include "Palette.h"

Palette::Palette()
{
	baseColors = std::vector<glm::vec3>();
	convertedColors = std::vector<glm::vec4>();
	baseColors.push_back(glm::vec3((0 / 255.f), (255 / 255.f), (255 / 255.f)));
	baseColors.push_back(glm::vec3((255 / 255.f), (0 / 255.f), (255 / 255.f)));
	convertedColors.push_back(glm::vec4((255 / 255.f), (255 / 255.f), (255 / 255.f), 0.f));
	convertedColors.push_back(glm::vec4((255 / 255.f), (255 / 255.f), (255 / 255.f), 0.f));
	paletteCount = 2;
	generatePalMap();
}

Palette::Palette(std::string _fileName, std::string _baseFileName)
{
	baseColors = std::vector<glm::vec3>();
	convertedColors = std::vector<glm::vec4>();
	paletteCount = 0;

	std::ifstream basePaletteFile(_baseFileName);

	if (basePaletteFile.is_open())
	{
		std::string tempString;
		int count = 0;
		int phase = 0;
		glm::vec3 tempVec3;
		std::vector<int> tempSplitVector;
		std::string space_delimiter = " ";
		while (!std::getline(basePaletteFile, tempString).eof())
		{
			count++;
			if (count == 3)
			{
				paletteCount = std::stoi(tempString);
			}
			else if (count > 3)
			{
				std::string number = "";
				for (auto x : tempString)
				{
					if (x == ' ')
					{
						tempSplitVector.push_back(std::stoi(number));
						number = "";
					}
					else {
						number = number + x;
					}
				}
				tempSplitVector.push_back(std::stoi(number));
				/*
				size_t pos = 0;
				while ((pos = tempString.find(space_delimiter)) != std::string::npos) {
					tempSplitVector.push_back(std::stoi(tempString.substr(0, pos)));
					tempString.erase(0, pos + space_delimiter.length());
				}*/
				tempVec3.x = tempSplitVector[0] / 255.f;
				tempVec3.y = tempSplitVector[1] / 255.f;
				tempVec3.z = tempSplitVector[2] / 255.f;
				baseColors.push_back(tempVec3);
				tempSplitVector = std::vector<int>();
				tempVec3 = glm::vec3();
			}
		}

		std::ifstream convertedPaletteFile(_fileName);
		if (convertedPaletteFile.is_open())
		{
			tempString = "";
			count = 0;
			phase = 0;
			glm::vec4 tempVec4;

			while (!std::getline(convertedPaletteFile, tempString).eof())
			{
				count++;
				if (count == 3)
				{
					if (std::stoi(tempString) < paletteCount)
						paletteCount = std::stoi(tempString);
				}
				else if (count > 3)
				{
					//std::cout << tempString << std::endl;
					size_t pos = 0;

					std::string number = "";
					for (auto x : tempString)
					{
						if (x == ' ')
						{
							tempSplitVector.push_back(std::stoi(number));
							number = "";
						}
						else {
							number = number + x;
						}
					}
					tempSplitVector.push_back(std::stoi(number));

					/*while ((pos = tempString.find(space_delimiter)) != std::string::npos) {
						std::cout << tempString.substr(0, pos) << std::endl;
						tempSplitVector.push_back(std::stoi(tempString.substr(0, pos)));
						tempString.erase(0, pos + space_delimiter.length());
					}*/
					tempVec4.x = tempSplitVector[0] / 255.f;
					tempVec4.y = tempSplitVector[1] / 255.f;
					tempVec4.z = tempSplitVector[2] / 255.f;
					tempVec4.w = 1;
					convertedColors.push_back(tempVec4);
					tempSplitVector = std::vector<int>();
					tempVec4 = glm::vec4();
				}
			}

			if (convertedColors.size() > 0)
				convertedColors[0].w = 0;
			generatePalMap();
		}
		else
		{
			std::cout << "Could not load palette file at " + _fileName << std::endl;
		}
	}
	else
	{
		std::cout << "Could not load palette file at " + _baseFileName << std::endl;
	}
}

int Palette::generatePalMap()
{
	GLfloat colorArray[256][2][4] = { {1.f,1.f,1.f,1.f} };
	for (int i = 0; i < paletteCount; i++)
	{
		colorArray[i][0][0] = baseColors[i].x;
		colorArray[i][0][1] = baseColors[i].y;
		colorArray[i][0][2] = baseColors[i].z;
		colorArray[i][0][3] = 1;
		colorArray[i][1][0] = convertedColors[i].x;
		colorArray[i][1][1] = convertedColors[i].y;
		colorArray[i][1][2] = convertedColors[i].z;
		colorArray[i][1][3] = convertedColors[i].w;
	}

	//---------------------------
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &paletteTexIndex);
	glBindTexture(GL_TEXTURE_2D, paletteTexIndex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 256, 0, GL_RGBA, GL_FLOAT, colorArray);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return paletteTexIndex;
}
