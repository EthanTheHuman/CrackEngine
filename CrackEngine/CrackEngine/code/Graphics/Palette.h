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
#include "../../includes/xml/pugixml.hpp"

class Palette
{
public:
	Palette();
	Palette(std::string _fileName, std::string _baseFileName);
	Palette(pugi::xml_node _data, std::string _baseFileName);
	void init(std::string _fileName, std::string _baseFileName);
	int generatePalMap();
	void saveToXml(pugi::xml_node& node) const;
	std::string paletteName;
	std::string paletteAuthor;
	std::string paletteFileName;
	int paletteIndex = 1;  // Default to 1
	std::vector<glm::vec3> baseColors;
	std::vector<glm::vec4> convertedColors;
	unsigned int paletteCount;
	unsigned int paletteTexIndex = 0;

	// Add getters
	std::string getTemplateFile() const { return baseFileName; }
	std::string getPaletteFile() const { return paletteFileName; }

private:
	std::string baseFileName;  // Store template file name
};

