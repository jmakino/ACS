/* File : test.h */

class Shape {
 public:
    double  x, y;   
    Shape(double xx=0.0, double yy=0.0){
	x = xx;
	y = yy;
    }
    void    move(double dx, double dy){
	x += dx;
	y += dy;
    };
};
