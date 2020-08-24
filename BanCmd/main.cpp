#include "global.h"

#include <dllentry.h>

Settings settings;
std::set<std::string> bannedCommands;
DEFAULT_SETTINGS(settings);

void dllenter() {
}
void dllexit() {}

void PreInit() {
}
void PostInit() {
  for (std::string &str : settings.bannedCommandsVector) { 
      bannedCommands.emplace(str);
  }
  settings.bannedCommandsVector.clear();
}

THook(
    void *, "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVCommandRequestPacket@@@Z",
    ServerNetworkHandler &snh, NetworkIdentifier const &netid, void *pk) {
  auto &db = Mod::PlayerDatabase::GetInstance();
  auto it  = db.Find(netid);
  if (!it || it->player->getCommandPermissionLevel() > CommandPermissionLevel::Any) { return original(snh, netid, pk); }
  std::string command(direct_access<std::string>(pk, 0x28));
  command = command.substr(1);
  std::vector<std::string> results;
  boost::split(results, command, [](char c) { return c == ' '; });
  if (settings.logCommands) { 
      LOGV("%s used: %s") % it->name % command;
  }
  if (bannedCommands.count(results[0])) {
      auto packet = TextPacket::createTextPacket<TextPacketType::SystemMessage>(settings.blockedCommand);
      it->player->sendNetworkPacket(packet);
      return nullptr;
  }
  return original(snh, netid, pk);
}