#pragma once
#include "String_piece.h"
#if _HAS_CXX17==1
#include <string_view>
typedef std::string_view String_view;
#else
typedef alime::StringPiece String_view;
#endif // 