#include "src/SystemSimulator.h"

int main(){
	
	SystemSimulator* pSS;
	pSS = SystemSimulator::getSystemSimulator();
	pSS->setSimType(2);
	pSS->Create(); 
	
	return 0;
}
