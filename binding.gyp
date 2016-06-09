{
  "targets": [
    {
      "target_name": "engine",
      "sources": [
        "engine.cc",
        "sha1.cpp",
        "sha256.cpp"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "<(node_root_dir)/deps/openssl/openssl/include",
        "/usr/local/include"
      ],
      "library_dirs": [
        "/usr/local/lib"
      ]
    }
  ]
}
