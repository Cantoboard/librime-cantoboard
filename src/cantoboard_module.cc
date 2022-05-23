#include <algorithm>
#include <iostream>
#include <vector>

#include <rime_api.h>
#include <rime/candidate.h>
#include <rime/common.h>
#include <rime/context.h>
#include <rime/dict/dictionary.h>
#include <rime/dict/prism.h>
#include <rime/menu.h>
#include <rime/registry.h>
#include <rime/service.h>

#include "cantoboard_module.h"

using namespace std;

using namespace rime;
using namespace rime::prism;

static const char* table_name = "jyut6ping3";
static const char* prism_name = "jyut6ping3_10keys";

static an<Dictionary> dict;

static void rime_cantoboard_initialize() {
  LOG(INFO) << "registering components from module 'cantoboard'.";
}

static void rime_cantoboard_finalize() {
  if (dict) dict.reset();
}

namespace cantoboard {

static void Load10KeysDict() {
  if (dict) return;

  Registry& registry = Registry::instance();
  DictionaryComponent* dictionaryComponent = dynamic_cast<DictionaryComponent*>(registry.Find("dictionary"));
  if (!dictionaryComponent) {
    LOG(ERROR) << "cannot find DictionaryComponent.";
    return;
  }

  dict.reset(dictionaryComponent->Create(table_name, prism_name, vector<string>()));
  if (!dict) {
    LOG(ERROR) << "Failed loading dictionary(" << table_name << ", " << prism_name << ").";
  }
}

size_t GetSelectedTextEndIndex(RimeSessionId session_id) {
  an<Session> session(Service::instance().GetSession(session_id));
  if (!session) return;
  Context *ctx = session->context();
  if (!ctx) return;
  const Composition& comp = ctx->composition();
  
  size_t maxEndIndex = 0;
  for (const auto& seg : comp) {
    // if (auto cand = seg.GetSelectedCandidate()) {
    //   std::cout << cand->text() << " start " << seg.start << ", " << seg.end << ", " << seg.status << "\n";
    // }
    if (seg.status != rime::Segment::kSelected && seg.status != rime::Segment::kConfirmed) { continue; }
    maxEndIndex = max(maxEndIndex, seg.end);
  }
  return maxEndIndex;
}

void List10KeysPrefixes(const char* prefix, set<string> &results) {
  Load10KeysDict();
  if (!dict->Load()) {
    LOG(ERROR) << "Failed to load dictionary.";
    return;
  }
  
  an<Table> table = dict->primary_table();
  an<Prism> prism = dict->prism();
  if (!table || !prism) {
    LOG(ERROR) << "Table and prism shouldn't be null.";
    return;
  }
  
  vector<Prism::Match> matches;
  prism->CommonPrefixSearch(prefix, &matches);
  
  for (auto&& match : matches) {
    int spelling_id = match.value;
    SpellingAccessor accessor(prism->QuerySpelling(spelling_id));
    while (!accessor.exhausted()) {
      SyllableId syllable_id = accessor.syllable_id();
      
      // Filter out 特定詞組異讀字 e.g 粉絲 fen1 si2
      TableAccessor table_accessor = table->QueryWords(syllable_id);
      if (!table_accessor.exhausted()) {
        string syllable = table->GetSyllableById(syllable_id);
        syllable.pop_back(); // Remove tone
        results.insert(syllable);
      }
      
      accessor.Next();
    }
  }
  
  return results;
}

void SetInput(RimeSessionId session_id, const string& value) {
  an<Session> session(Service::instance().GetSession(session_id));
  if (!session) return;
  Context *ctx = session->context();
  if (!ctx) return;
  ctx->set_input(value);
}

bool UnlearnCandidate(RimeSessionId session_id, size_t candidate_index) {
  an<Session> session(Service::instance().GetSession(session_id));
  if (!session) return false;
  Context *ctx = session->context();
  if (!ctx) return false;
  
  Composition& comp = ctx->composition();
  if (comp.empty() || !comp.back().menu)
    return false;
  
  int index = candidate_index;
  int candidate_count = comp.back().menu->Prepare(index + 1);
  if (candidate_count <= index)
    return false;
  comp.back().selected_index = index;
  comp.back().tags.insert("unlearn");
  return ctx->DeleteCurrentSelection();
}

} // namepsace cantoboard

RIME_REGISTER_MODULE(cantoboard)
