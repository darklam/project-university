cd Debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --target test_project
cp ../stop_words.txt .
./test/test_project