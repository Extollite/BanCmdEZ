#pragma once

#include <yaml.h>
#include <string>
#include <map>
#include <base/log.h>
#include "base/playerdb.h"
#include <Actor/Actor.h>
#include <Packet/TextPacket.h>
#include <Actor/Player.h>

#include <boost/scope_exit.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

struct Settings {
  std::vector<std::string> bannedCommandsVector;
  bool logCommands           = false;
  std::string blockedCommand = "You cannot use this command!";
  template <typename IO> static inline bool io(IO f, Settings &settings, YAML::Node &node) {
    return f(settings.bannedCommandsVector, node["bannedCommands"]) &&
           f(settings.blockedCommand, node["blockedCommandMessage"]) &&
           f(settings.logCommands, node["logCommands"]);
  }
};

DEF_LOGGER("BC");

extern Settings settings;

