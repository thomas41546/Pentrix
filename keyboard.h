#ifndef KEYBOARD
#define KEYBOARD
class Keyboard{
public:
  bool Up;
  bool Down;
  bool Left;
  bool Right;
  bool Space;
  bool r,p,s;
  
  bool Quit;    
   
  Keyboard();
  void Poll();
  void Clear();
};
#endif
