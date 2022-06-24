#include "publisher.h"
#include "observer.h"
#include <stdexcept>

class Publisher::PublisherImpl
{
  using ObserverList = std::unordered_map<std::string, std::unique_ptr<Observer>>;
  using Event = std::unordered_map<std::string, ObserverList>;
  Event events;

public:
  void attach(const std::string &event, std::unique_ptr<Observer> observer);
  std::unique_ptr<Observer> detach(const std::string &event, const std::string &name);
  void raise(const std::string &event, std::shared_ptr<EventData> eventData);
};

void Publisher::PublisherImpl::attach(const std::string &event, std::unique_ptr<Observer> observer)
{
  if (const auto [_, inserted] = events[event].insert(std::make_pair(observer->name(), std::move(observer))); !inserted) {
    throw std::out_of_range("Observer not registered");
  }
}
std::unique_ptr<Observer> Publisher::PublisherImpl::detach(const std::string &event, const std::string &name)
{
  auto tmp = std::move(events.at(event).at(name));
  events[event].erase(name);
  return tmp;
}
void Publisher::PublisherImpl::raise(const std::string &event, std::shared_ptr<EventData> eventData)
{
  auto &subscribers = events[event];
  for (auto &[name, observer] : subscribers) {
    observer->notify(eventData);
  }
}
Publisher::Publisher()
{
  pimpl = std::make_unique<PublisherImpl>();
}
void Publisher::attach(const std::string &event, std::unique_ptr<Observer> observer)
{
  pimpl->attach(event, std::move(observer));
}

std::unique_ptr<Observer> Publisher::detach(const std::string &event, const std::string &name)
{
  return pimpl->detach(event, name);
}
Publisher::~Publisher() = default;
void Publisher::raise(const std::string &event, std::shared_ptr<EventData> eventData)
{
  pimpl->raise(event, eventData);
}
