#ifndef CANTOBOARD_MODULE_H_
#define CANTOBOARD_MODULE_H_

#include <set>
#include <string>
#include <rime_api.h>

namespace cantoboard {

extern void SetInput(RimeSessionId session_id, const std::string& value);
extern void List10KeysPrefixes(const char* prefix, std::set<std::string> &results);
extern size_t GetSelectedTextEndIndex(RimeSessionId session_id);

}  // namespace cantoboard

#endif  // CANTOBOARD_MODULE_H_
