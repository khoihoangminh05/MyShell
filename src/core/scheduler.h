#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <string>
#include <vector>

namespace scheduler {

void executeCommand(const std::string &command, const std::vector<std::string> &args);

void scheduleCommand(const std::vector<std::string> &args);

} // namespace scheduler

#endif // SCHEDULER_H
