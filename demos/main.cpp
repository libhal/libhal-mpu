// Copyright 2024 Khalil Estell
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "hardware_map.hpp"

int main()
{
  try {
    auto hardware_map = initialize_platform();
    application(hardware_map);
  } catch (...) {
    hal::halt();
  }

  return 0;
}

namespace __cxxabiv1 {                                 // NOLINT
std::terminate_handler __terminate_handler = nullptr;  // NOLINT
}

extern "C"
{
  void _exit([[maybe_unused]] int rc)
  {
    std::terminate();
  }

  int kill(int, int)
  {
    return -1;
  }

  struct _reent* _impure_ptr = nullptr;  // NOLINT

  int getpid()
  {
    return 1;
  }

  std::array<std::uint8_t, 256> exception_storage{};

  void* __wrap___cxa_allocate_exception(unsigned int p_size)  // NOLINT
  {
    // Size of the GCC exception object header
    constexpr size_t header_size = 128;

    if (exception_storage.size() < header_size + p_size) {
      std::terminate();
    }

    // Required for GCC's impl to work correctly as it assumes that all bytes
    // default to 0.
    exception_storage.fill(0);
    return exception_storage.data() + header_size;
  }

  void __wrap___cxa_call_unexpected(void*)  // NOLINT
  {
    std::terminate();
  }

  void __wrap___cxa_free_exception(void*)  // NOLINT
  {
    // Clear the contents of the storage buffer as the exception runtime expects
    // the contents to already be cleared.
    exception_storage.fill(0);
  }

  void __assert_func([[maybe_unused]] const char* p_file,
                     [[maybe_unused]] int p_line,
                     [[maybe_unused]] const char* p_function,
                     [[maybe_unused]] const char* p_failed_expr)
  {
    abort();
  }
}  // extern "C"
