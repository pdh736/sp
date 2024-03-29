#include <stdio.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>

//string to 진법
//ref
//https://shaeod.tistory.com/247
int main( )
{
	char *strStrLong = "-1234567890";
	char *strStrDouble = "3.14e+100";
	char *strStrDoubleL = "1234567890123456789012345678901234567890";
	char *strString = "The answer is 50 points";
	char *stop;

	long nLong;
	double nDouble;
	unsigned long nULong;

	nLong = strtol( strStrLong, &stop, 10 );
	printf( "%d\n", nLong );

	nLong = strtol( strString, &stop, 10 );
	printf( "%d stop at '%s'\n", nLong, stop );

	nLong = strtol( &strString[14], &stop, 10 );
	printf( "%d stop at '%s'\n", nLong, stop );

	nULong = strtoul( strStrLong, &stop, 10 );
	printf( "%u\n", nULong );

	nDouble = strtod( strStrDouble, &stop );
	printf( "%.2e\n", nDouble );

	nDouble = strtod( strStrLong, &stop );
	printf( "%.2e\n", nDouble );

	nDouble = strtod( strStrDoubleL, &stop );
	printf( "%.2e\n", nDouble );

	//2진수, 8진수, 16진수를 10진수로 변환
	char *strStr2  = "1010";		//2진수
	char *strStr8  = "0123";		//8진수
	char *strStr16 = "0xFF";		//16진수
	int radix;

	radix = 2;
	nLong = strtol( strStr2, &stop, radix );
	printf( "%d\n", nLong );

	radix = 8;
	nLong = strtol( strStr8, &stop, radix );
	printf( "%d\n", nLong );

	radix = 16;
	nLong = strtol( strStr16, &stop, radix );
	printf( "%d\n", nLong );

	return 0;
}
