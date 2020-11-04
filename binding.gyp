{
  "targets": [
    {
      "target_name": "SimpleNativeAddon",
      "include_dirs" : [
        "<!(node -e \"require('nan')\")"
      ],
      "sources": [
        "library.cpp"
      ],
      "configurations": {
        "Release": {
          "msvs_settings": {
            "VCCLCompilerTool": {
              "RuntimeLibrary": "0",
              "ExceptionHandling": 2,
              "WholeProgramOptimization": "true",
            },
            "VCLinkerTool": {
              "OptimizeReferences": 2,
              "EnableCOMDATFolding": 2,
              "LinkIncremental": 2,
              "AdditionalOptions": [
                "/LTCG:INCREMENTAL"
              ]
            }
          },
          "msbuild_toolset": "v142",
        }
      } 
    }
  ]
}
