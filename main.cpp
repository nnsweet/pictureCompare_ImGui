#include "picCompare_app.h"

using namespace std::literals::chrono_literals;//µ÷ÓÃsleep

int main(int, char**)
{	
	std::cout << "int main() start" << std::endl;
	picCompare_app picCom_app;
	
	picCom_app.run();

	std::cout << "int main() end" << std::endl;
	return 0;
}