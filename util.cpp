#include <iostream>

#include "util.h"

void error(std::string errMessage) {
	std::cout << "Error occurred: " << errMessage << "!\n";
}

void stub(std::string stubMessage) {
	std::cout << "STUB: " << stubMessage << ".\n";
}
