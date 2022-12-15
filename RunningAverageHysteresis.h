#ifndef RUNNING_AVERAGE_HYSTERESIS
#define RUNNING_AVERAGE_HYSTERESIS

#include <Arduino.h>

class RunningAverageHysteresis {
 public:
  RunningAverageHysteresis(uint8_t readings, uint8_t diff) : numOfReadings(readings), hysteresis(diff) {
    values = (uint16_t*)calloc(readings, sizeof(uint16_t));
  }
  ~RunningAverageHysteresis() { free(values); }

  void setHysteresis(uint8_t val) {
    hysteresis = val;
  }

  void reset() {
    head = 0;
    entries = 0;
  }

  // Add the value to the running average only if it is outside the hysteresis of the last value
  void addValue(uint16_t value) {
    // Return early if the value is to similar to the current average
    if (abs(value - getFastAverage()) < hysteresis) {
      return;
    }
    // Remove old value if there is more than one entry
    if (entries) {
      sum -= values[head];
    }
    values[head] = value;
    sum += value;
    lastValue = value;
    head++;

    // Increase entries if buffer is not full yet (for calculating with less than a full buffer)
    if (entries < numOfReadings) {
      entries++;
    }
    // Loop index to emulate circular buffer
    if (head == numOfReadings) {
      head = 0;
    }
  }

  uint16_t getAverage() {
    sum = 0;
    for (int i = 0; i < entries; i++) {
      sum += values[i];
    }
    return sum / entries;
  }

  // Retreive and abritrary entry in the buffer
  uint16_t getEntry(uint8_t index) {
    if (index <= entries) {
      return values[index];
    }
  }

  // General getters
  uint16_t getLastValue() { return lastValue; }
  uint16_t getHead() { return head; }
  uint16_t getFastAverage() { return sum / entries; }

 private:
  uint32_t sum;  // Will never overflow with readings 0-256 and uint16s
  uint8_t numOfReadings;
  uint8_t entries;
  uint16_t* values;
  uint16_t lastValue{0};
  uint8_t head{0};
  uint8_t hysteresis;  // Do not add values unless they differ by at least this much
};

#endif