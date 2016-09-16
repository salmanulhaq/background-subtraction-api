{
  "targets": [{ 
      "target_name": "bgsub",
      "sources": [ 
        "src/init.cc",
        "src/internal.cc",
        "src/bgsub.cc",
        "src/subtract.cc"
        ],
      "include_dirs": ["<!(node -e \"require('nan')\")"],
      "libraries": ["<!@(pkg-config --libs bgsub)"],
      "cflags": ["-std=c++11"],
      "cflags_cc!": ["-fno-exceptions", "-fpermissive", "-std=c++11"]
  }]
}
