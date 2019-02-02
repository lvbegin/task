AM_CPPFLAGS=-I$(top_srcdir)/include
instdir=/usr/local/lib

inst_LIBRARIES=libtask.a
libtask_a_SOURCES=src/Executor.cpp src/ExecutorPool.cpp src/Task.cpp src/WorkQueue.cpp

check_PROGRAMS=task_tests
TESTS=task_tests

task_tests_SOURCES=test/test_task.cpp
task_tests_LDADD=libtask.a 
task_tests_LDFLAGS=-pthread
