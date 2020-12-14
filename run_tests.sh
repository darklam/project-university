cd Debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --target test_project
./test/test_project