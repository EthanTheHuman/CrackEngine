#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <istream>
#include <fstream>
#include <ostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Palette
{
public:
	Palette();
	Palette(std::string _fileName, std::string _baseFileName);
	std::string paletteFileName;
	std::vector<glm::vec3> baseColors;
	std::vector<glm::vec4> convertedColors;
	unsigned int paletteCount;
	unsigned int paletteTexIndex = 0;
	int generatePalMap();
};

