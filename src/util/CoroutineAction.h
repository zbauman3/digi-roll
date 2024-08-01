#ifndef Z_CoroutineAction
#define Z_CoroutineAction

#include <AceRoutine.h>
#include <Arduino.h>

using namespace ace_routine;

#define ACTION_COROUTINE_IDLE 0

class CoroutineAction : public Coroutine {
protected:
  uint8_t actionId = ACTION_COROUTINE_IDLE;

public:
  void actionChange(uint8_t actionId);
  bool actionIs(uint8_t actionId);
  bool actionIsIdle();
  void actionSetIdle(bool reset = false);
};

#endif // Z_CoroutineAction