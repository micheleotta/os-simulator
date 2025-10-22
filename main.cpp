#include "src/SystemSimulator.h"

int main(){
	
	SystemSimulator* pSS;
	pSS = SystemSimulator::getSystemSimulator();
	pSS->Create();
	
	return 0;
}
