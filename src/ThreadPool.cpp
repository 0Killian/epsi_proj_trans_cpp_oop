//
// Created by Killian on 17/03/2023.
//
#include <ThreadPool.h>
#include <Application.h>

void ThreadPool::Init()
{
    // Reserve one less thread than the hardware concurrency because the main thread will be used as well
    // hardware_concurrency() returns the number of logical cores, not the number of physical cores.
    // For example, a quad-core processor with hyperthreading enabled will return 8.
    m_threads.reserve(std::thread::hardware_concurrency() - 1);

    // Create the threads
    for (int i = 0; i < std::thread::hardware_concurrency() - 1; i++)
    {
        m_threads.emplace_back([this]()
        {
            // Set the thread id
            int id = m_nextThreadId++;

            // Initialize the OpenGL context. This is required because OpenGL contexts are not shared
            // between threads. This means that we need to create a new context for each thread, and
            // share the OpenGL display list with the main context.
            sf::Context context;
            auto mainContext = Application::GetInstance().GetContextId();

            if(!context.setActive(true))
            {
                SPDLOG_ERROR("Failed to set context active in another thread!");
                return;
            }

            // We need to use platform-specific code to share the OpenGL display list
#ifdef _WIN32
            wglShareLists(mainContext, wglGetCurrentContext());
#else
#error Not Supported!
#endif

            // The thread is now ready to execute tasks
            while (true)
            {
                std::function<void()> task;
                {
                    // Lock the mutex and wait for a task to be available
                    std::unique_lock<std::mutex> lock(m_mutex);
                    m_condition.wait(lock, [this]()
                    {
                        return m_shouldStop || !m_tasks.empty();
                    });

                    // If the thread pool is terminating, and there are no more tasks, exit the thread
                    if (m_shouldStop && m_tasks.empty())
                    {
                        return;
                    }

                    // Get the next task and remove it from the queue
                    task = std::move(m_tasks.front());
                    m_tasks.pop_front();
                }

                // Execute the task
                task();
            }
        });
    }
}

void ThreadPool::Terminate()
{
    {
        // Set the flag to stop the threads
        std::unique_lock<std::mutex> lock(m_mutex);
        m_shouldStop = true;
    }
    // Notify all threads that a task is available
    m_condition.notify_all();

    for (auto& thread : m_threads)
    {
        // Wait for the thread to finish
        thread.join();
    }
}