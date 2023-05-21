# --------------------------------------------------------------------
# This file was automatically generated by `ext/rgm_defines_gen.rb`
# --------------------------------------------------------------------
# 本文件仅用于展示 RGM 内置函数和常量，请不要 load 此文件。
# --------------------------------------------------------------------
module RGM
  module Base
    def bitmap_blt(id, x, y, src_id, rect, opacity); end
    def bitmap_capture_screen(id); end
    def bitmap_create(id, width, height); end
    def bitmap_dispose(id); end
    def bitmap_draw_text(id, font, rect, text, align); end
    def bitmap_fill_rect(id, rect, color); end
    def bitmap_get_pixel(id, x, y); end
    def bitmap_grayscale(id); end
    def bitmap_hue_change(id, hue); end
    def bitmap_reload_autotile(id); end
    def bitmap_save_png(id, path); end
    def bitmap_stretch_blt(id, dst_rect, src_id, src_rect, opacity); end
    def bitmap_text_size(font, text); end
    def check_delay(frame_rate); end
    def controller_axis_value(axis, joy_index); end
    def controller_bind(button, input_key, joy_index); end
    def controller_rumble(joy_index, low, high, duration); end
    def controller_rumble_triggers(joy_index, left, right, duration); end
    def drawable_dispose(viewport, id); end
    def drawable_set_z(drawable, viewport, z); end
    def embeded_load(); end
    def embeded_load(path); end
    def font_create(path); end
    def graphics_transition(freeze_id, current_id, rate, transition_id, vague); end
    def graphics_update(); end
    def input_bind(sdl_key, input_key); end
    def input_last_press(); end
    def input_last_release(); end
    def input_press(input_key); end
    def input_repeat(input_key); end
    def input_reset(); end
    def input_trigger(input_key); end
    def input_update(); end
    def load_script(); end
    def load_script(path); end
    def message_show(text); end
    def music_create(id, path); end
    def music_dispose(id); end
    def music_fade_in(id, iteration, duration); end
    def music_fade_out(duration); end
    def music_finish_callback(); end
    def music_get_position(id); end
    def music_get_volume(); end
    def music_halt(); end
    def music_pause(); end
    def music_play(id, iteration); end
    def music_resume(); end
    def music_rewind(); end
    def music_set_position(position); end
    def music_set_soundfonts(path); end
    def music_set_volume(volume); end
    def new_id(); end
    def palette_convert_to_bitmap(id, bitmap_id, rect); end
    def palette_create(id, width, height); end
    def palette_dispose(id); end
    def palette_get_pixel(id, x, y); end
    def palette_save_png(id, path); end
    def palette_set_pixel(id, x, y, color); end
    def present_window(); end
    def resize_screen(width, height); end
    def resize_window(width, height, scale_mode); end
    def set_fullscreen(mode); end
    def set_title(title); end
    def sound_create(id, path); end
    def sound_dispose(id); end
    def sound_fade_in(id, duration); end
    def sound_fade_out(id, duration); end
    def sound_get_channel(id); end
    def sound_get_state(id); end
    def sound_play(id, iteration); end
    def sound_set_pitch(id, pitch, loop); end
    def sound_set_volume(id, volume); end
    def sound_stop(id); end
    def synchronize(worker_id); end
    def table_create(id, x_size, y_size, z_size); end
    def table_dispose(id); end
    def table_dump(id); end
    def table_get(data_ptr, index); end
    def table_load(id, string); end
    def table_resize(id, x_size, y_size, z_size); end
    def table_set(data_ptr, index, value); end
    def viewport_create(viewport); end
    def viewport_dispose(id); end
    def viewport_refresh_value(data_ptr, type); end
    def viewport_set_z(viewport, z); end
  end

  module Ext
    def async_ping(hint); end
    def external_check(path); end
    def external_load(path); end
    def external_regist(path, password); end
    def mouse_double_click(button, interval); end
    def mouse_press(button); end
    def mouse_reset(); end
    def mouse_trigger(button); end
    def mouse_update(); end
    def mouse_wheel(); end
    def mouse_x(); end
    def mouse_y(); end
    def textinput_edit_clear(); end
    def textinput_edit_pos(); end
    def textinput_edit_text(); end
    def textinput_start(x, y, width, height); end
    def textinput_stop(); end
  end

  module Config
    Battle_Test
    Build_Mode
    Config_Path
    Controller_Axis_Threshold
    Debug
    Game_Title
    Max_Workers
    Render_Driver
    Render_Driver_Name
    Resource_Prefix
    Screen_Height
    Screen_Width
    Synchronized
    Tileset_Texture_Height
    Window_Height
    Window_Width
  end
end
