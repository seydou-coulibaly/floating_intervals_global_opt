#include <iostream>
#include "gaol/gaol.h"

class Function{
  public:
    Function() {}
    virtual float eval(float x) =0;
    virtual interval intervalExtension(interval x) =0;
    virtual interval derivative(interval x) =0;
    virtual ~Function() {}
};
class MyFunction : public Function{
  // (ax + b/x)
private:
  int a ;
  int b ;
public:
  MyFunction(){a = 1 ; b = 1;std::cout << "### a and b are initialized to 1 ### " << '\n';}
  MyFunction(int a1, int b1) {
    if (a1 != 0 && b1 != 0){
      a = a1 ; b = b1;
      std::cout << " ### a set to " << a << " and b set to " << b << "###\n";
    }else{
      std::cout << "Error : a and b must be different to 0" << '\n';
    }
  }
  float eval(float x){
    // Precondition : ensure x != 0
    return a*x + b/x;
  }
  void set(int a1,int b1){
    a = a1 ;
    b = b1 ;
  }
  interval intervalExtension(interval x){
    interval y ;
    if (x.left() == x.right() && x.right() == 0) {
      //y = interval::emptyset;
      y = interval(4,6) & interval(7,9);
    }else if(x.left() < 0 && x.right() >0) {
      y = interval(-GAOL_INFINITY,+GAOL_INFINITY);
    }else{
      if (a > 0 && b > 0) {
        if (x.left() >= sqrt(b/a)) {
          y = interval(eval(x.left()),eval(x.right()));
        }else if(0 < x.left() && x.left() <= sqrt(b/a) && x.right() >= sqrt(b/a) ){
          y = interval(eval(sqrt(b/a)),maximum(eval(x.left()),eval(x.right())));
        }else if(0 < x.left() && x.left() <= sqrt(b/a) && 0 < x.right() && x.right() <= sqrt(b/a) ){
          y = interval(eval(x.right()), eval(x.left()));
        }else if (x.right() <= -sqrt(b/a)){
          y = interval(eval(x.left()),eval(x.right()));
        }else if(-sqrt(b/a) <= x.right() && x.right() < 0 && x.left() <= -sqrt(b/a)){
          y = interval(minimum(eval(x.left()),eval(x.right())), eval(-sqrt(b/a)));
        }else if (-sqrt(b/a) <= x.right() && x.right() < 0 && -sqrt(b/a) <= x.left() && x.left() < 0){
          y = interval(eval(x.right()), eval(x.left()));
        }else if(x.left() == 0 && x.right() >= sqrt(b/a)){
          y = interval(eval(sqrt(b/a)),+GAOL_INFINITY);
        }else if(x.left() == 0 && x.right() < sqrt(b/a)){
          y = interval(eval(x.right()),+GAOL_INFINITY);
        }else if(x.right() == 0 && x.left() <= -sqrt(b/a) ){
          y = interval(-GAOL_INFINITY, eval(-sqrt(b/a)));
        }else if (x.right() == 0 && x.left() > -sqrt(b/a) ){
          y = interval(-GAOL_INFINITY, eval(x.left()));
        }
      }else if(a > 0 && b < 0){
        if (x.right() < 0) {
          y = interval(eval(x.left()),eval(x.right()));
        }else if(x.left() > 0){
          y = interval(eval(x.left()),eval(x.right()));
        }else if (x.left() == 0) {
          y = interval(-GAOL_INFINITY, eval(x.right()));
        }else if(x.right() == 0){
          y = interval(eval(x.left()), +GAOL_INFINITY);
        }
      }else if(a < 0 && b > 0){
        set(-a,-b);
        y = -intervalExtension(x);
        set(-a,-b);
      }else{
        // a < 0 && b < 0
        set(-a,-b);
        y = -intervalExtension(x);
        set(-a,-b);
      }
    }
    return y;
  }
  interval derivative(interval x){
    return a - b/sqr(x);
  }
};

int main(void)
{
  gaol::init();
  // Declaration of parameters a and b
  int a = 1 ;
  int b = 1 ;
  // an instance of MyFunction
  MyFunction f = MyFunction(a,b);
  // Declaration of an interval X
  interval x (interval(1,5));
  // y is the optimal interval extension of X
  interval y = f.intervalExtension(x);
  // Print the results
  std::cout << "\nLet X = [" << x.left() << ","<< x.right() << "]"<< std::endl;
  std::cout << "Optimal interval extension : ";
  std::cout << "G(X) = [" << y.left() << ","<< y.right() << "]"<< std::endl;
  // Simulators realisalisation
  char j = 'o'; // stop critérion
  float inf,sup; // lower and Upper bound
  std::cout << "\n" << '\n'<< std::endl;
  do {
    std::cout << "****************************************************************" << '\n';
    std::cout << "Time to give an interval value X :" << '\n';
    std::cout << "1. Lower bound\t";
    std::cin >> inf;
    std::cout << "2. Upper bound\t";
    std::cin >> sup;
    // assume b >= a
    x = interval(inf,sup);
    y = f.intervalExtension(x);
    std::cout << "\nX = [" << x.left() << ","<< x.right() << "]"<< std::endl;
    std::cout << "G(X) = [" << y.left() << ","<< y.right() << "]"<< std::endl;
    std::cout << "****************************************************************" << '\n';
    std::cout << "Do you want to continue simulations ? (n to say no) :\t";
    std::cin >> j;
  } while(j!= 'n');

  gaol::cleanup();
  std::cout << "End of program "<<  std::endl;
}

double dist(interval X,interval Y){
  double inf_XY = abs(X.left()-Y.left());
  double sup_XY = abs(X.right()-Y.right());
  return inf_XY > sup_XY ? inf_XY:sup_XY;
}
float maximum(float a, float b){a > b ? a:b;}
float minimum(float a, float b){a < b ? a:b;}
