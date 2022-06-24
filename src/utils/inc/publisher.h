#ifndef PRACTICALCPPDESIGN_PUBLISHER_H
#define PRACTICALCPPDESIGN_PUBLISHER_H
#include <memory>
#include <string>
#include <unordered_map>
class Observer;

class EventData {
public:
    virtual ~EventData() = default;
};

class Publisher {
    class PublisherImpl;
    std::unique_ptr<PublisherImpl> pimpl;


public:
    Publisher();
    void attach(const std::string &event, std::unique_ptr<Observer>);
    std::unique_ptr<Observer> detach(const std::string &event, const std::string &name);

protected:
    ~Publisher();
    void raise(const std::string &event, std::shared_ptr<EventData> eventData);
};


#endif//PRACTICALCPPDESIGN_PUBLISHER_H
