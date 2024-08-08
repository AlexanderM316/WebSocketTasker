#ifndef TASK_H
#define TASK_H

#include <QString>

class Task {
public:
    Task(int id, const QString &name);

    int getId() const;
    QString getName() const;
    bool isCompleted() const;

    void setName(const QString &newName);
    void markCompleted();

private:
    int id;
    QString name;
    bool completed;
};

#endif // TASK_H
