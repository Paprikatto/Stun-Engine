project('StunEngine', ['cpp', 'c'],
  version: '0.1.0',
  meson_version: '>= 0.60.0',
  default_options: [
    'cpp_std=c++20',
    'default_library=static',
    'warning_level=2',
    'buildtype=debugoptimized',
    'werror=true'
  ]
)
system = host_machine.system()
subdir('src')
subdir('thirdparty/imgui')
subdir('res')

add_project_arguments('-DGLEW_STATIC', language: 'cpp')
add_project_link_arguments('-static-libstdc++', language: 'cpp')

opengl_dep = dependency('opengl', required: true)
glfw_dep = declare_dependency(
  include_directories: include_directories('thirdparty/glfw/include'),
  link_args: ['-L' + meson.project_source_root() + '/thirdparty/glfw/lib-mingw-w64', '-lglfw3']
)
glew_lib = library('glew', ['thirdparty/glew-2.1.0/src/glew.c'],
    include_directories: include_directories('thirdparty/glew-2.1.0/include'),
    dependencies: [opengl_dep],
    c_args: ['-DGLEW_STATIC', '-w'],
    install: false,
)
glew_dep = declare_dependency(
    include_directories: include_directories('thirdparty/glew-2.1.0/include'),
    link_with: glew_lib,
)

#if system == 'windows'
#    assimp_path = '\thirdparty\assimp\windows'
#elif system == 'linux'
#    assimp_path = '/thirdparty/assimp/linux'
#else
#    error('Unsupported system: ' + system)
#endif

assimp_dep = declare_dependency(
    include_directories: include_directories('thirdparty/assimp/include'),
    link_args: ['-L' + meson.project_source_root() + '/thirdparty/assimp' + '-lassimp']
)

exe = executable('StunEngine', sources + imgui_sources, shaders, 
  dependencies: [glfw_dep, glew_dep, opengl_dep, assimp_dep],
  include_directories: include_directories('thirdparty/glm', 'thirdparty/imgui'),
) 
