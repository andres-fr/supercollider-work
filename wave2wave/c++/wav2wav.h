#ifndef FLOATSIGNAL_H
#define FLOATSIGNAL_H

class FloatSignal {
 public:
  explicit FloatSignal(int size, int delay);
  explicit FloatSignal(int size) : FloatSignal(size, 0){};
  FloatSignal(const FloatSignal& copyFrom)=delete;
  FloatSignal& operator= (const FloatSignal& copyFrom)=delete;
  ~FloatSignal();
  float at(int idx); // returns contents[idx-delay], never throws error
  void set(int idx, float val); // not affected by delay, throws out-of-bounds
  void normalize(float norm, float mean);
  void print(); // not affected by the delay
 private:
  int sz;
  float* contents;
  int delay;
};

#endif
