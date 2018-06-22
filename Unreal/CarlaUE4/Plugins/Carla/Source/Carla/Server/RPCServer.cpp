// Fill out your copyright notice in the Description page of Project Settings.

#include "Carla.h"
#include "Server.h"

#include <compiler/disable-ue4-macros.h>
#include <carla/Version.h>
#include <carla/networking/Transform.h>
#include <carla/networking/VehicleControl.h>
#include <rpc/server.h>
#include <compiler/enable-ue4-macros.h>

static FTransform MakeTransform(const carla::networking::Transform &cTransform)
{
  FVector Translation(
      cTransform.location.x,
      cTransform.location.y,
      cTransform.location.z);
  FRotator Rotation(
      cTransform.rotation.pitch,
      cTransform.rotation.yaw,
      cTransform.rotation.roll);
  FVector Scale(1.0f, 1.0f, 1.0f);
  return FTransform(Rotation, Translation, Scale);
}

static FVehicleControl MakeControl(const carla::networking::VehicleControl &cControl)
{
  FVehicleControl Control;
  Control.Throttle = cControl.throttle;
  Control.Steer = cControl.steer;
  Control.Brake = cControl.brake;
  Control.bHandBrake = cControl.hand_brake;
  Control.bReverse = cControl.reverse;
  return Control;
}

class FRPCServer::Pimpl
{
public:
  Pimpl(uint16_t port) : Server(port) {}
  rpc::server Server;
};

FRPCServer::FRPCServer() : _Pimpl(nullptr) {}

FRPCServer::~FRPCServer() {}

void FRPCServer::Initialize(AServer &Server, uint16_t Port)
{
  UE_LOG(LogTemp, Error, TEXT("Initializing rpc-server at port %d"), Port);

  // try {
    _Pimpl = std::make_unique<Pimpl>(Port);
  // } catch (const std::exception &e) {
  //   UE_LOG(LogTemp, Error, TEXT("Exception thrown"));
  //   UE_LOG(LogTemp, Error, TEXT("%s"), e.what());
  //   return;
  // }

  namespace cn = carla::networking;

  auto &srv = _Pimpl->Server;

  srv.suppress_exceptions(true);

  srv.bind("ping", [](){ return true; });

  srv.bind("version", [](){ return std::string(carla::version()); });

  srv.bind("spawn", [&](cn::Transform transform) -> int32_t {
    return Server.SpawnAgentAsync(MakeTransform(transform)).get();
  });

  srv.bind("control", [&](int32 id, const cn::VehicleControl &control) {
    return Server.ApplyControl(id, MakeControl(control));
  });
}

void FRPCServer::Run()
{
  _Pimpl->Server.async_run(4);
}
