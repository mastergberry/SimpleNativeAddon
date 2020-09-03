{
  "targets": [
    {
      "target_name": "binding",
      "include_dirs" : [ "<!(node -e \"require('nan')\")" ],
      "sources": [ "library.cpp" ]
    }
  ]
}
