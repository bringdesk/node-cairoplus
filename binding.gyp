{
  "targets": [
    {
      "target_name": "screen",
      "sources": [
          "src/screen_cairo.c",
          "src/screen_wrap.cc",
      ],
      'conditions': [
          ['OS=="linux"', {
              'libraries': [
                  '<!@(pkg-config cairo --libs)',
              ],
              'include_dirs': [
                  '<!@(pkg-config cairo --cflags-only-I | sed s/-I//g)',
              ]
          }]
      ]
    }
  ]
}
