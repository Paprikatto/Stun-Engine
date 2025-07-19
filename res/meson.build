lit_vert = custom_target(
    'lit.vert',
    input: 'shaders/lit.vert',
    output: 'lit_vert.h',
    command: ['xxd', '-i', '@INPUT@'],
    capture: true,
)
lit_frag = custom_target(
    'lit.frag',
    input: 'shaders/lit.frag',
    output: 'lit_frag.h',
    command: ['xxd', '-i', '@INPUT@'],
    capture: true,
)
shaders = [lit_vert, lit_frag]
