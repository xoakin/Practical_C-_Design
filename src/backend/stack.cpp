#include "stack.h"
#include <deque>

class Stack::StackImpl
{
  using Data = std::deque<double>;
  Data data;

public:
  void push(double x);
  double pop();
  std::vector<double> getElements(int n);
  void swapTop();
  std::size_t size() const;
};
void Stack::StackImpl::push(double x)
{
  data.push_back(x);
}

double Stack::StackImpl::pop()
{
  const auto tmp = data.back();
  data.pop_back();
  return tmp;
}
std::vector<double> Stack::StackImpl::getElements(int n)
{
  std::vector<double> elements;
  elements.reserve(static_cast<unsigned long>(n));
  std::copy_n(data.crbegin(), n, std::back_inserter(elements));
  return elements;
}
void Stack::StackImpl::swapTop()
{
  if (data.size() < 2) {
    throw std::out_of_range("Need 2 elements at least");
  }
  const auto first = data.back();
  data.pop_back();
  const auto second = data.back();
  data.pop_back();
  data.push_back(first);
  data.push_back(second);
}

std::size_t Stack::StackImpl::size() const
{
  return data.size();
}

Stack &Stack::Instance()
{
  static Stack instance_;
  return instance_;
}
Stack::Stack() : pimpl(std::make_unique<StackImpl>())
{
}
void Stack::push(double x)
{
  pimpl->push(x);
}
double Stack::pop()
{
  return pimpl->pop();
}
std::vector<double> Stack::getElements(int n)
{
  return pimpl->getElements(n);
}
void Stack::swapTop()
{
  pimpl->swapTop();
}
std::size_t Stack::size() const
{
  return pimpl->size();
}
Stack::~Stack() = default;
