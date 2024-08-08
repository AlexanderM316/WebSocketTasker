#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QList>
#include <QString>
#include "Task.h"

class TaskManager {
public:
    TaskManager();

    void createTask(const QString &name);
    QList<Task*> getTasks() const;
    void updateTask(int id, const QString &name);
    void deleteTask(int id);
    void completeTask(int id);

private:
    QList<Task*> tasks;
    int nextId;
};

#endif // TASKMANAGER_H
