/* Copyright 2018 Laurent Van Begin
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "Executor.h"
#include "ExecutorPool.h"
#include "WorkQueue.h"
#include "Task.h"

#include <stdlib.h>
#include <future>
#include <functional>
#include <algorithm>
#include <iostream>

template<class o>
std::future<o> create_future(o v) {
    std::promise<o> p;
    p.set_value(v);
    return p.get_future();
}

template<class o>
void print(std::function <o(void)> arg) {
    std::cout << arg() << std::endl;
}

static int test_promise() {
    auto f = create_future<int>(3);
    print<int>([&f]() { return f.get(); });
    return 0;
}

static int test_work() {
    task::internal::WorkQueue c;
    task::internal::Executor e(c);
    return 0;
}

static int test_WorkQueue() {
    task::internal::WorkQueue p;
    bool called = false;
    p.put([&called ](void) { called = true; std::cout << "hello again!"<< std::endl; } );
    auto t = p.get();
    t();
    if (called)
        std::cout << "function called!" << std::endl;
    else
        std::cout << "function not called!" << std::endl;
    return 0;
}

static int test_ExecutorPool() {
    auto p = std::make_unique<task::internal::WorkQueue>();
    task::internal::Executors executors;
    executors.emplace_back(std::make_unique<task::internal::Executor>(*p));
    executors.emplace_back(std::make_unique<task::internal::Executor>(*p));
    executors.emplace_back(std::make_unique<task::internal::Executor>(*p));

    task::internal::ExecutorPool pool(std::move(p), std::move(executors));
    bool called = false;
    pool.newWork([&called]() {called = true; });

    return 0;
}

static int test_task() {
    auto f = task::TASK<int>([]() { 
        return 1; 
    });
    f.wait();
    std::cout << "value = " << f.get() << std::endl;
    return 0;
}

static int test_task_that_raise_exception() {
    auto f = task::TASK<int>([]() {
        throw std::runtime_error("This is an exception"); 
        return 1; 
    });
    f.wait();

    try {
        std::cout << "value = " << f.get() << std::endl;
    }
    catch (std::runtime_error &e) {
        std::cout << "exception caught" << std::endl;
    }
    return 0;
}

static int test_set_worker_pool_size_fails() {
    try {
        task::setWorkPoolSize(50);
        std::cout << "ERROR: no exception caugh" << std::endl;
    }
    catch (std::runtime_error &e) {
        std::cout << "exception caugh" << std::endl;
    }
    return 0;
}

static int nbFailure = 0;

#define EXECUTE_TEST(t) executeTest(t,#t)


static void executeTest(std::function<int(void)> test, std::string testName)
{
    std::cout << "start " << testName << "." << std::endl;
    int result = 0;
    try {
        result = test();
    } catch(std::exception &e) {
        result = 1;
    }
    std::cout << "end " << testName << ": " << std::flush;
    if (result == 0) {
        std::cout << "OK" << std::endl;
    } else {
        nbFailure++;
        std::cout << "KO" << std::endl;
    }
}

int testsResult()
{
    return (nbFailure == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int main() {
    EXECUTE_TEST(test_work);
    EXECUTE_TEST(test_promise);
    EXECUTE_TEST(test_WorkQueue);
    EXECUTE_TEST(test_ExecutorPool);
    EXECUTE_TEST(test_task);
    EXECUTE_TEST(test_task_that_raise_exception);
    EXECUTE_TEST(test_set_worker_pool_size_fails);
    return testsResult();
}