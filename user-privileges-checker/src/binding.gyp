{
  "targets": [
    {
      "target_name": "checkPrivileges",
      "sources": [ "src/checkPrivileges.cpp" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "defines": [ "NAPI_CPP_EXCEPTIONS" ],
      "conditions": [
        [ 'OS=="win"', {
          "libraries": ["-lnetapi32"]
        }]
      ]
    }
  ]
}
