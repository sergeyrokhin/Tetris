#pragma once

#include "control.h"
#include <memory>

class InputController {

private:
    std::shared_ptr<ControlCenter> control_;
};