#include "TaskManager.h"
#include <algorithm> // For std::remove_if

TaskManager::TaskManager() : nextId(1) {}

void TaskManager::createTask(const QString &name) {
    tasks.append(new Task(nextId++, name));
}

QList<Task*> TaskManager::getTasks() const {
    return tasks;
}

void TaskManager::updateTask(int id, const QString &name) {
    for (Task *task : tasks) {
        if (task->getId() == id) {
            task->setName(name);
            break;
        }
    }
}

void TaskManager::deleteTask(int id) {
    tasks.erase(std::remove_if(tasks.begin(), tasks.end(),
                                [=](Task *task) {
                                    if (task->getId() == id) {
                                        delete task;
                                        return true;
                                    }
                                    return false;
                                }), tasks.end());
}

void TaskManager::completeTask(int id) {
    for (Task *task : tasks) {
        if (task->getId() == id) {
            task->markCompleted();
            break;
        }
    }
}
