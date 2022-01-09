#include "TrafficLight.h"

#include <iostream>
#include <random>

template <typename R>
R _random(R range_from, R range_to) {
  std::random_device rand_dev;
  std::mt19937 generator(rand_dev());
  std::uniform_int_distribution<R> distr(range_from, range_to);
  return distr(generator);
}

/* Implementation of class "MessageQueue" */

template <typename T>
T MessageQueue<T>::receive() {
  std::unique_lock<std::mutex> lk(_mtx);
  _condition.wait(lk, [this] { return !_queue.empty(); });
  T received(std::move(_queue.back()));
  _queue.clear();
  return received;
}

template <typename T>
void MessageQueue<T>::send(T &&msg) {
  std::lock_guard<std::mutex> guard_lock(_mtx);
  _queue.emplace_back(msg);
  _condition.notify_one();
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
  int randNum = _random<int>(4000, 6000);
  while (true) {
    // start time measurement
    std::chrono::high_resolution_clock::time_point t1 =
        std::chrono::high_resolution_clock::now();

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    // int randNum = rand() % (6000 - 4000 + 1) + 4000;
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
    std::cout << "Semaphore toggled after " << (duration / 1000)
              << " milliseconds" << std::endl;
  }
}
