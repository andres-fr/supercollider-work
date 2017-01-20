#ifndef FLOATSIGNAL_H
#define FLOATSIGNAL_H

class FloatSignal {
 private:
  int sz;
  float* contents;
  int delay;
 public:
  // constructors and destructor
  explicit FloatSignal(int size, int delay);
  explicit FloatSignal(int size) : FloatSignal(size, 0){};
  FloatSignal(const FloatSignal& copyFrom)=delete;
  FloatSignal& operator= (const FloatSignal& copyFrom)=delete;
  ~FloatSignal();
  // setters and getters
  int getSize();
  int getDelay();
  void setDelay(int d);
  float at(int idx); // returns contents[idx-delay], never throws error
  void set(int idx, float val); // not affected by delay, throws out-of-bounds
  // further functionality
  void normalize(float norm, float mean);
  void print(); // not affected by the delay
};

#endif
