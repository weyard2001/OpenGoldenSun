#pragma once

#include <string>
#include <fstream>

namespace gse {
	class InputFile {
	public:
		InputFile() = default;
		~InputFile() = default;

		bool openFile(const std::string& path = "");

	private:
		std::string m_inFileName;
	};
}