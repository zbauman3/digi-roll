#include "./CoroutineAction.h"

void CoroutineAction::actionChange(uint8_t actionId) {
  this->actionId = actionId;
  this->reset();
}

bool CoroutineAction::actionIs(uint8_t actionId) {
  return this->actionId == actionId;
}

bool CoroutineAction::actionIsIdle() {
  return this->actionIs(ACTION_COROUTINE_IDLE);
}

void CoroutineAction::actionSetIdle(bool reset = false) {
  this->actionId = ACTION_COROUTINE_IDLE;
  if (reset) {
    this->reset();
  }
}