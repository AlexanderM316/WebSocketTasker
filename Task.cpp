#include "Task.h"

Task::Task(int id, const QString &name)
    : id(id), name(name), completed(false) {}

int Task::getId() const { return id; }

QString Task::getName() const { return name; }

bool Task::isCompleted() const { return completed; }

void Task::setName(const QString &newName) { name = newName; }

void Task::markCompleted() { completed = true; }
