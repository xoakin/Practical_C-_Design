#ifndef PRACTICALCPPDESIGN_OBSERVER_H
#define PRACTICALCPPDESIGN_OBSERVER_H
#include <string>
#include <utility>
#include <memory>
class EventData;
class Observer {
public:
    explicit Observer(std::string name) : name_(std::move(name)){}
    virtual ~Observer() = default;
    virtual void notify(std::shared_ptr<EventData> eventData) = 0;
    [[nodiscard]] std::string name() const {return name_;};

private:
    std::string name_;
};

#endif//PRACTICALCPPDESIGN_OBSERVER_H
