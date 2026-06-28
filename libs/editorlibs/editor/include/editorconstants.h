#pragma once

#include <string>

using namespace std;

#define ctrl(x) ((x) & 0x1f)

namespace Editor::Constants {
    constexpr const string NormalState = "Normal";
    constexpr const string InsertState = "Insert";
}
