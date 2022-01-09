#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <condition_variable>
#include <deque>
#include <mutex>
#include <utility>

#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;

template <class T>
class MessageQueue {
 public:
  T receive();
  void send(T &&msg);

 private:
  std::deque<T> _queue;
  std::condition_variable _condition;
  std::mutex _mtx;
};

class TrafficLight : public TrafficObject {
 public:
  // constructor / desctructor
  TrafficLight();
  //~TrafficLight();
  // getters / setters
  void waitForGreen();
  void simulate() override;
  enum class TrafficLightPhase { red, green };
  TrafficLightPhase getCurrentPhase();
  void cycleThroughPhases();
  // typical behaviour methods

 private:
  // typical behaviour methods
  TrafficLightPhase _currentPhase;
  MessageQueue<TrafficLightPhase> _messageQueue;

  std::condition_variable _condition;
  std::mutex _mutex;
};

#endif
