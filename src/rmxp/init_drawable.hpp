// zlib License
//
// copyright (C) 2023 Guoxiaomi and Krimiston
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once
#include "base/base.hpp"
#include "drawable.hpp"
#include "tilemap_manager.hpp"

namespace rgm::rmxp {
void drawables::set_z(const z_index& key, const int new_z) {
  auto node = extract(key);

  // fixed delta_z overlayer 的处理
  auto set_z_visitor = [=, &key, this]([[maybe_unused]] auto&& item) {
    if constexpr (requires { item.fixed_overlayer_zs; }) {
      for (uint16_t delta_z : item.fixed_overlayer_zs) {
        set_z(z_index{key.z + delta_z, key.id}, new_z + delta_z);
      }
    }
  };
  std::visit(set_z_visitor, node.mapped());

  if (!node.empty()) {
    node.key() = z_index{new_z, key.id};
    insert(std::move(node));
  }
}

size_t drawables::total_size() {
  size_t size = this->size();
  for (auto& [zi, item] : *this) {
    if (std::holds_alternative<viewport>(item)) {
      viewport& v = std::get<viewport>(item);
      size += v.m_data.total_size();
    }
  }
  return size;
}

/**
 * @brief 创建 drawable 通用的 ruby 方法
 * @note 方法包括：
 * 1. dispose，将 drawable 从 drawables 中移除，释放相关资源
 * 2. set_z，修改 drawable 的 z 值。z 虽然也是值属性，
 *    但是修改 z 会影响 drawable 的绘制顺序，所以单独处理。
 * 3. refresh_value，同步更新值类型的属性
 */
struct init_drawable_base {
  using data = std::tuple<drawables, id2z>;

  static void before(auto& this_worker) {
    static decltype(auto) worker = this_worker;

    struct wrapper {
      static VALUE dispose(VALUE, VALUE viewport_, VALUE id_) {
        drawables& data = RGMDATA(drawables);
        id2z& cache_z = RGMDATA(id2z);

        RGMLOAD(id, uint64_t);

        auto it = cache_z.find(id);

        // 如果 cache_z 中没有这个 id，那么相应的 drawable 已经析构。
        if (it == cache_z.end()) return Qnil;

        int z = it->second;
        cache_z.erase(it);
        // 从 tilemap_manager 中移除 infos
        RGMDATA(tilemap_manager).infos.erase(id);

        drawables* p_data = &data;
        if (viewport_ != Qnil) {
          z_index v_zi;
          v_zi << viewport_;
          viewport& v = std::get<viewport>(data[v_zi]);
          p_data = &v.m_data;
        }
        auto node = p_data->extract(z_index{z, id});
        if (!node.empty()) {
          // fixed delta_z overlayer 的处理
          auto erase_visitor = [=]([[maybe_unused]] auto&& item) {
            if constexpr (requires { item.fixed_overlayer_zs; }) {
              for (uint16_t delta_z : item.fixed_overlayer_zs) {
                p_data->erase(z_index{z + delta_z, id});
              }
            }
          };
          std::visit(erase_visitor, node.mapped());
        }
        return Qnil;
      }

      static VALUE set_z(VALUE, VALUE drawable_, VALUE viewport_, VALUE z_) {
        drawables& data = RGMDATA(drawables);
        id2z& cache_z = RGMDATA(id2z);

        z_index zi;
        zi << drawable_;

        RGMLOAD(z, int);
        int new_z = z;

        auto it = cache_z.find(zi.id);
        if (it == cache_z.end()) return z_;
        it->second = new_z;

        drawables* p_data = &data;
        if (viewport_ != Qnil) {
          z_index v_zi;
          v_zi << viewport_;
          viewport& v = std::get<viewport>(data[v_zi]);
          p_data = &v.m_data;
        }
        p_data->set_z(zi, new_z);
        return z_;
      }

      static VALUE refresh_value(VALUE, VALUE drawable_, VALUE viewport_,
                                 VALUE type_) {
        drawables& data = RGMDATA(drawables);
        z_index zi;
        zi << drawable_;

        Check_Type(type_, T_FIXNUM);
        auto type = static_cast<word>(FIX2INT(type_));

        auto visitor_refresh = [=]<typename T>(T& item) {
          if constexpr (std::is_base_of_v<drawable_object<T>, T>) {
            item.refresh_value(type);
          }
        };

        drawables* p_data = &data;
        if (viewport_ != Qnil) {
          z_index v_zi;
          v_zi << viewport_;
          viewport& v = std::get<viewport>(data[v_zi]);
          p_data = &v.m_data;
        }
        std::visit(visitor_refresh, p_data->at(zi));
        return Qnil;
      }
    };

    VALUE rb_mRGM = rb_define_module("RGM");
    VALUE rb_mRGM_Base = rb_define_module_under(rb_mRGM, "Base");
    rb_define_module_function(rb_mRGM_Base, "drawable_dispose",
                              wrapper::dispose, 2);
    rb_define_module_function(rb_mRGM_Base, "drawable_set_z", wrapper::set_z,
                              3);
    rb_define_module_function(rb_mRGM_Base, "drawable_refresh_value",
                              wrapper::refresh_value, 3);
  }
};

/**
 * @brief 创建 drawable 专用的 ruby 方法，目前只有 create_xxx 方法
 *
 * @tparam T_Drawable 目标类型
 */
template <typename T_Drawable>
struct init_drawable {
  static void before(auto& this_worker) {
    static decltype(auto) worker = this_worker;

    struct wrapper {
      static VALUE create(VALUE, VALUE drawable_) {
        drawables& data = RGMDATA(drawables);
        id2z& cache_z = RGMDATA(id2z);

        z_index zi;
        T_Drawable drawable;
        zi << drawable_;
        drawable << drawable_;

        // 注册到 cache_z 中。
        cache_z.insert({zi.id, zi.z});
        // tilemap 的处理，添加 info 到 tilemap_manager 中
        if constexpr (std::same_as<T_Drawable, tilemap>) {
          tilemap_manager& tm = RGMDATA(tilemap_manager);
          tm.infos.emplace(zi.id, tilemap_info());
        }

        VALUE viewport_ = detail::get<word::viewport>(drawable_);

        drawables* p_data = &data;
        if (viewport_ != Qnil) {
          z_index v_zi;
          v_zi << viewport_;
          viewport& v = std::get<viewport>(data[v_zi]);
          p_data = &v.m_data;
        }
        p_data->emplace(zi, std::move(drawable));
        // fixed delta_z overlayer 的处理
        if constexpr (requires { T_Drawable::fixed_overlayer_zs; }) {
          T_Drawable* p_drawable = &std::get<T_Drawable>(p_data->at(zi));
          size_t index = 0;
          for (uint16_t delta_z : T_Drawable::fixed_overlayer_zs) {
            p_data->emplace(z_index{zi.z + delta_z, zi.id},
                            overlayer<T_Drawable>{p_drawable, index});
            ++index;
          }
        }
        return Qnil;
      }
    };

    VALUE rb_mRGM = rb_define_module("RGM");
    VALUE rb_mRGM_Base = rb_define_module_under(rb_mRGM, "Base");
    std::string method_name = T_Drawable::name;
    method_name += "_create";
    rb_define_module_function(rb_mRGM_Base, method_name.data(), wrapper::create,
                              1);
  }
};
}  // namespace rgm::rmxp