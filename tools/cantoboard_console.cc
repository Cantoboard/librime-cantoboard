/*
 * Copyright RIME Developers
 * Distributed under the BSD License
 *
 * 2014-01-04 GONG Chen <chen.sst@gmail.com>
 */

#include <iostream>

#include <stdio.h>
#include <string.h>
#include <rime_api.h>

#include "../src/cantoboard_module.h"

void on_message(void* context_object,
                RimeSessionId session_id,
                const char* message_type,
                const char* message_value) {
  printf("message: [%lu] [%s] %s\n", session_id, message_type, message_value);
}

static RIME_MODULE_LIST(cantoboard_modules, "default", "cantoboard");

void query(const char* q) {
  std::set<std::string> possible_prefixes;
  cantoboard::List10KeysPrefixes(q, possible_prefixes);
  std::cout << q << ": ";
  for (auto&& p : possible_prefixes) {
    std::cout << p << " " ;
  }
  std::cout << "\n";
}

int main(int argc, char *argv[]) {
  RimeApi* rime = rime_get_api();

  RIME_STRUCT(RimeTraits, traits);
  traits.app_name = "cantoboard.console";
  traits.modules = cantoboard_modules;
  rime->setup(&traits);

  rime->set_notification_handler(&on_message, NULL);

  fprintf(stderr, "initializing...\n");
  rime->initialize(&traits);
  Bool full_check = True;
  if (rime->start_maintenance(full_check))
    rime->join_maintenance_thread();
  fprintf(stderr, "ready.\n");
  
  query("DGT");
  query("D");
  query("dGT");

  rime->finalize();

  return 0;
}
