#include<iostream.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<iomanip.h>

#define LEN 13340					
#define UNIT 10000

class HugeInt
{
	friend ostream &operator<<( ostream &, HugeInt & );

public:
	HugeInt( long = 0 );			
	HugeInt operator+( int );		
	HugeInt operator-( int );
	HugeInt operator*( HugeInt & );

private:
	long integer[LEN];
	int length;
	static int steps;			
};

int HugeInt::steps = (int)log10((double)UNIT);

HugeInt::HugeInt( long val ):length(0)
{
	int i;

	length = int(ceil(log10(val)));

	memset(integer, 0, LEN * sizeof(long));

	for( i = LEN - 1; val != 0 && i >= 0; i -- )
	{
		integer[i] = val % UNIT;
		val /= UNIT;
	}
}


HugeInt HugeInt::operator+( int )							
{
	HugeInt result = *this;
	result.integer[LEN - 1] += 1;
	for(int i = LEN - 1; result.integer[i] >= UNIT; i --)
	{
		result.integer[i] %= UNIT;
		result.integer[i - 1] += 1;
	}
	return result;
}


HugeInt HugeInt::operator-( int )
{
	HugeInt result = *this;
	result.integer[LEN - 1] -= 1; 
	for(int i = LEN - 1; result.integer[i] < 0; i --)
	{
		result.integer[i] += UNIT;
		result.integer[i - 1] -= 1;
	}
	return result;
}



HugeInt HugeInt::operator*( HugeInt &op2 )
{
	HugeInt result;
	long carry = 0;

	for( int i = LEN - 1; i >= (LEN - op2.length); i -- )
	{
		if( op2.integer[i] == 0 )
			continue;
		for( int j = LEN - 1; j >= (LEN - length); j --)
		{
			if( integer[j] == 0 )
				continue;
			result.integer[i + j + 1 - LEN] += integer[j] * op2.integer[i];

			if( result.integer[i + j + 1 - LEN] >= UNIT )
			{
				carry = result.integer[i + j + 1 - LEN] / UNIT;
				result.integer[i + j + 1 - LEN] %= UNIT;
				result.integer[i + j - LEN] += carry;
			}
			else carry = 0;
		}
	}
	for( int k = LEN - 1; k >= 0; k -- )
	{
		if( result.integer[k] >= UNIT )
		{
			carry = result.integer[k] / UNIT;
			result.integer[k] %= UNIT;
			result.integer[k - 1] += carry;
		}
	}
	for( i = 0; i < LEN && result.integer[i] == 0; i ++ );
	result.length = int(ceil(log10(result.integer[i]))) + (LEN - 1 - i) * steps;

	return result;
}

ostream &operator<<( ostream &output, HugeInt &num )			
{
	int i;
	
	for( i = 0; (num.integer[i] == 0) && (i < LEN); i ++ );
	if( i == LEN )
		output << 0 << endl;
	else
	{
		cout << num.integer[i++];
		for( ; i < LEN; i ++ )
			output << setw(HugeInt::steps) << setfill('0') << num.integer[i];
		output << endl;
	}

	return output;
}


void main()
{
	int n;
	scanf("%d", &n);
	if( n >= 2 )
	{
		cout << 2 << endl << 3 << endl;
	}
	HugeInt result = 3;
	for( int i = 3; i <= n; i ++ )
	{
		result = result * (result - 1) + 1;
		cout << result;
	}
}