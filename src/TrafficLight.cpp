#include "TrafficLight.h"

#include <iostream>
#include <random>

/* Implementation of class "MessageQueue" */

template <typename T>
T MessageQueue<T>::receive() {
  std::unique_lock<std::mutex> lk(_mtx);
  _condition.wait(lk, [this] { return !_queue.empty(); });
  return std::move(_queue.back());
}

template <typename T>
void MessageQueue<T>::send(T &&msg) {
  std::lock_guard<std::mutex> guard_lock(_mtx);
  _condition.notify_one();
  _queue.push_back(msg);
}

/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight() { _currentPhase = TrafficLightPhase::red; }

void TrafficLight::waitForGreen() {
  while (_messageQueue.receive() == TrafficLightPhase::red)
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

TrafficLight::TrafficLightPhase TrafficLight::getCurrentPhase() {
  return _currentPhase;
}

void TrafficLight::simulate() {
  threads.emplace_back([this]() { cycleThroughPhases(); });
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases() {
  // FP.2a : Implement the function with an infinite loop that measures the
  // time between two loop cycles
  // and toggles the current phase of the traffic light between red and green
  // and sends an update method
  // to the message queue using move semantics. The cycle duration
  // should be a
  //    random value between 4 and
  // 6 seconds.
  // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms
  // between two cycles.
  while (true) {
    // start time measurement
    std::chrono::high_resolution_clock::time_point t1 =
        std::chrono::high_resolution_clock::now();

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    int randNum = rand() % (6000 - 4000 + 1) + 4000;
    std::this_thread::sleep_for(std::chrono::milliseconds(randNum));
    _currentPhase = (_currentPhase == TrafficLightPhase::red)
                        ? TrafficLightPhase::green
                        : TrafficLightPhase::red;
    _messageQueue.send(std::move(_currentPhase));
    // stop time measurement and print execution time
    std::chrono::high_resolution_clock::time_point t2 =
        std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    // std::cout << "Semaphore toggled after " << duration << " microseconds"
    //          << std::endl;
  }
}
