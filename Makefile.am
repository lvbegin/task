AM_CPPFLAGS=-I$(top_srcdir)/include
instdir=/usr/local/lib

inst_LIBRARIES=libtask.a
inst_HEADERS=include/Task.h
noinst_HEADERS=include/Executor.h include/ExecutorInterface.h include/ExecutorPool.h include/Work.h include/WorkQueue.h
libtask_a_SOURCES=src/Executor.cpp src/ExecutorPool.cpp src/Task.cpp src/WorkQueue.cpp
libtask_a_CXXFLAGS=$(DEFAULTCXXFLAGS)
check_PROGRAMS=task_tests_static task_tests_dynamic

inst_LTLIBRARIES=libtask.la
libtask_la_SOURCES=src/Executor.cpp src/ExecutorPool.cpp src/Task.cpp src/WorkQueue.cpp
libtask_la_CXXFLAGS=$(DEFAULTCXXFLAGS)

TESTS=task_tests_static task_tests_dynamic

task_tests_static_SOURCES=test/test_task.cpp
task_tests_static_LDADD=libtask.a 
task_tests_static_LDFLAGS=$(PTHREAD)

task_tests_dynamic_SOURCES=test/test_task.cpp
task_tests_dynamic_LDADD=-ltask 
task_tests_dynamic_LDFLAGS=$(PTHREAD)
