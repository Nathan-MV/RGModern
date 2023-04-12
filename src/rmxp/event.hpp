// zlib License

// copyright (C) 2023 Guoxiaomi and Krimiston

// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.

// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once
#include "base/base.hpp"
#include "controller.hpp"
#include "input.hpp"
#include "textinput.hpp"

namespace rgm::rmxp {
/**
 * @brief 绑定 event_dispatcher_t 对不同输入事件的响应
 * @note 事件分为以下 4 类：
 * 1. 退出事件，广播 interrupt_signal
 * 2. 窗口事件，处理后台运行时使用，未完成
 * 3. 键盘事件，广播 key_release 和 key_press
 * 4. 控制器事件，广播 controller_axis_move，
 *    controller_button_press 和 controller_button_release
 * 5. 鼠标事件，未完成
 */
struct init_event {
  static void before(auto& worker) {
    base::cen_library::event_dispatcher_t& d =
        RGMDATA(base::cen_library).event_dispatcher;

    d.bind<cen::quit_event>().to(
        [&worker]([[maybe_unused]] const cen::quit_event& e) {
          cen::log_info("[Input] quit");
          worker >> base::interrupt_signal{};
        });

    d.bind<cen::window_event>().to([&worker](const cen::window_event& e) {
      cen::log_info("[Input] window %s", cen::to_string(e.event_id()).data());
    });

    d.bind<cen::keyboard_event>().to([&worker](const cen::keyboard_event& e) {
      if (e.released()) {
        cen::log_debug("[Input] key '%s' is pressed", e.key().name().data());

        const int32_t key = static_cast<int32_t>(e.key().get());
        worker >> key_release{key};
      } else if (e.pressed()) {
        cen::log_debug("[Input] key '%s' is released", e.key().name().data());

        const int32_t key = static_cast<int32_t>(e.key().get());
        worker >> key_press{key};
      }
    });

    d.bind<cen::mouse_button_event>().to(
        [&worker](const cen::mouse_button_event& e) {
          if (e.released()) {
            cen::log_debug("[Input] mouse button '%s' is released",
                           cen::to_string(e.button()).data());
          } else if (e.pressed()) {
            cen::log_debug("[Input] mouse button '%s' is pressed",
                           cen::to_string(e.button()).data());
          }
        });

    d.bind<cen::text_editing_event>().to(
        [&worker](const cen::text_editing_event& e) {
          cen::log_debug("[Input] text edit\n");

          worker >> text_edit{std::string{e.text()}, e.start()};
        });

    d.bind<cen::text_input_event>().to(
        [&worker](const cen::text_input_event& e) {
          cen::log_info("[Input] text input '%s'\n", e.text_utf8().data());

          worker >> text_input{std::string{e.text_utf8()}};
        });

    d.bind<cen::controller_axis_event>().to(
        [&worker](const cen::controller_axis_event& e) {
          cen::log_debug("[Input] controller axis '%s', value %d\n",
                         cen::to_string(e.axis()).data(), e.value());

          worker >> controller_axis_move{e.axis(), e.which(), e.value()};
        });

    d.bind<cen::controller_button_event>().to(
        [&worker](const cen::controller_button_event& e) {
          if (e.is_released()) {
            cen::log_debug("[Input] controller %d button '%s' is released",
                           e.which(), cen::to_string(e.button()).data());

            const int key = static_cast<int>(e.button());
            worker >> controller_button_release{e.which(), key};
          } else if (e.is_pressed()) {
            cen::log_debug("[Input] controller %d button '%s' is pressed",
                           e.which(), cen::to_string(e.button()).data());

            const int key = static_cast<int>(e.button());
            worker >> controller_button_press{e.which(), key};
          }
        });
  }
};
}  // namespace rgm::rmxp