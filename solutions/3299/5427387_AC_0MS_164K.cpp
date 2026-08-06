#include<cstdio>
#include<cmath>
using namespace std;

double Temperature(double dewpoint,double humidex){
	double e=6.11*exp(5417.7530*(1.0/273.16-1.0/(dewpoint+273.16)));
	double h=0.5555*(e-10.0);
	return humidex-h;
}

double Dewpoint(double temperature,double humidex){
	double h=humidex-temperature;
	double e=h/0.5555+10.0;
	return 1.0/(1.0/273.16-log(e/6.11)/5417.7530)-273.16;
}

double Humidex(double temperature,double dewpoint){
	double e=6.11*exp(5417.7530*(1.0/273.16-1.0/(dewpoint+273.16)));
	double h=0.5555*(e-10.0);
	return temperature+h;
}

int main(){
	while(true){
		char code;
		scanf(" %c",&code);
		if(code=='E')
			break;
		double temperature,dewpoint,humidex;
		bool T=true,D=true,H=true;
		if(code=='T'){
			scanf("%lf",&temperature);
			T=false;
		}
		else if(code=='D'){
			scanf("%lf",&dewpoint);
			D=false;
		}
		else{
			scanf("%lf",&humidex);
			H=false;
		}
		scanf(" %c",&code);
		if(code=='T'){
			scanf("%lf",&temperature);
			T=false;
		}
		else if(code=='D'){
			scanf("%lf",&dewpoint);
			D=false;
		}
		else{
			scanf("%lf",&humidex);
			H=false;
		}
		if(T)
			temperature=Temperature(dewpoint,humidex);
		else if(D)
			dewpoint=Dewpoint(temperature,humidex);
		else
			humidex=Humidex(temperature,dewpoint);
		printf("T %.1lf D %.1lf H %.1lf\n",temperature,dewpoint,humidex);
	}
}