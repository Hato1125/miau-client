set_project('miau')

set_allowedplats('android')
set_languages('c++26')

if has_config('compile_commands') then
  ---@diagnostic disable-next-line: param-type-mismatch
  add_rules('plugin.compile_commands.autoupdate', { outputdir = 'build' })
end

option('mods_dir')
  set_default(path.join(os.getenv('HOME'), '.local/share/mcpelauncher/mods'))
  set_showmenu(true)
  set_description('Copy mods dir')
option_end()

option('mods_file')
  set_default('miau.so')
  set_showmenu(true)
  set_description('Copy mods file')
option_end()

option('compile_commands')
  set_default(false)
  set_showmenu(true)
  set_description('Generate compile_commands.json')
option_end()

target('nanovg')
  set_kind('static')
  add_files('nanovg/src/*.c')
  add_includedirs('nanovg/src', {public = true})

target('miau')
  set_kind('shared')
  add_files('src/**.cc')
  add_includedirs('src')
  add_includedirs('$(ndk)/sources/android/native_app_glue')
  add_syslinks('android', 'EGL', 'GLESv2')
  add_deps('nanovg')
  after_build(
    function(target)
      if is_mode("debug") then
        os.cp(
          target:targetfile(),
          path.join(
            get_config('mods_dir'),
            get_config('mods_file')
          )
        )
      end
    end
  )
