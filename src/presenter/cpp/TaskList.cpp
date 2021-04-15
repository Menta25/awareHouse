#include "TaskList.h"

TaskList::TaskList(QObject* parent)
    : QObject(parent)
{
    m_tasks.append(new TaskPresenter("amogus", {1,2,3}, 3, 5, this));
    m_tasks.append(new TaskPresenter("sus", {4}, 0, 7, this));
    m_tasks.append(new TaskPresenter("lajos", {2,4}, 2, 7, this));
}

QList<const TaskPresenter*>* TaskList::tasks() { return &m_tasks; }

bool TaskList::setTaskAt(int index, const TaskPresenter& task)
{
    if (index < 0 ||
        index >= m_tasks.size())
        return false;

    m_tasks[index] = &task;
    return true;
}

void TaskList::appendOrder(const TaskPresenter& task)
{
    emit preItemAppended();
    m_tasks.append(&task);
    emit postItemAppended();
}

void TaskList::removeOrder(int index)
{
    if (index < 0 ||
        index >= m_tasks.size())
        return;

    emit preItemRemoved(index);
    m_tasks.removeAt(index);
    emit postItemRemoved();
}

void TaskList::clear()
{
    emit preItemRemoved(0);
    m_tasks.clear();
    emit postItemRemoved();
}