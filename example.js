var addon = require('./cmake-build-debug/Debug/SimpleNativeAddon.node')

var callNativeAddon = function() {
  var result = addon.exampleMethod();
  console.log('result', result);
};

while (true) {
  callNativeAddon();
}


