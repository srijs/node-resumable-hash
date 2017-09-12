{
  "targets": [
    {
      "target_name": "engine",
      "sources": [
        "engine.cc",
        "workers.cpp",
        "hash.cpp",
        "sha1.cpp",
        "sha256.cpp",
        "buffer.cpp"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "<(node_root_dir)/deps/openssl/openssl/include",
        "/usr/local/include"
      ],
      "library_dirs": [
        "/usr/local/lib"
      ],
      "xcode_settings": {
        "OTHER_CFLAGS": [
          "-std=c++11",
          "-stdlib=libc++"
        ],
      },
    }
  ]
}
