#include <Windows.h>
#include <stdio.h>


int main(void) {

	printf("\n\n\t<<+>> $van-example.exe <<+>>\n");
	printf("  --> ProcessID\t| %d\n", GetCurrentProcessId());
	
	printf("  [#] Press <Enter> After \"ProcVanish.dll\" Is Injected ...\n");
	getchar();

	return 0;
}