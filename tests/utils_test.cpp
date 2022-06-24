#include "observer.h"
#include "publisher.h"
#include <catch2/catch_all.hpp>
#include <iostream>
#include <utility>


SCENARIO("First test publisher", "[publisher]")
{
  class TestEventData : public EventData
  {
    std::string text;

  public:
    explicit TestEventData(std::string t) : text(std::move(t)) {}
    void print() const { std::cout << text << '\n'; }
  };
  GIVEN("A publisher")
  {
    class TestPublisher : private Publisher
    {
    public:
      using Publisher::attach;
      using Publisher::detach;
      using Publisher::raise;
    };
    TestPublisher testPublisher;
    AND_GIVEN("An Observer")
    {
      class TestObserver : public Observer
      {
      public:
        TestObserver(std::string n, std::string testName) : Observer(std::move(n)), testWaterMark(std::move(testName)) {}
        void notify(std::shared_ptr<EventData> eventData) override
        {
          const auto p = std::dynamic_pointer_cast<TestEventData>(eventData);
          p->print();
        }

      private:
        std::string testWaterMark;
      };
      std::unique_ptr<Observer> testObserver = std::make_unique<TestObserver>("ojete", "moreno");

      WHEN("attaching the observer to the event 'test'")
      {
        testPublisher.attach("test", std::move(testObserver));
        THEN("Detach the observer")
        {
          auto p = testPublisher.detach("test", "ojete");
          REQUIRE(p != nullptr);
        }
      }

      AND_GIVEN("A bunch of different observers")
      {
        std::unique_ptr<Observer> testObserver2 = std::make_unique<TestObserver>("vivo", "con");
        std::unique_ptr<Observer> testObserver3 = std::make_unique<TestObserver>("tu", "madre");
        std::unique_ptr<Observer> testObserver4 = std::make_unique<TestObserver>("en", "un");
        std::unique_ptr<Observer> testObserver5 = std::make_unique<TestObserver>("castillo", "pijama");

        WHEN("Attaching them to the same event 'test'")
        {
          testPublisher.attach("test", std::move(testObserver));
          testPublisher.attach("test", std::move(testObserver2));
          testPublisher.attach("test", std::move(testObserver3));
          testPublisher.attach("test", std::move(testObserver4));
          testPublisher.attach("test", std::move(testObserver5));

          THEN("Raising event test must call every observer")
          {
            testPublisher.raise("test", std::make_shared<TestEventData>("very long string for event data"));
          }
          THEN("Detaching one of them should work")
          {
            auto p = testPublisher.detach("test", "en");
            REQUIRE(p != nullptr);
            AND_THEN("Raising test event won't show 'un'")
            {
              testPublisher.raise("test", std::make_shared<TestEventData>("some data"));
            }
          }
        }
      }
      WHEN("The same observer subscribes to many events")
      {
        std::unique_ptr<Observer> testObserver2 = std::make_unique<TestObserver>("obs", "con");
        std::unique_ptr<Observer> testObserver3 = std::make_unique<TestObserver>("obs", "madre");
        std::unique_ptr<Observer> testObserver4 = std::make_unique<TestObserver>("obs", "un");
        std::unique_ptr<Observer> testObserver5 = std::make_unique<TestObserver>("obs", "pijama");

        testPublisher.attach("event1", std::move(testObserver2));
        testPublisher.attach("event2", std::move(testObserver3));
        testPublisher.attach("event3", std::move(testObserver4));
        testPublisher.attach("event4", std::move(testObserver5));
        THEN("Detach by name should not fail")
        {
          auto p = testPublisher.detach("event3", "obs");
          REQUIRE(p != nullptr);
        }
      }
    }
  }
}

class TestEventData : public EventData
{
  std::string text;

public:
  explicit TestEventData(std::string t) : text(std::move(t)) {}
  void print() const { std::cout << text << '\n'; }
};
class TestPublisher : private Publisher
{
public:
  using Publisher::attach;
  using Publisher::detach;
  using Publisher::raise;
};
class TestObserver : public Observer
{
public:
  TestObserver(std::string n, std::string testName) : Observer(std::move(n)), testWaterMark(std::move(testName)) {}
  void notify(std::shared_ptr<EventData> eventData) override
  {
    const auto p = std::dynamic_pointer_cast<TestEventData>(eventData);
    p->print();
  }

private:
  std::string testWaterMark;
};
SCENARIO("Observer already registered")
{
  GIVEN("An observer registered in a publisher for the event 'xyz'")
  {
    std::unique_ptr<Observer> testObserver = std::make_unique<TestObserver>("ojete", "moreno");
    TestPublisher testPublisher;
    testPublisher.attach("xyz", std::move(testObserver));
    WHEN("Registering the same observer in the same publisher for the event 'xyz'")
    {
      std::unique_ptr<Observer> testObserver2 = std::make_unique<TestObserver>("ojete", "moreno");

      THEN("Should throw")
      {
        REQUIRE_THROWS(testPublisher.attach("xyz", std::move(testObserver2)));
      }
    }
  }
}
SCENARIO("Detaching from non existing event")
{
  GIVEN("An observer registered in a publisher for some event 'xyz'")
  {
    std::unique_ptr<Observer> testObserver = std::make_unique<TestObserver>("ojete", "moreno");
    TestPublisher testPublisher;
    testPublisher.attach("xyz", std::move(testObserver));
    WHEN("Detaching the same observer in the same publisher for a different event 'abc'")
    {
      THEN("Should throw")
      {
        REQUIRE_THROWS(testPublisher.detach("abc", "ojete"));
      }
    }
  }
}

SCENARIO("Detaching unregistered observer")
{
  GIVEN("An observer registered in a publisher for some event 'xyz'")
  {
    std::unique_ptr<Observer> testObserver = std::make_unique<TestObserver>("ojete", "moreno");
    TestPublisher testPublisher;
    testPublisher.attach("xyz", std::move(testObserver));
    WHEN("Detaching a different observer in the same publisher for the same event")
    {
      THEN("Should throw")
      {
        REQUIRE_THROWS(testPublisher.detach("xyz", "ojal"));
      }
    }
  }
}