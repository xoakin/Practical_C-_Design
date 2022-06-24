#ifndef PDCALC_STACK_H
#define PDCALC_STACK_H

#include <vector>
#include "publisher.h"

class Stack : private Publisher
{
  class StackImpl;

public:
  Stack(const Stack &) = delete;
  Stack(Stack &&) = delete;
  Stack &operator=(const Stack &) = delete;
  Stack &operator=(const Stack &&) = delete;

  static Stack &Instance();
  void push(double x);
  double pop();
  std::vector<double> getElements(int n);
  void swapTop();
  std::size_t size() const;

private:
  Stack();
  ~Stack();

  std::unique_ptr<StackImpl> pimpl;
};

#endif//PDCALC_STACK_H
