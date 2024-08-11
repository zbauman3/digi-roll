#include "./CoroutineAction.h"

void CoroutineAction::actionChange(uint8_t actionId, bool forceReset) {
  uint8_t lastActionId = this->actionId;
  this->actionId = actionId;
  if (forceReset || this->actionId != lastActionId) {
    this->reset();
  }
}

bool CoroutineAction::actionIs(uint8_t actionId) {
  return this->actionId == actionId;
}

bool CoroutineAction::actionIsIdle() {
  return this->actionIs(ACTION_COROUTINE_IDLE);
}

void CoroutineAction::actionSetIdle(bool reset) {
  this->actionId = ACTION_COROUTINE_IDLE;
  if (reset) {
    this->reset();
  }
}

void CoroutineAction::loop() { this->runCoroutine(); }