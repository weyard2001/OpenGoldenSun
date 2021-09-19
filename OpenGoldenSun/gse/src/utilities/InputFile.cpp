#include "utilities/InputFile.hpp"

using namespace gse;

bool InputFile::openFile(const std::string& path) {
	std::ifstream inFile(path);

	if (inFile.is_open()) {
		if (inFile.fail()) {
			return false;
		}

		std::string lineText;
		std::getline(inFile, lineText);
		if (lineText[0] == '{' || lineText[lineText.size() - 1] == '{') {

		}
	}

	return true;
}
