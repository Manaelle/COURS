#include <stdio.h>
#include <unistd.h>

int main()
{

	char c = 'a';
	while (c != 'q'){
		scanf("%c", &c);
	}
	return 0;
}
