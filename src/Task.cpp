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
#include "Task.h"
#include "WorkQueue.h"
#include "Executor.h"
#include "ExecutorPool.h"

#include <mutex>

namespace task {

static unsigned int numberExecutor = 5;

static std::unique_ptr<internal::ExecutorPool> pool;

static void initialize(unsigned int size) {
    auto p = std::make_unique<internal::WorkQueue>();
    internal::Executors executors;
    for (unsigned int i = 0; i < size; i++)
        executors.emplace_back(std::make_unique<internal::Executor>(*p));

    pool.reset(new internal::ExecutorPool(std::move(p), std::move(executors)));
}

void sendWorkToPool(std::function<void(void)> &&w) {
    static std::once_flag flag;
    std::call_once(flag, initialize, numberExecutor);
    pool->newWork(w);
}

void setWorkPoolSize(unsigned int size) {
    if (nullptr != pool)
        throw std::runtime_error("worker pool already initialized.");
    numberExecutor = size;
}

}