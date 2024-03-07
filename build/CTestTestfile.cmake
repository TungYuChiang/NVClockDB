# CMake generated Testfile for 
# Source directory: /home/oslab/Desktop/TungYu/PMDB
# Build directory: /home/oslab/Desktop/TungYu/PMDB/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(leveldb_tests "/home/oslab/Desktop/TungYu/PMDB/build/leveldb_tests")
add_test(c_test "/home/oslab/Desktop/TungYu/PMDB/build/c_test")
add_test(main "/home/oslab/Desktop/TungYu/PMDB/build/main")
add_test(self_bench "/home/oslab/Desktop/TungYu/PMDB/build/self_bench")
add_test(env_posix_test "/home/oslab/Desktop/TungYu/PMDB/build/env_posix_test")
subdirs("third_party/googletest")
subdirs("third_party/benchmark")
