#!/usr/bin/env bash

echo "Create new empty project"
echo -n "Project name: "
read -r new_project_name

echo "Creating project files for $new_project_name"
mkdir $new_project_name
mkdir $new_project_name/build

echo -n "Choose name of executable? (main): "
read -r exec_name

if [[ -z "$exec_name" ]]; then
  exec_name="main"
fi

cmake_file=$new_project_name/CMakeLists.txt
touch $cmake_file
echo -e "cmake_minimum_required(VERSION 3.2)""
set(CMAKE_CXX_STANDARD 20)""
project($new_project_name)""
set(CMAKE_BUILD_TYPE Debug)""
add_executable($exec_name main.cpp)" > $cmake_file

main_cpp_file=$new_project_name/main.cpp
touch $main_cpp_file
echo -e "#include <iostream>""
int main() {""
  std::cout << \"Hello world!\" << std::endl;""
  return 0;""
}" > $main_cpp_file

cmake -S $new_project_name/ -B $new_project_name/build/
cd $new_project_name/build/
make
./$exec_name
