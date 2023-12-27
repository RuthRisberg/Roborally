#ifndef Motors_h
#define Motors_h

class Motors{
  
  public:
    Motors() {}
    void init() {}
    void disable_outputs();
    bool home();
    void moveTo(float x, float y, float up, float angle);
    
    
  private:
    float motor1pos;
    float motor2pos;
    float motor3pos;
    float motor4pos;
    bool outputsOn;
};

#endif
