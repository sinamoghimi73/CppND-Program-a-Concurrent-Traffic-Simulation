#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <condition_variable>
#include <deque>
#include <mutex>

#include "TrafficObject.h"

enum TrafficLightPhase {
  red,
  green,
};

// forward declarations to avoid include cycle
class Vehicle;

/// (DONE!) Define a class „MessageQueue“ which has the public methods send and
// receive. Send should take an rvalue reference of type TrafficLightPhase
// whereas receive should return this type. Also, the class should define an
// std::dequeue called _queue, which stores objects of type TrafficLightPhase.
// Also, there should be an std::condition_variable as well as an std::mutex as
// private members.

template <class T>
class MessageQueue {
 public:
  T receive();
  void send(T &&q);

 private:
  std::mutex _mutex;
  std::condition_variable _condition;
  std::deque<T> _queue;
};

/// (DONE!) : Define a class „TrafficLight“ which is a child class of
// TrafficObject.
// The class shall have the public methods „void waitForGreen()“ and „void
// simulate()“ as well as „TrafficLightPhase getCurrentPhase()“, where
// TrafficLightPhase is an enum that can be either „red“ or „green“. Also, add
// the private method „void cycleThroughPhases()“. Furthermore, there shall be
// the private member _currentPhase which can take „red“ or „green“ as its
// value.

class TrafficLight : public TrafficObject {
 public:
  // constructor / desctructor
  TrafficLight();

  // getters / setters
  /// waitForGreen method added!
  void waitForGreen();

  /// simulate method added!
  void simulate();

  /// getCurrentPhase method added!
  TrafficLightPhase getCurrentPhase();
  // typical behaviour methods
  void setCurrentPhase(const TrafficLightPhase &color);
  void toggleColor(const TrafficLightPhase &color);

 private:
  // typical behaviour methods
  /// cycleThroughPhases method added!
  void cycleThroughPhases();

  /// _currentPhase member added!
  TrafficLightPhase _currentPhase;

  /// (DONE!) : create a private member of type MessageQueue for messages of
  // type TrafficLightPhase and use it within the infinite loop to push each
  // new TrafficLightPhase into it by calling send in conjunction with move
  // semantics.

  /// _phaseQueue member added!
  MessageQueue<TrafficLightPhase> _phaseQueue;

  /// _condition member added!
  std::condition_variable _condition;

  std::mutex _mutex;
};

#endif