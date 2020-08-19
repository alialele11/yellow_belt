#include <iostream>
#include <tuple>
#include <vector>
#include <cstdint>
#include <string>
#include <map>
#include <algorithm>
using namespace std;
 
// Перечислимый тип для статуса задачи
enum class TaskStatus {
    NEW,          // новая
    IN_PROGRESS,  // в разработке
    TESTING,      // на тестировании
    DONE          // завершена
};
 
// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;
 
class TeamTasks {
public:
    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo& GetPersonTasksInfo(const string& person) const {
        return db.at(person);
    }
    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const string& person) {
        if (db.count(person) == 0) {
            db.insert({ person, {{TaskStatus::NEW, 1}} });
        }
        else {
            db[person][TaskStatus::NEW] += 1;
        }
    }
    // Обновить статусы по данному количеству задач конкретного разработчика,
    // подробности см. ниже
    tuple<TasksInfo, TasksInfo> PerformPersonTasks(
        const string& person, int task_count) {
        map<string, TasksInfo> new_base;
        new_base = db;
        TasksInfo newt = { {TaskStatus::NEW, 0}, {TaskStatus::IN_PROGRESS, 0}, {TaskStatus::TESTING, 0}, {TaskStatus::DONE, 0} };
        TasksInfo oldt = { {TaskStatus::NEW, 0}, {TaskStatus::IN_PROGRESS, 0}, {TaskStatus::TESTING, 0}, {TaskStatus::DONE, 0} };
        for (int i = static_cast<int>(TaskStatus::NEW); i < static_cast<int>(TaskStatus::DONE); i++) {
            if (i == static_cast<int>(TaskStatus::TESTING)) {
                task_count = min(task_count, db[person][static_cast<TaskStatus>(i)]);
            }
            if (task_count <= db[person][static_cast<TaskStatus>(i)]) {
                db = new_base;
                new_base[person][static_cast<TaskStatus>(i)] -= task_count;
                new_base[person][static_cast<TaskStatus>(i+1)] += task_count;
                for (int j = static_cast<int>(TaskStatus::NEW); j <= static_cast<int>(TaskStatus::DONE); j++) {
                    if (new_base[person][static_cast<TaskStatus>(j)] - db[person][static_cast<TaskStatus>(j)] >= 0) {
                        newt[static_cast<TaskStatus>(j)] += (new_base[person][static_cast<TaskStatus>(j)] - db[person][static_cast<TaskStatus>(j)]);
                    }
                    if (new_base.at(person).at(static_cast<TaskStatus>(j)) - newt[static_cast<TaskStatus>(j)] >= 0) {
                        oldt[static_cast<TaskStatus>(j)] += (new_base[person][static_cast<TaskStatus>(j)] - newt[static_cast<TaskStatus>(j)]);
                    }
                }
                break;
            }
            else {
                new_base[person][static_cast<TaskStatus>(i + 1)] += db[person][static_cast<TaskStatus>(i)];
                new_base[person][static_cast<TaskStatus>(i)] -= db[person][static_cast<TaskStatus>(i)];
                task_count = task_count - db[person][static_cast<TaskStatus>(i)];
                if (new_base[person][static_cast<TaskStatus>(i+1)] - db[person][static_cast<TaskStatus>(i+1)] >= 0) {
                        newt[static_cast<TaskStatus>(i+1)] += (new_base[person][static_cast<TaskStatus>(i+1)] - db[person][static_cast<TaskStatus>(i+1)]);
                }
            }
        } 
        db = new_base;
        for (int l = static_cast<int>(TaskStatus::NEW); l <= static_cast<int>(TaskStatus::DONE); l++) {
            if (new_base[person][static_cast<TaskStatus>(l)] == 0) {
                db[person].erase(static_cast<TaskStatus>(l));
            }
        }
        TasksInfo n_t;
        TasksInfo un_t;
        for (int i = static_cast<int>(TaskStatus::IN_PROGRESS); i <= static_cast<int>(TaskStatus::DONE); i++) {
            if (newt[static_cast<TaskStatus>(i)] > 0) {
                n_t[static_cast<TaskStatus>(i)] = newt[static_cast<TaskStatus>(i)];
            }
        }
        for (int i = static_cast<int>(TaskStatus::NEW); i <= static_cast<int>(TaskStatus::TESTING); i++) {
            if (oldt[static_cast<TaskStatus>(i)] > 0) {
                un_t[static_cast<TaskStatus>(i)] = oldt[static_cast<TaskStatus>(i)];
            }
        }
        return tie(n_t, un_t);
    }
private:
    map<string, TasksInfo> db;
};
void PrintTasksInfo(const TasksInfo& tasks_info) {
    if (tasks_info.count(TaskStatus::NEW)) {
        std::cout << "NEW: " << tasks_info.at(TaskStatus::NEW) << " ";
    }
    if (tasks_info.count(TaskStatus::IN_PROGRESS)) {
        std::cout << "IN_PROGRESS: " << tasks_info.at(TaskStatus::IN_PROGRESS) << " ";
    }
    if (tasks_info.count(TaskStatus::TESTING)) {
        std::cout << "TESTING: " << tasks_info.at(TaskStatus::TESTING) << " ";
    }
    if (tasks_info.count(TaskStatus::DONE)) {
        std::cout << "DONE: " << tasks_info.at(TaskStatus::DONE) << " ";
    }
}
 
void PrintTasksInfo(const TasksInfo& updated_tasks, const TasksInfo& untouched_tasks) {
    std::cout << "Updated: [";
    PrintTasksInfo(updated_tasks);
    std::cout << "] ";
 
    std::cout << "Untouched: [";
    PrintTasksInfo(untouched_tasks);
    std::cout << "] ";
 
    std::cout << std::endl;
}
int main() {
    TeamTasks tasks;
    TasksInfo updated_tasks;
    TasksInfo untouched_tasks;
 
    /* TEST 1 */
    std::cout << "Alice" << std::endl;
 
    for (auto i = 0; i < 5; ++i) {
        tasks.AddNewTask("Alice");
    }
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice", 5);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 5}, {}]
 
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice", 5);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"TESTING": 5}, {}]
 
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice", 1);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"DONE": 1}, {"TESTING": 4}]
 
    for (auto i = 0; i < 5; ++i) {
        tasks.AddNewTask("Alice");
    }
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice",
        2);  // [{"IN_PROGRESS": 2}, {"NEW": 3, "TESTING": 4}]
    PrintTasksInfo(updated_tasks, untouched_tasks);
 
    PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));  // {"NEW": 3, "IN_PROGRESS": 2, "TESTING": 4, "DONE": 1}
    std::cout << std::endl;
 
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice", 4);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 3, "TESTING": 1}, {"IN_PROGRESS": 1, "TESTING": 4}]
 
    PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));  // {"IN_PROGRESS": 4, "TESTING": 5, "DONE": 1}
    std::cout << std::endl;
 
    /* TEST 2 */
    std::cout << "\nJack" << std::endl;
    tasks.AddNewTask("Jack");
 
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Jack", 1);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 1}, {}]
 
    tasks.AddNewTask("Jack");
 
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Jack", 2);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 1, "TESTING": 1}, {}]
 
    tasks.AddNewTask("Jack");
 
    PrintTasksInfo(tasks.GetPersonTasksInfo("Jack"));  // {"NEW": 1, "IN_PROGRESS": 1, "TESTING": 1, "DONE": 0}
    std::cout << std::endl;
 
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Jack", 3);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 1, "TESTING": 1, "DONE": 1}, {}]!!!!!
 
    PrintTasksInfo(tasks.GetPersonTasksInfo("Jack"));  // {"IN_PROGRESS": 1, "TESTING": 1, "DONE": 1}!!!!!!!!!!
    std::cout << std::endl;
 
    /* TEST 3 */
    std::cout << "\nLisa" << std::endl;
 
    for (auto i = 0; i < 5; ++i) {
        tasks.AddNewTask("Lisa");
    }
 
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 5);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 5}, {}]
 
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 5);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"TESTING": 5}, {}]
 
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 1);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"DONE": 1}, {"TESTING": 4}]
    for (auto i = 0; i < 5; ++i) {
        tasks.AddNewTask("Lisa");
    }
 
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 2);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 2}, {"NEW": 3, "TESTING": 4}]
 
    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"NEW": 3, "IN_PROGRESS": 2, "TESTING": 4, "DONE": 1}
    std::cout << std::endl;
 
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 4);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 3, "TESTING": 1}, {"IN_PROGRESS": 1, "TESTING": 4}]
 
    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"IN_PROGRESS": 4, "TESTING": 5, "DONE": 1}
    std::cout << std::endl;
 
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 5);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"TESTING": 4, "DONE": 1}, {"TESTING": 4}]
 
    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"TESTING": 8, "DONE": 2}
    std::cout << std::endl;
 
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 10);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"DONE": 8}, {}]
 
    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"DONE": 10}
    std::cout << std::endl;
 
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 10);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{}, {}]
 
    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"DONE": 10}
    std::cout << std::endl;
    tasks.AddNewTask("Lisa");
 
    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"NEW": 1, "DONE": 10}
    std::cout << std::endl;
 
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 2);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 1}, {}]
 
    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"IN_PROGRESS": 1, "DONE": 10}
    std::cout << std::endl;
 
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Bob", 3);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{}, {}]
 
    return 0;
}
