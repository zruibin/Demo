{
  "targets": [
    {
        "target_name": "native",
        "sources": [ 
            "./native/native.cc",
        ],
        "include_dirs": ["<!@(node -p \"require('node-addon-api').include\")"],
        "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],
        "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"],
        "cflags!": ["-fno-exceptions"],
        "cflags_cc!": ["-fno-exceptions"],
    },
    {
        "target_name": "copy_binary",
        "type":"none",
        "dependencies" : [ "native" ],
        "copies":[
            {
                'destination': '<(module_root_dir)/lib/',
                'files': ['<(module_root_dir)/build/release/native.node']
            }
        ]
    }
  ]
}
