#include  <iostream>
using namespace std;
class body
{
private:
    double pos[3];
    double vel[3];
    double mass;
public:
    body(double inmass, double inpos[3], double invel[3]){
	mass = inmass;
	for(int i=0;i<3;i++){
	    pos[i] = inpos[i];
	    vel[i] = invel[i];
	}

    }
    void print(){
	cout << mass <<endl;
	cout << pos[0] <<" "<< pos[1] <<" "<< pos[2] <<endl;
	cout << vel[0] <<" "<< vel[1] <<" "<< vel[2] <<endl;
    }
	
};
main()
{
    double zero3[3]={0.0,0.0,0.0};
    body x =  body(0.0,zero3,zero3);
    x.print();
}


