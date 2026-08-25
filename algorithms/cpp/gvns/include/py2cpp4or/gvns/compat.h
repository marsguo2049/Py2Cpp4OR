#pragma once

// C++14 has general attribute syntax but not the C++17 nodiscard attribute.
// GCC 6.3 warns that [[nodiscard]] is ignored, so use its supported equivalent.
#if defined(__GNUC__) || defined(__clang__)
#define PY2CPP4OR_NODISCARD __attribute__((warn_unused_result))
#else
#define PY2CPP4OR_NODISCARD
#endif
