set(SOURCE_FILES memory.c
                stack.c
                framebuffer.c
                registers.c
                main.c)

include_directories(include/)
add_executable(neovm ${SOURCE_FILES})
