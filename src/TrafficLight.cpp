#include "TrafficLight.h"

#include <iostream>
#include <random>

#include "TrafficObject.h"

/* Implementation of class "MessageQueue" */

template <typename T> T MessageQueue<T>::receive() {
  /// (DONE!) : The method receive should use std::unique_lock<std::mutex> and
  // _condition.wait()
  // to wait for and receive new messages and pull them from the queue using
  // move semantics.
  // The received object should then be returned by the receive function.

  /// perform queue modification under the lock
  std::unique_lock<std::mutex> uLock(_mutex);

  /// pass unique lock to condition variable
  _condition.wait(uLock, [this] { return !_queue.empty(); });

  /// remove last vector element from queue
  T q = std::move(_queue.back());
  _queue.pop_back();

  return q;
}

template <typename T> void MessageQueue<T>::send(T &&q) {
  /// (DONE!) : The method send should use the mechanisms
  // std::lock_guard<std::mutex>
  // as well as _condition.notify_one() to add a new message to the queue and
  // afterwards send a notification.

  /// simulate some work
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  /// perform vector modification under the lock
  std::lock_guard<std::mutex> Lock_g(_mutex);

  /// add vector to queue
  _queue.push_back(std::move(q));
  _condition.notify_one();
}

/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight() : _currentPhase{TrafficLightPhase::red} {}

void TrafficLight::waitForGreen() {
  /// (DONE!) : add the implementation of the method waitForGreen, in which an
  // infinite while-loop
  // runs and repeatedly calls the receive function on the message queue.
  // Once it receives TrafficLightPhase::green, the method returns.

  /// 0 means read and 1 means green. Therefore, when it receives
  /// TrafficLightPhase::green, in order to get out of the loop, the condition
  /// in while loop should become false. In this case we can write
  /// _phaseQueue.receive() != TrafficLightPhase::green or simply use
  /// !_phaseQueue.receive().
  while (!_phaseQueue.receive())
    ;
}

TrafficLightPhase TrafficLight::getCurrentPhase() { return _currentPhase; }

void TrafficLight::setCurrentPhase(const TrafficLightPhase &color) {
  _currentPhase = color;
}

void TrafficLight::toggleColor(const TrafficLightPhase &color) {
  setCurrentPhase(TrafficLightPhase(!color));
}

void TrafficLight::simulate() {
  /// (DONE!) : Finally, the private method „cycleThroughPhases“ should be
  // started in a thread when the public method „simulate“ is called. To do
  // this, use the thread queue in the base class.
  threads.push_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases() {
  /// (DONE!) : Implement the function with an infinite loop that measures the
  // time between two loop cycles and toggles the current phase of the
  // traffic light between red and green and sends an update method to the
  // message queue using move semantics. The cycle duration should be a
  // random value between 4 and 6 seconds. Also, the while-loop should use
  // std::this_thread::sleep_for to wait 1ms between two cycles.

  std::chrono::time_point<std::chrono::system_clock> lastUpdate{
      std::chrono::system_clock::now()};
  std::random_device rd;
  std::uniform_real_distribution<float> dist(4000, 6000);
  TrafficLightPhase color;
  float cycleDuration{dist(rd)};
  while (true) {
    /// sleep at every iteration to reduce CPU usage
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    /// compute time difference to stop watch
    long timeSinceLastUpdate =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now() - lastUpdate)
            .count();

    /// Toggle colors!
    color = this->getCurrentPhase();
    if (timeSinceLastUpdate >= cycleDuration) {
      toggleColor(color);

      /// Moving the update to message queue is not done yet!
      _phaseQueue.send(std::move(color));

      /// reset stop watch for next cycle
      lastUpdate = std::chrono::system_clock::now();

      /// Generate cycleDuration between 4 and 6.
      cycleDuration = dist(rd);
    }
  }
}
